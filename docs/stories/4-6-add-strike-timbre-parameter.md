# Story 4.6: Add Strike Timbre Parameter

Status: ready-for-dev

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

- [ ] Task 1: Audit current Strike Timbre exposure (AC: 1)
  - [ ] Search `src/parameter_adapter.h` for existing strike timbre param
  - [ ] Check if `kParamStrikeTimbre` exists
  - [ ] If not exposed, document the gap

- [ ] Task 2: Add parameter enum if missing (AC: 1, 4)
  - [ ] Add `kParamStrikeTimbre` to ParameterIndices enum
  - [ ] Position with other timbre parameters

- [ ] Task 3: Define parameter in factory (AC: 4, 5)
  - [ ] Add parameter definition in factory `params()` callback
  - [ ] Set range 0-100, default 50
  - [ ] Set display name "Strike Timbre"

- [ ] Task 4: Map parameter to patch structure (AC: 1)
  - [ ] In `parameterChanged()` callback, add case for `kParamStrikeTimbre`
  - [ ] Map to `patch.exciter_strike_timbre = value / 100.0f`

- [ ] Task 5: Add to Timbre page (AC: 2)
  - [ ] Open `src/parameter_pages.h`
  - [ ] Add Strike Timbre to Page 2 (Timbre) - Pot 3
  - [ ] Completes the timbre trio: Bow Timbre, Blow Timbre, Strike Timbre

- [ ] Task 6: Update OLED display (AC: 3)
  - [ ] Add label "StrTim" or "StrikeTim" for display
  - [ ] Verify percentage formatting

- [ ] Task 7: Verify default initialization (AC: 5)
  - [ ] Confirm `exciter_strike_timbre` is set to 0.5f in defaults

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

### Completion Notes List

### File List
