# Sound Quality A/B Validation - Test Execution Checklist

This checklist guides you through the manual validation process for Story 2.6.

---

## Phase 1: Test Environment Preparation

### Hardware Acquisition
- [ ] Obtain or borrow Mutable Instruments Elements Eurorack module
  - Note firmware version
  - Note serial number (if available)
- [ ] Verify disting NT is running latest nt_elements plugin
  - Note plugin version / git commit hash
  - Note disting NT firmware version

### Equipment Setup
- [ ] Set up Eurorack case with Elements module
- [ ] Ensure Elements module has proper power and is working
- [ ] Set up disting NT in Eurorack case or test environment
- [ ] Connect audio interface to computer
  - Verify sample rate: 48kHz or 96kHz
  - Verify bit depth: 24-bit
- [ ] Connect Elements output to audio interface input channels 1-2
- [ ] Connect disting NT output to audio interface input channels 3-4
  - OR: Record sequentially using same input channels with consistent settings

### MIDI Setup
- [ ] Connect MIDI controller or sequencer to both modules
  - Option 1: MIDI splitter to send to both simultaneously
  - Option 2: MIDI sequencer with multiple outputs
  - Option 3: Record hardware first, then send same MIDI to nt_elements
- [ ] Test MIDI connectivity on both modules
- [ ] Verify both modules respond to MIDI notes correctly

### Recording Software Setup
- [ ] Open DAW or recording software (e.g., Audacity, Reaper, Logic, Ableton)
- [ ] Configure project sample rate: 48kHz or 96kHz
- [ ] Configure bit depth: 24-bit
- [ ] Create recording tracks:
  - Tracks 1-2: Hardware Elements (stereo)
  - Tracks 3-4: nt_elements on disting NT (stereo)
  - OR: Two separate sessions with identical settings
- [ ] Test recording levels (aim for -6dB to -12dB peak)
- [ ] Document complete signal chain

### Analysis Software Setup
- [ ] Install or verify spectral analysis software:
  - Recommended: Sonic Visualiser (free)
  - Alternative: Audacity (free)
  - Alternative: MATLAB or Python with audio analysis libraries
  - Alternative: Voxengo SPAN (free VST)
- [ ] Test software with sample audio files
- [ ] Familiarize yourself with FFT spectrum analysis features

---

## Phase 2: Configure Test Patches

### Test Patch 1: Bowed String
- [ ] Configure hardware Elements:
  - Bow Level: 80%
  - Blow Level: 0%
  - Strike Level: 0%
  - Geometry: 15% (String range)
  - Brightness: 55%
  - Damping: 70%
  - Position: [your choice, but document]
  - Space: [your choice, but document]
  - Reverb: [on/off, document]
- [ ] Configure nt_elements with IDENTICAL settings
- [ ] Verify settings match exactly
- [ ] Document any differences in parameter implementation

### Test Patch 2: Blown Pipe
- [ ] Configure hardware Elements:
  - Bow Level: 0%
  - Blow Level: 80%
  - Strike Level: 0%
  - Geometry: 45% (Tube range)
  - Brightness: 50%
  - Damping: 60%
  - Position: [your choice, but document]
  - Space: [your choice, but document]
  - Reverb: [on/off, document]
- [ ] Configure nt_elements with IDENTICAL settings
- [ ] Verify settings match exactly

### Test Patch 3: Strike Bell
- [ ] Configure hardware Elements:
  - Bow Level: 0%
  - Blow Level: 0%
  - Strike Level: 85%
  - Geometry: 75% (Metallic range)
  - Brightness: 70%
  - Damping: 40%
  - Position: [your choice, but document]
  - Space: [your choice, but document]
  - Reverb: [on/off, document]
- [ ] Configure nt_elements with IDENTICAL settings
- [ ] Verify settings match exactly

### Test Patch 4: Mixed Exciters
- [ ] Configure hardware Elements:
  - Bow Level: 40%
  - Blow Level: 40%
  - Strike Level: 20%
  - Geometry: 50% (Mid-range)
  - Brightness: 60%
  - Damping: 55%
  - Position: [your choice, but document]
  - Space: [your choice, but document]
  - Reverb: [on/off, document]
- [ ] Configure nt_elements with IDENTICAL settings
- [ ] Verify settings match exactly

### Test Patch 5: Extreme Settings
- [ ] Configure hardware Elements:
  - Bow Level: 0%
  - Blow Level: 0%
  - Strike Level: 100%
  - Geometry: 100% (Maximum metallic)
  - Brightness: 100%
  - Damping: 0%
  - Position: [your choice, but document]
  - Space: [your choice, but document]
  - Reverb: [on/off, document]
- [ ] Configure nt_elements with IDENTICAL settings
- [ ] Verify settings match exactly

---

## Phase 3: Record Test Sequences

### MIDI Test Sequence 1: Pitch Accuracy
- [ ] Program MIDI sequence: C2, C3, C4, C5, C6 (3 seconds each)
- [ ] Record Patch 1 (Bowed String) - Hardware and nt_elements
  - File: `Hardware_Patch1_PitchAccuracy.wav`
  - File: `NT_Patch1_PitchAccuracy.wav`
- [ ] Record Patch 2 (Blown Pipe) - Hardware and nt_elements
  - File: `Hardware_Patch2_PitchAccuracy.wav`
  - File: `NT_Patch2_PitchAccuracy.wav`
- [ ] Record Patch 3 (Strike Bell) - Hardware and nt_elements
  - File: `Hardware_Patch3_PitchAccuracy.wav`
  - File: `NT_Patch3_PitchAccuracy.wav`
- [ ] Verify recordings are clean and usable

### MIDI Test Sequence 2: Chromatic Scale
- [ ] Program MIDI sequence: C4 through C5 (13 semitones, 1 second each)
- [ ] Record all 5 patches with chromatic scale
- [ ] Save files with naming convention: `Hardware_PatchN_Chromatic.wav`

### MIDI Test Sequence 3: Parameter Sweeps
For each patch, record three parameter sweeps:

**Geometry Sweep**:
- [ ] Patch 1: Hold C4, sweep Geometry 0% to 100% over 10 seconds
  - Record hardware: `Hardware_Patch1_GeometrySweep.wav`
  - Record nt_elements: `NT_Patch1_GeometrySweep.wav`
- [ ] Repeat for Patches 2-5

**Brightness Sweep**:
- [ ] Patch 1: Hold C4, sweep Brightness 0% to 100% over 10 seconds
  - Record hardware: `Hardware_Patch1_BrightnessSweep.wav`
  - Record nt_elements: `NT_Patch1_BrightnessSweep.wav`
- [ ] Repeat for Patches 2-5

**Damping Sweep**:
- [ ] Patch 1: Hold C4, sweep Damping 0% to 100% over 10 seconds
  - Record hardware: `Hardware_Patch1_DampingSweep.wav`
  - Record nt_elements: `NT_Patch1_DampingSweep.wav`
- [ ] Repeat for Patches 2-5

### MIDI Test Sequence 4: Dynamic Response
- [ ] Program MIDI sequence: C4 at velocities 30, 60, 90, 127 (2 seconds each)
- [ ] Record all 5 patches with dynamic sequence
- [ ] Save files: `Hardware_PatchN_Dynamics.wav`

### Recording Quality Check
- [ ] Verify all recordings are saved
- [ ] Check for clipping (should have none)
- [ ] Check for noise or interference
- [ ] Verify file formats: WAV, 24-bit, 48kHz or 96kHz
- [ ] Normalize recordings to same peak level (e.g., -3dB)
- [ ] Organize files in folder structure:
  ```
  docs/validation/recordings/
    hardware/
    nt_elements/
  ```

---

## Phase 4: Spectral Analysis

### Setup Analysis
- [ ] Open Sonic Visualiser (or chosen analysis tool)
- [ ] Configure FFT settings:
  - Window size: 2048 or 4096 bins
  - Window function: Hanning
  - Overlap: 50%

### Analyze Each Patch (C4 Note)
For each of the 5 test patches:

- [ ] Load hardware recording (pitch accuracy test, C4 note)
- [ ] Generate spectrum plot (frequency domain)
- [ ] Measure fundamental frequency
- [ ] Identify and measure first 10 harmonics
- [ ] Load nt_elements recording (same patch, C4 note)
- [ ] Generate spectrum plot
- [ ] Measure fundamental frequency
- [ ] Identify and measure first 10 harmonics
- [ ] Create overlay plot (hardware vs nt_elements)
- [ ] Export overlay image: `Spectrum_PatchN_C4.png`
- [ ] Document findings in table:

| Harmonic | Hardware Freq | NT Freq | Amplitude Diff (dB) |
|----------|---------------|---------|---------------------|
| f0       |               |         |                     |
| 2f0      |               |         |                     |
| ...      |               |         |                     |

### Analyze Parameter Sweeps
For each parameter sweep (Geometry, Brightness, Damping):

- [ ] Load hardware recording
- [ ] Generate spectrogram (time vs frequency)
- [ ] Load nt_elements recording
- [ ] Generate spectrogram
- [ ] Create side-by-side comparison image
- [ ] Export: `Spectrogram_PatchN_ParameterName.png`
- [ ] Note any differences in parameter response

### Analysis Summary
- [ ] Complete spectral analysis for all 5 patches
- [ ] Identify any frequency response mismatches
- [ ] Calculate maximum deviation in fundamental frequency
- [ ] Note any missing or extra harmonics
- [ ] Document findings in validation report

---

## Phase 5: Pitch Accuracy Validation

### Measure Fundamental Frequencies
For each note (C2, C3, C4, C5, C6) in pitch accuracy recordings:

**C2 (MIDI 36) - Expected: 65.41 Hz**
- [ ] Measure hardware Elements fundamental frequency: _____ Hz
- [ ] Measure nt_elements fundamental frequency: _____ Hz
- [ ] Calculate hardware deviation (cents): _____ cents
- [ ] Calculate nt_elements deviation (cents): _____ cents
- [ ] Formula: `cents = 1200 * log2(measured / 65.41)`

**C3 (MIDI 48) - Expected: 130.81 Hz**
- [ ] Measure hardware Elements: _____ Hz (_____ cents)
- [ ] Measure nt_elements: _____ Hz (_____ cents)

**C4 (MIDI 60) - Expected: 261.63 Hz**
- [ ] Measure hardware Elements: _____ Hz (_____ cents)
- [ ] Measure nt_elements: _____ Hz (_____ cents)

**C5 (MIDI 72) - Expected: 523.25 Hz**
- [ ] Measure hardware Elements: _____ Hz (_____ cents)
- [ ] Measure nt_elements: _____ Hz (_____ cents)

**C6 (MIDI 84) - Expected: 1046.50 Hz**
- [ ] Measure hardware Elements: _____ Hz (_____ cents)
- [ ] Measure nt_elements: _____ Hz (_____ cents)

### Pitch Accuracy Assessment
- [ ] All measurements within 1 cent tolerance? (AC4 requirement)
- [ ] Any notes exceed 1 cent deviation?
- [ ] If deviation found, investigate root cause:
  - [ ] Check MIDI note-to-Hz conversion
  - [ ] Check sample rate handling (32kHz vs 48kHz)
  - [ ] Check oscillator stability
- [ ] Document findings in validation report

---

## Phase 6: Exciter Mode Validation

### Bow Exciter (Test Patch 1)
- [ ] Listen to hardware recording
- [ ] Listen to nt_elements recording
- [ ] Compare side-by-side:
  - [ ] Sustain quality: [identical / similar / different]
  - [ ] Bowing noise texture: [identical / similar / different]
  - [ ] Attack envelope: [identical / similar / different]
  - [ ] Decay behavior: [identical / similar / different]
- [ ] Review spectral analysis for Patch 1
- [ ] Note any timbral differences
- [ ] Document findings

### Blow Exciter (Test Patch 2)
- [ ] Listen to hardware recording
- [ ] Listen to nt_elements recording
- [ ] Compare side-by-side:
  - [ ] Airy/breathy tone: [identical / similar / different]
  - [ ] Turbulence noise: [identical / similar / different]
  - [ ] Flutter characteristics: [identical / similar / different]
  - [ ] Sustain behavior: [identical / similar / different]
- [ ] Review spectral analysis for Patch 2
- [ ] Note any timbral differences
- [ ] Document findings

### Strike Exciter (Test Patch 3)
- [ ] Listen to hardware recording
- [ ] Listen to nt_elements recording
- [ ] Compare side-by-side:
  - [ ] Transient attack: [identical / similar / different]
  - [ ] Initial impulse spectrum: [identical / similar / different]
  - [ ] Decay behavior: [identical / similar / different]
  - [ ] Ring characteristics: [identical / similar / different]
- [ ] Review spectral analysis for Patch 3
- [ ] Note any timbral differences
- [ ] Document findings

### Exciter Validation Summary
- [ ] All exciter modes behave identically? (AC5 requirement)
- [ ] Any significant differences requiring fixes?
- [ ] Document assessment in validation report

---

## Phase 7: Resonator Response Validation

### Geometry Parameter Response
- [ ] Load geometry sweep spectrograms (hardware vs nt_elements)
- [ ] Compare material transitions:
  - [ ] String to tube transition (~30%): [match / mismatch]
  - [ ] Tube to metallic transition (~60%): [match / mismatch]
- [ ] Compare frequency content evolution
- [ ] Note any differences in geometry response
- [ ] Document findings

### Brightness Parameter Response
- [ ] Load brightness sweep spectrograms
- [ ] Compare filter cutoff modulation
- [ ] Compare harmonic content evolution
- [ ] Note any differences in brightness response
- [ ] Document findings

### Damping Parameter Response
- [ ] Load damping sweep recordings
- [ ] Measure decay times at three settings:
  - [ ] Damping 0%: Hardware _____ sec, NT _____ sec
  - [ ] Damping 50%: Hardware _____ sec, NT _____ sec
  - [ ] Damping 100%: Hardware _____ sec, NT _____ sec
- [ ] Compare decay envelopes
- [ ] Note any differences in damping response
- [ ] Document findings

### Resonator Validation Summary
- [ ] Resonator response matches across all geometry settings? (AC6)
- [ ] Any significant differences requiring fixes?
- [ ] Document assessment in validation report

---

## Phase 8: Blind Listening Tests

### Prepare Test Materials
- [ ] Select 10 recording pairs (mix of patches and test sequences)
- [ ] Create anonymous files:
  - [ ] Test01_A.wav, Test01_B.wav (randomly assign hardware or NT)
  - [ ] Test02_A.wav, Test02_B.wav
  - [ ] ... through Test10_A.wav, Test10_B.wav
- [ ] Create answer key (store separately): which is hardware?
- [ ] Level-match all audio files (same peak level)

### Recruit Beta Testers
- [ ] Post recruitment message in:
  - [ ] disting NT Discord community
  - [ ] Mutable Instruments Users Facebook group
  - [ ] ModWiggler forums
  - [ ] Lines forum
- [ ] Target: 3+ testers with Elements experience
- [ ] Verify testers have:
  - [ ] Experience with hardware Elements module
  - [ ] Good listening environment (headphones or monitors)
  - [ ] Audio playback software

### Distribute Test Materials
- [ ] Share test files via:
  - [ ] Dropbox / Google Drive link
  - [ ] WeTransfer
  - [ ] Direct download
- [ ] Provide test instructions:
  ```
  For each test pair (Test01 through Test10):
  1. Listen to version A
  2. Listen to version B
  3. Identify which version is hardware Elements: A or B?
  4. Rate your confidence: 1 (guessing) to 5 (very confident)
  5. Optionally note any differences you hear
  ```
- [ ] Set deadline for completion (1-2 weeks recommended)

### Collect Results
- [ ] Receive responses from Tester 1
  - [ ] Record: Correct identifications: _____ / 10
  - [ ] Calculate accuracy: _____ %
  - [ ] Record confidence rating average: _____
  - [ ] Note feedback/comments
- [ ] Receive responses from Tester 2
  - [ ] Record results
- [ ] Receive responses from Tester 3
  - [ ] Record results
- [ ] (Optional) Additional testers

### Analyze Results
- [ ] Calculate average accuracy across all testers: _____ %
- [ ] Compare to random guess baseline (50%)
- [ ] Compare to success criterion (< 60% accuracy = PASS)
- [ ] Review tester feedback for common themes
- [ ] Document findings in validation report

### Blind Test Summary
- [ ] AC3 requirement met? (≥3 testers, < 60% accuracy)
- [ ] Testers confirm sound authenticity?
- [ ] Document assessment in validation report

---

## Phase 9: Document Differences

### Identify All Differences
Review all test results and list every difference discovered:

- [ ] Difference 1:
  - Observation: _________________________________
  - Root Cause: _________________________________
  - Severity: [Minor / Moderate / Major]
  - Acceptable? [Yes / No]
  - Action: _____________________________________

- [ ] Difference 2:
  - [Same format]

- [ ] Difference 3:
  - [Same format]

### Sample Rate Testing (If Differences Found)
If significant differences detected, test both sample rate modes:

- [ ] Configure nt_elements for 32kHz mode (Elements native rate)
- [ ] Re-record key test sequences
- [ ] Re-analyze spectral response
- [ ] Compare 32kHz vs 48kHz results
- [ ] Document sample-rate-specific effects

### Root Cause Analysis
For each difference, investigate potential causes:
- [ ] Sample rate effects (32kHz vs 48kHz)
- [ ] DSP precision differences (hardware analog vs digital)
- [ ] Platform differences (Eurorack vs disting NT)
- [ ] Parameter mapping issues
- [ ] Filter coefficient differences
- [ ] Frequency calculation errors

### Difference Assessment
- [ ] Classify all differences as acceptable or unacceptable
- [ ] Acceptable: minor noise floor, sample rate artifacts, < 1 cent pitch deviation
- [ ] Unacceptable: pitch drift > 1 cent, timbral changes, missing harmonics
- [ ] Document justifications clearly

---

## Phase 10: Create Validation Report

- [ ] Use template: `docs/validation/sound-quality-report-template.md`
- [ ] Fill in Executive Summary
- [ ] Document test methodology
- [ ] Complete all test patch specifications
- [ ] Fill in all test results:
  - [ ] AC1: Recordings
  - [ ] AC2: Spectral analysis
  - [ ] AC3: Blind listening tests
  - [ ] AC4: Pitch accuracy
  - [ ] AC5: Exciter modes
  - [ ] AC6: Resonator response
  - [ ] AC7: Documented differences
- [ ] Include all images:
  - [ ] Spectral overlay plots
  - [ ] Spectrograms
  - [ ] Charts/graphs
- [ ] Complete validation summary checklist
- [ ] Write overall assessment
- [ ] Write conclusion with clear PASS/FAIL determination
- [ ] Write production readiness recommendation
- [ ] Save final report: `docs/validation/sound-quality-report.md`

---

## Phase 11: Update Story File

After completing validation and creating report:

- [ ] Open story file: `docs/stories/2-6-sound-quality-a-b-validation.md`
- [ ] Check off all completed tasks
- [ ] Update Dev Agent Record → Completion Notes with summary
- [ ] Update File List section with created files:
  - [ ] `docs/validation/sound-quality-report.md`
  - [ ] `docs/validation/recordings/` (directory)
  - [ ] `docs/validation/spectral-analysis/` (directory)
- [ ] Update Status field to "review" if all ACs pass
- [ ] Update Status field to "blocked" if hardware not available
- [ ] Update Status field to "in-progress" if fixes needed

---

## Completion Criteria

Story is complete when:
- [ ] All 7 acceptance criteria have been validated (PASS or FAIL)
- [ ] Validation report is complete and detailed
- [ ] All test recordings and analysis artifacts are saved
- [ ] All differences are documented with root cause analysis
- [ ] Clear PASS/FAIL determination has been made
- [ ] Production readiness recommendation has been provided
- [ ] Story status has been updated appropriately

---

## Notes and Observations

Use this space for any additional notes during testing:

```
[Your notes here]
```

---

**Test Execution Started**: _______________

**Test Execution Completed**: _______________

**Test Operator**: _______________

**Overall Result**: [PASS / FAIL / BLOCKED]
