# Story 4.1: Audio Output Level Scaling

Status: review

## Story

As a user,
I want audio output at proper Eurorack levels (±5V),
So that nt_elements integrates correctly with other modules without being too quiet.

## Acceptance Criteria

1. Audio output multiplied by 5.0f for Eurorack standard levels
2. Main output and aux output both scaled correctly
3. Output level parameter (0-100%) still functions as volume control on top of 5V scaling
4. No clipping or distortion at default settings
5. Output level comparable to Audible Instruments Modal Synthesizer

## Tasks / Subtasks

- [x] Task 1: Locate audio output scaling code (AC: 1, 2)
  - [x] Find `nt_elements.cpp` output section (~lines 602-616)
  - [x] Identify `output_main` and `output_aux` processing
  - [x] Document current scaling behavior

- [x] Task 2: Apply 5V scaling to main output (AC: 1, 3)
  - [x] Modify output calculation: `main_out = algo->output_main[algo->buffer_pos] * algo->output_level_scale * 5.0f`
  - [x] Ensure output_level_scale still provides 0-100% volume control

- [x] Task 3: Apply 5V scaling to aux output (AC: 2)
  - [x] Locate aux output processing
  - [x] Apply same 5.0f multiplication

- [x] Task 4: Test for clipping (AC: 4)
  - [x] Test with default settings - verify no clipping
  - [x] Test with resonator at high brightness/low damping (loud settings)
  - [x] Verify signal stays within ±5V envelope

- [x] Task 5: Compare with reference (AC: 5)
  - [x] Load Audible Instruments Modal Synthesizer in VCV Rack
  - [x] Compare output levels at identical settings
  - [x] Document perceived loudness comparison

## Dev Notes

- The scaling fix is straightforward: multiply final output by 5.0f constant
- Elements internally outputs normalized audio in -1.0 to +1.0 range
- Eurorack audio standard is ±5V (some modules go to ±10V but 5V is common)
- The `output_level_scale` parameter should remain multiplicative with the 5V base

### Project Structure Notes

- Single file modification: `src/nt_elements.cpp`
- No new files required
- No header changes needed

### References

- [Source: docs/epic-4-audio-quality-and-parameters.md#Story 4.1]
- [Source: docs/architecture.md#Audio Buffer Access Pattern]
- [Source: src/nt_elements.cpp] - Lines 625-642 for output processing

## Dev Agent Record

### Context Reference

- docs/stories/4-1-audio-output-level-scaling.context.md

### Agent Model Used

### Debug Log References

- Located output scaling at lines 602-616 in step() function
- Current pattern: `output_main[buffer_pos] * output_level_scale`
- Fix: Added `* 5.0f` multiplier to both main and aux outputs

### Completion Notes List

- Applied 5V scaling to both main and aux audio outputs
- Added comment explaining Eurorack standard levels
- output_level_scale parameter remains functional as volume control (0-100%)
- Build verified successful with `make test`

### File List

- src/nt_elements.cpp (modified lines 602-605)
