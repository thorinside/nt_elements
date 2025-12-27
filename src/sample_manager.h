// Copyright 2025 Neal Sanche
// SPDX-License-Identifier: MIT

#ifndef SAMPLE_MANAGER_H
#define SAMPLE_MANAGER_H

#include <stdint.h>
#include <stddef.h>
#include <distingnt/wav.h>

/**
 * SampleManager - Loads Elements wavetable and noise samples from SD card
 *
 * This class manages the loading of sample data from WAV files stored on
 * the disting NT's SD card. It allocates DRAM for sample storage and provides
 * accessors for the Elements DSP code.
 *
 * Memory layout in DRAM:
 *   - sampleData_[0..128012]  (256,026 bytes) - 9 wavetables concatenated
 *   - noiseSample_[0..40962]  (81,926 bytes)  - Noise sample
 *
 * Usage:
 *   1. Add SampleManager::kTotalDramBytes to DRAM requirements
 *   2. Call init() with DRAM pointer in construct()
 *   3. Call loadStep() from step() when SD card is mounted
 *   4. Use accessors to get pointers for Elements DSP (zeros if not loaded)
 *
 * Loading is non-blocking - call loadStep() each frame and it will
 * incrementally load files one at a time without blocking the audio thread.
 */
class SampleManager {
public:
    // Memory size constants
    static constexpr size_t kSampleDataSize = 128013;   // int16_t samples (9 wavetables)
    static constexpr size_t kNoiseSampleSize = 40963;   // int16_t samples (noise)
    static constexpr size_t kTotalSamples = kSampleDataSize + kNoiseSampleSize;
    static constexpr size_t kTotalDramBytes = kTotalSamples * sizeof(int16_t);  // ~338KB

    // Number of wavetables and boundary entries
    static constexpr size_t kNumWavetables = 9;
    static constexpr size_t kNumBoundaries = kNumWavetables + 1;
    static constexpr size_t kNumTotalFiles = kNumWavetables + 1;  // 9 wavetables + 1 noise

    // Sample folder name on SD card
    static constexpr const char* kSampleFolderName = "elements";

    // Retry settings for failed loads
    static constexpr uint32_t kRetryDelayFrames = 48000;  // ~1 second at 48kHz step rate
    static constexpr uint32_t kMaxRetries = 3;

    // Loading state machine
    enum class LoadState : uint8_t {
        IDLE,               // Not loading, ready to start or already complete
        VALIDATING,         // Validating folder contents before loading
        LOADING_WAVETABLE,  // Currently loading a wavetable (index in currentFileIndex_)
        LOADING_NOISE,      // Currently loading the noise sample
        COMPLETE,           // All files loaded successfully
        FAILED              // Loading failed, can retry
    };

    /**
     * Initialize the sample manager with a DRAM buffer.
     * Zeros all memory and sets up internal pointers.
     *
     * @param dramBuffer Pointer to DRAM buffer of at least kTotalDramBytes
     */
    void init(int16_t* dramBuffer);

    /**
     * Non-blocking step function - call from step() each frame.
     * Manages the async loading state machine.
     * Does nothing if already loaded or SD card not mounted.
     *
     * @return true if samples are ready (loaded or already loaded)
     */
    bool loadStep();

    /**
     * Reset loading state to allow retry.
     * Call this when SD card is unmounted or to force reload.
     */
    void reset();

    /**
     * Get pointer to wavetable sample data.
     * Contains all 9 wavetables concatenated.
     *
     * @return Pointer to sample data (zeros if not loaded)
     */
    const int16_t* getSampleData() const { return sampleData_; }

    /**
     * Get pointer to noise sample data.
     *
     * @return Pointer to noise sample (zeros if not loaded)
     */
    const int16_t* getNoiseSample() const { return noiseSample_; }

    /**
     * Get pointer to wavetable boundaries array.
     * Contains 10 values: start offsets for each of 9 wavetables plus end offset.
     *
     * @return Pointer to boundaries array
     */
    const size_t* getBoundaries() const { return boundaries_; }

    /**
     * Check if samples have been loaded.
     *
     * @return true if samples are loaded, false otherwise
     */
    bool isLoaded() const { return loadState_ == LoadState::COMPLETE; }

    /**
     * Check current loading state.
     *
     * @return Current load state
     */
    LoadState getLoadState() const { return loadState_; }

private:
    // Validate folder contents (file names, formats)
    bool validateFolder(uint32_t folderIndex);

    // Start loading the next file in sequence
    bool startNextLoad();

    // Find the "elements" sample folder index
    // Returns -1 if not found
    int32_t findSampleFolder() const;

    // Callback for async WAV loading
    static void onSampleLoaded(void* userData, bool success);

    // DRAM buffer pointers
    int16_t* sampleData_;       // Points into DRAM (9 wavetables)
    int16_t* noiseSample_;      // Points into DRAM (after sampleData_)

    // Precomputed boundary offsets for wavetables
    // boundaries_[i] = start offset of wavetable i
    // boundaries_[9] = end offset (total size)
    size_t boundaries_[kNumBoundaries];

    // Persistent WAV request (must persist until callback)
    _NT_wavRequest currentRequest_;

    // Loading state machine
    LoadState loadState_;
    uint32_t folderIndex_;          // Cached folder index once found
    uint32_t currentFileIndex_;     // Which file we're loading (0-8 wavetables, 9 = noise)
    volatile bool callbackPending_; // True if waiting for callback
    volatile bool callbackSuccess_; // Result from last callback

    // Retry state
    uint32_t retryCount_;
    uint32_t retryDelayCounter_;

    // File index mapping (maps expected file index to actual folder file index)
    // fileIndices_[0] = folder index of wavetable_00.wav
    // fileIndices_[9] = folder index of noise.wav
    uint32_t fileIndices_[kNumTotalFiles];
};

#endif // SAMPLE_MANAGER_H
