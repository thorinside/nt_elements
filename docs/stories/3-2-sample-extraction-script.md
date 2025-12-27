# Story 3.2: Sample Extraction Script

Status: done

## Story

As a developer,
I want a script that extracts the static sample arrays into WAV files,
So that samples can be loaded from SD card at runtime.

## Acceptance Criteria

1. Python script reads `resources.cc` or `resources_minimal_wavetables.cc`
2. Extracts `smp_sample_data` into 9 separate WAV files (one per wavetable region)
3. Extracts `smp_noise_sample` into 1 WAV file
4. Output files: `samples/elements/wavetable_00.wav` through `wavetable_08.wav` plus `noise.wav`
5. WAV format: mono, 16-bit PCM, 48kHz (matching disting NT native rate)
6. Script is idempotent (skips extraction if files exist)
7. `samples/elements/` added to `.gitignore`
8. Makefile target `extract-samples` added

## Tasks / Subtasks

- [x] Create Python extraction script (AC: #1, #2, #3)
  - [x] Create `scripts/extract_samples.py`
  - [x] Parse `smp_boundaries[]` array to identify wavetable regions
  - [x] Read int16_t values from `smp_sample_data[]` array
  - [x] Read int16_t values from `smp_noise_sample[]` array
  - [x] Handle C++ array syntax parsing

- [x] Implement WAV file writing (AC: #4, #5)
  - [x] Write each wavetable region as separate WAV file
  - [x] Configure WAV header: mono, 16-bit PCM, 48kHz
  - [x] Output to `samples/elements/wavetable_NN.wav`
  - [x] Write noise sample to `samples/elements/noise.wav`
  - [x] Create output directory if doesn't exist

- [x] Add idempotency check (AC: #6)
  - [x] Check if output files already exist
  - [x] Skip extraction if all files present
  - [x] Add `--force` flag to override existing files
  - [x] Print informative messages about skipped/created files

- [x] Update project configuration (AC: #7, #8)
  - [x] Add `samples/elements/` to `.gitignore`
  - [x] Add Makefile target `extract-samples`
  - [x] Make extraction conditional on file existence

- [x] Document sample format (AC: #4, #5)
  - [x] Create `samples/elements/README.md`
  - [x] Document wavetable boundaries and sample counts
  - [x] Explain WAV format specifications
  - [x] Note SD card folder structure requirements

## Dev Notes

### Data Mapping

| Source Array Region | Boundary Start | Boundary End | Samples | Output File |
|---------------------|----------------|--------------|---------|-------------|
| smp_sample_data[0..17098] | 0 | 17,099 | 17,099 | wavetable_00.wav |
| smp_sample_data[17099..20851] | 17,099 | 20,852 | 3,753 | wavetable_01.wav |
| smp_sample_data[20852..30368] | 20,852 | 30,369 | 9,517 | wavetable_02.wav |
| smp_sample_data[30369..63049] | 30,369 | 63,050 | 32,681 | wavetable_03.wav |
| smp_sample_data[63050..85806] | 63,050 | 85,807 | 22,757 | wavetable_04.wav |
| smp_sample_data[85807..95951] | 85,807 | 95,952 | 10,145 | wavetable_05.wav |
| smp_sample_data[95952..106296] | 95,952 | 106,297 | 10,345 | wavetable_06.wav |
| smp_sample_data[106297..117605] | 106,297 | 117,606 | 11,309 | wavetable_07.wav |
| smp_sample_data[117606..128012] | 117,606 | 128,013 | 10,407 | wavetable_08.wav |
| smp_noise_sample[0..40962] | 0 | 40,963 | 40,963 | noise.wav |

### Source File Location

The sample data is in:
- `src/resources_minimal_wavetables.cc` (project file with reduced wavetables)
- OR `external/mutable-instruments/elements/resources/resources.cc` (original full data)

### WAV File Format

```
RIFF/WAVE format:
- Sample rate: 48000 Hz (matches disting NT)
- Channels: 1 (mono)
- Bits per sample: 16
- Data format: signed 16-bit PCM (little-endian)
```

### Python Implementation Approach

```python
# Pseudocode for extraction
import struct
import wave
import os
import re

def parse_cpp_array(content, array_name):
    """Extract int16_t values from C++ array definition."""
    pattern = rf'{array_name}\[\]\s*=\s*\{{([^}}]+)\}}'
    match = re.search(pattern, content, re.DOTALL)
    values = [int(v.strip()) for v in match.group(1).split(',') if v.strip()]
    return values

def write_wav(filename, samples, sample_rate=48000):
    """Write samples to WAV file."""
    with wave.open(filename, 'w') as wav:
        wav.setnchannels(1)
        wav.setsampwidth(2)  # 16-bit
        wav.setframerate(sample_rate)
        wav.writeframes(struct.pack(f'<{len(samples)}h', *samples))
```

### SD Card Folder Structure

```
SD Card Root/
└── samples/
    └── elements/
        ├── wavetable_00.wav
        ├── wavetable_01.wav
        ├── wavetable_02.wav
        ├── wavetable_03.wav
        ├── wavetable_04.wav
        ├── wavetable_05.wav
        ├── wavetable_06.wav
        ├── wavetable_07.wav
        ├── wavetable_08.wav
        └── noise.wav
```

### Makefile Target

```makefile
extract-samples:
	@if [ ! -f samples/elements/wavetable_00.wav ]; then \
		python3 scripts/extract_samples.py; \
	else \
		echo "Samples already extracted"; \
	fi
```

### Project Structure Notes

- Script location: `scripts/extract_samples.py`
- Output location: `samples/elements/`
- This story can run in parallel with Story 3.1

### References

- [Source: docs/epic-3-dynamic-sample-loading.md#Story 3.2] - Full story specification
- [Source: src/resources_minimal_wavetables.cc] - Project sample data
- [Source: external/mutable-instruments/elements/resources/resources.cc] - Original sample data

## Dev Agent Record

### Context Reference

- docs/stories/3-2-sample-extraction-script.context.md

### Agent Model Used

Claude Opus 4.5 (claude-opus-4-5-20251101)

### Debug Log References

- Analyzed `external/mutable-instruments/elements/resources.cc` to understand C++ array syntax and `smp_boundaries` structure
- Found 10 boundary values defining 9 wavetable regions (128,013 total samples)
- Found 40,963 noise samples in `smp_noise_sample` array
- Verified WAV output format with `file` command - all 10 files correctly formatted as 16-bit PCM mono 48kHz

### Completion Notes List

- Created `scripts/extract_samples.py` - Full-featured extraction script with:
  - Robust C++ array parsing (handles comments, multi-line arrays)
  - WAV file writing using Python standard library (wave, struct)
  - Idempotency check with --force override
  - Configurable source/output paths via CLI arguments
- Updated `.gitignore` to exclude `samples/elements/` directory
- Added `make extract-samples` target to Makefile with conditional extraction
- Created `samples/elements/README.md` documenting file format, boundaries, and SD card structure
- All 10 WAV files verified: 9 wavetables + 1 noise sample

### File List

- scripts/extract_samples.py (new)
- samples/elements/README.md (new)
- samples/elements/wavetable_00.wav through wavetable_08.wav (generated, gitignored)
- samples/elements/noise.wav (generated, gitignored)
- .gitignore (modified - added samples/elements/)
- Makefile (modified - added extract-samples target)

### Change Log

- 2025-12-25: Story implemented - Created sample extraction script and supporting infrastructure
- 2025-12-26: Senior Developer Review notes appended

## Senior Developer Review (AI)

- Reviewer: Neal
- Date: 2025-12-26
- Outcome: Approve

### Summary

The extraction script and supporting docs/build wiring meet the story’s requirements. WAV outputs are correctly generated as 16-bit PCM mono @ 48kHz, the workflow is idempotent, and generated assets are appropriately gitignored.
Reviewed changed files: `scripts/extract_samples.py`, `Makefile`, `.gitignore`, and `samples/elements/README.md`; validated sample outputs via `file`.
WARNING: No `tech-spec-epic-3*.md` found under `docs/`; used `docs/epic-3-dynamic-sample-loading.md` as the epic specification reference.

### Key Findings

**High**
- None.

**Medium**
- None.

**Low**
- None.

### Acceptance Criteria Coverage

1. Script reads `resources.cc` or `resources_minimal_wavetables.cc`: `scripts/extract_samples.py` supports `--source` and defaults to `external/mutable-instruments/elements/resources.cc`.
2. Extracts `smp_sample_data` into 9 WAV files: uses `smp_boundaries` to slice 9 regions and writes `wavetable_00.wav`–`wavetable_08.wav`.
3. Extracts `smp_noise_sample` into 1 WAV file: writes `samples/elements/noise.wav`.
4. Output filenames and location: `samples/elements/wavetable_00.wav`–`wavetable_08.wav` plus `noise.wav`.
5. WAV format: verified locally via `file` as 16-bit PCM mono 48000 Hz.
6. Idempotency: skips when all outputs already exist; supports `--force` for regeneration.
7. `.gitignore`: `samples/elements/` is ignored.
8. Makefile target: `extract-samples` target added.

### Test Coverage and Gaps

- Manual verification performed (per completion notes): regenerated outputs and validated WAV headers via `file`.
- No automated tests for the script; this is acceptable for a tooling-only story, but a future small smoke test could be added if desired.

### Architectural Alignment

- Fits the repo’s build/tooling shape (`scripts/` + `make extract-samples`) without impacting plugin runtime code paths.
- Uses only Python standard library modules (`argparse`, `wave`, `struct`, `pathlib`).
- Detected stack: C++/Make for the plugin, plus Python 3 tooling for extraction.
- Referenced `docs/architecture.md`; no architectural conflicts for this tooling-only story.

### Security Notes

- Low risk: local file parsing + local file generation, no network operations.
- Primary risk class is “silent mismatch” (if source arrays differ unexpectedly); current behavior warns on boundary mismatch but proceeds.

### Best-Practices and References (with links)

- Python `wave` module: https://docs.python.org/3/library/wave.html
- Python `struct` module: https://docs.python.org/3/library/struct.html
- Python `argparse` module: https://docs.python.org/3/library/argparse.html

### Action Items

- None.
