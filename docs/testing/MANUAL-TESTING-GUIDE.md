# Manual Testing Guide for Story 1.9

## Overview

Story 1.9 is a **validation story** that requires manual testing in VCV Rack with the nt_emu module. This guide explains what needs to be done and why automated testing isn't possible for this story.

## Why Manual Testing?

This story validates the plugin's behavior in a real desktop environment (VCV Rack). The acceptance criteria require:

1. **Visual verification** - Plugin loads correctly, displays in nt_emu
2. **Audio verification** - Listening tests for synthesis quality, timbral characteristics
3. **Performance measurement** - CPU usage from VCV Rack's module display
4. **Stress testing** - 30-minute runtime observation
5. **User experience** - Parameter behavior, MIDI responsiveness

These cannot be automated without VCV Rack running and require human judgment (audio quality, timbre assessment).

## What's Been Done Programmatically

✅ **Completed by dev agent:**
- Test validation checklist created (`docs/testing/test-validation-checklist.md`)
- Test results document template created (`docs/testing/desktop-validation-results.md`)
- Build verification (plugin compiles successfully)
- Memory allocation analysis from code (AC #4 partially complete)

## What Requires Manual Testing

The following tasks require you (Neal) to physically run VCV Rack:

### Task 2: Validate Story 1.3 - Plugin Load and Audio Passthrough
- **Action:** Load `plugins/nt_elements.dylib` in VCV Rack nt_emu module
- **Verify:** Plugin name displays, audio passes through, no crashes
- **Document:** Fill in Story 1.3 section in `desktop-validation-results.md`

### Task 3: Validate Story 1.4 - Elements Synthesis
- **Action:** Send MIDI note, listen for synthesis sound
- **Verify:** Modal character (bell-like/string-like), sustained notes stable
- **Document:** Describe sonic character in results document

### Task 4: Validate Story 1.5 - Pitch Accuracy
- **Action:** Use Spectrum Analyzer to measure frequencies for MIDI notes C1-C6
- **Verify:** Frequencies match expected values within ±1 cent
- **Document:** Fill in frequency measurement table

### Task 5: Validate Story 1.6 - MIDI Integration
- **Action:** Test note-on, note-off, pitch bend, rapid notes, legato
- **Verify:** Responsive MIDI handling, smooth pitch bend
- **Document:** MIDI behavior observations

### Task 6: Validate Story 1.7 - Parameter Adapter
- **Action:** Change Geometry, Brightness, Damping parameters
- **Verify:** Parameters affect synthesis immediately, no zipper noise
- **Document:** Parameter behavior and sonic effects

### Task 7: Validate Story 1.8 - Bus Routing
- **Action:** Route VCO to input, test external excitation
- **Verify:** Resonator responds to external audio, output modes work
- **Document:** Bus routing behavior

### Task 8: Performance Profiling (AC #2)
- **Action:** Measure VCV Rack CPU % in idle, synthesis, parameter sweep conditions
- **Verify:** Desktop CPU usage reasonable (estimate hardware CPU)
- **Document:** Fill in CPU measurements table

### Task 9: Stress Testing (AC #3)
- **Action:** Run plugin for 30 minutes with MIDI notes and parameter changes
- **Verify:** No crashes, glitches, or memory leaks
- **Document:** Fill in stress test log (every 5 minutes)

### Task 11: Audio Quality Assessment (AC #1, #5)
- **Action:** Listen carefully to synthesis across parameter ranges
- **Verify:** Authentic Elements character, no artifacts
- **Document:** Describe audio quality, note any issues

### Task 12: Fix Any Issues
- **Action:** If issues discovered, report them and/or attempt fixes
- **Verify:** Re-test after fixes
- **Document:** Update known issues section

## Step-by-Step Manual Testing Workflow

### 1. Setup VCV Rack Environment

**Install VCV Rack modules:**
```
1. VCV Rack v2.x (latest)
2. nt_emu module (disting NT emulator)
3. MIDI-CV module (for MIDI input)
4. Scope module (waveform visualization)
5. Spectrum Analyzer (frequency measurement)
6. VCO (for external audio tests)
7. Audio output module
```

**Create test patch:**
```
MIDI-CV → nt_emu (MIDI input)
VCO → nt_emu (audio input for bus routing tests)
nt_emu output → VCV Audio output
nt_emu output → Scope
nt_emu output → Spectrum Analyzer
```

### 2. Load nt_elements Plugin

1. Launch VCV Rack
2. Add nt_emu module to patch
3. Right-click nt_emu → "Load Plugin"
4. Navigate to `/Users/nealsanche/nosuch/nt_elements/plugins/`
5. Select `nt_elements.dylib`
6. Verify plugin loads without errors

### 3. Execute Test Checklist

Open `docs/testing/test-validation-checklist.md` and work through each test systematically:

- Check boxes `[ ]` as you complete each test
- Take notes on observations
- Record measurements (CPU %, frequencies)
- Document any issues or unexpected behavior

### 4. Fill In Results Document

As you complete tests, fill in `docs/testing/desktop-validation-results.md`:

- Replace all `[TBD]` placeholders with actual results
- Check boxes `[ ]` → `[x]` for passing tests
- Use `[ ]` → `[✗]` for failing tests
- Add detailed notes in "Notes" sections

### 5. Verify Memory Usage (AC #4)

Memory usage has been calculated from code analysis:
- **SRAM:** ~8.3 KB (well within 128 KB limit) ✅
- **DRAM:** 64 KB (well within 2 MB limit) ✅
- **DTC:** ~2-4 KB (well within 64 KB limit) ✅

**Optional:** If you want to verify actual runtime allocations, add debug logging to `construct()` in `src/nt_elements.cpp` to print allocation sizes.

### 6. Performance Profiling (AC #2)

**VCV Rack CPU measurement:**
1. Hover over nt_emu module in VCV Rack
2. VCV displays CPU % in tooltip
3. Record CPU % for each test condition (idle, synthesis, etc.)

**Hardware estimation:**
- Multiply desktop CPU % by 5-10× to estimate hardware CPU
- Example: 5% desktop → 25-50% hardware (approximate)
- Target: < 30% hardware CPU

### 7. Stress Test (AC #3)

**30-minute stress test procedure:**
1. Set timer for 30 minutes
2. Configure MIDI sequencer or play live
3. Send continuous MIDI notes (varied pitches)
4. Sweep parameters throughout test
5. Monitor VCV Rack for:
   - Crashes or freezes
   - Audio glitches or dropouts
   - CPU % increase (memory leaks)
   - Visual anomalies in Scope

**Every 5 minutes:** Record checkpoint in stress test log

### 8. Audio Quality Assessment (AC #1, #5)

**Listening tests:**
1. **Exciter modes:** Adjust parameters to hear bow/blow/strike sounds
2. **Geometry sweep:** Listen for material transitions (string → tube → bell)
3. **Brightness sweep:** Listen for filter cutoff changes
4. **Damping sweep:** Listen for decay time changes
5. **Pitch range:** Test C1 (low), C4 (mid), C6 (high)

**Listen for:**
- ✅ Modal resonance (bell-like, string-like quality)
- ✅ Rich harmonic content
- ✅ Smooth parameter transitions
- ✅ Authentic Elements timbre

**Red flags:**
- ✗ Clicks, pops, digital artifacts
- ✗ Harsh or aliased sound
- ✗ Parameter zipper noise
- ✗ Inaccurate pitch
- ✗ Crashes or silence

### 9. Document Results

After completing all tests:

1. **Review results document:**
   - All [TBD] sections filled in
   - All checkboxes marked (pass or fail)
   - Known issues documented

2. **Update story file:**
   - Mark remaining tasks as complete in `docs/stories/1-9-desktop-testing-validation.md`
   - Add completion notes

3. **Mark story ready for review:**
   - Update story Status to "review" (if all tests pass)
   - Update `docs/sprint-status.yaml` to "review"

---

## Quick Reference

### Files You Need

| File | Purpose |
|------|---------|
| `plugins/nt_elements.dylib` | Plugin to test |
| `docs/testing/test-validation-checklist.md` | Detailed test procedures |
| `docs/testing/desktop-validation-results.md` | Results template to fill in |
| `docs/stories/1-9-desktop-testing-validation.md` | Story file with tasks |

### Tools You Need

| Tool | Purpose |
|------|---------|
| VCV Rack v2.x | Desktop modular synthesizer |
| nt_emu module | disting NT emulator |
| MIDI-CV | MIDI input conversion |
| Scope | Waveform visualization |
| Spectrum Analyzer | Frequency measurement |
| VCO | Audio source for bus tests |
| Audio output | Listen to synthesis |

### Key Metrics to Record

| Metric | Where to Record |
|--------|-----------------|
| Plugin load time | Story 1.3 section |
| Synthesis character | Story 1.4 section |
| Pitch frequencies (C1-C6) | Story 1.5 frequency table |
| MIDI responsiveness | Story 1.6 section |
| Parameter behavior | Story 1.7 section |
| Bus routing behavior | Story 1.8 section |
| CPU % (idle, synthesis, etc.) | Performance profiling table |
| Stress test observations | Stress test log (every 5 min) |
| Audio quality assessment | Audio quality results table |

---

## After Manual Testing

### If All Tests Pass

1. Mark all tasks complete in story file
2. Update story Status to "review"
3. Update `sprint-status.yaml` to "review"
4. Proceed to Story 1.10 (hardware validation)

### If Issues Discovered

1. Document issues in "Known Issues" section
2. Assess severity:
   - **Critical:** Blocks hardware deployment (must fix)
   - **Major:** Significant problem (should fix)
   - **Minor:** Cosmetic or edge case (can defer)

3. Fix critical/major issues:
   - Update code in `src/`
   - Rebuild: `make clean && make test`
   - Re-test affected areas

4. Update results document with fixes and re-test results

---

## Questions?

If you encounter:
- **Plugin won't load:** Check VCV Rack log (Help → Show Log) for errors
- **No synthesis sound:** Verify MIDI routing and gate signal
- **Audio artifacts:** Note conditions and parameter values when they occur
- **Crashes:** Note exact steps to reproduce, check VCV Rack log

Document all observations in the results document - even unexpected behavior is valuable data for hardware validation (Story 1.10).
