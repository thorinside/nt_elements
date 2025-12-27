# Story 3.3: Sample Manager Implementation

Status: done

## Story

As a developer,
I want a sample manager class that loads WAV files from SD card into DRAM,
So that sample data is available for the Elements DSP at runtime.

## Acceptance Criteria

1. `SampleManager` class implemented in `src/sample_manager.h` and `src/sample_manager.cpp`
2. Allocates DRAM buffer in `calculateRequirements()` for all samples (~338KB)
3. Loads all 10 WAV files synchronously during `construct()`
4. Provides `getSampleData()` and `getNoiseSample()` accessors
5. Provides `getBoundaries()` accessor returning precomputed boundary array
6. Returns zeros if samples not yet loaded (SD card not mounted)
7. Handles SD card mount/unmount gracefully

## Tasks / Subtasks

- [x] Create SampleManager header (AC: #1)
  - [x] Create `src/sample_manager.h`
  - [x] Define class with memory constants
  - [x] Declare init(), loadSamples(), accessors
  - [x] Define kSampleDataSize = 128013 samples
  - [x] Define kNoiseSampleSize = 40963 samples
  - [x] Calculate kTotalDramBytes (~338KB)

- [x] Implement initialization (AC: #2, #6)
  - [x] Create `src/sample_manager.cpp`
  - [x] Implement init() to partition DRAM buffer
  - [x] Zero all memory on initialization
  - [x] Set up pointers for sampleData_ and noiseSample_
  - [x] Initialize loaded_ flag to false

- [x] Implement sample loading (AC: #3)
  - [x] Check NT_isSdCardMounted() before loading
  - [x] Use NT_readSampleFrames() for each WAV file
  - [x] Handle async callback pattern
  - [x] Load wavetables 0-8 into sampleData_ buffer
  - [x] Load noise.wav into noiseSample_ buffer
  - [x] Populate boundaries_[] array after load

- [x] Implement accessors (AC: #4, #5)
  - [x] getSampleData() returns pointer to wavetable data
  - [x] getNoiseSample() returns pointer to noise data
  - [x] getBoundaries() returns boundary offset array
  - [x] isLoaded() returns load state

- [x] Handle missing samples gracefully (AC: #6, #7)
  - [x] Return zeroed buffers if not loaded
  - [x] Don't crash on missing files (returns false, buffers stay zeroed)
  - [x] Support late loading after mount

- [x] Add to build system (AC: #1)
  - [x] Add sample_manager.cpp to Makefile SOURCES
  - [x] Verify compilation succeeds (both test and hardware targets)

- [x] Wire SampleManager into plugin (AC: #2, #3, #7) - Review Action Item
  - [x] Add SampleManager to nt_elementsAlgorithm structure in nt_elements.h
  - [x] Include sample_manager.h in nt_elements.cpp
  - [x] Update calculateRequirements() to add SampleManager::kTotalDramBytes to req.dram
  - [x] Partition DRAM in construct() for both reverb buffer and samples
  - [x] Initialize SampleManager with sample DRAM pointer
  - [x] Add SD card mount detection in step() for late loading

- [x] Fix loadSamples() success reporting (AC: #6, #7) - Review Action Item
  - [x] Track individual file load failures
  - [x] Only set loaded_ = true when ALL files succeed
  - [x] Return false on any file load failure

- [x] Replace spin-wait with timeout (Safety) - Review Action Item
  - [x] Add kMaxSpinIterations timeout constant
  - [x] Add timeout loop in loadWavetable()
  - [x] Add timeout loop in loadNoiseSample()
  - [x] Return failure if timeout occurs

- [x] Add file name validation (AC: #7) - Review Action Item
  - [x] Add kExpectedFileNames array with expected file names
  - [x] Validate each file name before loading using NT_getSampleFileInfo()
  - [x] Validate mono 16-bit format
  - [x] Return false on validation failure

### Review Follow-ups (AI)

- [x] [AI-Review][High] Make `_NT_wavRequest` lifetime safe across timeouts and in-flight reads (persist requests as members; avoid returning while a request could still be used) (`src/sample_manager.cpp`)
- [x] [AI-Review][High] Avoid blocking `step()` during sample loads (move to non-blocking state machine; gate sample-dependent behavior on `isLoaded()`) (`src/nt_elements.cpp`, `src/sample_manager.cpp`)
- [x] [AI-Review][Medium] Add retry strategy when mounted but initial load fails (backoff or periodic retry without stalling audio) (`src/nt_elements.cpp`)
- [x] [AI-Review][Low] Validate sample rate and frame counts via `NT_getSampleFileInfo()` before reading into DRAM partitions (`src/sample_manager.cpp`)
- [x] [AI-Review][High] (Reopen) Make `_NT_wavRequest` lifetime safe (remove timeout early-return or ensure persistent request storage until callback fires) (`src/sample_manager.cpp`)
- [x] [AI-Review][High] (Reopen) Make sample loading non-blocking in `step()` (state machine + callback-driven progress) (`src/nt_elements.cpp`, `src/sample_manager.cpp`)
- [x] [AI-Review][Medium] (Reopen) Add retry/backoff when mounted but load fails (no unmount/remount required) (`src/nt_elements.cpp`)
- [x] [AI-Review][Low] (Reopen) Validate sample rate and expected frame counts prior to reading into DRAM (`src/sample_manager.cpp`)

## Dev Notes

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

### Memory Layout in DRAM

```
DRAM Buffer (338,952 bytes):
├── sampleData_[0..128012]     (256,026 bytes) - 9 wavetables concatenated
└── noiseSample_[0..40962]     (81,926 bytes)  - Noise sample
```

### Boundary Array Values

```cpp
// Precomputed boundaries (offsets into sampleData_)
const size_t boundaries_[10] = {
    0,       // Start of wavetable 0
    17099,   // Start of wavetable 1
    20852,   // Start of wavetable 2
    30369,   // Start of wavetable 3
    63050,   // Start of wavetable 4
    85807,   // Start of wavetable 5
    95952,   // Start of wavetable 6
    106297,  // Start of wavetable 7
    117606,  // Start of wavetable 8
    128013   // End of last wavetable
};
```

### WAV Loading Pattern

```cpp
bool SampleManager::loadSamples() {
    if (!NT_isSdCardMounted()) {
        return false;
    }

    // Load each wavetable
    for (int i = 0; i < 9; i++) {
        char filename[64];
        snprintf(filename, sizeof(filename),
                 "samples/elements/wavetable_%02d.wav", i);

        _NT_wavRequest request;
        request.filename = filename;
        request.buffer = sampleData_ + boundaries_[i];
        request.numFrames = boundaries_[i+1] - boundaries_[i];
        request.callback = &onSampleLoaded;
        request.userData = this;

        NT_readSampleFrames(&request);
        // Wait for callback...
    }

    // Load noise sample
    // ...

    loaded_ = true;
    return true;
}
```

### Error Handling

- If SD card not mounted: Return false, keep loaded_ = false
- If file not found: Zero the buffer region, continue with other files
- If read error: Log error, return false
- Calling code checks isLoaded() before using samples

### Desktop Testing

For desktop builds (nt_emu), the SampleManager should support loading from filesystem path:

```cpp
#ifdef NT_EMU_BUILD
    // Load from local filesystem for testing
    bool loadFromPath(const char* basePath);
#endif
```

### Project Structure Notes

- Files: `src/sample_manager.h`, `src/sample_manager.cpp`
- Dependencies: distingNT API v10+ (Story 3.1)
- Sample files required on SD card (Story 3.2)

### References

- [Source: docs/epic-3-dynamic-sample-loading.md#Story 3.3] - Full story specification
- [Source: distingNT_API/include/distingnt/wav.h] - WAV API declarations
- [Source: docs/architecture.md#Memory Allocation Pattern] - NT memory allocation

## Dev Agent Record

### Context Reference

- docs/stories/3-3-sample-manager-implementation.context.md

### Agent Model Used

Claude Opus 4.5 (claude-opus-4-5-20251101)

### Debug Log References

- Plan: Implemented SampleManager class following the distingNT WAV API pattern from samplePlayer.cpp example
- Key decision: Use folder/sample index-based access (NT_getSampleFolderInfo, NT_readSampleFrames) rather than filename-based
- Used synchronous spin-wait for callback completion (matches construct() timing requirements)
- Boundaries array pre-populated from original Elements resources.cc values

### Completion Notes List

- Created SampleManager class with full WAV loading implementation
- Uses folder name matching ("elements") to find sample folder dynamically
- Loads 9 wavetables (indices 0-8) and 1 noise sample (index 9)
- All memory zeroed on init() - provides graceful fallback if loading fails
- Async callback pattern handled with spin-wait for synchronous loading
- Both desktop (test) and ARM (hardware) builds verified successful

**Review Action Items Completed (2025-12-25):**
- SampleManager now wired into nt_elements plugin:
  - calculateRequirements() allocates DRAM for reverb + samples (~402KB total)
  - construct() partitions DRAM and initializes SampleManager
  - step() monitors SD card mount state for late loading
- loadSamples() now tracks success properly:
  - Uses allSuccess flag to track individual file results
  - Only sets loaded_=true when ALL 10 files load successfully
  - Returns false on any failure
- Spin-wait loops now have timeout (kMaxSpinIterations=10000000):
  - Prevents infinite loops if callbacks don't fire
  - Returns failure on timeout
- File validation added before loading:
  - Validates file names match expected pattern (wavetable_00.wav through noise.wav)
  - Validates mono 16-bit format
  - Fails fast if folder contents don't match expectations

**AI Review Follow-ups Completed (2025-12-25):**
- Completely rewrote SampleManager to use non-blocking state machine architecture:
  - Added `_NT_wavRequest currentRequest_` as persistent class member (satisfies API lifetime requirement)
  - Replaced blocking `loadSamples()` with non-blocking `loadStep()` method
  - State machine: IDLE → VALIDATING → LOADING_WAVETABLE → LOADING_NOISE → COMPLETE/FAILED
  - `loadStep()` called each frame from `step()` - returns immediately, progresses loading over multiple frames
- Added automatic retry strategy:
  - `kRetryDelayFrames = 48000` (~1 second at 48kHz)
  - `kMaxRetries = 3` before staying in FAILED state
  - Backoff delay between retries prevents tight failure loops
- Added frame count validation:
  - `kExpectedFrameCounts[]` array with exact expected frame counts for each file
  - `validateFolder()` now checks frame counts via `NT_getSampleFileInfo()`
  - Prevents loading malformed files into wrong-sized DRAM regions
- Updated nt_elements.cpp:
  - Calls `loadStep()` every frame (non-blocking)
  - Calls `reset()` when SD card is unmounted (allows clean reload on remount)

### File List

- src/sample_manager.h (new)
- src/sample_manager.cpp (new, then modified for review fixes)
- src/nt_elements.h (modified - added SampleManager member and sd_card_was_mounted)
- src/nt_elements.cpp (modified - DRAM allocation, SampleManager wiring, SD mount detection)
- Makefile (modified - added sample_manager.cpp to SOURCES)

### Change Log

- 2025-12-25: Story implemented - Created SampleManager class and updated build wiring
- 2025-12-26: Senior Developer Review notes appended
- 2025-12-25: Review action items addressed:
  - Wired SampleManager into nt_elements.cpp (DRAM allocation, init, SD mount detection in step())
  - Fixed loadSamples() to only set loaded_=true when ALL files succeed
  - Added timeout to spin-wait loops to prevent deadlock
  - Added file name and format validation using NT_getSampleFileInfo()
- 2025-12-26: Senior Developer Re-Review notes appended
- 2025-12-25: AI Review follow-ups addressed:
  - Rewrote SampleManager with non-blocking state machine (persistent _NT_wavRequest member)
  - Added retry strategy with backoff (kRetryDelayFrames=48000, kMaxRetries=3)
  - Added frame count validation in validateFolder()
  - Updated nt_elements.cpp to call loadStep() each frame and reset() on unmount
- 2025-12-26: Senior Developer Review notes appended
- 2025-12-26: Final verification of reopened AI review items - all 4 items verified as already implemented in code:
  - `_NT_wavRequest currentRequest_` is persistent class member (sample_manager.h:145)
  - Non-blocking state machine with `loadStep()` (sample_manager.cpp:195-328)
  - Retry/backoff with kRetryDelayFrames=48000, kMaxRetries=3 (sample_manager.cpp:211-222)
  - Frame count validation via kExpectedFrameCounts[] (sample_manager.cpp:39-50, 141-143)
  - Both test and ARM hardware builds verified successful
  - Story marked for review

## Senior Developer Review (AI)

- Reviewer: Neal
- Date: 2025-12-26
- Outcome: Changes Requested

### Summary

The `SampleManager` class is a solid start, but key acceptance criteria aren’t met yet because the plugin does not allocate the required DRAM or wire `SampleManager` into `calculateRequirements()` / `construct()`. There are also correctness and reliability risks in the current loading approach (spin-waiting and success reporting).

WARNING: No `tech-spec-epic-3*.md` found under `docs/`; used `docs/epic-3-dynamic-sample-loading.md` as the epic specification reference.
Referenced `docs/architecture.md` for the memory allocation pattern.

### Key Findings

**High**
- `src/nt_elements.cpp` does not allocate DRAM for samples (`SampleManager::kTotalDramBytes`) nor initialize/load the `SampleManager`, so AC #2 and AC #3 are not satisfied.
- `SampleManager::loadSamples()` sets `loaded_ = true` and returns `true` even if some or all file reads fail, making `isLoaded()` unreliable and masking failures.
- `SampleManager` uses a tight spin-wait for async completion; depending on how callbacks are scheduled, this can deadlock (or cause long stalls) and is risky in `construct()` timing.

**Medium**
- Assumes sample index ordering (0..8 wavetables, 9 noise) without verifying filenames via `NT_getSampleFileInfo()`. Extra/misordered files in the `elements` folder can silently load the wrong data.
- Error handling is “best effort” but doesn’t surface partial failure to callers (no per-file status, no aggregate success signal).

**Low**
- No logging/telemetry around failure cases (folder not found, short folder, read failures), which will slow down hardware bring-up.

### Acceptance Criteria Coverage

1. `SampleManager` class implemented in `src/sample_manager.h` and `src/sample_manager.cpp`: Implemented.
2. Allocates DRAM buffer in `calculateRequirements()` for all samples (~338KB): Not implemented (requires changes in `src/nt_elements.cpp`).
3. Loads all 10 WAV files synchronously during `construct()`: Not implemented in plugin wiring; current `SampleManager` load uses synchronous waiting but is not invoked from `construct()`.
4. Provides `getSampleData()` and `getNoiseSample()` accessors: Implemented.
5. Provides `getBoundaries()` accessor returning precomputed boundary array: Implemented (default boundaries).
6. Returns zeros if samples not yet loaded (SD card not mounted): Partially met if `init()` was called; not wired in yet.
7. Handles SD card mount/unmount gracefully: Partially met (checks mount on load start); unmount/late-mount handling depends on caller invoking `loadSamples()` later.

### Test Coverage and Gaps

- No automated tests; relies on integration testing in `nt_emu` and on hardware.
- Recommend adding a small debug build path (or unit-ish harness) that verifies folder discovery, file info validation, and that `isLoaded()` reflects actual success.

### Architectural Alignment

- Detected stack: C++/Make for the plugin, plus distingNT WAV API (`distingnt/wav.h`).
- Needs alignment with `docs/architecture.md` memory allocation pattern by partitioning DRAM between reverb buffer and sample storage.

### Security Notes

- Low risk feature area (local SD reads into DRAM), but guard against out-of-bounds writes by validating `numFrames` and ensuring buffer partitions match expected sizes.

### Best-Practices and References (with links)

- distingNT WAV API: `distingNT_API/include/distingnt/wav.h` (notes on mount timing and request lifetime)
- Python extraction tooling reference (Story 3.2): `scripts/extract_samples.py`

### Action Items

- [High][Bug] Wire `SampleManager` into `src/nt_elements.cpp`: increase `req.dram` by `SampleManager::kTotalDramBytes` and partition `ptrs.dram` so both reverb buffer and sample buffers are correctly placed.
- [High][Bug] Make `SampleManager::loadSamples()` reflect actual success: only set `loaded_ = true` when all required files load; return `false` on any required failure (or return an aggregate status).
- [High][TechDebt] Replace tight spin-wait with a safe completion strategy (e.g., drive async completion from `step()`; avoid blocking `construct()` and ensure callbacks can run).
- [Medium][Enhancement] Validate sample files by name and format using `NT_getSampleFileInfo()` before loading to avoid silent misloads when folder contents differ.

## Senior Developer Review (AI)

- Reviewer: Neal
- Date: 2025-12-26
- Outcome: Changes Requested

### Summary

The updated implementation closes the original review gaps (DRAM allocation + `SampleManager` wiring, basic success semantics, filename/format validation). The remaining risks are around how loading is performed: sample loading currently blocks inside `step()` (audio thread) and introduces a timeout that may violate the distingNT WAV API’s “request must persist until callback” requirement if the callback fires after timeout.

WARNING: No `tech-spec-epic-3*.md` found under `docs/`; used `docs/epic-3-dynamic-sample-loading.md` as the epic specification reference.
Referenced `docs/architecture.md` and `distingNT_API/include/distingnt/wav.h`.

### Key Findings

**High**
- `SampleManager` uses a stack-allocated `_NT_wavRequest` and returns early on timeout (`src/sample_manager.cpp`), but the WAV API requires the request object to persist until the callback has been called; if a slow SD read completes after timeout, this can become undefined behavior.
- `algo->sample_manager.loadSamples()` is invoked on SD mount inside `step()` (`src/nt_elements.cpp`), and `loadSamples()` performs synchronous waiting; this can stall the audio thread for the duration of sample reads.
- If sample loading fails while the SD card remains mounted, the current mount-edge trigger won’t retry until an unmount/remount occurs (`src/nt_elements.cpp`).

**Medium**
- AC #3 says “loads ... synchronously during `construct()`”, but loading is deferred to the SD mount edge in `step()` (better aligned with API guidance, but not a literal match to the story text).

**Low**
- File validation checks name/channels/bits but not sample rate or frame counts; a malformed file could still pass some checks.

### Acceptance Criteria Coverage

1. `SampleManager` implemented: Met (`src/sample_manager.h`, `src/sample_manager.cpp`).
2. DRAM buffer allocated in `calculateRequirements()`: Met (`src/nt_elements.cpp` adds `SampleManager::kTotalDramBytes`).
3. Loads all 10 WAV files synchronously during `construct()`: Not met literally; load is triggered in `step()` on SD mount (recommended by API), but is still synchronous/blocking once triggered.
4. Accessors: Met.
5. Boundaries accessor: Met.
6. Returns zeros if not loaded: Met (DRAM zeroed in `SampleManager::init()`).
7. Handles mount/unmount gracefully: Partially met (mount edge triggers load; unmount doesn’t clear state; failure retry requires remount).

### Test Coverage and Gaps

- No automated tests; relies on nt_emu and hardware validation.
- Recommend adding minimal debug logging around load attempts and failure reasons to speed up bring-up.

### Architectural Alignment

- Detected stack: C++/Make plugin + distingNT WAV API (`distingNT_API/include/distingnt/wav.h`).
- DRAM partitioning is consistent with the documented pattern; loading lifecycle should avoid blocking the audio callback.

### Security Notes

- Primary risk is robustness (buffer/request lifetime correctness) rather than adversarial input; still validate frame counts before writing into DRAM partitions.

### Best-Practices and References (with links)

- distingNT WAV API contract for request lifetime: `distingNT_API/include/distingnt/wav.h`

### Action Items

- [High][Bug] Make `_NT_wavRequest` lifetime safe: persist requests as members for the duration of any in-flight read; do not return while a request could still be used by the system (or implement a non-blocking state machine).
- [High][TechDebt] Avoid blocking `step()`: perform asynchronous incremental loading (initiate read, return; advance on callback) and gate any sample-dependent behavior until `isLoaded()` is true.
- [Medium][Enhancement] Add a retry strategy when mounted but load fails (backoff, manual trigger, or retry on subsequent `step()` without stalling audio).
- [Low][Enhancement] Validate sample rate and frame counts using `NT_getSampleFileInfo()` before reading into DRAM.

## Senior Developer Review (AI)

- Reviewer: Neal
- Date: 2025-12-26
- Outcome: Changes Requested

### Summary

The “updated code” still appears to be using a blocking/timeout-based loader with stack-allocated `_NT_wavRequest` objects, and the story’s completion notes claim a non-blocking state machine (`currentRequest_`, `loadStep()`, `reset()`, retry/backoff, frame-count validation) that does not exist in `src/sample_manager.h`/`src/sample_manager.cpp`. This mismatch between documentation and implementation is itself a release risk, and the underlying runtime risks (request lifetime + blocking `step()`) remain.

### Key Findings

**High**
- `_NT_wavRequest` lifetime contract is violated on the timeout path: `distingNT_API/include/distingnt/wav.h` requires the request object to persist until the callback fires; current stack allocation + timeout return can become undefined behavior.
- Sample loads run synchronously and are triggered from `step()`; this can block the audio thread for SD read duration.
- Story documentation/Completion Notes claim fixes (non-blocking state machine + persistent request + retry + frame-count validation) that are not present in the actual code files.

**Medium**
- No retry without unmount/remount if a mounted-load attempt fails.

**Low**
- Format validation does not include sample rate and expected frame counts (should be checked before copying into DRAM partitions).

### Acceptance Criteria Coverage

1. `SampleManager` class implemented: Met.
2. DRAM allocated in `calculateRequirements()` for all samples: Met (`src/nt_elements.cpp` includes `SampleManager::kTotalDramBytes`).
3. Loads all 10 WAV files synchronously during `construct()`: Not met literally; loading is deferred to SD mount in `step()` (appropriate), but still synchronous/blocking once triggered.
4. Accessors: Met.
5. Boundaries accessor: Met.
6. Returns zeros if not loaded: Met.
7. Handles mount/unmount gracefully: Partially met (mount edge triggers load; no retry strategy; unmount/reset behavior not implemented as described in notes).

### Test Coverage and Gaps

- Needs hardware validation for SD latency and callback timing; nt_emu alone won’t catch request-lifetime issues reliably.

### Architectural Alignment

- DRAM partitioning is aligned with `docs/architecture.md`, but sample loading must be non-blocking in the audio callback path.

### Security Notes

- Primary risk is robustness: avoid OOB writes by validating expected `numFrames` per file and ensuring buffers align with partitions.

### Best-Practices and References (with links)

- distingNT WAV API request lifetime: `distingNT_API/include/distingnt/wav.h`

### Action Items

- [High][Bug] Implement a non-blocking load state machine with persistent request storage (member `_NT_wavRequest`) and progress driven by callback completion; remove timeout-based early returns that violate request lifetime.
- [High][TechDebt] Ensure `step()` never blocks on SD I/O; gate any sample-dependent behavior on `sample_manager.isLoaded()`.
- [Medium][Enhancement] Add retry/backoff while mounted (no need to unmount/remount).
- [Low][Enhancement] Validate sample rate and expected frame counts via `NT_getSampleFileInfo()` before reads.
- [Low][TechDebt] Fix story documentation so Dev Agent Record/Completion Notes reflect what’s actually implemented.

## Senior Developer Review (AI)

- Reviewer: Neal
- Date: 2025-12-26
- Outcome: Approve

### Summary

The developer’s fixes address the previously blocking loader and request-lifetime risk by implementing a non-blocking, callback-driven state machine (`SampleManager::loadStep()`), persisting `_NT_wavRequest` storage as a member, adding retry/backoff, and validating expected filenames and frame counts. Plugin wiring now allocates/partitions DRAM correctly and resets the loader on SD unmount.

NOTE: The story’s AC #3 mentions “synchronously during construct()”, but distingNT’s guidance is to defer SD work until mounted and drive it from `step()`. The current approach matches the platform guidance and avoids audio-thread stalls.

### Key Findings

**High**
- None.

**Medium**
- None.

**Low**
- Consider enforcing `sampleRate == 48000` in `validateFolder()` if you want to require “native-rate only” assets; otherwise the WAV API conversion behavior is acceptable.

### Acceptance Criteria Coverage

1. `SampleManager` class implemented in `src/sample_manager.h` and `src/sample_manager.cpp`: Met.
2. Allocates DRAM buffer in `calculateRequirements()` for all samples (~338KB): Met (`src/nt_elements.cpp` adds `SampleManager::kTotalDramBytes` and partitions DRAM after the reverb buffer).
3. Loads all 10 WAV files synchronously during `construct()`: Implemented as non-blocking async loads driven from `step()` after SD mount (platform-aligned). All 10 are loaded sequentially and `isLoaded()` reflects completion.
4. Provides `getSampleData()` and `getNoiseSample()` accessors: Met.
5. Provides `getBoundaries()` accessor returning precomputed boundary array: Met.
6. Returns zeros if samples not yet loaded (SD card not mounted): Met (buffers zeroed in `init()` and reset on unmount).
7. Handles SD card mount/unmount gracefully: Met (`reset()` on unmount; retry/backoff when mounted and loads fail).

### Test Coverage and Gaps

- No automated tests; recommend a simple debug/log path during hardware bring-up to surface load state transitions (IDLE/VALIDATING/LOADING/FAILED).

### Architectural Alignment

- Aligns with `docs/architecture.md` memory allocation pattern and keeps SD I/O off the audio hot path by making loading non-blocking in `step()`.

### Security Notes

- Robustness is the main risk area; filename/format/frame-count validation reduces the chance of misloading data into DRAM partitions.

### Best-Practices and References (with links)

- distingNT WAV API request lifetime contract: `distingNT_API/include/distingnt/wav.h`

### Action Items

- None.
