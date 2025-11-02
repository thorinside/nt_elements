# Testing Documentation for Story 1.9

## Story Status: BLOCKED - Manual Testing Required

Story 1.9 (Desktop Testing Validation) cannot be fully completed by automated development agents because it requires **manual validation in VCV Rack with the nt_emu module**.

## What's Been Done

The dev agent has completed all automated work possible for this story:

### ✅ Completed Automatically

1. **Test Validation Checklist** (`test-validation-checklist.md`)
   - Detailed test procedures for all Epic 1 features (Stories 1.1-1.8)
   - VCV Rack patch configuration
   - Pass/fail criteria for each test
   - Systematic testing workflow

2. **Test Results Document Template** (`desktop-validation-results.md`)
   - Pre-filled sections for all test areas
   - Tables ready for data entry
   - Acceptance criteria tracking
   - Known issues section

3. **Manual Testing Guide** (`MANUAL-TESTING-GUIDE.md`)
   - Step-by-step instructions for executing tests
   - Quick reference for tools and metrics
   - Guidance on what to do if issues found

4. **Build Verification**
   - Plugin compiles successfully: `plugins/nt_elements.dylib` (442 KB)
   - No compilation errors (only upstream Mutable Instruments warnings)

5. **Memory Usage Analysis** (AC #4 partially complete)
   - Analyzed allocations from code
   - SRAM: ~8.3 KB (within 128 KB limit) ✅
   - DRAM: 64 KB (within 2 MB limit) ✅
   - DTC: ~2-4 KB (within 64 KB limit) ✅
   - Total: ~72-76 KB (well within hardware limits) ✅

## What Requires Manual Testing

The following acceptance criteria **require human interaction** with VCV Rack:

### ⏳ Requires Manual Testing

**AC #1: All features from Stories 1.1-1.8 work in nt_emu**
- Validate plugin loading, audio passthrough (Story 1.3)
- Validate Elements synthesis sound, modal character (Story 1.4)
- Validate pitch accuracy across C1-C6 using spectrum analyzer (Story 1.5)
- Validate MIDI note-on/off, pitch bend responsiveness (Story 1.6)
- Validate parameter adapter (geometry, brightness, damping) (Story 1.7)
- Validate bus routing and external audio excitation (Story 1.8)

**AC #2: Performance metrics captured (CPU usage estimate)**
- Measure VCV Rack CPU % in various conditions
- Estimate hardware CPU usage (desktop CPU × 5-10)

**AC #3: No crashes during 30-minute stress test**
- Run plugin continuously for 30 minutes
- Send MIDI bursts, parameter changes
- Monitor for crashes, glitches, memory leaks

**AC #5: Test results documented for hardware comparison**
- Fill in test results document with actual measurements
- Create baseline for Story 1.10 hardware validation

## How to Proceed

### For Neal (Manual Testing)

1. **Read the Manual Testing Guide:**
   ```
   docs/testing/MANUAL-TESTING-GUIDE.md
   ```

2. **Set up VCV Rack environment:**
   - Install VCV Rack v2.x
   - Install nt_emu module
   - Install required modules (MIDI-CV, Scope, Spectrum Analyzer, VCO, Audio output)

3. **Load the plugin:**
   ```
   plugins/nt_elements.dylib
   ```

4. **Execute tests using the checklist:**
   ```
   docs/testing/test-validation-checklist.md
   ```

5. **Fill in results document:**
   ```
   docs/testing/desktop-validation-results.md
   ```
   Replace all `[TBD]` placeholders with actual results

6. **Complete story tasks:**
   - Mark remaining tasks in story file as complete
   - Fix any issues discovered
   - Update story Status to "review" when done
   - Update `docs/sprint-status.yaml` from "blocked" to "review"

### For Automated Agents

This story is **blocked** until manual testing completes. Automated agents cannot:
- Run VCV Rack or load plugins
- Listen to audio or assess synthesis quality
- Measure CPU usage from VCV Rack GUI
- Perform 30-minute runtime observation
- Make subjective judgments about timbre or audio artifacts

## Files in This Directory

| File | Purpose | Status |
|------|---------|--------|
| `README.md` | This file - explains testing situation | Documentation |
| `MANUAL-TESTING-GUIDE.md` | Step-by-step manual testing instructions | Ready for use |
| `test-validation-checklist.md` | Detailed test procedures and pass/fail criteria | Ready for use |
| `desktop-validation-results.md` | Test results template to fill in | Awaiting manual testing |

## Acceptance Criteria Status

| AC | Criterion | Status | Notes |
|----|-----------|--------|-------|
| #1 | All features work in nt_emu | ⏳ MANUAL TESTING REQUIRED | Need VCV Rack |
| #2 | Performance metrics captured | ⏳ MANUAL TESTING REQUIRED | Need VCV CPU measurements |
| #3 | No crashes during 30-min test | ⏳ MANUAL TESTING REQUIRED | Need runtime observation |
| #4 | Memory usage within bounds | ✅ PASS (code analysis) | ~72-76 KB total |
| #5 | Test results documented | 🔄 TEMPLATE READY | Awaiting manual test data |

## Next Steps

**Once manual testing is complete:**

1. All tasks in story file marked complete ✅
2. Story Status updated to "review"
3. Sprint status updated from "blocked" to "review"
4. Baseline established for Story 1.10 (hardware validation)

**If issues are discovered during testing:**

1. Document in "Known Issues" section
2. Fix critical/major issues
3. Rebuild: `make clean && make test`
4. Re-test affected areas
5. Update results document

---

**Questions?** See the Manual Testing Guide for detailed instructions and troubleshooting tips.
