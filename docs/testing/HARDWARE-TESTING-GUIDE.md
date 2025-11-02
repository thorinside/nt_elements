# Hardware Testing Guide - Story 1.10

## Overview

This guide provides step-by-step instructions for completing Story 1.10: Hardware Deployment and Validation. This story requires manual testing on physical disting NT hardware.

**Current Status:**
- ✅ Build phase complete (AC #1)
- ⏸️ Hardware testing phase pending (AC #2-6)

**Time Required:** 2-3 hours (including 1-hour stability test)

---

## Prerequisites

### Required Equipment:
- ✅ disting NT Eurorack module (firmware v1.11.0 or later)
- ✅ SD card (FAT32 formatted, at least 1 GB)
- ✅ MIDI keyboard or MIDI sequencer
- ✅ Eurorack audio interface or recorder
- ✅ Eurorack patch cables (audio and CV)
- ✅ Powered Eurorack case
- ⏸️ Optional: Audio analyzer or DAW for spectral analysis

### Required Files:
- ✅ `plugins/nt_elements.o` (183 KB ARM binary)
- ✅ Desktop validation baseline: `docs/testing/desktop-validation-results.md`
- ⏸️ Hardware validation report: `docs/testing/hardware-validation-results.md` (to be filled)

---

## Phase 1: Deployment (Tasks 2-3)

### Task 2: Prepare disting NT for Plugin Loading

**Estimated Time:** 10 minutes

**Steps:**

1. **Format SD Card (if needed):**
   ```
   - Insert SD card into computer
   - Format as FAT32
   - Label: "DISTING_NT" (optional)
   ```

2. **Copy Plugin to SD Card:**
   ```
   - Copy plugins/nt_elements.o to SD card root directory
   - Verify file copied successfully (183 KB)
   ```

3. **Safely Eject SD Card:**
   ```
   - Eject SD card from computer properly
   - Do not remove before eject completes
   ```

4. **Insert into disting NT:**
   ```
   - Power OFF disting NT
   - Insert SD card into disting NT slot
   - Power ON disting NT
   ```

**Checkpoint:** SD card properly inserted, disting NT powered on

---

### Task 3: Load Plugin on Hardware

**Estimated Time:** 5 minutes

**Steps:**

1. **Navigate to File Browser:**
   ```
   - On disting NT, navigate to file browser mode
   - (Consult disting NT manual for file browser navigation)
   ```

2. **Select Plugin File:**
   ```
   - Browse to root directory
   - Find "nt_elements.o" in file list
   - Select file
   ```

3. **Load Plugin:**
   ```
   - Confirm load (follow disting NT prompts)
   - Wait for loading to complete
   ```

4. **Verify Load Success:**
   ```
   ✅ OLED display shows "nt_elements"
   ✅ No error messages displayed
   ✅ Plugin appears in algorithm list
   ```

**Checkpoint:** Plugin loaded successfully, appears in algorithm list

**Troubleshooting:**
- **Plugin not in list:** Check SD card format (must be FAT32)
- **Load error:** Rebuild plugin (`make clean && make hardware`)
- **File not visible:** Check file is in root directory, not subdirectory

**Documentation:**
- Update hardware-validation-results.md → Load Validation section
- Mark all load test cases PASS or FAIL

---

## Phase 2: Functional Testing (Tasks 4-6, 9-10)

### Task 4: Test Basic Audio Output

**Estimated Time:** 15 minutes

**Setup:**
```
MIDI Keyboard → disting NT MIDI IN
disting NT Audio OUT → Eurorack mixer → Audio interface/speakers
```

**Steps:**

1. **Test MIDI Note-On:**
   ```
   - Select nt_elements algorithm on disting NT
   - Press MIDI key (try C4, MIDI note 60)
   - Listen for audio output
   - Expected: Bell-like/resonant modal synthesis sound
   ```

2. **Compare to Desktop Version:**
   ```
   - Recall sound from desktop testing (Story 1.9)
   - Compare timbre, resonance, decay
   - Expected: Identical character
   ```

3. **Record Output (Optional):**
   ```
   - Record MIDI sequence on hardware
   - Record identical sequence on desktop (nt_emu)
   - Compare recordings
   ```

**Verification Checklist:**
- ✅ Audio output produced on MIDI note-on
- ✅ Sound has modal/resonant character
- ✅ Matches desktop version timbre
- ✅ No audio artifacts or glitches
- ✅ Clean decay on note-off

**Documentation:**
- Update hardware-validation-results.md → Functional Validation → Basic Audio Output
- Note any sonic differences from desktop

---

### Task 5: Test Parameter Control

**Estimated Time:** 20 minutes

**Steps:**

1. **Test Geometry Parameter:**
   ```
   - Hold MIDI note (C4)
   - Adjust Geometry pot/encoder (0% → 100%)
   - Listen for timbre change (string → tube)
   - Expected: Smooth material/structure change
   ```

2. **Test Brightness Parameter:**
   ```
   - Hold MIDI note
   - Adjust Brightness pot/encoder (0% → 100%)
   - Listen for brightness/filter change
   - Expected: Smooth tonal brightness change
   ```

3. **Test Damping Parameter:**
   ```
   - Trigger note, let decay
   - Adjust Damping pot/encoder (0% → 100%)
   - Listen for decay time change
   - Expected: Short decay → Long decay
   ```

4. **Test for Artifacts:**
   ```
   - Sweep parameters while synthesizing
   - Listen for zipper noise, clicks, pops
   - Expected: Smooth, artifact-free changes
   ```

**Verification Checklist:**
- ✅ Geometry affects timbre (material change)
- ✅ Brightness affects tone (filter response)
- ✅ Damping affects decay time
- ✅ Parameters respond immediately (< 1 frame)
- ✅ No zipper noise during sweeps
- ✅ No glitches during rapid changes

**Documentation:**
- Update hardware-validation-results.md → Functional Validation → Parameter Control
- Document parameter response and any issues

---

### Task 6: Test Bus Routing and Audio I/O

**Estimated Time:** 20 minutes

**Setup:**
```
External VCO/noise → disting NT Audio IN (input bus)
disting NT Audio OUT (output bus) → Eurorack mixer → Audio interface
```

**Steps:**

1. **Test Input Bus Routing:**
   ```
   - Configure input bus routing on disting NT
   - Patch external audio to input
   - Verify audio received by plugin
   ```

2. **Test External Audio Excitation:**
   ```
   - Send audio (noise, VCO) to input
   - Verify resonator responds to external excitation
   - Expected: Resonator "rings" with external audio character
   ```

3. **Test Output Bus Routing:**
   ```
   - Configure output bus routing
   - Verify audio routes to output correctly
   - Test routing to other Eurorack modules
   ```

4. **Test Output Modes:**
   ```
   - Test "replace" mode: Overwrites bus contents
   - Test "add" mode: Sums with bus contents
   - Verify correct behavior for each mode
   ```

**Verification Checklist:**
- ✅ Input bus routing works
- ✅ External audio excites resonator
- ✅ Output bus routing works
- ✅ Output mode "replace" works correctly
- ✅ Output mode "add" works correctly
- ✅ No audio artifacts at bus boundaries

**Documentation:**
- Update hardware-validation-results.md → Functional Validation → Bus Routing and Audio I/O
- Document bus routing behavior

---

### Task 9: Test MIDI Functionality on Hardware

**Estimated Time:** 15 minutes

**Steps:**

1. **Test Note-On/Off:**
   ```
   - Press key: Sound starts
   - Release key: Sound decays
   - Expected: Immediate response
   ```

2. **Test Pitch Accuracy:**
   ```
   - Play C1, C2, C3, C4, C5, C6
   - Listen/measure fundamental frequency
   - Expected: Accurate pitch (±1 cent)
   ```

3. **Test Pitch Bend:**
   ```
   - Hold note, apply pitch bend
   - Expected: Smooth frequency modulation (±2 semitones)
   ```

4. **Test Rapid Note Changes:**
   ```
   - Play rapid sequence (10+ notes/sec)
   - Expected: No dropouts, crashes, or glitches
   ```

5. **Test Legato Playing:**
   ```
   - Play overlapping notes
   - Expected: Last-note priority, smooth transitions
   ```

**Verification Checklist:**
- ✅ Note-on triggers synthesis immediately
- ✅ Note-off stops synthesis (natural decay)
- ✅ Pitch accuracy across C1-C6
- ✅ Pitch bend works smoothly
- ✅ Rapid notes handled without glitches
- ✅ Legato works correctly

**Documentation:**
- Update hardware-validation-results.md → Functional Validation → MIDI Functionality
- Document MIDI response and pitch accuracy

---

## Phase 3: Performance Testing (Task 7)

### Task 7: Measure CPU Usage

**Estimated Time:** 20 minutes

**Steps:**

1. **Measure Idle CPU:**
   ```
   - Load plugin, do not play notes
   - Observe disting NT CPU % on main display
   - Record measurement
   - Expected: < 5%
   ```

2. **Measure Synthesis CPU:**
   ```
   - Hold single note (C4)
   - Observe CPU %
   - Record measurement
   - Expected: 15-25%
   ```

3. **Measure CPU with Parameter Sweep:**
   ```
   - Hold note, sweep parameters
   - Observe CPU %
   - Record measurement
   - Expected: 20-30%
   ```

4. **Measure Maximum CPU:**
   ```
   - Play rapid notes + parameter changes
   - Observe peak CPU %
   - Record measurement
   - Expected: < 50% (interim target)
   ```

**CPU Usage Table:**

| Test Condition | Measured CPU (%) | Target | Status |
|----------------|------------------|--------|--------|
| Idle | [TBD] | < 5% | [ ] |
| Single note | [TBD] | < 30% | [ ] |
| Parameter sweep | [TBD] | < 40% | [ ] |
| Rapid notes | [TBD] | < 50% | [ ] |
| Maximum stress | [TBD] | < 50% | [ ] |

**Verification:**
- ✅ All measurements < 50% (interim target)
- ⏸️ If > 50%, note for optimization in Story 2.5

**Documentation:**
- Update hardware-validation-results.md → Performance Metrics
- Record all CPU measurements
- Compare to desktop baseline (2.1% desktop, 10.5-21% estimated)

---

## Phase 4: Stability Testing (Task 8)

### Task 8: Perform 1-Hour Stability Test

**Estimated Time:** 60+ minutes (mostly unattended)

**Setup:**
```
MIDI Sequencer → disting NT (continuous note pattern)
disting NT Audio OUT → Monitor (audio + visual monitoring)
```

**Steps:**

1. **Configure MIDI Sequencer:**
   ```
   - Create repeating pattern:
     - Varied note pitches (C2-C5)
     - Varied note lengths
     - Mix of single notes and chords
   - Loop pattern continuously
   ```

2. **Start Stability Test:**
   ```
   - Start MIDI sequence
   - Note start time
   - Monitor audio output
   - Monitor OLED display
   ```

3. **Monitor During Test:**
   ```
   - Check status every 15 minutes
   - Listen for audio dropouts or glitches
   - Watch for OLED anomalies or freezes
   - Check CPU % remains stable
   - Verify no crashes or resets
   ```

4. **Stability Test Log:**
   ```
   | Time (min) | CPU % | Observation |
   |------------|-------|-------------|
   | 0          | [TBD] | Test start  |
   | 15         | [TBD] | [notes]     |
   | 30         | [TBD] | [notes]     |
   | 45         | [TBD] | [notes]     |
   | 60         | [TBD] | Test end    |
   ```

5. **After Test:**
   ```
   - Stop sequence
   - Test plugin responsiveness (play notes manually)
   - Check parameters still work
   - Verify no issues introduced during test
   ```

**Pass Criteria:**
- ✅ No crashes for 1+ hour
- ✅ No audio glitches or dropouts
- ✅ Stable CPU usage (no runaway increase)
- ✅ Plugin remains functional after test
- ✅ No OLED anomalies or freezes

**Documentation:**
- Update hardware-validation-results.md → Stability Test
- Record stability test log with observations
- Document any issues encountered

---

## Phase 5: Comparison Testing (Task 9, 11)

### Task 11: Compare Desktop vs Hardware Results

**Estimated Time:** 30 minutes

**Steps:**

1. **Record Identical Sequences:**
   ```
   Desktop:
   - Open VCV Rack with nt_emu
   - Load plugins/nt_elements.dylib
   - Record MIDI sequence output
   - Save recording

   Hardware:
   - Load nt_elements on disting NT
   - Play identical MIDI sequence
   - Record audio output
   - Save recording
   ```

2. **Normalize Recordings:**
   ```
   - Match levels for fair comparison
   - Align recordings in time
   ```

3. **Spectral Analysis (Optional):**
   ```
   - Import both recordings to DAW
   - Analyze spectrum of both
   - Compare frequency content
   - Expected: Identical spectra
   ```

4. **Listening Test:**
   ```
   - A/B compare recordings
   - Listen for timbral differences
   - Check pitch accuracy
   - Verify modal character identical
   ```

5. **Compare to Baseline:**
   ```
   - Review desktop validation results (Story 1.9)
   - Compare hardware behavior
   - Document any differences
   ```

**Verification Checklist:**
- ✅ Overall sound identical
- ✅ Pitch accuracy matches (±1 cent)
- ✅ Timbre identical
- ✅ Parameter response identical
- ✅ Spectral analysis matches (if performed)
- ⏸️ Document acceptable differences (noise floor, etc.)

**Documentation:**
- Update hardware-validation-results.md → Desktop vs Hardware Comparison
- Document comparison results
- Note any differences found

---

## Phase 6: Documentation and Completion (Tasks 10, 12)

### Task 10: Create Hardware Validation Report

**Estimated Time:** 30 minutes

**Steps:**

1. **Complete All Test Sections:**
   ```
   - Review hardware-validation-results.md
   - Ensure all [TBD] sections filled
   - Mark all test cases PASS or FAIL
   ```

2. **Document Test Results:**
   ```
   - Fill in Environment section (firmware version, hardware setup)
   - Document all CPU measurements
   - Record stability test results
   - Note comparison results
   ```

3. **Document Any Issues:**
   ```
   - List any issues discovered
   - Document severity and impact
   - Note workarounds or fixes needed
   ```

4. **Compare to Acceptance Criteria:**
   ```
   - Verify each AC (1-6) marked PASS or FAIL
   - Document reasons for any failures
   ```

**Documentation:**
- Complete all sections in hardware-validation-results.md
- Mark all ACs PASS or FAIL
- Document overall test result

---

### Task 12: Address Hardware-Specific Issues

**Estimated Time:** Variable (depends on issues found)

**If Issues Found:**

1. **Crashes or Instability:**
   ```
   - Document crash conditions
   - Check memory allocations
   - Verify Elements DSP initialization
   - Fix and re-test
   ```

2. **Audio Artifacts:**
   ```
   - Check CPU usage (optimize if > 50%)
   - Verify bus routing configuration
   - Check for buffer overruns
   - Fix and re-test
   ```

3. **Performance Issues:**
   ```
   - If CPU > 50%, document for Story 2.5
   - Consider reverb bypass or quality reduction
   - Profile hot paths
   ```

4. **Incorrect Sound:**
   ```
   - Check sample rate conversion
   - Verify parameter mapping
   - Check Elements DSP initialization
   - Fix and re-test
   ```

**If No Issues:**
- Document "No issues found" in validation report
- Proceed to story completion

---

## Story Completion Checklist

### Tasks Completion:
- [x] Task 1: Build hardware plugin binary (COMPLETE)
- [ ] Task 2: Prepare disting NT for plugin loading
- [ ] Task 3: Load plugin on hardware
- [ ] Task 4: Test basic audio output
- [ ] Task 5: Test parameter control
- [ ] Task 6: Test bus routing and audio I/O
- [ ] Task 7: Measure CPU usage
- [ ] Task 8: Perform 1-hour stability test
- [ ] Task 9: Compare desktop vs hardware results
- [ ] Task 10: Test MIDI functionality on hardware
- [ ] Task 11: Create hardware validation report
- [ ] Task 12: Address hardware-specific issues

### Acceptance Criteria:
- [x] AC #1: Plugin compiles for ARM hardware (PASS)
- [ ] AC #2: Loads on disting NT without errors
- [ ] AC #3: Audio output sounds identical to nt_emu
- [ ] AC #4: CPU usage measured and documented (< 50%)
- [ ] AC #5: Operates stably for 1-hour test
- [ ] AC #6: MIDI and audio I/O work correctly

### Documentation:
- [x] Build validation documented (AC #1)
- [ ] Load validation documented (AC #2)
- [ ] Functional validation documented (AC #3, #6)
- [ ] Performance metrics documented (AC #4)
- [ ] Stability test documented (AC #5)
- [ ] Comparison results documented (AC #3)
- [ ] Issues documented (if any)
- [ ] All test sections complete

### Final Steps:
- [ ] All tasks checked [ ] → [x]
- [ ] All ACs verified PASS
- [ ] All documentation complete
- [ ] hardware-validation-results.md finalized
- [ ] Update story status to "review" in story file
- [ ] Update sprint-status.yaml: in-progress → review

---

## Troubleshooting Guide

### Common Issues:

**Issue: Plugin won't load**
```
Cause: File format, SD card format, corrupted file
Solution:
  - Verify SD card is FAT32
  - Check .o file not corrupted (183 KB)
  - Rebuild: make clean && make hardware
  - Try different SD card
  - Check disting NT firmware version
```

**Issue: No audio output**
```
Cause: Bus routing, MIDI connection, initialization
Solution:
  - Check bus routing configuration
  - Verify MIDI connection and note-on sent
  - Check output cables and levels
  - Monitor CPU percentage (should show activity)
  - Reload plugin
```

**Issue: Audio artifacts/glitches**
```
Cause: CPU overload, buffer issues
Solution:
  - Check CPU usage (should be < 50%)
  - Verify patch parameters
  - Check for buffer overruns
  - Monitor for stability issues
  - Note for optimization in Story 2.5
```

**Issue: Crashes or freezes**
```
Cause: Memory issues, pointer errors
Solution:
  - Document crash conditions
  - Check memory allocations
  - Verify Elements DSP initialization
  - Check for stack overflow
  - Rebuild and re-test
```

**Issue: Sound different from desktop**
```
Cause: Sample rate issues, parameter mapping, DSP bugs
Solution:
  - Check sample rate conversion
  - Verify parameter mapping
  - Compare parameter values desktop vs hardware
  - Check Elements DSP initialization
  - Review DSP processing chain
```

---

## Time Estimates

| Phase | Estimated Time |
|-------|----------------|
| Phase 1: Deployment | 15 minutes |
| Phase 2: Functional Testing | 70 minutes |
| Phase 3: Performance Testing | 20 minutes |
| Phase 4: Stability Testing | 75 minutes (mostly unattended) |
| Phase 5: Comparison Testing | 30 minutes |
| Phase 6: Documentation | 30 minutes |
| **Total Active Time** | **2.5 hours** |
| **Total Elapsed Time** | **3+ hours** (with stability test) |

---

## Success Criteria

**Story 1.10 is complete when:**
- ✅ All tasks (1-12) checked as complete
- ✅ All acceptance criteria (1-6) marked PASS
- ✅ Hardware validation report fully documented
- ✅ No blocking issues discovered (or all issues resolved)
- ✅ Story file updated with completion notes
- ✅ Story status updated to "review"
- ✅ Sprint status updated: in-progress → review

**Expected Results:**
- Plugin loads successfully on hardware
- Audio output identical to desktop version
- CPU usage < 50% (interim target)
- 1-hour stability test passes
- MIDI and I/O work correctly
- No hardware-specific issues discovered
