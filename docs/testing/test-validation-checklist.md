# nt_elements Desktop Testing Validation Checklist (Story 1.9)

## Purpose

This document provides a systematic checklist for validating all Epic 1 functionality in VCV Rack's nt_emu environment before hardware deployment. Each feature from Stories 1.1-1.8 is tested with defined pass/fail criteria.

## Test Environment Setup

### Required Software
- VCV Rack v2.x
- nt_emu module (disting NT emulator)
- nt_elements plugin (.dylib for macOS, .so for Linux)

### Required VCV Rack Modules
- **MIDI-CV**: MIDI input conversion
- **Scope**: Audio visualization
- **Spectrum Analyzer**: Frequency measurement
- **VCO**: Audio source for bus routing tests
- **VCV Audio**: Audio output module

### VCV Rack Patch Configuration

```
Signal Flow:
┌──────────┐
│ MIDI-CV  │──(V/Oct, Gate)──┐
└──────────┘                 │
                             ▼
┌──────────┐              ┌──────────┐
│   VCO    │──(Audio)────►│ nt_emu   │──(L/R)──┐
└──────────┘              │          │         │
                          └──────────┘         │
                             │                 │
                             ▼                 ▼
                          ┌──────────┐    ┌──────────┐
                          │  Scope   │    │  Audio   │
                          └──────────┘    │  Output  │
                             │            └──────────┘
                             ▼
                          ┌──────────┐
                          │ Spectrum │
                          │ Analyzer │
                          └──────────┘
```

**Configuration Details:**
1. **MIDI-CV** → nt_emu MIDI input (for note-on, note-off, pitch bend)
2. **VCO** → nt_emu audio input (for bus routing and external excitation tests)
3. **nt_emu output** → VCV Audio output (for listening)
4. **nt_emu output** → Scope (for visual monitoring of waveform)
5. **nt_emu output** → Spectrum Analyzer (for frequency measurement)

### Build Verification

Before testing, verify the plugin builds correctly:

- [ ] `make test` produces `build/test/libnt_elements.dylib` (macOS) or `.so` (Linux)
- [ ] `make` produces `build/nt_elements.o` (hardware target)
- [ ] No compilation errors or warnings
- [ ] Plugin file size reasonable (~100-500KB expected)

---

## Story 1.1-1.2: Development Environment and Source Integration

**Goal:** Verify build system and dependencies are correctly configured.

### Test Procedure

1. Clean build from scratch
2. Verify submodules initialized
3. Build desktop target
4. Build hardware target
5. Check build outputs

### Pass/Fail Criteria

| Criterion | Pass Condition | Status |
|-----------|----------------|--------|
| ARM toolchain installed | `arm-none-eabi-gcc --version` succeeds | [ ] |
| Submodules initialized | `external/mutable-instruments/elements/` exists | [ ] |
| Desktop build succeeds | `make test` completes without errors | [ ] |
| Hardware build succeeds | `make` completes without errors | [ ] |
| Plugin .dylib exists | `build/test/libnt_elements.dylib` file present | [ ] |
| Plugin .o exists | `build/nt_elements.o` file present | [ ] |

**Notes:**
_Record any issues or observations here_

---

## Story 1.3: Plugin Stub - Load and Audio Passthrough

**Goal:** Verify plugin loads in nt_emu and passes audio cleanly.

### Test Procedure

1. Launch VCV Rack
2. Add nt_emu module to patch
3. Right-click nt_emu → Load Plugin → select `libnt_elements.dylib`
4. Connect audio source → nt_emu input
5. Connect nt_emu output → Audio module
6. Play audio and listen
7. Check VCV Rack log (Help → Show Log)

### Pass/Fail Criteria

| Criterion | Pass Condition | Status |
|-----------|----------------|--------|
| Plugin loads without errors | nt_emu displays "nt_elements" name | [ ] |
| No error messages in log | VCV Rack log shows successful load | [ ] |
| Audio passthrough works | Input signal audible at output unmodified | [ ] |
| No memory leaks | Reload plugin 5x, no crashes or slowdown | [ ] |
| Load time acceptable | Plugin loads in < 2 seconds | [ ] |

**Notes:**
_Record load time, any warnings, or behavior observations_

---

## Story 1.4: Elements Part Integration - Synthesis Sound

**Goal:** Verify Elements Part instantiates and produces modal synthesis sound.

### Test Procedure

1. Load nt_elements plugin in nt_emu
2. Connect MIDI-CV (V/Oct, Gate) → nt_emu MIDI input
3. Connect nt_emu output → Audio module and Scope
4. Send MIDI note-on (C4, MIDI note 60)
5. Listen for synthesis sound
6. Hold note for 10+ seconds
7. Send MIDI note-off
8. Observe decay behavior

### Pass/Fail Criteria

| Criterion | Pass Condition | Status |
|-----------|----------------|--------|
| Elements Part instantiated | Plugin construct() succeeds, no crash | [ ] |
| Synthesis sound produced | Audible tone on MIDI note-on | [ ] |
| Modal character present | Bell-like or string-like resonance quality | [ ] |
| Sustained note stable | 10+ second note without dropouts or glitches | [ ] |
| Note-off stops sound | Synthesis decays naturally after note-off | [ ] |
| No crashes | Plugin runs for 1+ minute without crash | [ ] |

**Notes:**
_Describe sonic character: timbre, resonance, decay behavior_

---

## Story 1.5: Sample Rate Conversion - Pitch Accuracy

**Goal:** Verify pitch accuracy across MIDI note range after 32kHz → 48kHz conversion.

### Test Procedure

1. Connect nt_emu output → Spectrum Analyzer
2. Send MIDI note C4 (MIDI note 60)
3. Measure fundamental frequency in Spectrum Analyzer
4. Expected: 261.63 Hz (±1 cent = ±1.51 Hz)
5. Test chromatic scale C1 (32.70 Hz) → C6 (1046.50 Hz)
6. Verify pitch accuracy for each note

### Test Notes

**Chromatic Scale Test:**

| MIDI Note | Expected Freq (Hz) | Measured Freq (Hz) | Error (cents) | Pass/Fail |
|-----------|--------------------|--------------------|---------------|-----------|
| C1 (24)   | 32.70              |                    |               | [ ]       |
| C2 (36)   | 65.41              |                    |               | [ ]       |
| C3 (48)   | 130.81             |                    |               | [ ]       |
| C4 (60)   | 261.63             |                    |               | [ ]       |
| C5 (72)   | 523.25             |                    |               | [ ]       |
| C6 (84)   | 1046.50            |                    |               | [ ]       |

### Pass/Fail Criteria

| Criterion | Pass Condition | Status |
|-----------|----------------|--------|
| C4 pitch accurate | 261.63 Hz ±1.51 Hz (±1 cent) | [ ] |
| C1-C6 pitch accurate | All notes within ±1 cent | [ ] |
| No timbral changes | Sound quality consistent with original Elements | [ ] |
| Frequency response correct | Resonator frequencies align with MIDI notes | [ ] |

**Notes:**
_Record any detuning issues or timbral observations_

---

## Story 1.6: MIDI Integration - Note and Pitch Bend

**Goal:** Verify MIDI note-on, note-off, and pitch bend work correctly.

### Test Procedure

1. **Note-on test:** Send MIDI note-on, verify synthesis starts
2. **Note-off test:** Send MIDI note-off, verify synthesis stops
3. **Pitch bend test:** Hold note, send pitch bend messages, listen for smooth modulation
4. **Rapid notes test:** Send 10+ note-on messages per second
5. **Legato test:** Send overlapping notes (note-on before previous note-off)

### Pass/Fail Criteria

| Criterion | Pass Condition | Status |
|-----------|----------------|--------|
| Note-on triggers synthesis | Sound starts immediately on MIDI note-on | [ ] |
| Note-off stops synthesis | Sound decays naturally on MIDI note-off | [ ] |
| Pitch tracking accurate | MIDI note maps to correct frequency | [ ] |
| Pitch bend smooth | Frequency modulates without glitches | [ ] |
| Rapid notes handled | 10+ notes/sec without crashes or dropouts | [ ] |
| Legato works | Last-note priority, smooth transitions | [ ] |

**Notes:**
_Record response time, pitch bend range, legato behavior_

---

## Story 1.7: Parameter Adapter - Parameter Mapping

**Goal:** Verify NT parameters map correctly to Elements Patch and affect synthesis.

### Test Procedure

1. **Geometry parameter:**
   - Change geometry parameter (0.0 → 1.0)
   - Listen for timbre change (string → tube → etc)
   - Verify immediate response

2. **Brightness parameter:**
   - Change brightness parameter (0.0 → 1.0)
   - Listen for brightness/filter cutoff change
   - Verify immediate response

3. **Damping parameter:**
   - Change damping parameter (0.0 → 1.0)
   - Listen for decay time change
   - Verify immediate response

4. **Zipper noise test:**
   - Hold note
   - Slowly sweep each parameter
   - Listen for zipper noise or artifacts

5. **Rapid changes test:**
   - Rapidly change parameters while synthesizing
   - Verify no glitches or crashes

### Pass/Fail Criteria

| Criterion | Pass Condition | Status |
|-----------|----------------|--------|
| Geometry affects timbre | Clear material/structure change | [ ] |
| Brightness affects tone | Clear brightness/filter change | [ ] |
| Damping affects decay | Clear decay time change | [ ] |
| Immediate response | Parameters update within 1 frame | [ ] |
| No zipper noise | Smooth parameter sweeps without artifacts | [ ] |
| Rapid changes handled | No glitches or crashes | [ ] |

**Notes:**
_Describe parameter ranges and sonic effects_

---

## Story 1.8: Bus Routing and Audio I/O

**Goal:** Verify audio bus routing, external excitation, and output modes work correctly.

### Test Procedure

1. **Input bus routing:**
   - Connect VCO → nt_emu audio input
   - Verify plugin receives audio

2. **External excitation:**
   - Send VCO audio to nt_emu
   - Verify resonator responds to external audio
   - Listen for resonator being "excited" by input

3. **Output bus routing:**
   - Verify nt_emu output routes to VCV Audio correctly
   - Test stereo output (if applicable)

4. **Output mode (add vs replace):**
   - Test "replace" mode (overwrite bus)
   - Test "add" mode (sum with existing bus)
   - Verify correct behavior for each mode

5. **Audio artifacts test:**
   - Listen for clicks, pops, or glitches at bus boundaries
   - Verify clean audio at all times

### Pass/Fail Criteria

| Criterion | Pass Condition | Status |
|-----------|----------------|--------|
| Input bus routing works | Plugin receives input audio correctly | [ ] |
| External excitation works | Resonator responds to external audio | [ ] |
| Output bus routing works | Audio routes to output correctly | [ ] |
| Output mode "replace" works | Output overwrites bus contents | [ ] |
| Output mode "add" works | Output sums with bus contents | [ ] |
| No audio artifacts | Clean audio at bus boundaries | [ ] |

**Notes:**
_Describe external excitation behavior and output routing_

---

## Performance Profiling (AC #2)

**Goal:** Measure CPU usage and estimate hardware performance.

### Test Procedure

1. **Idle CPU:** No notes playing, measure nt_emu CPU %
2. **Single note CPU:** Hold C4 note, measure nt_emu CPU %
3. **Parameter sweep CPU:** Sweep parameters while holding note, measure CPU %
4. **Rapid notes CPU:** Send 10+ notes/sec, measure CPU %
5. **External input CPU:** Route VCO to input, measure CPU %

### CPU Measurements

| Test Condition | Desktop CPU (%) | Estimated Hardware CPU (%) | Notes |
|----------------|-----------------|----------------------------|-------|
| Idle           |                 |                            |       |
| Single note    |                 |                            |       |
| Parameter sweep|                 |                            |       |
| Rapid notes    |                 |                            |       |
| External input |                 |                            |       |

**Hardware Estimation Formula:**
- Hardware CPU ≈ Desktop CPU × 5-10 (approximate)
- Target: < 30% hardware CPU usage

### Pass/Fail Criteria

| Criterion | Pass Condition | Status |
|-----------|----------------|--------|
| Desktop CPU < 10% | nt_emu shows < 10% CPU usage | [ ] |
| Estimated hardware < 30% | Extrapolated hardware CPU < 30% | [ ] |
| CPU usage stable | No gradual increase over time | [ ] |
| Performance documented | All measurements recorded | [ ] |

**Notes:**
_Record VCV Rack CPU display values and estimation methodology_

---

## Stress Testing (AC #3)

**Goal:** Verify stability during 30-minute continuous operation.

### Test Procedure

1. Start timer
2. Configure MIDI sequencer or live playing
3. Send continuous MIDI notes (varied pitches and timings)
4. Sweep parameters throughout test period
5. Switch buses and modes periodically
6. Monitor for:
   - Crashes or freezes
   - Audio glitches or dropouts
   - Memory leaks (increasing CPU/memory)
   - Visual anomalies in Scope

### Stress Test Log

| Time (min) | Event | CPU % | Observation |
|------------|-------|-------|-------------|
| 0          | Start |       |             |
| 5          |       |       |             |
| 10         |       |       |             |
| 15         |       |       |             |
| 20         |       |       |             |
| 25         |       |       |             |
| 30         | End   |       |             |

### Pass/Fail Criteria

| Criterion | Pass Condition | Status |
|-----------|----------------|--------|
| No crashes | Plugin runs 30+ minutes without crash | [ ] |
| No glitches | No audible artifacts or dropouts | [ ] |
| Stable CPU | CPU usage remains constant | [ ] |
| Stable memory | No memory leaks (gradual increase) | [ ] |
| Stable audio | Scope shows consistent waveform | [ ] |

**Notes:**
_Record any anomalies, crashes, or issues during stress test_

---

## Memory Usage Validation (AC #4)

**Goal:** Verify memory allocations are within hardware limits.

### Test Procedure

1. Check build logs or add debug logging to construct()
2. Track memory allocations:
   - DTC (patch structure, parameter state, algorithm state)
   - SRAM (input buffer, output buffer, Elements Part, resonator state)
   - DRAM (reverb buffer)
3. Sum total allocations
4. Compare against architecture.md memory budget

### Memory Allocations

| Memory Region | Expected Size | Actual Size | Status |
|---------------|---------------|-------------|--------|
| DTC           | ~4 KB         |             | [ ]    |
| SRAM          | ~20 KB        |             | [ ]    |
| DRAM          | ~32 KB        |             | [ ]    |
| **Total**     | **~56 KB**    |             |        |

### Pass/Fail Criteria

| Criterion | Pass Condition | Status |
|-----------|----------------|--------|
| DTC < 64 KB | DTC allocation within limit | [ ] |
| SRAM < 128 KB | SRAM allocation within limit | [ ] |
| DRAM < 2 MB | DRAM allocation within limit | [ ] |
| Total within budget | Matches architecture.md estimates | [ ] |

**Notes:**
_Record actual allocation sizes and compare to estimates_

---

## Audio Quality Assessment (AC #1, #5)

**Goal:** Verify authentic Elements sound character and identify any artifacts.

### Test Procedure

1. **Exciter modes:**
   - Test "bow" sound (sustained bowing)
   - Test "blow" sound (wind/breath)
   - Test "strike" sound (percussion)

2. **Parameter sweeps:**
   - Sweep geometry (string → tube → bell)
   - Sweep brightness (dark → bright)
   - Sweep damping (short decay → long decay)

3. **Pitch range:**
   - Test low notes (C1)
   - Test mid notes (C4)
   - Test high notes (C6)

4. **Listen for:**
   - Modal resonance quality (bell-like, string-like)
   - Rich harmonic content
   - Smooth parameter transitions
   - Authentic Elements timbre

5. **Check for red flags:**
   - Clicks, pops, or digital artifacts
   - Harsh or aliased sound
   - Parameter zipper noise
   - Inaccurate pitch
   - Crashes or silence

### Audio Quality Assessment

| Aspect | Expected Character | Observed Character | Pass/Fail |
|--------|--------------------|--------------------|-----------|
| Modal resonance | Bell-like/string-like quality | | [ ] |
| Harmonic content | Rich overtones | | [ ] |
| Exciter modes | Bow/blow/strike distinct | | [ ] |
| Parameter sweeps | Smooth transitions | | [ ] |
| Pitch range | Accurate C1-C6 | | [ ] |
| Overall timbre | Authentic Elements sound | | [ ] |

### Artifacts Check

| Artifact Type | Present? | Severity | Notes |
|---------------|----------|----------|-------|
| Clicks/pops | [ ] Y / [ ] N | | |
| Zipper noise | [ ] Y / [ ] N | | |
| Aliasing | [ ] Y / [ ] N | | |
| Pitch error | [ ] Y / [ ] N | | |
| Glitches | [ ] Y / [ ] N | | |

**Notes:**
_Describe sonic character, record audio examples if helpful_

---

## Summary

### Overall Test Results

| Story | Feature | Pass/Fail | Notes |
|-------|---------|-----------|-------|
| 1.1-1.2 | Environment & Build | [ ] | |
| 1.3 | Plugin Load & Passthrough | [ ] | |
| 1.4 | Elements Synthesis | [ ] | |
| 1.5 | Sample Rate Conversion | [ ] | |
| 1.6 | MIDI Integration | [ ] | |
| 1.7 | Parameter Adapter | [ ] | |
| 1.8 | Bus Routing | [ ] | |

### Performance Summary

- Desktop CPU (idle): ____%
- Desktop CPU (synthesis): ____%
- Estimated hardware CPU: ____%
- Stress test result: [ ] Pass / [ ] Fail
- Memory usage: _____KB total

### Known Issues

_List any issues discovered during testing:_

1.
2.
3.

### Hardware Validation Baseline (for Story 1.10)

_Metrics to compare against hardware testing:_

- Desktop CPU metrics (for comparison)
- Expected hardware CPU range
- Audio quality baseline
- Known limitations or issues to verify on hardware

---

## Next Steps

After completing this checklist:

1. Document results in `desktop-validation-results.md`
2. Fix any critical issues discovered
3. Re-test after fixes
4. Proceed to Story 1.10 (hardware validation) when all tests pass
