# Story 4.6: Add Strike Timbre Parameter

Status: review

## Story

As a user,
I want access to the Strike Timbre parameter,
So that I can control the brightness/speed of the percussive excitation.

## Acceptance Criteria

1. Verify `exciter_strike_timbre` is exposed as parameter
2. Parameter added to Timbre page (Page 2)
3. OLED displays "StrTim" or similar abbreviation
4. Range: 0-100% mapped to 0.0-1.0
5. Default value: 50%

## Tasks / Subtasks

- [x] Task 1: Audit current Strike Timbre exposure (AC: 1)
  - [x] Searched `src/parameter_adapter.h` - kParamStrikeTimbre did NOT exist
  - [x] Only setDefaultPatch() set exciter_strike_timbre to 0.5f
  - [x] Documented gap and proceeding to add it

- [x] Task 2: Add parameter enum if missing (AC: 1, 4)
  - [x] Added `kParamStrikeTimbre` to ParameterIndices enum
  - [x] Position after kParamBlowTimbre with other timbre parameters

- [x] Task 3: Define parameter in factory (AC: 4, 5)
  - [x] Added parameter definition in parameters[] array
  - [x] Set range 0-100, default 50
  - [x] Set display name "Str Timbre" (abbreviated for display)

- [x] Task 4: Map parameter to patch structure (AC: 1)
  - [x] Added case for `kParamStrikeTimbre` in `parameterChanged()`
  - [x] Maps to `patch->exciter_strike_timbre = ntToElements(value)`

- [x] Task 5: Add to Timbre page (AC: 2)
  - [x] Updated `src/parameter_pages.h`
  - [x] Added Strike Timbre to Page 2 (Timbre) - Pot 3
  - [x] Completes timbre trio: Bow Timbre, Blow Timbre, Strike Timbre

- [x] Task 6: Update OLED display (AC: 3)
  - [x] "Str Timbre" label displays via parameter name
  - [x] Percentage formatting via kNT_unitPercent

- [x] Task 7: Verify default initialization (AC: 5)
  - [x] Confirmed `exciter_strike_timbre` is set to 0.5f in setDefaultPatch() (line 289)

## Dev Notes

- Looking at current code, Strike Timbre may not be exposed as user parameter
- `exciter_strike_timbre` is set to 0.5f in defaults but possibly not user-controllable
- This completes the set of timbre parameters for all three exciters
- From Elements manual for TIMBRE (F):
  - BOW: smoothness/granularity of bow material
  - BLOW: pitch/granulation rate of noise
  - STRIKE: brightness/speed of percussive excitation

### Project Structure Notes

- Files to modify:
  - `src/parameter_adapter.h` - Add/verify parameter enum
  - `src/nt_elements.cpp` - Add parameter handling
  - `src/parameter_pages.h` - Add to Page 2 layout
  - `src/oled_display.cpp` - Add label

### References

- [Source: docs/epic-4-audio-quality-and-parameters.md#Story 4.6]
- [Source: external/mutable-instruments/elements/dsp/patch.h] - Patch structure
- Elements Manual: TIMBRE (F) description
- Prerequisite: Story 4.5 (page reorganization must be done first)

## Dev Agent Record

### Context Reference

- docs/stories/4-6-add-strike-timbre-parameter.context.md

### Agent Model Used

### Debug Log References

- Added kParamStrikeTimbre enum to parameter_adapter.h after kParamBlowTimbre
- Added "Str Timbre" parameter definition to parameters[] array
- Added pageExciter entry for kParamStrikeTimbre
- Added parameterChanged() case mapping to patch->exciter_strike_timbre
- Updated page_timbre POT_MAPPING to use kParamStrikeTimbre

### Completion Notes List

- Strike Timbre now exposed as controllable parameter
- Completes the trio: Bow Timbre, Blow Timbre, Strike Timbre
- On Page 2 (Timbre) as Pot 3
- Default 50% matches existing setDefaultPatch() value
- Build verified successful

### File List

- src/parameter_adapter.h (added kParamStrikeTimbre enum)
- src/nt_elements.cpp (added parameter definition, pageExciter entry, parameterChanged case)
- src/parameter_pages.h (updated page_timbre POT_MAPPING)
