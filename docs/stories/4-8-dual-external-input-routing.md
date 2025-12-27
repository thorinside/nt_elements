# Story 4.8: Dual External Input Routing

Status: ready-for-dev

## Story

As a user,
I want two separate external audio inputs (Blow path and Strike path),
So that I can excite the resonator with external audio through either signal path.

## Acceptance Criteria

1. Two input bus parameters: `kParamBlowInputBus` and `kParamStrikeInputBus`
2. **Blow Input**: Audio routed through diffusion → envelope → STRENGTH VCA → resonator
3. **Strike Input**: Audio routed directly to resonator (unprocessed)
4. Both inputs can be set independently to any NT bus (0=none, 1-28=bus)
5. Existing single input bus parameter removed or repurposed
6. OLED shows input bus assignments on Routing page (NT firmware UI)
7. External audio correctly excites resonator through appropriate path

## Tasks / Subtasks

- [ ] Task 1: Understand current input routing (AC: 1, 5)
  - [ ] Find existing `kParamInputBus` in code
  - [ ] Document how external input currently enters Part::Process()
  - [ ] Understand `blow_in` and `strike_in` buffer parameters

- [ ] Task 2: Add dual input bus parameters (AC: 1)
  - [ ] Add `kParamBlowInputBus` to parameter enum
  - [ ] Add `kParamStrikeInputBus` to parameter enum
  - [ ] Define in factory with bus selection range (0-28)

- [ ] Task 3: Implement Blow input path (AC: 2)
  - [ ] Read audio from selected blow input bus
  - [ ] Copy to `blow_in` buffer for Part::Process()
  - [ ] This path goes through diffusion → envelope → STRENGTH VCA

- [ ] Task 4: Implement Strike input path (AC: 3)
  - [ ] Read audio from selected strike input bus
  - [ ] Copy to `strike_in` buffer for Part::Process()
  - [ ] This path goes directly to resonator (unprocessed)

- [ ] Task 5: Update step() function (AC: 2, 3, 7)
  - [ ] Modify audio buffer preparation before Part::Process()
  - [ ] Handle bus index 0 = no external input (use internal exciter)
  - [ ] Handle bus indices 1-28 = external audio source

- [ ] Task 6: Remove/repurpose old input bus (AC: 5)
  - [ ] Deprecate `kParamInputBus` if exists
  - [ ] Or repurpose as one of the new parameters

- [ ] Task 7: Verify routing UI (AC: 6)
  - [ ] Confirm bus parameters visible in NT firmware UI
  - [ ] Test setting different buses for each input

- [ ] Task 8: Test external audio excitation (AC: 7)
  - [ ] Test blow input with sustained audio (ambient, drone)
  - [ ] Test strike input with percussive audio (drums)
  - [ ] Verify different signal paths produce different results

## Dev Notes

From Elements manual:
- "External IN 1: The audio signal sent to this input goes through the same path as the blowing noise - it gets diffused, amplified by the envelope, then amplified by the STRENGTH VCA."
- "External IN 2: This second audio input is unprocessed and directly sent to the resonator."

Current implementation has single `kParamInputBus` - needs to become two separate buses.

The `Part::Process()` function takes `blow_in` and `strike_in` buffers - these should receive the external audio from the respective input buses.

### Project Structure Notes

- Files to modify:
  - `src/parameter_adapter.h` - Add dual input bus enums
  - `src/nt_elements.cpp` - Add bus parameters, modify step() for dual routing
  - Possibly `src/parameter_pages.h` if adding to custom pages

### References

- [Source: docs/epic-4-audio-quality-and-parameters.md#Story 4.8]
- [Source: docs/epic-4-audio-quality-and-parameters.md#External Input Routing]
- [Source: external/mutable-instruments/elements/dsp/part.h] - Part::Process signature
- [Source: docs/architecture.md#Audio Buffer Access Pattern]
- Prerequisite: Story 4.4 (Strength parameter needed for blow path VCA)

## Dev Agent Record

### Context Reference

- docs/stories/4-8-dual-external-input-routing.context.md

### Agent Model Used

### Debug Log References

### Completion Notes List

### File List
