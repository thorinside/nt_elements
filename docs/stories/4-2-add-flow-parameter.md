# Story 4.2: Add Flow Parameter (Blow Meta)

Status: ready-for-dev

## Story

As a user,
I want access to the Flow parameter,
So that I can control the air flow character of the blow exciter.

## Acceptance Criteria

1. New parameter `kParamBlowFlow` added to parameter adapter
2. Parameter maps to `patch->exciter_blow_meta`
3. Range: 0-100% mapped to 0.0-1.0
4. Parameter added to Exciter page (Page 1)
5. OLED displays "Flow" with percentage value
6. Default value: 50%

## Tasks / Subtasks

- [ ] Task 1: Add parameter enum (AC: 1)
  - [ ] Open `src/parameter_adapter.h` (or equivalent parameter definitions)
  - [ ] Add `kParamBlowFlow` to ParameterIndices enum
  - [ ] Position after existing exciter parameters

- [ ] Task 2: Define parameter in factory (AC: 1, 3, 6)
  - [ ] Add parameter definition in factory `params()` callback
  - [ ] Set range 0-100, default 50
  - [ ] Set display name "Flow"

- [ ] Task 3: Map parameter to patch structure (AC: 2)
  - [ ] In `parameterChanged()` callback, add case for `kParamBlowFlow`
  - [ ] Map to `patch.exciter_blow_meta = value / 100.0f`

- [ ] Task 4: Add to parameter page (AC: 4)
  - [ ] Open `src/parameter_pages.h`
  - [ ] Add Flow to Page 1 (Exciter) controls
  - [ ] Assign to appropriate pot or encoder

- [ ] Task 5: Update OLED display (AC: 5)
  - [ ] Ensure "Flow" label displays correctly
  - [ ] Verify percentage formatting

- [ ] Task 6: Update default patch initialization (AC: 6)
  - [ ] Locate `setDefaultPatch()` function
  - [ ] Verify `exciter_blow_meta` is set to 0.5f (matching 50% default)

## Dev Notes

- `exciter_blow_meta` controls the blow exciter's "meta" parameter
- In original Elements hardware, this is the large FLOW knob
- Currently hardcoded to 0.5f in `setDefaultPatch()`
- From Elements manual: "Air flow of BLOW generator. Wavetable scanning through various colors of noise. CV modulation creates turbulent air flow textures."

### Project Structure Notes

- Files to modify:
  - `src/parameter_adapter.h` - Add parameter enum
  - `src/nt_elements.cpp` - Add parameter handling in factory and callbacks
  - `src/parameter_pages.h` - Add to page layout
  - `src/oled_display.cpp` - Verify label display

### References

- [Source: docs/epic-4-audio-quality-and-parameters.md#Story 4.2]
- [Source: external/mutable-instruments/elements/dsp/patch.h] - Patch structure
- [Source: docs/architecture.md#Parameter Update Pattern]
- Elements Manual: FLOW (D) description

## Dev Agent Record

### Context Reference

- docs/stories/4-2-add-flow-parameter.context.md

### Agent Model Used

### Debug Log References

### Completion Notes List

### File List
