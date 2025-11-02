# Story 2.6: Sound Quality A/B Validation

Status: ready-for-dev

## Story

As a user,
I want nt_elements to sound identical to hardware Elements,
So that I get authentic modal synthesis.

## Acceptance Criteria

1. Side-by-side recordings captured (hardware Elements vs nt_elements)
2. Spectral analysis shows matching frequency response
3. At least 3 beta testers confirm sound authenticity in blind tests
4. Pitch accuracy validated across full MIDI range
5. All exciter modes (bow/blow/strike) behave identically
6. Resonator response matches hardware across geometry settings
7. Any differences documented with explanation

## Tasks / Subtasks

- [ ] Prepare test environment (AC: #1)
  - [ ] Obtain hardware Elements module (borrow or access)
  - [ ] Set up recording interface for both hardware and NT
  - [ ] Prepare identical MIDI test sequences
  - [ ] Configure identical parameter settings on both
  - [ ] Document test setup and methodology

- [ ] Create test patches (AC: #1, #5, #6)
  - [ ] Test Patch 1: Bow exciter, string geometry
  - [ ] Test Patch 2: Blow exciter, tube geometry
  - [ ] Test Patch 3: Strike exciter, metallic geometry
  - [ ] Test Patch 4: Mixed exciters, mid-range geometry
  - [ ] Test Patch 5: Extreme settings (edge cases)
  - [ ] Document parameter values for each test patch

- [ ] Record hardware Elements (AC: #1)
  - [ ] Load each test patch on hardware Elements
  - [ ] Record MIDI test sequences (C2, C4, C6 notes)
  - [ ] Record parameter sweeps (geometry, brightness, damping)
  - [ ] Save recordings with clear labels
  - [ ] Document recording settings (sample rate, bit depth)

- [ ] Record nt_elements (AC: #1)
  - [ ] Load identical patches on nt_elements
  - [ ] Record identical MIDI test sequences
  - [ ] Record identical parameter sweeps
  - [ ] Use same recording settings as hardware
  - [ ] Save recordings with matching labels

- [ ] Perform spectral analysis (AC: #2)
  - [ ] Load recordings into spectral analysis software
  - [ ] Generate FFT plots for each recording pair
  - [ ] Overlay hardware and nt_elements spectra
  - [ ] Measure frequency peaks (should match)
  - [ ] Measure harmonic content (should match)
  - [ ] Document spectral comparison results

- [ ] Validate pitch accuracy (AC: #4)
  - [ ] Measure fundamental frequency for C2 (65.41 Hz)
  - [ ] Measure fundamental frequency for C4 (261.63 Hz)
  - [ ] Measure fundamental frequency for C6 (1046.50 Hz)
  - [ ] Compare hardware vs nt_elements frequencies
  - [ ] Verify accuracy within 1 cent deviation
  - [ ] Document pitch accuracy measurements

- [ ] Validate exciter modes (AC: #5)
  - [ ] Compare bow exciter recordings (hardware vs nt_elements)
  - [ ] Compare blow exciter recordings
  - [ ] Compare strike exciter recordings
  - [ ] Listen for timbral differences
  - [ ] Analyze attack/decay characteristics
  - [ ] Document exciter mode comparison results

- [ ] Validate resonator response (AC: #6)
  - [ ] Compare recordings across geometry range (0%, 50%, 100%)
  - [ ] Compare recordings across brightness range
  - [ ] Compare recordings across damping range
  - [ ] Verify material transitions sound identical
  - [ ] Verify decay characteristics match
  - [ ] Document resonator comparison results

- [ ] Conduct blind listening tests (AC: #3)
  - [ ] Recruit 3+ beta testers with Elements experience
  - [ ] Prepare blind test: randomly ordered recording pairs
  - [ ] Ask testers to identify hardware vs nt_elements
  - [ ] Collect feedback on sound authenticity
  - [ ] Analyze results (should be indistinguishable)
  - [ ] Document beta tester feedback

- [ ] Identify and document differences (AC: #7)
  - [ ] List any sonic differences discovered
  - [ ] Explain root cause of each difference
  - [ ] Assess whether differences are acceptable
  - [ ] Note any sample rate or platform-specific effects
  - [ ] Document known limitations

- [ ] Address significant differences (AC: #2, #5, #6)
  - [ ] If pitch inaccurate: adjust frequency calculations
  - [ ] If timbre different: review parameter mappings
  - [ ] If resonator different: check filter coefficients
  - [ ] Re-record and re-test after fixes
  - [ ] Verify fixes resolve differences

- [ ] Create sound quality validation report (AC: #1-7)
  - [ ] Document test methodology
  - [ ] Present spectral analysis results
  - [ ] Present pitch accuracy measurements
  - [ ] Present beta tester feedback
  - [ ] Document any differences and explanations
  - [ ] Conclude validation results (pass/fail)

## Dev Notes

### Sound Quality Validation Goals

**Primary Goal:**
nt_elements should sound indistinguishable from hardware Mutable Instruments Elements module in A/B comparison.

**Why This Matters:**
- User expectation: authentic Elements sound
- Project value proposition: Elements on NT platform
- Quality assurance: verify DSP port is correct

**Acceptable Differences:**
- Minor noise floor differences (hardware vs digital platform)
- Sample rate differences (if 32kHz mode used)
- Reverb differences (if reverb implementation differs)

**Unacceptable Differences:**
- Pitch drift or inaccuracy
- Timbral changes (brighter/darker than hardware)
- Missing harmonics or resonances
- Different attack/decay characteristics

### Test Environment Setup

**Hardware Requirements:**
- Mutable Instruments Elements module (hardware)
- disting NT running nt_elements plugin
- Audio interface for recording both
- MIDI controller or sequencer

**Recording Setup:**
```
MIDI Controller → Hardware Elements → Audio Interface (Input 1-2)
MIDI Controller → disting NT → Audio Interface (Input 3-4)

OR

MIDI Controller → Hardware Elements → Audio Interface → Recording
[Configure nt_elements with identical settings]
MIDI Controller → disting NT → Audio Interface → Recording
```

**Recording Settings:**
- Sample rate: 48kHz (or 96kHz for analysis)
- Bit depth: 24-bit
- Format: WAV (uncompressed)
- Normalize recordings for level matching

### Test Patches Design

**Test Patch 1: Bowed String**
- Bow Level: 80%
- Geometry: 15% (String)
- Brightness: 55%
- Damping: 70%
- Purpose: Test bow exciter and string resonator

**Test Patch 2: Blown Pipe**
- Blow Level: 80%
- Geometry: 45% (Tube)
- Brightness: 50%
- Damping: 60%
- Purpose: Test blow exciter and tube resonator

**Test Patch 3: Strike Bell**
- Strike Level: 85%
- Geometry: 75% (Metallic)
- Brightness: 70%
- Damping: 40%
- Purpose: Test strike exciter and metallic resonator

**Test Patch 4: Mixed Exciters**
- Bow: 40%, Blow: 40%, Strike: 20%
- Geometry: 50% (Mid-range)
- Brightness: 60%
- Damping: 55%
- Purpose: Test exciter mixing and mid-range geometry

**Test Patch 5: Extreme Settings**
- Strike: 100%
- Geometry: 100%
- Brightness: 100%
- Damping: 0%
- Purpose: Test edge cases and extreme parameter values

### MIDI Test Sequences

**Test Sequence 1: Pitch Accuracy**
- Play notes: C2, C3, C4, C5, C6 (one octave apart)
- Hold each note for 3 seconds
- Measure fundamental frequency of each

**Test Sequence 2: Chromatic Scale**
- Play C4 through C5 (13 semitones)
- Hold each note for 1 second
- Verify semitone spacing

**Test Sequence 3: Parameter Sweeps**
- Hold C4 continuously
- Sweep geometry from 0% to 100% over 10 seconds
- Repeat for brightness and damping
- Record parameter sweep audio

**Test Sequence 4: Dynamic Response**
- Play C4 at velocities: 30, 60, 90, 127
- Hold each for 2 seconds
- Verify velocity response (if implemented)

### Spectral Analysis Methodology

**Analysis Tools:**
- Sonic Visualiser (free, excellent for spectral analysis)
- Audacity (free, basic spectrum analysis)
- MATLAB/Python (advanced analysis)
- Voxengo SPAN (free VST spectrum analyzer)

**Analysis Procedure:**
1. Load hardware recording
2. Generate FFT spectrum (1024-4096 bins)
3. Load nt_elements recording
4. Generate matching FFT spectrum
5. Overlay spectra for comparison
6. Identify frequency peaks
7. Measure peak frequencies and amplitudes
8. Compare hardware vs nt_elements

**Metrics:**
- Fundamental frequency (should match within 1 cent)
- Harmonic spacing (should be identical)
- Harmonic amplitude ratios (should be similar)
- Inharmonic content (for metallic/bell timbres)

**Expected Results:**
Spectra should overlay closely. Minor differences acceptable due to:
- Recording chain differences
- Analog vs digital noise floor
- Sample rate (if 32kHz mode used on NT)

### Blind Listening Test Methodology

**Test Design:**
- Prepare 10 recording pairs (hardware vs nt_elements, identical patches)
- Randomize order (A/B or B/A)
- Ask tester: "Which is hardware Elements?"
- Collect guesses and confidence levels

**Hypothesis:**
If nt_elements is authentic, testers should guess randomly (~50% accuracy).

**Success Criteria:**
- Tester accuracy < 60% (indicates indistinguishable)
- Tester feedback: "Very difficult to tell apart" or "Sounds identical"

**Beta Tester Selection:**
- Must have experience with hardware Elements
- Prefer users with trained ears (musicians, sound designers)
- Minimum 3 testers for statistical validity

### Pitch Accuracy Validation

**Measurement Methodology:**
1. Isolate sustained portion of note (after attack)
2. Use pitch detection algorithm or manual FFT measurement
3. Compare measured frequency to expected MIDI frequency
4. Calculate cent deviation: `cents = 1200 * log2(measured / expected)`

**Expected Frequencies:**
- C2 (MIDI 36): 65.41 Hz
- C3 (MIDI 48): 130.81 Hz
- C4 (MIDI 60): 261.63 Hz
- C5 (MIDI 72): 523.25 Hz
- C6 (MIDI 84): 1046.50 Hz

**Tolerance:**
- Acceptable: < 1 cent deviation (< 0.06% frequency error)
- Excellent: < 0.5 cent deviation

**Common Issues:**
- Tuning offset: Check MIDI note-to-Hz conversion
- Sample rate error: Verify 48kHz vs 32kHz handling
- Pitch drift: Check oscillator stability in Elements code

### Difference Documentation

**If Differences Found:**

**Format:**
```markdown
## Sound Quality Differences

### Difference 1: [Description]
**Observation:** [What sounds different]
**Root Cause:** [Technical explanation]
**Severity:** Minor/Moderate/Major
**Acceptable:** Yes/No
**Action:** [Fix applied or explanation why acceptable]

### Difference 2: ...
```

**Example:**
```markdown
### Difference 1: Slightly Brighter Timbre at High Brightness Settings
**Observation:** nt_elements sounds 5-10% brighter than hardware at brightness > 80%
**Root Cause:** 48kHz sample rate changes filter response vs 32kHz hardware
**Severity:** Minor
**Acceptable:** Yes - within natural variation, not perceptible in blind tests
**Action:** None - 32kHz mode available for authentic sample rate if desired
```

### Sound Quality Validation Report

**Report Structure:**
```markdown
# nt_elements Sound Quality Validation Report

## Executive Summary
[Pass/Fail, key findings]

## Test Methodology
- Hardware: Mutable Instruments Elements (firmware version)
- nt_elements: Version [commit hash], disting NT
- Recording: [equipment, settings]
- Analysis: [tools used]

## Test Results

### Pitch Accuracy
- C2: 65.40 Hz (expected 65.41 Hz) - 0.27 cents deviation ✓
- C4: 261.65 Hz (expected 261.63 Hz) - 0.13 cents deviation ✓
- C6: 1046.48 Hz (expected 1046.50 Hz) - 0.03 cents deviation ✓

### Spectral Analysis
[Spectral overlay images, frequency comparison tables]

### Exciter Mode Validation
- Bow exciter: Identical ✓
- Blow exciter: Identical ✓
- Strike exciter: Identical ✓

### Resonator Validation
- String geometry: Identical ✓
- Tube geometry: Identical ✓
- Metallic geometry: Minor difference (documented below)

### Blind Listening Tests
- Tester 1: 4/10 correct (50% accuracy)
- Tester 2: 6/10 correct (60% accuracy)
- Tester 3: 5/10 correct (50% accuracy)
- Average: 53% accuracy (random guess level) ✓

## Documented Differences
[Any differences found, with explanations]

## Conclusion
nt_elements achieves authentic Elements sound quality. All critical
metrics pass validation. Plugin is production-ready.
```

### Project Structure

**Files Created:**
- docs/validation/sound-quality-report.md (new) - Validation report
- docs/validation/recordings/ (directory) - Test recordings (if shareable)
- docs/validation/spectral-analysis/ (directory) - Spectral plots

### References

- [Source: docs/epics.md#Story 2.6] - Full story specification and acceptance criteria
- [Source: docs/PRD.md#NFR002] - Sound quality requirement (authentic Elements)
- [Source: docs/PRD.md#Goals] - Preserve 100% sound authenticity goal
- [Source: docs/architecture.md#Audio Quality] - Sound quality validation methodology

## Dev Agent Record

### Context Reference

- docs/stories/2-6-sound-quality-a-b-validation.context.md

### Agent Model Used

<!-- Model information will be filled in during implementation -->

### Debug Log References

### Completion Notes List

### File List
