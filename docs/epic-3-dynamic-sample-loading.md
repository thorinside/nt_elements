# Epic 3: Dynamic Sample Loading from SD Card

**Author:** Neal (assisted by Mary, Business Analyst)
**Date:** 2025-12-25
**Status:** Approved

---

## Overview

This epic introduces dynamic sample loading from the disting NT's SD card, replacing the 338KB of static sample data currently compiled into the plugin binary. This reduces binary size by ~1.5MB (source file) and enables future customization of sample sets.

**Business Value:**
- Significantly smaller plugin binary
- Foundation for user-customizable wavetables
- Cleaner separation of code and audio assets
- Aligns with disting NT's new WAV file API (v1.12.0+)

**Technical Approach:**
- Create extraction script to convert static arrays to WAV files
- Patch Elements exciter.cc to use indirection layer
- Implement sample manager that loads all samples at plugin construction
- Store samples in DRAM (~338KB allocation)

---

## Prerequisites

- distingNT_API submodule updated to v1.12.0+ (commit `fb30ae4`) for WAV API
- Existing patch infrastructure (`patches/` directory, Makefile integration)
- nt_emu emulator updated with virtual SD card support (Story 3.0)

---

## Story 3.0: Virtual SD Card Support in nt_emu

**Status:** Done
**Project:** ../nt_emu (sibling project)

### Story

As a developer,
I want the nt_emu VCV Rack emulator to support a virtual SD card backed by a local folder,
So that I can test SD card-dependent plugins without hardware.

### Acceptance Criteria

1. nt_emu's api.h updated to include WAV API declarations (version 10+)
2. Virtual SD card implementation that maps to a user-selected folder
3. Folder path persisted in VCV module settings (remembered across sessions)
4. UI element to select/change the virtual SD card folder
5. WAV API stub implementations in NTApiWrapper.cpp:
   - `NT_isSdCardMounted()` — returns true if folder is set and exists
   - `NT_getNumSampleFolders()` — counts subdirectories
   - `NT_getSampleFolderInfo()` — returns folder name and file count
   - `NT_getSampleFileInfo()` — returns WAV file metadata
   - `NT_readSampleFrames()` — loads WAV data from filesystem
6. Async callback pattern emulated (can be synchronous in VCV context)

### Technical Approach

**Virtual SD Card Mapping:**
```
User-selected folder (e.g., ~/nt_virtual_sd/)
├── samples/
│   └── elements/
│       ├── wavetable_00.wav
│       ├── wavetable_01.wav
│       └── ...
└── presets/
    └── ...
```

**API Stub Implementation Strategy:**

```cpp
// In NTApiWrapper.cpp or new file VirtualSdCard.cpp

static std::string g_virtualSdPath;  // Persisted in module JSON

extern "C" {
    bool NT_isSdCardMounted(void) {
        return !g_virtualSdPath.empty() &&
               std::filesystem::exists(g_virtualSdPath);
    }

    uint32_t NT_getNumSampleFolders(void) {
        // Count subdirs in g_virtualSdPath + "/samples/"
    }

    void NT_getSampleFolderInfo(uint32_t folder, _NT_wavFolderInfo& info) {
        // Enumerate folder at index, count WAV files
    }

    void NT_getSampleFileInfo(uint32_t folder, uint32_t sample, _NT_wavInfo& info) {
        // Read WAV header, return metadata
    }

    bool NT_readSampleFrames(const _NT_wavRequest& request) {
        // Load WAV file synchronously
        // Call request.callback immediately with result
        return true;
    }
}
```

### Tasks

- [ ] Add `distingnt/wav.h` to nt_emu's emulator/include/distingnt/
- [ ] Update nt_emu's api.h to kNT_apiVersion10+
- [ ] Create `VirtualSdCard.cpp` with filesystem-backed implementations
- [ ] Add folder selection UI in VCV module (right-click menu or panel widget)
- [ ] Persist folder path in module's `dataToJson()` / `dataFromJson()`
- [ ] Add WAV file reading using standard library (dr_wav or similar)
- [ ] Test with sample loading plugin

### Files to Modify (in nt_emu project)

- `emulator/include/distingnt/api.h` — Update API version
- `emulator/include/distingnt/wav.h` — New file (copy from distingNT_API)
- `vcv-plugin/src/api/NTApiWrapper.cpp` — Add WAV API stubs
- `vcv-plugin/src/api/VirtualSdCard.cpp` — New file
- `vcv-plugin/src/api/VirtualSdCard.hpp` — New file
- `vcv-plugin/src/NtEmu.cpp` — Add folder selection UI, persist path

### Prerequisites

None (independent of nt_elements changes)

### Estimated Effort

Medium-Large (3-4 hours)

### Notes

This is a cross-project dependency. The nt_emu changes should be completed before testing Epic 3 stories in VCV Rack. Hardware testing can proceed independently.

---

## Story 3.1: Update distingNT_API Submodule for WAV Support

**Status:** Pending

### Story

As a developer,
I want the distingNT_API submodule updated to include the new WAV file API,
So that I can use `NT_readSampleFrames()` and related functions.

### Acceptance Criteria

1. distingNT_API submodule pinned to commit `fb30ae4` or later
2. New header `distingnt/wav.h` available in include path
3. Project compiles with new API version (kNT_apiVersion10+)
4. Existing functionality unaffected by API update

### Tasks

- [ ] Update submodule: `git -C distingNT_API fetch origin && git -C distingNT_API checkout fb30ae4`
- [ ] Update parent repo's submodule reference
- [ ] Verify build succeeds with new API
- [ ] Test existing plugin functionality unchanged

### Prerequisites

None

### Estimated Effort

Small (< 1 hour)

---

## Story 3.2: Sample Extraction Script

**Status:** Pending

### Story

As a developer,
I want a script that extracts the static sample arrays into WAV files,
So that samples can be loaded from SD card at runtime.

### Acceptance Criteria

1. Python script reads `resources.cc` or `resources_minimal_wavetables.cc`
2. Extracts `smp_sample_data` into 9 separate WAV files (one per wavetable region)
3. Extracts `smp_noise_sample` into 1 WAV file
4. Output files: `samples/elements/wavetable_00.wav` through `wavetable_08.wav` plus `noise.wav`
5. WAV format: mono, 16-bit PCM, 48kHz (matching disting NT native rate)
6. Script is idempotent (skips extraction if files exist)
7. `samples/elements/` added to `.gitignore`
8. Makefile target `extract-samples` added

### Tasks

- [ ] Create `scripts/extract_samples.py`
  - [ ] Parse `smp_boundaries[]` to identify wavetable regions
  - [ ] Read int16_t values from `smp_sample_data[]`
  - [ ] Write each region as separate WAV file
  - [ ] Read and write `smp_noise_sample[]` as `noise.wav`
- [ ] Add `samples/elements/` to `.gitignore`
- [ ] Add Makefile target:
  ```makefile
  extract-samples:
      @if [ ! -f samples/elements/wavetable_00.wav ]; then \
          python3 scripts/extract_samples.py; \
      fi
  ```
- [ ] Document sample format in `samples/elements/README.md`

### Data Mapping

| Source Array Region | Boundary Start | Boundary End | Output File |
|---------------------|----------------|--------------|-------------|
| smp_sample_data[0..17098] | 0 | 17,099 | wavetable_00.wav |
| smp_sample_data[17099..20851] | 17,099 | 20,852 | wavetable_01.wav |
| smp_sample_data[20852..30368] | 20,852 | 30,369 | wavetable_02.wav |
| smp_sample_data[30369..63049] | 30,369 | 63,050 | wavetable_03.wav |
| smp_sample_data[63050..85806] | 63,050 | 85,807 | wavetable_04.wav |
| smp_sample_data[85807..95951] | 85,807 | 95,952 | wavetable_05.wav |
| smp_sample_data[95952..106296] | 95,952 | 106,297 | wavetable_06.wav |
| smp_sample_data[106297..117605] | 106,297 | 117,606 | wavetable_07.wav |
| smp_sample_data[117606..128012] | 117,606 | 128,013 | wavetable_08.wav |
| smp_noise_sample[0..40962] | 0 | 40,963 | noise.wav |

### Prerequisites

None (can be done in parallel with Story 3.1)

### Estimated Effort

Medium (2-3 hours)

---

## Story 3.3: Sample Manager Implementation

**Status:** Pending

### Story

As a developer,
I want a sample manager class that loads WAV files from SD card into DRAM,
So that sample data is available for the Elements DSP at runtime.

### Acceptance Criteria

1. `SampleManager` class implemented in `src/sample_manager.h` and `src/sample_manager.cpp`
2. Allocates DRAM buffer in `calculateRequirements()` for all samples (~338KB)
3. Loads all 10 WAV files synchronously during `construct()`
4. Provides `getSampleData()` and `getNoiseSample()` accessors
5. Provides `getBoundaries()` accessor returning precomputed boundary array
6. Returns zeros if samples not yet loaded (SD card not mounted)
7. Handles SD card mount/unmount gracefully

### API Design

```cpp
class SampleManager {
public:
    // Memory requirements (call from calculateRequirements)
    static constexpr size_t kSampleDataSize = 128013;  // int16_t samples
    static constexpr size_t kNoiseSampleSize = 40963;  // int16_t samples
    static constexpr size_t kTotalDramBytes =
        (kSampleDataSize + kNoiseSampleSize) * sizeof(int16_t);  // ~338KB

    // Initialize with DRAM pointer from construct()
    void init(int16_t* dramBuffer);

    // Load all samples (call from construct or first step)
    bool loadSamples();

    // Accessors for DSP
    const int16_t* getSampleData() const;
    const int16_t* getNoiseSample() const;
    const size_t* getBoundaries() const;

    // State
    bool isLoaded() const;

private:
    int16_t* sampleData_;      // Points into DRAM
    int16_t* noiseSample_;     // Points into DRAM (after sampleData_)
    size_t boundaries_[10];    // Precomputed boundary offsets
    bool loaded_;
};
```

### Tasks

- [ ] Create `src/sample_manager.h` with class declaration
- [ ] Create `src/sample_manager.cpp` with implementation
  - [ ] `init()`: Partition DRAM buffer, zero memory
  - [ ] `loadSamples()`: Use NT_readSampleFrames() for each WAV
  - [ ] Handle async callback pattern (wait for completion)
  - [ ] Populate `boundaries_[]` array after successful load
- [ ] Add `sample_manager.cpp` to Makefile SOURCES
- [ ] Unit test with mock data (desktop build)

### Memory Layout in DRAM

```
DRAM Buffer (338,952 bytes):
├── sampleData_[0..128012]     (256,026 bytes) - 9 wavetables concatenated
└── noiseSample_[0..40962]     (81,926 bytes)  - Noise sample
```

### Prerequisites

- Story 3.1 (WAV API available)
- Story 3.2 (WAV files exist on SD card)

### Estimated Effort

Medium-Large (3-4 hours)

---

## Story 3.4: Elements Exciter Patch for Sample Indirection

**Status:** Pending

### Story

As a developer,
I want the Elements exciter code patched to use indirect sample access,
So that samples can be provided by the SampleManager instead of static arrays.

### Acceptance Criteria

1. New patch file `patches/elements-dynamic-samples.patch` created
2. Patch modifies `exciter.cc` to use function-based sample access
3. Patch modifies `resources.h` to declare external sample accessors
4. Original `smp_sample_data` and `smp_noise_sample` arrays no longer compiled
5. Makefile applies new patch alongside existing `elements-dynamic-sample-rate.patch`
6. Build succeeds with patched code
7. No changes to exciter algorithm logic (only data access pattern)

### Patch Strategy

**Option A: Pointer Injection (Recommended)**
- Add extern pointers in patched `resources.h`
- `nt_elements.cpp` sets pointers after SampleManager loads
- Minimal changes to exciter.cc (just pointer names)

### Patch Details

**File: `elements/resources.h` (additions)**
```cpp
// Dynamic sample data - set by nt_elements after loading
extern const int16_t* smp_sample_data_ptr;
extern const int16_t* smp_noise_sample_ptr;
extern const size_t* smp_boundaries_ptr;

// Macros to redirect access
#define smp_sample_data smp_sample_data_ptr
#define smp_noise_sample smp_noise_sample_ptr
#define smp_boundaries smp_boundaries_ptr
```

**File: `exciter.cc`**
- No changes needed if using macro redirection
- Existing code `&smp_sample_data[offset]` works unchanged

### Tasks

- [ ] Create `patches/elements-dynamic-samples.patch`
  - [ ] Add extern pointer declarations to resources.h
  - [ ] Add macro redirections
  - [ ] Remove static array definitions (or make them conditional)
- [ ] Update Makefile to apply new patch
- [ ] Update `patches/README.md` with new patch documentation
- [ ] Test patch applies cleanly to fresh submodule
- [ ] Test patch reversal works

### Prerequisites

- Story 3.3 (SampleManager provides the pointers)

### Estimated Effort

Medium (2-3 hours)

---

## Story 3.5: Integration and Validation

**Status:** Pending

### Story

As a developer,
I want the complete dynamic sample loading system integrated and tested,
So that nt_elements works correctly with samples loaded from SD card.

### Acceptance Criteria

1. nt_elements.cpp integrates SampleManager
2. DRAM allocation increased to include sample buffer (~338KB additional)
3. Sample pointers set before first audio processing
4. Plugin produces correct audio output (matches static version)
5. Graceful behavior when SD card not mounted (silence, no crash)
6. CPU usage impact documented (expected: minimal)
7. Memory usage documented
8. Hardware validation on actual disting NT

### Integration Points in nt_elements.cpp

```cpp
// calculateRequirements()
req.dram = 32768 * sizeof(uint16_t)           // Existing reverb buffer
         + SampleManager::kTotalDramBytes;     // New sample buffer

// construct()
// Partition DRAM
uint8_t* dramPtr = ptrs.dram;
self->reverbBuffer = reinterpret_cast<uint16_t*>(dramPtr);
dramPtr += 32768 * sizeof(uint16_t);
self->sampleManager.init(reinterpret_cast<int16_t*>(dramPtr));

// Load samples
self->sampleManager.loadSamples();

// Set global pointers for Elements code
smp_sample_data_ptr = self->sampleManager.getSampleData();
smp_noise_sample_ptr = self->sampleManager.getNoiseSample();
smp_boundaries_ptr = self->sampleManager.getBoundaries();
```

### Tasks

- [ ] Modify `nt_elements.cpp` to use SampleManager
  - [ ] Update DRAM allocation
  - [ ] Initialize SampleManager in construct()
  - [ ] Set global sample pointers
- [ ] Remove `resources_minimal_wavetables.cc` from SOURCES
- [ ] Add conditional fallback for desktop testing (load from file path)
- [ ] Test audio output matches original (A/B comparison)
- [ ] Test on hardware with samples on SD card
- [ ] Document memory usage before/after
- [ ] Document any startup time impact

### Validation Checklist

- [ ] Plugin loads without samples on SD card (silent operation)
- [ ] Plugin loads with samples on SD card (correct audio)
- [ ] Wavetable morphing works correctly across all 9 wavetables
- [ ] Noise sample works in granular mode
- [ ] No audio glitches during playback
- [ ] Memory usage within expected bounds
- [ ] CPU usage unchanged from static version

### Prerequisites

- Story 3.4 (Patch applied)
- Story 3.3 (SampleManager implemented)
- Story 3.2 (WAV files available)

### Estimated Effort

Medium-Large (3-4 hours)

---

## Epic Summary

| Story | Title | Effort | Dependencies |
|-------|-------|--------|--------------|
| 3.0 | Virtual SD Card Support in nt_emu | Medium-Large | None (nt_emu project) |
| 3.1 | Update distingNT_API Submodule | Small | None |
| 3.2 | Sample Extraction Script | Medium | None |
| 3.3 | Sample Manager Implementation | Medium-Large | 3.1, 3.2 |
| 3.4 | Elements Exciter Patch | Medium | 3.3 |
| 3.5 | Integration and Validation | Medium-Large | 3.0, 3.4 |

**Total Estimated Effort:** 2-3 days

**Critical Path:**
- Hardware: 3.1 → 3.3 → 3.4 → 3.5
- Desktop/VCV: 3.0 → 3.5 (3.0 can run in parallel with 3.1-3.4)
- Story 3.2 can run in parallel with all others

---

## Technical References

### Files to Modify

- `distingNT_API/` — Submodule update
- `Makefile` — New targets, patch application, source list
- `.gitignore` — Add `samples/elements/`
- `patches/elements-dynamic-samples.patch` — New patch file
- `patches/README.md` — Document new patch
- `src/sample_manager.h` — New file
- `src/sample_manager.cpp` — New file
- `src/nt_elements.cpp` — Integrate SampleManager
- `scripts/extract_samples.py` — New file

### Files to Remove from Build

- `src/resources_minimal_wavetables.cc` — No longer needed (data on SD card)

### New API Functions Used

From `distingnt/wav.h`:
- `NT_isSdCardMounted()` — Check card availability
- `NT_getNumSampleFolders()` — Enumerate folders
- `NT_getSampleFolderInfo()` — Get folder metadata
- `NT_getSampleFileInfo()` — Get WAV file metadata
- `NT_readSampleFrames()` — Async load WAV data

### Memory Budget

| Component | Current | After Epic 3 |
|-----------|---------|--------------|
| Static sample data | ~338KB (in binary) | 0 (removed) |
| DRAM allocation | 64KB (reverb) | 402KB (reverb + samples) |
| Binary size reduction | — | ~1.5MB smaller |

---

## Design Decisions

1. **Sample folder location:** `/samples/elements/` — matches plugin name, easy to find

2. **Fallback for missing samples:** Return zeros — simplest, user will notice and add samples

3. **Desktop testing:** Conditional compilation to load from filesystem path in desktop builds

---

## Research Sources

- [Expert Sleepers distingNT_API](https://github.com/expertsleepersltd/distingNT_API) — WAV API documentation
- [Mutable Instruments Elements](https://github.com/pichenettes/eurorack) — Original DSP source
- [disting NT User Manual](https://www.expert-sleepers.co.uk/distingNTfirmwareupdates.html) — Firmware updates

## Post-Review Follow-ups

- (Story 3.3) Make `_NT_wavRequest` lifetime safe across timeouts/in-flight reads; persist requests as members until callback fires.
- (Story 3.3) Avoid blocking `step()` while loading samples; move to non-blocking state machine and gate sample-dependent behavior on `isLoaded()`.
- (Story 3.3) Add retry strategy when mounted but initial load fails (no audio-thread stalls).
- (Story 3.3) Validate sample rate and frame counts via `NT_getSampleFileInfo()` before reading into DRAM partitions.
- (Story 3.3) Keep story completion notes accurate and in sync with the actual implementation (avoid claiming features that aren’t in `src/sample_manager.*`).
