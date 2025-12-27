# Story 4.4: Add Strength Parameter (Gate Strength)

Status: ready-for-dev

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

- [ ] Task 1: Add parameter enum (AC: 1)
  - [ ] Open `src/parameter_adapter.h`
  - [ ] Add `kParamStrength` to ParameterIndices enum
  - [ ] Position with performance parameters

- [ ] Task 2: Define parameter in factory (AC: 1, 3, 6)
  - [ ] Add parameter definition in factory `params()` callback
  - [ ] Set range 0-100, default 50
  - [ ] Set display name "Strength"

- [ ] Task 3: Map parameter to performance state (AC: 2)
  - [ ] Locate where `PerformanceState` is populated before `Part::Process()`
  - [ ] In `parameterChanged()` callback, store strength value
  - [ ] Map to `perf_state.strength = value / 100.0f` in step()

- [ ] Task 4: Add to parameter page (AC: 4)
  - [ ] Open `src/parameter_pages.h`
  - [ ] Add Strength to Page 4 (Performance) controls
  - [ ] Assign to encoder (based on Epic 4 layout)

- [ ] Task 5: Update OLED display (AC: 5)
  - [ ] Ensure "Strength" or "Str" label displays correctly
  - [ ] Verify percentage formatting

- [ ] Task 6: Implement MIDI velocity modulation (AC: 7)
  - [ ] In `midiMessage()` callback, capture velocity (0-127)
  - [ ] Optional: Blend velocity with parameter value
  - [ ] Consider making velocity influence configurable or always-on

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

### Completion Notes List

### File List
