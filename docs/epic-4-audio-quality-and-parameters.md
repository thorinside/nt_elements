# Epic 4: Audio Quality and Missing Parameters

**Author:** Neal (assisted by Bob, Scrum Master)
**Date:** 2025-12-26
**Status:** Draft

---

## Overview

This epic addresses audio quality issues and missing synthesis parameters discovered during testing. Comparison with the Audible Instruments "Modal Synthesizer" VCV Rack module (based on the same Mutable Instruments Elements source code) revealed several gaps in our parameter exposure and audio output scaling.

**Business Value:**
- Proper Eurorack-level audio output (±5V standard)
- Full access to Elements synthesis parameters
- Parity with reference implementations
- Enhanced sound design possibilities
- Proper dual external input routing matching original hardware

**Story Count:** 9 stories

**Technical Context:**
- Elements outputs normalized audio (-1.0 to +1.0 range)
- Eurorack standard is ±5V audio signals
- Several Patch structure fields are not exposed as user parameters
- `exciter_blow_meta` (Flow), `exciter_strike_meta` (Mallet), and `strength` (gate strength) are hardcoded

---

## Prerequisites

- Epic 3 complete (dynamic sample loading working)
- Understanding of Elements Patch structure (`dsp/patch.h`)
- Reference: Audible Instruments Modal Synthesizer for parameter comparison

---

## Story 4.1: Audio Output Level Scaling (5V)

### Story

As a user,
I want audio output at proper Eurorack levels (±5V),
So that nt_elements integrates correctly with other modules without being too quiet.

### Acceptance Criteria

1. Audio output multiplied by 5.0f for Eurorack standard levels
2. Main output and aux output both scaled correctly
3. Output level parameter (0-100%) still functions as volume control on top of 5V scaling
4. No clipping or distortion at default settings
5. Output level comparable to Audible Instruments Modal Synthesizer

### Technical Notes

Current code in `nt_elements.cpp` (lines 625-642):
```cpp
outputChunk[i] = algo->temp_main_out[i] * algo->output_level_scale;
```

Should become:
```cpp
outputChunk[i] = algo->temp_main_out[i] * algo->output_level_scale * 5.0f;
```

### Prerequisites
None (can start immediately)

---

## Story 4.2: Add Flow Parameter (Blow Meta)

### Story

As a user,
I want access to the Flow parameter,
So that I can control the air flow character of the blow exciter.

### Acceptance Criteria

1. New parameter `kParamBlowFlow` added to parameter adapter
2. Parameter maps to `patch->exciter_blow_meta`
3. Range: 0-100% mapped to 0.0-1.0
4. Parameter added to Exciter page (Page 1)
5. OLED displays "Flow" with percentage value
6. Default value: 50%

### Technical Notes

- `exciter_blow_meta` controls the blow exciter's "meta" parameter
- In original Elements hardware, this is the large FLOW knob
- Currently hardcoded to 0.5f in `setDefaultPatch()`

### Prerequisites
None

---

## Story 4.3: Add Mallet Parameter (Strike Meta)

### Story

As a user,
I want access to the Mallet parameter,
So that I can control the mallet/strike character of the strike exciter.

### Acceptance Criteria

1. New parameter `kParamStrikeMallet` added to parameter adapter
2. Parameter maps to `patch->exciter_strike_meta`
3. Range: 0-100% mapped to 0.0-1.0
4. Parameter added to Exciter page (Page 1)
5. OLED displays "Mallet" with percentage value
6. Default value: 50%

### Technical Notes

- `exciter_strike_meta` controls the strike exciter's "meta" parameter
- In original Elements hardware, this is the large MALLET knob
- Currently hardcoded to 0.5f in `setDefaultPatch()`

### Prerequisites
None

---

## Story 4.4: Add Strength Parameter (Gate Strength)

### Story

As a user,
I want access to the Strength parameter,
So that I can control the excitation intensity/velocity.

### Acceptance Criteria

1. New parameter `kParamStrength` added to parameter adapter
2. Parameter maps to `perf_state.strength` in PerformanceState
3. Range: 0-100% mapped to 0.0-1.0
4. Parameter added to Performance page (Page 4)
5. OLED displays "Strength" with percentage value
6. Default value: 50%
7. MIDI velocity can optionally modulate this parameter

### Technical Notes

- `PerformanceState.strength` controls the gate/excitation strength
- In original Elements, this comes from the STRENGTH CV input
- Currently hardcoded to 0.5f in step() function
- Could be modulated by MIDI velocity for expressive playing

### Prerequisites
None

---

## Story 4.5: Reorganize Parameter Pages (Manual-Aligned Layout)

### Story

As a user,
I want parameter pages organized to match the original Elements layout,
So that I can access all sound design controls intuitively.

### Acceptance Criteria

1. **Page 1 (Exciter)**: Contour, Bow, Blow on pots; Strike, Strength on encoders
2. **Page 2 (Timbre)**: Bow Timbre, Blow Timbre, Strike Timbre on pots; Flow, Mallet on encoders
3. **Page 3 (Resonator)**: Geometry, Brightness, Damping on pots; Position, Space on encoders
4. **Page 4 (Tuning)**: Coarse, Fine, FM Amount on pots; Output Level on encoder
5. OLED display shows page names and parameter labels matching manual terminology
6. Contour prominently placed as first pot on Exciter page (matches hardware layout)
7. Routing parameters (buses, MIDI, CV) remain accessible via NT firmware UI

### Technical Notes

This organization mirrors the original Elements hardware panel layout:
- Contour affects BOW and BLOW envelope shapes
- Exciter levels (Bow/Blow/Strike) are the main mixing controls
- Timbre parameters shape the character of each exciter type
- Flow and Mallet are the "meta" parameters for blow and strike
- Resonator page groups all modal filter bank controls
- Space combines stereo width and reverb (at max + high CV = freeze)

Current page structure needs reorganization from 5 pages to 4 custom pages in `parameter_pages.h`.

### Prerequisites
Stories 4.2, 4.3, 4.4

---

## Story 4.6: Add Strike Timbre Parameter

### Story

As a user,
I want access to the Strike Timbre parameter,
So that I can control the brightness/speed of the percussive excitation.

### Acceptance Criteria

1. Verify `exciter_strike_timbre` is exposed as parameter
2. Parameter added to Timbre page (Page 2)
3. OLED displays "StrTim" or similar abbreviation
4. Range: 0-100% mapped to 0.0-1.0
5. Default value: 50%

### Technical Notes

- Looking at current code, Strike Timbre may not be exposed
- `exciter_strike_timbre` is set to 0.5f in defaults but not user-controllable
- This completes the set of timbre parameters for all three exciters

### Prerequisites
Story 4.5

---

## Story 4.8: Dual External Input Routing

### Story

As a user,
I want two separate external audio inputs (Blow path and Strike path),
So that I can excite the resonator with external audio through either signal path.

### Acceptance Criteria

1. Two input bus parameters: `kParamBlowInputBus` and `kParamStrikeInputBus`
2. **Blow Input**: Audio routed through diffusion → envelope → STRENGTH VCA → resonator
3. **Strike Input**: Audio routed directly to resonator (unprocessed)
4. Both inputs can be set independently to any NT bus (0=none, 1-28=bus)
5. Existing single input bus parameter removed or repurposed
6. OLED shows input bus assignments on Routing page (NT firmware UI)
7. External audio correctly excites resonator through appropriate path

### Technical Notes

From Elements manual:
- "External IN 1: The audio signal sent to this input goes through the same path as the blowing noise - it gets diffused, amplified by the envelope, then amplified by the STRENGTH VCA."
- "External IN 2: This second audio input is unprocessed and directly sent to the resonator."

Current implementation has single `kParamInputBus` - needs to become two separate buses.

The `Part::Process()` function takes `blow_in` and `strike_in` buffers - these should receive the external audio from the respective input buses.

### Prerequisites
Stories 4.4 (Strength parameter needed for blow path VCA)

---

## Story 4.9: Add Emilie Gillet Attribution

### Story

As a user,
I want to see "By Emilie Gillet" displayed on the OLED screen,
So that the original creator of Elements receives proper credit.

### Acceptance Criteria

1. Text "By Emilie Gillet" displayed below the main title text
2. Uses small/subtle font size (not competing with page title)
3. Visible on all parameter pages
4. Properly positioned and readable

### Technical Notes

Emilie Gillet is the creator of Mutable Instruments and designed the original Elements module. This attribution honors her exceptional work in synthesis algorithm design.

Update `oled_display.cpp` to add the attribution line in `renderDisplay()`.

### Prerequisites
None

---

## Story 4.7: Validation and A/B Testing

### Story

As a developer,
I want to validate all parameter and audio changes against the reference implementation,
So that nt_elements sounds and behaves correctly.

### Acceptance Criteria

1. A/B comparison with Audible Instruments Modal Synthesizer
2. Audio output levels match (both at similar perceived loudness)
3. All parameters function identically to reference
4. Flow, Mallet, Strength produce expected timbral changes
5. No regressions in existing functionality
6. CPU usage remains within acceptable bounds

### Prerequisites
All previous Epic 4 stories (4.1-4.6, 4.8-4.9)

---

## Implementation Notes

### Parameter Budget

Current parameter count: 28 (including system params)
New parameters: 3 (Flow, Mallet, Strength)
Total after Epic 4: 31 parameters

NT supports ample parameters, so no concern about limits.

### Page Organization (Aligned with Elements Manual)

Based on the official Elements manual, the parameter organization should be:

**Page 1: Exciter** (Controls excitation signal generation)
- Pots: Contour, Bow, Blow
- Encoders: Strike, Strength

**Page 2: Timbre** (Controls exciter character/color)
- Pots: Bow Timbre, Blow Timbre, Strike Timbre
- Encoders: Flow, Mallet

**Page 3: Resonator** (Controls modal filter bank)
- Pots: Geometry, Brightness, Damping
- Encoders: Position, Space

**Page 4: Tuning** (Controls pitch and modulation)
- Pots: Coarse, Fine, FM Amount
- Encoders: Output Level, (reserved)

*Note: Routing/System configuration (buses, MIDI, CV) handled by NT firmware UI, not custom pages.*

### Manual Parameter Descriptions

From the Elements manual:

**CONTOUR (A)**: Envelope applied to BOW and BLOW exciters. Interpolates between short AD → slower ADSR → faster AR envelopes.

**BOW/BLOW/STRIKE (B)**: Exciter mixer levels. BOW simulates bow pressure (brighter/noisier at high). BLOW controls granular blowing noise. STRIKE controls percussive noise (bleeds to output past 2 o'clock).

**FLOW (D)**: Air flow of BLOW generator. "Wavetable scanning through various colors of noise." CV modulation creates turbulent air flow textures.

**MALLET (E)**: Type of percussive noise from STRIKE generator. Morphs through:
- Actual samples of hammers, mallets, sticks
- Physical models of mallets (with/without damping)
- Physical models of plectrums (varying picking speeds)
- Physical models of particles bouncing on surface

**TIMBRE (F)**:
- BOW: smoothness/granularity of bow material
- BLOW: pitch/granulation rate of noise
- STRIKE: brightness/speed of percussive excitation

**STRENGTH (4)**: VCA for excitation signal. +/- 15dB range at 3dB/V scale.

**GEOMETRY (C)**: Geometry and stiffness of resonating structure. Plates → strings → bars/tubes → bells/bowls.

**BRIGHTNESS (D)**: How muted high-frequency modes are. Low = wood/nylon. High = glass/steel.

**DAMPING (E)**: How quickly energy dissipates. CV modulation recreates hand-damping/muting.

**POSITION (F)**: Where excitation is applied on string/surface. Middle = even harmonics cancel (hollow, square-wave-like).

**SPACE (G)**: Stereo width and reverb. Min = exciter/resonator on separate outputs. Increases stereo angle, then adds reverb. Max + high CV = freeze effect.

### External Input Routing

From the manual, Elements has two external inputs with different signal paths:

- **External IN 1 (Blow path)**: Audio goes through diffusion → envelope → STRENGTH VCA → resonator. Same processing as the BLOW generator.
- **External IN 2 (Strike path)**: Audio is unprocessed, sent directly to resonator. Suitable for percussive/transient external excitation.

This allows using external audio sources to excite the resonator through either the "blown" or "struck" signal paths. Story 4.8 addresses implementing this dual-input routing.

### Files to Modify

- `src/parameter_adapter.h` - Add new parameter enums (Flow, Mallet, Strength)
- `src/nt_elements.cpp` - Add parameter handling, fix 5V output scaling
- `src/parameter_pages.h` - Reorganize to 4-page layout per above
- `src/oled_display.cpp` - Update parameter name labels

---

## References

- [Elements Patch Structure](../external/mutable-instruments/elements/dsp/patch.h)
- [Audible Instruments Source](https://github.com/VCVRack/AudibleInstruments)
- [Elements Hardware Manual](https://mutable-instruments.net/modules/elements/manual/)
