# Manual Validation Required - Story 2.6

## Summary

**Story 2.6: Sound Quality A/B Validation** cannot be executed by the automated development agent (Amelia) because it requires:

1. Physical hardware (Mutable Instruments Elements module)
2. Audio recording equipment
3. Manual audio analysis
4. Human beta testers
5. Subjective sound assessment

## What the Agent Did

The agent created comprehensive documentation to support manual validation:

### 1. Overview and Guide
**File**: `/docs/validation/README.md`
- Complete overview of validation process
- Requirements and prerequisites
- Expected outputs and deliverables
- Estimated time: 8-16 hours

### 2. Step-by-Step Checklist
**File**: `/docs/validation/test-execution-checklist.md`
- 11 detailed phases
- 200+ checkboxes covering every task
- Guides you through complete validation workflow
- Start here when ready to execute

### 3. Report Template
**File**: `/docs/validation/sound-quality-report-template.md`
- Pre-formatted validation report
- Sections for all 7 acceptance criteria
- Tables for measurements and results
- Fill this out as you complete testing

### 4. Test Patch Reference
**File**: `/docs/validation/test-patch-reference.md`
- Quick reference for 5 test patches
- MIDI test sequence specifications
- Recording settings and file naming
- Analysis settings
- Print this for testing sessions

## What You Need to Do

### Prerequisites
1. **Obtain Hardware**: Borrow or access Mutable Instruments Elements module
2. **Recording Setup**: Audio interface, cables, MIDI controller
3. **Analysis Software**: Install Sonic Visualiser (free) or Audacity
4. **Time**: Block 8-16 hours for testing + 1 week for beta testers

### Execution Steps
1. Read `/docs/validation/README.md` for overview
2. Follow `/docs/validation/test-execution-checklist.md` step-by-step
3. Use `/docs/validation/test-patch-reference.md` as quick reference
4. Fill out `/docs/validation/sound-quality-report-template.md` during testing
5. Save completed report as `sound-quality-report.md`
6. Update story file with results

### Test Patches (5 total)
1. **Bowed String** - Tests bow exciter and string geometry
2. **Blown Pipe** - Tests blow exciter and tube geometry
3. **Strike Bell** - Tests strike exciter and metallic geometry
4. **Mixed Exciters** - Tests multiple exciters simultaneously
5. **Extreme Settings** - Tests edge cases

### MIDI Test Sequences (4 total)
1. **Pitch Accuracy** - C2, C3, C4, C5, C6 (validates pitch across MIDI range)
2. **Chromatic Scale** - C4 through C5 (validates semitone spacing)
3. **Parameter Sweeps** - Geometry, Brightness, Damping 0-100%
4. **Dynamic Response** - Velocities 30, 60, 90, 127

### Recording Requirements
- **Total recordings**: 60 files (5 patches × 6 sequences × 2 systems)
- **Format**: 48kHz or 96kHz, 24-bit, WAV uncompressed
- **Level**: -6dB to -12dB peak (no clipping)
- **Organization**: Separate folders for hardware and nt_elements

### Analysis Requirements
- **Spectral Analysis**: FFT plots comparing frequency response
- **Pitch Accuracy**: Measure fundamental frequencies (< 1 cent tolerance)
- **Blind Listening Tests**: Recruit 3+ beta testers with Elements experience
- **Success**: Testers should guess randomly (< 60% accuracy)

## Acceptance Criteria to Validate

All 7 acceptance criteria must be addressed:

1. ✓ Side-by-side recordings captured (hardware Elements vs nt_elements)
2. ✓ Spectral analysis shows matching frequency response
3. ✓ At least 3 beta testers confirm sound authenticity in blind tests
4. ✓ Pitch accuracy validated across full MIDI range
5. ✓ All exciter modes (bow/blow/strike) behave identically
6. ✓ Resonator response matches hardware across geometry settings
7. ✓ Any differences documented with explanation

## Success Criteria

**PASS** if:
- Spectral analysis shows matching frequency response
- Pitch accuracy within 1 cent across MIDI range
- Blind test accuracy < 60% (indistinguishable)
- All exciter modes behave identically
- Resonator response matches across all parameters
- Any minor differences are documented and acceptable

**FAIL** if:
- Pitch deviation > 1 cent
- Timbral differences detected in blind tests
- Missing harmonics or resonances
- Exciter/resonator behavior mismatch
- Unacceptable differences requiring code fixes

## After Validation

### If PASS:
1. Save completed report as `docs/validation/sound-quality-report.md`
2. Check off all tasks in `docs/stories/2-6-sound-quality-a-b-validation.md`
3. Update story status from "blocked" to "review"
4. Update sprint-status.yaml: `2-6-sound-quality-a-b-validation: review`
5. Proceed to Story 2.7: Final Testing and Release Preparation

### If FAIL:
1. Document all differences with root cause analysis
2. Create new stories for required fixes
3. Re-run validation after fixes are implemented
4. Update story status to "in-progress" with fix requirements

## Questions?

If you have questions or need clarification:
1. Review the detailed Dev Notes in the story file
2. Consult the story context file: `docs/stories/2-6-sound-quality-a-b-validation.context.md`
3. Reference PRD: `docs/PRD.md` (NFR002: Sound Quality)
4. Reference Architecture: `docs/architecture.md` (ADR-001, ADR-002)

## Current Status

**Story File**: `docs/stories/2-6-sound-quality-a-b-validation.md`
**Story Status**: blocked (requires hardware access)
**Sprint Status**: ready-for-dev (waiting for manual execution)
**Blocker**: Physical access to Mutable Instruments Elements module

---

**The validation documentation is complete and ready for you to execute when hardware is available.**

**Good luck! The goal is to confirm that nt_elements achieves authentic Elements sound quality worthy of release.**
