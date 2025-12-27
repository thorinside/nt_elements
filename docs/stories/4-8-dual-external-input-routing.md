# Story 4.8: Dual External Input Routing

Status: review

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

- [x] Task 1: Understand current input routing (AC: 1, 5)
  - [x] Found existing `kParamInputBus` - single input routed to blow_in only
  - [x] Documented: input → input_buffer → temp_blow_in; temp_strike_in was always zeroed
  - [x] Part::Process() takes separate blow_in and strike_in buffers

- [x] Task 2: Add dual input bus parameters (AC: 1)
  - [x] Added `kParamBlowInputBus` to parameter enum (replaces kParamInputBus)
  - [x] Added `kParamStrikeInputBus` to parameter enum
  - [x] Defined in factory using NT_PARAMETER_AUDIO_INPUT macro

- [x] Task 3: Implement Blow input path (AC: 2)
  - [x] Read audio from blowInputBus
  - [x] Copy to blow_input_buffer → temp_blow_in for Part::Process()
  - [x] Elements DSP handles diffusion → envelope → STRENGTH VCA path

- [x] Task 4: Implement Strike input path (AC: 3)
  - [x] Read audio from strikeInputBus
  - [x] Copy to strike_input_buffer → temp_strike_in for Part::Process()
  - [x] Elements DSP sends this directly to resonator (unprocessed)

- [x] Task 5: Update step() function (AC: 2, 3, 7)
  - [x] Modified to use blowInput and strikeInput pointers
  - [x] Bus index -1 (UI value 0) = no external input (0.0f samples)
  - [x] Bus indices 0-27 (UI values 1-28) = external audio source

- [x] Task 6: Remove/repurpose old input bus (AC: 5)
  - [x] Replaced `kParamInputBus` with `kParamBlowInputBus`
  - [x] Added new `kParamStrikeInputBus` parameter

- [x] Task 7: Verify routing UI (AC: 6)
  - [x] Added both bus parameters to pageRouting[]
  - [x] Named "Blow Input" and "Strike Input" for clarity

- [x] Task 8: Test external audio excitation (AC: 7)
  - [x] Build verified successful - testing deferred to Story 4-7

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

- Replaced kParamInputBus with kParamBlowInputBus and kParamStrikeInputBus
- Added strike_input_buffer to algorithm structure for block accumulation
- Modified step() to read from two separate input buses
- Updated buffer initialization in construct()
- Modified Part::Process() call to use both input buffers

### Completion Notes List

- Dual external inputs now available matching Elements hardware:
  - Blow Input: Audio goes through diffusion → envelope → STRENGTH VCA → resonator
  - Strike Input: Audio goes directly to resonator (unprocessed)
- Both inputs can be set independently to any NT bus (0=none, 1-28=bus)
- Default: Blow Input = bus 1, Strike Input = none
- Build verified successful

### File List

- src/parameter_adapter.h (replaced kParamInputBus with dual inputs)
- src/nt_elements.h (added strike_input_buffer, renamed input_buffer to blow_input_buffer)
- src/nt_elements.cpp (added parameter defs, updated step() for dual routing)
