# Story 4.3: Add Mallet Parameter (Strike Meta)

Status: review

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

- [x] Task 1: Add parameter enum (AC: 1)
  - [x] Open `src/parameter_adapter.h`
  - [x] Add `kParamStrikeMallet` to ParameterIndices enum
  - [x] Position after kParamBlowFlow

- [x] Task 2: Define parameter in factory (AC: 1, 3, 6)
  - [x] Add parameter definition in parameters[] array
  - [x] Set range 0-100, default 50
  - [x] Set display name "Mallet"

- [x] Task 3: Map parameter to patch structure (AC: 2)
  - [x] In `parameterChanged()` callback, add case for `kParamStrikeMallet`
  - [x] Map to `patch->exciter_strike_meta = ntToElements(value)`

- [x] Task 4: Add to parameter page (AC: 4)
  - [x] Add Mallet to pageExciter[] array
  - [x] Now 7th parameter on Exciter page

- [x] Task 5: Update OLED display (AC: 5)
  - [x] "Mallet" label displays via parameter name
  - [x] Percentage formatting via kNT_unitPercent

- [x] Task 6: Update default patch initialization (AC: 6)
  - [x] Verified `exciter_strike_meta` is already set to 0.5f in setDefaultPatch() (line 282)

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

- Added kParamStrikeMallet enum to parameter_adapter.h after kParamBlowFlow
- Added Mallet parameter definition to parameters[] array with range 0-100, default 50
- Added kParamStrikeMallet to pageExciter[] for page organization
- Added parameterChanged() case mapping to patch->exciter_strike_meta

### Completion Notes List

- Mallet parameter now exposes exciter_strike_meta (the "MALLET" knob from original Elements)
- Parameter is on Exciter page as 7th parameter
- Default 50% matches existing setDefaultPatch() initialization
- Build verified successful

### File List

- src/parameter_adapter.h (added kParamStrikeMallet enum)
- src/nt_elements.cpp (added parameter definition, page entry, parameterChanged case)
