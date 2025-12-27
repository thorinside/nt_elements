# Story 4.3: Add Mallet Parameter (Strike Meta)

Status: ready-for-dev

## Story

As a user,
I want access to the Mallet parameter,
So that I can control the mallet/strike character of the strike exciter.

## Acceptance Criteria

1. New parameter `kParamStrikeMallet` added to parameter adapter
2. Parameter maps to `patch->exciter_strike_meta`
3. Range: 0-100% mapped to 0.0-1.0
4. Parameter added to Exciter page (Page 1)
5. OLED displays "Mallet" with percentage value
6. Default value: 50%

## Tasks / Subtasks

- [ ] Task 1: Add parameter enum (AC: 1)
  - [ ] Open `src/parameter_adapter.h`
  - [ ] Add `kParamStrikeMallet` to ParameterIndices enum
  - [ ] Position after existing exciter parameters

- [ ] Task 2: Define parameter in factory (AC: 1, 3, 6)
  - [ ] Add parameter definition in factory `params()` callback
  - [ ] Set range 0-100, default 50
  - [ ] Set display name "Mallet"

- [ ] Task 3: Map parameter to patch structure (AC: 2)
  - [ ] In `parameterChanged()` callback, add case for `kParamStrikeMallet`
  - [ ] Map to `patch.exciter_strike_meta = value / 100.0f`

- [ ] Task 4: Add to parameter page (AC: 4)
  - [ ] Open `src/parameter_pages.h`
  - [ ] Add Mallet to Page 1 (Exciter) controls
  - [ ] Assign to appropriate pot or encoder

- [ ] Task 5: Update OLED display (AC: 5)
  - [ ] Ensure "Mallet" label displays correctly
  - [ ] Verify percentage formatting

- [ ] Task 6: Update default patch initialization (AC: 6)
  - [ ] Locate `setDefaultPatch()` function
  - [ ] Verify `exciter_strike_meta` is set to 0.5f (matching 50% default)

## Dev Notes

- `exciter_strike_meta` controls the strike exciter's "meta" parameter
- In original Elements hardware, this is the large MALLET knob
- Currently hardcoded to 0.5f in `setDefaultPatch()`
- From Elements manual: "Type of percussive noise. Morphs through:
  - Actual samples of hammers, mallets, sticks
  - Physical models of mallets (with/without damping)
  - Physical models of plectrums (varying picking speeds)
  - Physical models of particles bouncing on surface"

### Project Structure Notes

- Files to modify:
  - `src/parameter_adapter.h` - Add parameter enum
  - `src/nt_elements.cpp` - Add parameter handling in factory and callbacks
  - `src/parameter_pages.h` - Add to page layout
  - `src/oled_display.cpp` - Verify label display

### References

- [Source: docs/epic-4-audio-quality-and-parameters.md#Story 4.3]
- [Source: external/mutable-instruments/elements/dsp/patch.h] - Patch structure
- [Source: docs/architecture.md#Parameter Update Pattern]
- Elements Manual: MALLET (E) description

## Dev Agent Record

### Context Reference

- docs/stories/4-3-add-mallet-parameter.context.md

### Agent Model Used

### Debug Log References

### Completion Notes List

### File List
