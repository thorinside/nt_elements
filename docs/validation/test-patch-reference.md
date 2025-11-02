# Test Patch Reference - Quick Guide

This is a quick reference for the 5 test patches used in Story 2.6 sound quality validation.

## Test Patch 1: Bowed String

**Purpose**: Test bow exciter and string geometry resonator

| Parameter   | Value | Notes                           |
|-------------|-------|---------------------------------|
| Bow Level   | 80%   | Primary exciter                 |
| Blow Level  | 0%    | Not used                        |
| Strike Level| 0%    | Not used                        |
| Geometry    | 15%   | String range (low geometry)     |
| Brightness  | 55%   | Mid-range filter                |
| Damping     | 70%   | Moderate damping                |
| Position    | TBD   | Your choice, document it        |
| Space       | TBD   | Your choice, document it        |
| Reverb      | TBD   | On/Off, document it             |

**Expected Sound**: Smooth, sustained bowed string tone with characteristic bowing noise texture.

---

## Test Patch 2: Blown Pipe

**Purpose**: Test blow exciter and tube geometry resonator

| Parameter   | Value | Notes                           |
|-------------|-------|---------------------------------|
| Bow Level   | 0%    | Not used                        |
| Blow Level  | 80%   | Primary exciter                 |
| Strike Level| 0%    | Not used                        |
| Geometry    | 45%   | Tube range (mid-low geometry)   |
| Brightness  | 50%   | Mid-range filter                |
| Damping     | 60%   | Moderate damping                |
| Position    | TBD   | Your choice, document it        |
| Space       | TBD   | Your choice, document it        |
| Reverb      | TBD   | On/Off, document it             |

**Expected Sound**: Airy, breathy flute-like tone with turbulence and flutter characteristics.

---

## Test Patch 3: Strike Bell

**Purpose**: Test strike exciter and metallic geometry resonator

| Parameter   | Value | Notes                           |
|-------------|-------|---------------------------------|
| Bow Level   | 0%    | Not used                        |
| Blow Level  | 0%    | Not used                        |
| Strike Level| 85%   | Primary exciter                 |
| Geometry    | 75%   | Metallic range (high geometry)  |
| Brightness  | 70%   | Bright filter                   |
| Damping     | 40%   | Light damping, long sustain     |
| Position    | TBD   | Your choice, document it        |
| Space       | TBD   | Your choice, document it        |
| Reverb      | TBD   | On/Off, document it             |

**Expected Sound**: Bell or metallic struck tone with sharp attack, inharmonic partials, and long sustain.

---

## Test Patch 4: Mixed Exciters

**Purpose**: Test multiple exciters simultaneously and mid-range geometry

| Parameter   | Value | Notes                           |
|-------------|-------|---------------------------------|
| Bow Level   | 40%   | Contributing exciter            |
| Blow Level  | 40%   | Contributing exciter            |
| Strike Level| 20%   | Contributing exciter            |
| Geometry    | 50%   | Mid-range (tube/metallic blend) |
| Brightness  | 60%   | Moderate brightness             |
| Damping     | 55%   | Moderate damping                |
| Position    | TBD   | Your choice, document it        |
| Space       | TBD   | Your choice, document it        |
| Reverb      | TBD   | On/Off, document it             |

**Expected Sound**: Complex hybrid tone combining bowed sustain, airy texture, and struck transients.

---

## Test Patch 5: Extreme Settings

**Purpose**: Test edge cases and extreme parameter values

| Parameter   | Value | Notes                           |
|-------------|-------|---------------------------------|
| Bow Level   | 0%    | Not used                        |
| Blow Level  | 0%    | Not used                        |
| Strike Level| 100%  | Maximum exciter level           |
| Geometry    | 100%  | Maximum metallic geometry       |
| Brightness  | 100%  | Fully bright, wide open filter  |
| Damping     | 0%    | No damping, infinite sustain    |
| Position    | TBD   | Your choice, document it        |
| Space       | TBD   | Your choice, document it        |
| Reverb      | TBD   | On/Off, document it             |

**Expected Sound**: Extremely bright, aggressive metallic strike with very long sustain and prominent high-frequency content.

---

## MIDI Test Sequences

### Sequence 1: Pitch Accuracy Test
- **Notes**: C2 (MIDI 36), C3 (48), C4 (60), C5 (72), C6 (84)
- **Duration**: 3 seconds per note
- **Purpose**: Validate pitch accuracy across MIDI range
- **Expected Frequencies**:
  - C2: 65.41 Hz
  - C3: 130.81 Hz
  - C4: 261.63 Hz
  - C5: 523.25 Hz
  - C6: 1046.50 Hz

### Sequence 2: Chromatic Scale Test
- **Notes**: C4 (60) through C5 (72) - 13 semitones
- **Duration**: 1 second per note
- **Purpose**: Validate semitone spacing consistency

### Sequence 3: Parameter Sweep Tests
- **Base Note**: C4 (261.63 Hz) held continuously
- **Sweeps**:
  1. Geometry: 0% to 100% over 10 seconds
  2. Brightness: 0% to 100% over 10 seconds
  3. Damping: 0% to 100% over 10 seconds
- **Purpose**: Validate parameter response and resonator behavior

### Sequence 4: Dynamic Response Test
- **Note**: C4 (60)
- **Velocities**: 30, 60, 90, 127
- **Duration**: 2 seconds per velocity
- **Purpose**: Validate velocity response (if implemented)

---

## Recording Checklist

For each test patch, record the following:

- [ ] Pitch Accuracy Test (Sequence 1)
- [ ] Chromatic Scale Test (Sequence 2)
- [ ] Geometry Sweep (Sequence 3a)
- [ ] Brightness Sweep (Sequence 3b)
- [ ] Damping Sweep (Sequence 3c)
- [ ] Dynamic Response Test (Sequence 4)

Record each on **both** hardware Elements and nt_elements.

**Total recordings per patch**: 6 sequences × 2 systems = 12 files

**Total recordings for all patches**: 5 patches × 12 files = 60 files

---

## File Naming Convention

Use consistent naming for easy organization:

```
Hardware_Patch1_PitchAccuracy.wav
Hardware_Patch1_Chromatic.wav
Hardware_Patch1_GeometrySweep.wav
Hardware_Patch1_BrightnessSweep.wav
Hardware_Patch1_DampingSweep.wav
Hardware_Patch1_Dynamics.wav

NT_Patch1_PitchAccuracy.wav
NT_Patch1_Chromatic.wav
NT_Patch1_GeometrySweep.wav
NT_Patch1_BrightnessSweep.wav
NT_Patch1_DampingSweep.wav
NT_Patch1_Dynamics.wav

... (repeat for Patch2 through Patch5)
```

---

## Recording Settings

**Critical**: Use identical settings for all recordings

- **Sample Rate**: 48 kHz or 96 kHz (document which)
- **Bit Depth**: 24-bit
- **Format**: WAV (uncompressed)
- **Level**: Aim for -6 dB to -12 dB peak (no clipping)
- **Normalization**: Apply after recording to match peak levels

---

## Analysis Settings

**FFT Spectrum Analysis**:
- Window Size: 2048 or 4096 bins
- Window Function: Hanning
- Overlap: 50%
- Frequency Range: 20 Hz - 20 kHz

**Spectrogram**:
- Window Size: 2048 bins
- Window Function: Hanning
- Hop Size: 512 samples
- Color Scale: dB (logarithmic)

**Pitch Detection**:
- Algorithm: Autocorrelation or YIN
- Tolerance: < 1 cent (< 0.06% frequency error)

---

## Quick Reference: Parameter Mapping

### Geometry Parameter
- **0-30%**: String range (string-like resonance)
- **30-60%**: Tube range (pipe/flute-like resonance)
- **60-100%**: Metallic range (bell/metallic resonance)

### Brightness Parameter
- **0-30%**: Dark, muted tone (low-pass filter cutoff)
- **30-70%**: Mid-range brightness
- **70-100%**: Bright, open tone (high filter cutoff)

### Damping Parameter
- **0-30%**: Very light damping, long sustain
- **30-70%**: Moderate damping
- **70-100%**: Heavy damping, short decay

---

## Success Criteria Summary

| Acceptance Criterion | Success Threshold |
|---------------------|-------------------|
| AC2: Spectral Match | Frequency peaks match within 1 Hz, harmonics match within 1-2 dB |
| AC3: Blind Test     | Average tester accuracy < 60% (random guess level ~50%) |
| AC4: Pitch Accuracy | Fundamental frequency within 1 cent of expected across MIDI range |
| AC5: Exciter Modes  | Bow, blow, strike exciters sound identical in timbral characteristics |
| AC6: Resonator      | Geometry, brightness, damping responses match across parameter ranges |

---

## Quick Tips

1. **Level Matching**: Normalize all recordings to same peak level before comparison - level differences can bias listening tests

2. **Parameter Precision**: Use MIDI CCs or CV control for precise parameter settings - ensure both systems are configured identically

3. **Recording Order**: Record hardware first, then nt_elements - allows you to match parameters precisely from hardware reference

4. **Listening Environment**: Use good headphones or monitors in quiet environment for critical listening

5. **Take Notes**: Document any observations during recording - "Patch 3 hardware had slight click at note-on" - helps during analysis

6. **Save Everything**: Keep all raw recordings even if some tests fail - you may need to re-analyze later

7. **Spectral Analysis**: Focus on fundamental and first 10 harmonics - these contain most perceptually relevant information

8. **Blind Test Design**: Randomize A/B ordering - don't always put hardware as "A" or testers may learn the pattern

---

**Print this page for quick reference during testing sessions!**
