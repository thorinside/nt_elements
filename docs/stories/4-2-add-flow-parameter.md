# Story 4.2: Add Flow Parameter (Blow Meta)

Status: review

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

- [x] Task 1: Add parameter enum (AC: 1)
  - [x] Open `src/parameter_adapter.h`
  - [x] Add `kParamBlowFlow` to ParameterIndices enum
  - [x] Position after existing exciter parameters (after kParamBlowTimbre)

- [x] Task 2: Define parameter in factory (AC: 1, 3, 6)
  - [x] Add parameter definition in parameters[] array
  - [x] Set range 0-100, default 50
  - [x] Set display name "Flow"

- [x] Task 3: Map parameter to patch structure (AC: 2)
  - [x] In `parameterChanged()` callback, add case for `kParamBlowFlow`
  - [x] Map to `patch->exciter_blow_meta = ntToElements(value)`

- [x] Task 4: Add to parameter page (AC: 4)
  - [x] Add Flow to pageExciter[] array
  - [x] Now 6th parameter on Exciter page

- [x] Task 5: Update OLED display (AC: 5)
  - [x] "Flow" label displays via parameter name
  - [x] Percentage formatting via kNT_unitPercent

- [x] Task 6: Update default patch initialization (AC: 6)
  - [x] Verified `exciter_blow_meta` is already set to 0.5f in setDefaultPatch() (line 279)

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

- Added kParamBlowFlow enum to parameter_adapter.h after kParamBlowTimbre
- Added Flow parameter definition to parameters[] array with range 0-100, default 50
- Added kParamBlowFlow to pageExciter[] for page organization
- Added parameterChanged() case mapping to patch->exciter_blow_meta

### Completion Notes List

- Flow parameter now exposes exciter_blow_meta (the "FLOW" knob from original Elements)
- Parameter is on Exciter page as 6th parameter
- Default 50% matches existing setDefaultPatch() initialization
- Build verified successful

### File List

- src/parameter_adapter.h (added kParamBlowFlow enum)
- src/nt_elements.cpp (added parameter definition, page entry, parameterChanged case)
