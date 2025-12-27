// Copyright 2025 Neal Sanche
// SPDX-License-Identifier: MIT

#include "sample_manager.h"
#include <distingnt/api.h>
#include <string.h>

// Precomputed boundary values from original Elements resources.cc
// These define the start offset of each wavetable region in sampleData_
static const size_t kDefaultBoundaries[SampleManager::kNumBoundaries] = {
    0,       // Start of wavetable 0
    17099,   // Start of wavetable 1
    20852,   // Start of wavetable 2
    30369,   // Start of wavetable 3
    63050,   // Start of wavetable 4
    85807,   // Start of wavetable 5
    95952,   // Start of wavetable 6
    106297,  // Start of wavetable 7
    117606,  // Start of wavetable 8
    128013   // End of last wavetable (total size)
};

// Expected sample file names in the elements folder
// Files must be in this exact order: wavetable_00.wav through wavetable_08.wav, then noise.wav
static const char* const kExpectedFileNames[] = {
    "wavetable_00.wav",
    "wavetable_01.wav",
    "wavetable_02.wav",
    "wavetable_03.wav",
    "wavetable_04.wav",
    "wavetable_05.wav",
    "wavetable_06.wav",
    "wavetable_07.wav",
    "wavetable_08.wav",
    "noise.wav"
};

// Expected frame counts for validation
static const uint32_t kExpectedFrameCounts[] = {
    17099,   // wavetable_00.wav
    3753,    // wavetable_01.wav (20852 - 17099)
    9517,    // wavetable_02.wav (30369 - 20852)
    32681,   // wavetable_03.wav (63050 - 30369)
    22757,   // wavetable_04.wav (85807 - 63050)
    10145,   // wavetable_05.wav (95952 - 85807)
    10345,   // wavetable_06.wav (106297 - 95952)
    11309,   // wavetable_07.wav (117606 - 106297)
    10407,   // wavetable_08.wav (128013 - 117606)
    40963    // noise.wav
};

void SampleManager::init(int16_t* dramBuffer) {
    // Set up pointers into DRAM buffer
    sampleData_ = dramBuffer;
    noiseSample_ = dramBuffer + kSampleDataSize;

    // Zero all memory (samples will be zeros if loading fails)
    memset(sampleData_, 0, kSampleDataSize * sizeof(int16_t));
    memset(noiseSample_, 0, kNoiseSampleSize * sizeof(int16_t));

    // Initialize boundaries with default values
    memcpy(boundaries_, kDefaultBoundaries, sizeof(boundaries_));

    // Initialize state machine
    loadState_ = LoadState::IDLE;
    folderIndex_ = 0;
    currentFileIndex_ = 0;
    callbackPending_ = false;
    callbackSuccess_ = false;

    // Initialize retry state
    retryCount_ = 0;
    retryDelayCounter_ = 0;

    // Zero the request structure
    memset(&currentRequest_, 0, sizeof(currentRequest_));

    // Zero the file index mapping
    memset(fileIndices_, 0, sizeof(fileIndices_));
}

void SampleManager::reset() {
    // Reset to allow new load attempt
    loadState_ = LoadState::IDLE;
    currentFileIndex_ = 0;
    callbackPending_ = false;
    callbackSuccess_ = false;
    retryCount_ = 0;
    retryDelayCounter_ = 0;

    // Re-zero buffers
    memset(sampleData_, 0, kSampleDataSize * sizeof(int16_t));
    memset(noiseSample_, 0, kNoiseSampleSize * sizeof(int16_t));
}

int32_t SampleManager::findSampleFolder() const {
    if (!NT_isSdCardMounted()) {
        return -1;
    }

    uint32_t numFolders = NT_getNumSampleFolders();

    for (uint32_t i = 0; i < numFolders; i++) {
        _NT_wavFolderInfo info;
        NT_getSampleFolderInfo(i, info);

        if (info.name && strcmp(info.name, kSampleFolderName) == 0) {
            return static_cast<int32_t>(i);
        }
    }

    return -1;  // Folder not found
}

void SampleManager::onSampleLoaded(void* userData, bool success) {
    SampleManager* self = static_cast<SampleManager*>(userData);
    self->callbackSuccess_ = success;
    self->callbackPending_ = false;  // Mark callback as received
}

bool SampleManager::validateFolder(uint32_t folderIndex) {
    // Check that folder has expected number of files
    _NT_wavFolderInfo folderInfo;
    NT_getSampleFolderInfo(folderIndex, folderInfo);
    if (folderInfo.numSampleFiles < kNumTotalFiles) {
        return false;
    }

    // Search for each expected file by name (files may be in any order)
    for (size_t expectedIdx = 0; expectedIdx < kNumTotalFiles; expectedIdx++) {
        bool found = false;

        // Search all files in folder for this expected file name
        for (uint32_t fileIdx = 0; fileIdx < folderInfo.numSampleFiles; fileIdx++) {
            _NT_wavInfo fileInfo;
            NT_getSampleFileInfo(folderIndex, fileIdx, fileInfo);

            // Check file name matches expected
            if (!fileInfo.name || strcmp(fileInfo.name, kExpectedFileNames[expectedIdx]) != 0) {
                continue;  // Not this file, keep searching
            }

            // Found the file - validate format and size
            // Validate format: should be mono 16-bit
            if (fileInfo.channels != kNT_WavMono || fileInfo.bits != kNT_WavBits16) {
                return false;  // Wrong format
            }

            // Validate frame count matches expected size
            if (fileInfo.numFrames != kExpectedFrameCounts[expectedIdx]) {
                return false;  // Wrong size
            }

            // Store the mapping from expected index to actual folder file index
            fileIndices_[expectedIdx] = fileIdx;
            found = true;
            break;
        }

        if (!found) {
            return false;  // Required file not found
        }
    }

    return true;
}

bool SampleManager::startNextLoad() {
    // Determine destination buffer and size based on current file index
    void* destBuffer;
    uint32_t numFrames;

    if (currentFileIndex_ < kNumWavetables) {
        // Loading a wavetable
        size_t startOffset = boundaries_[currentFileIndex_];
        destBuffer = sampleData_ + startOffset;
        numFrames = static_cast<uint32_t>(boundaries_[currentFileIndex_ + 1] - startOffset);
    } else {
        // Loading noise sample (index 9)
        destBuffer = noiseSample_;
        numFrames = static_cast<uint32_t>(kNoiseSampleSize);
    }

    // Set up the persistent request structure
    // Use the file index mapping to get actual folder file index
    currentRequest_.folder = folderIndex_;
    currentRequest_.sample = fileIndices_[currentFileIndex_];
    currentRequest_.dst = destBuffer;
    currentRequest_.numFrames = numFrames;
    currentRequest_.startOffset = 0;
    currentRequest_.channels = kNT_WavMono;
    currentRequest_.bits = kNT_WavBits16;
    currentRequest_.progress = kNT_WavNoProgress;
    currentRequest_.callback = &SampleManager::onSampleLoaded;
    currentRequest_.callbackData = this;

    // Mark that we're waiting for callback
    callbackPending_ = true;
    callbackSuccess_ = false;

    // Initiate the read
    if (!NT_readSampleFrames(currentRequest_)) {
        callbackPending_ = false;
        return false;
    }

    return true;
}

bool SampleManager::loadStep() {
    // Already complete?
    if (loadState_ == LoadState::COMPLETE) {
        return true;
    }

    // SD card not mounted?
    if (!NT_isSdCardMounted()) {
        // If we were loading, reset state
        if (loadState_ != LoadState::IDLE && loadState_ != LoadState::FAILED) {
            reset();
        }
        return false;
    }

    // Handle retry delay
    if (loadState_ == LoadState::FAILED) {
        if (retryCount_ >= kMaxRetries) {
            // Max retries exceeded, stay in FAILED state
            return false;
        }
        if (retryDelayCounter_ > 0) {
            --retryDelayCounter_;
            return false;
        }
        // Delay expired, try again
        loadState_ = LoadState::IDLE;
    }

    // State machine
    switch (loadState_) {
        case LoadState::IDLE: {
            // Find the elements folder
            int32_t folder = findSampleFolder();
            if (folder < 0) {
                // Folder not found - go to failed with retry
                loadState_ = LoadState::FAILED;
                retryDelayCounter_ = kRetryDelayFrames;
                ++retryCount_;
                return false;
            }
            folderIndex_ = static_cast<uint32_t>(folder);
            loadState_ = LoadState::VALIDATING;
            return false;
        }

        case LoadState::VALIDATING: {
            // Validate folder contents (quick synchronous check)
            if (!validateFolder(folderIndex_)) {
                loadState_ = LoadState::FAILED;
                retryDelayCounter_ = kRetryDelayFrames;
                ++retryCount_;
                return false;
            }
            // Start loading first wavetable
            currentFileIndex_ = 0;
            loadState_ = LoadState::LOADING_WAVETABLE;
            if (!startNextLoad()) {
                loadState_ = LoadState::FAILED;
                retryDelayCounter_ = kRetryDelayFrames;
                ++retryCount_;
                return false;
            }
            return false;
        }

        case LoadState::LOADING_WAVETABLE: {
            // Check if we're waiting for callback
            if (callbackPending_) {
                return false;  // Still waiting
            }

            // Callback received - check result
            if (!callbackSuccess_) {
                // This wavetable failed to load
                loadState_ = LoadState::FAILED;
                retryDelayCounter_ = kRetryDelayFrames;
                ++retryCount_;
                return false;
            }

            // Move to next file
            ++currentFileIndex_;

            if (currentFileIndex_ < kNumWavetables) {
                // More wavetables to load
                if (!startNextLoad()) {
                    loadState_ = LoadState::FAILED;
                    retryDelayCounter_ = kRetryDelayFrames;
                    ++retryCount_;
                    return false;
                }
            } else {
                // All wavetables done, load noise
                loadState_ = LoadState::LOADING_NOISE;
                if (!startNextLoad()) {
                    loadState_ = LoadState::FAILED;
                    retryDelayCounter_ = kRetryDelayFrames;
                    ++retryCount_;
                    return false;
                }
            }
            return false;
        }

        case LoadState::LOADING_NOISE: {
            // Check if we're waiting for callback
            if (callbackPending_) {
                return false;  // Still waiting
            }

            // Callback received - check result
            if (!callbackSuccess_) {
                loadState_ = LoadState::FAILED;
                retryDelayCounter_ = kRetryDelayFrames;
                ++retryCount_;
                return false;
            }

            // All files loaded successfully!
            loadState_ = LoadState::COMPLETE;
            return true;
        }

        case LoadState::COMPLETE:
            return true;

        case LoadState::FAILED:
            // Handled above at the start of the function
            return false;
    }

    return false;
}
