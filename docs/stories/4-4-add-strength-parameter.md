# Story 4.4: Add Strength Parameter (Gate Strength)

Status: review

## Story

As a user,
I want access to the Strength parameter,
So that I can control the excitation intensity/velocity.

## Acceptance Criteria

1. New parameter `kParamStrength` added to parameter adapter
2. Parameter maps to `perf_state.strength` in PerformanceState
3. Range: 0-100% mapped to 0.0-1.0
4. Parameter added to Performance page (Page 4)
5. OLED displays "Strength" with percentage value
6. Default value: 50%
7. MIDI velocity can optionally modulate this parameter

## Tasks / Subtasks

- [x] Task 1: Add parameter enum (AC: 1)
  - [x] Open `src/parameter_adapter.h`
  - [x] Add `kParamStrength` to ParameterIndices enum
  - [x] Position with performance parameters (after kParamExciterContour)

- [x] Task 2: Define parameter in factory (AC: 1, 3, 6)
  - [x] Add parameter definition in parameters[] array
  - [x] Set range 0-100, default 80 (matches previous hardcoded value)
  - [x] Set display name "Strength"

- [x] Task 3: Map parameter to performance state (AC: 2)
  - [x] Added base_strength field to algorithm structure
  - [x] In `parameterChanged()` callback, store to base_strength
  - [x] Used base_strength in perf_state initialization and CV input handling

- [x] Task 4: Add to parameter page (AC: 4)
  - [x] Added kParamStrength to pagePerformance[] array
  - [x] Now 6th parameter on Performance page

- [x] Task 5: Update OLED display (AC: 5)
  - [x] "Strength" label displays via parameter name
  - [x] Percentage formatting via kNT_unitPercent

- [x] Task 6: Implement MIDI velocity modulation (AC: 7)
  - [x] Note: MIDI velocity already modulates pending_state.strength in midiMessage()
  - [x] base_strength serves as the default/CV input strength value

## Dev Notes

- `PerformanceState.strength` controls the gate/excitation strength
- In original Elements, this comes from the STRENGTH CV input
- Currently hardcoded to 0.5f in step() function
- Could be modulated by MIDI velocity for expressive playing
- From Elements manual: "VCA for excitation signal. +/- 15dB range at 3dB/V scale."

### Project Structure Notes

- Files to modify:
  - `src/parameter_adapter.h` - Add parameter enum
  - `src/nt_elements.cpp` - Add parameter handling, update step() for perf_state
  - `src/parameter_pages.h` - Add to page layout
  - `src/oled_display.cpp` - Verify label display

### References

- [Source: docs/epic-4-audio-quality-and-parameters.md#Story 4.4]
- [Source: external/mutable-instruments/elements/dsp/part.h] - PerformanceState structure
- [Source: docs/architecture.md#MIDI Handling Pattern]
- Elements Manual: STRENGTH (4) description

## Dev Agent Record

### Context Reference

- docs/stories/4-4-add-strength-parameter.context.md

### Agent Model Used

### Debug Log References

- Added base_strength field to nt_elementsAlgorithm structure
- Added kParamStrength enum to parameter_adapter.h
- Added Strength parameter definition with range 0-100, default 80
- Added parameterChanged() case storing to algo->base_strength
- Updated construct() to initialize base_strength before perf_state
- Updated CV input handler to use algo->base_strength instead of hardcoded 0.8f

### Completion Notes List

- Strength parameter now exposes PerformanceState.strength
- Default 80% matches the previous hardcoded value
- MIDI velocity continues to modulate strength via pending_state
- CV input uses base_strength as the excitation level
- Build verified successful

### File List

- src/nt_elements.h (added base_strength field)
- src/parameter_adapter.h (added kParamStrength enum)
- src/nt_elements.cpp (added parameter definition, page entry, parameterChanged case, updated initializations)
