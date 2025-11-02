# nt_elements Sound Quality Validation

This directory contains artifacts for Story 2.6: Sound Quality A/B Validation.

## Overview

Story 2.6 is a **manual validation story** that requires physical hardware testing to verify that nt_elements sounds identical to the Mutable Instruments Elements Eurorack module.

## Required for Story Completion

This story **cannot be executed by an automated development agent**. It requires:

1. Physical access to Mutable Instruments Elements hardware module
2. Audio recording equipment (interface, cables, etc.)
3. MIDI controller or sequencer
4. Spectral analysis software (Sonic Visualiser, Audacity, etc.)
5. Beta testers with Elements experience for blind listening tests
6. Manual audio analysis and subjective sound quality assessment

## Artifacts in This Directory

### `test-execution-checklist.md`
**Step-by-step checklist** for executing the validation manually. This comprehensive checklist covers:
- Test environment preparation
- Hardware setup and configuration
- MIDI test sequence recording
- Spectral analysis procedures
- Pitch accuracy validation
- Exciter mode validation
- Resonator response validation
- Blind listening test coordination
- Difference documentation
- Validation report creation

**Start here** when executing Story 2.6.

### `sound-quality-report-template.md`
**Report template** to be filled out during validation. Includes sections for:
- Executive Summary
- Test Methodology
- Test Results (all 7 acceptance criteria)
- Spectral Analysis
- Blind Listening Tests
- Pitch Accuracy Measurements
- Exciter Mode Validation
- Resonator Response Validation
- Documented Differences
- Overall Conclusion and Production Readiness

Fill this out as you complete testing, then save as `sound-quality-report.md` when complete.

## Test Execution Workflow

1. **Preparation Phase**:
   - Obtain hardware Elements module (borrow or purchase)
   - Set up recording equipment
   - Install spectral analysis software
   - Review test-execution-checklist.md thoroughly

2. **Recording Phase**:
   - Configure 5 test patches on both hardware and nt_elements
   - Record MIDI test sequences (pitch accuracy, chromatic scale, parameter sweeps, dynamics)
   - Save all recordings with clear naming convention
   - Organize recordings in subdirectories (hardware/, nt_elements/)

3. **Analysis Phase**:
   - Perform spectral analysis using FFT plots
   - Measure pitch accuracy across MIDI range
   - Compare exciter mode behavior
   - Compare resonator response across parameter ranges
   - Generate spectral overlay images and spectrograms

4. **Beta Testing Phase**:
   - Recruit 3+ beta testers with Elements experience
   - Prepare blind test materials (randomized A/B pairs)
   - Distribute test files and instructions
   - Collect responses and calculate accuracy
   - Analyze tester feedback

5. **Documentation Phase**:
   - Document all discovered differences with root cause analysis
   - Fill out sound-quality-report-template.md
   - Include all spectral plots and analysis results
   - Write conclusion with clear PASS/FAIL determination
   - Provide production readiness recommendation

6. **Story Completion**:
   - Save final report as `sound-quality-report.md`
   - Check off all tasks in story file
   - Update story status to "review" (if PASS) or "in-progress" (if fixes needed)
   - Update File List in story with all created files

## Expected Outputs

After completing Story 2.6, this directory should contain:

- `sound-quality-report.md` - Completed validation report
- `recordings/` - Directory with all audio recordings
  - `hardware/` - Hardware Elements recordings
  - `nt_elements/` - nt_elements recordings
- `spectral-analysis/` - Directory with spectral plots
  - Spectrum overlay images
  - Spectrogram images
  - Analysis charts
- Raw test data (optional):
  - CSV files with frequency measurements
  - Pitch accuracy tables
  - Beta tester response data

## Acceptance Criteria Summary

Story 2.6 requires validating 7 acceptance criteria:

1. **AC1**: Side-by-side recordings captured (hardware Elements vs nt_elements)
2. **AC2**: Spectral analysis shows matching frequency response
3. **AC3**: At least 3 beta testers confirm sound authenticity in blind tests
4. **AC4**: Pitch accuracy validated across full MIDI range
5. **AC5**: All exciter modes (bow/blow/strike) behave identically
6. **AC6**: Resonator response matches hardware across geometry settings
7. **AC7**: Any differences documented with explanation

All 7 acceptance criteria must be addressed (PASS or FAIL) for story completion.

## Success Criteria

**PASS conditions**:
- Spectral analysis shows matching frequency response (AC2)
- Pitch accuracy within 1 cent across MIDI range (AC4)
- Blind test accuracy < 60% (testers cannot distinguish, AC3)
- All exciter modes behave identically (AC5)
- Resonator response matches across all parameters (AC6)
- Any minor differences are documented and acceptable (AC7)

**FAIL conditions**:
- Pitch deviation > 1 cent
- Timbral differences detected in blind tests (accuracy > 60%)
- Missing harmonics or resonances
- Exciter mode behavior mismatch
- Resonator response mismatch
- Unacceptable differences requiring code fixes

## Test Patches

Five test patches are specified for validation:

1. **Test Patch 1: Bowed String** - Bow 80%, String geometry 15%, tests bow exciter
2. **Test Patch 2: Blown Pipe** - Blow 80%, Tube geometry 45%, tests blow exciter
3. **Test Patch 3: Strike Bell** - Strike 85%, Metallic geometry 75%, tests strike exciter
4. **Test Patch 4: Mixed Exciters** - Bow 40%, Blow 40%, Strike 20%, Mid-range geometry 50%
5. **Test Patch 5: Extreme Settings** - Strike 100%, Geometry 100%, Brightness 100%, Damping 0%

Each patch must be configured identically on hardware Elements and nt_elements.

## MIDI Test Sequences

Four MIDI test sequences are defined:

1. **Pitch Accuracy**: C2, C3, C4, C5, C6 (3 seconds each) - validates pitch across MIDI range
2. **Chromatic Scale**: C4 through C5 (13 semitones, 1 second each) - validates semitone spacing
3. **Parameter Sweeps**: Hold C4, sweep Geometry/Brightness/Damping 0-100% over 10 seconds each
4. **Dynamic Response**: C4 at velocities 30, 60, 90, 127 (2 seconds each)

Record each sequence with all 5 test patches on both hardware and nt_elements.

## Tools and Software

**Recommended Spectral Analysis Tools**:
- **Sonic Visualiser** (free, recommended) - Excellent for FFT plots and spectrograms
- **Audacity** (free) - Basic spectrum analysis
- **MATLAB or Python** (advanced) - For automated frequency measurements
- **Voxengo SPAN** (free VST) - Real-time spectrum analyzer

**Recording Software** (your choice):
- Audacity, Reaper, Logic Pro, Ableton Live, Pro Tools, etc.
- Must support 48kHz or 96kHz, 24-bit recording

**Audio Interface Requirements**:
- At least 2 input channels (record sequentially) or 4+ channels (record simultaneously)
- 48kHz or 96kHz sample rate support
- 24-bit depth
- Clean preamps with low noise floor

## Estimated Time

**Total validation time**: 8-16 hours (depending on experience)

- Test setup: 1-2 hours
- Recording sessions: 2-4 hours
- Spectral analysis: 2-4 hours
- Beta test coordination: 1 week (async)
- Report writing: 2-4 hours

## Questions or Issues

If you encounter issues during validation:

1. **Hardware not available**: Story should be marked "blocked" until hardware can be obtained
2. **Significant differences found**: Document thoroughly, investigate root causes, may require code fixes
3. **Beta testers unavailable**: Minimum 3 required - consider posting in multiple communities
4. **Analysis tools unfamiliar**: Sonic Visualiser tutorials available online, or use simpler tools like Audacity

## References

- Story file: `docs/stories/2-6-sound-quality-a-b-validation.md`
- Story context: `docs/stories/2-6-sound-quality-a-b-validation.context.md`
- PRD quality requirements: `docs/PRD.md` (NFR002: Sound Quality)
- Architecture decisions: `docs/architecture.md` (ADR-001, ADR-002)

---

**Good luck with the validation! The goal is to confirm that nt_elements achieves authentic Elements sound quality worthy of release.**
