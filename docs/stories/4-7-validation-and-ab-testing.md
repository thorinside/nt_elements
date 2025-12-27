# Story 4.7: Validation and A/B Testing

Status: ready-for-dev

## Story

As a developer,
I want to validate all parameter and audio changes against the reference implementation,
So that nt_elements sounds and behaves correctly.

## Acceptance Criteria

1. A/B comparison with Audible Instruments Modal Synthesizer
2. Audio output levels match (both at similar perceived loudness)
3. All parameters function identically to reference
4. Flow, Mallet, Strength produce expected timbral changes
5. No regressions in existing functionality
6. CPU usage remains within acceptable bounds

## Tasks / Subtasks

- [ ] Task 1: Set up A/B testing environment (AC: 1)
  - [ ] Install Audible Instruments in VCV Rack
  - [ ] Create comparison patch with Modal Synthesizer and nt_elements
  - [ ] Route both to separate outputs for switching

- [ ] Task 2: Validate audio output levels (AC: 2)
  - [ ] Compare peak levels on oscilloscope/meter
  - [ ] Verify both produce similar loudness at unity gain
  - [ ] Document any level differences with explanation

- [ ] Task 3: Test existing parameters (AC: 3, 5)
  - [ ] Geometry: Full range sweep comparison
  - [ ] Brightness: Full range sweep comparison
  - [ ] Damping: Full range sweep comparison
  - [ ] Exciter levels (Bow/Blow/Strike): Verify behavior
  - [ ] Reverb parameters: Compare wet/dry mix

- [ ] Task 4: Test new parameters (AC: 4)
  - [ ] Flow: Verify air flow character changes (noise color)
  - [ ] Mallet: Verify strike character changes (hammer → plectrum)
  - [ ] Strength: Verify excitation intensity changes

- [ ] Task 5: Regression testing (AC: 5)
  - [ ] Test MIDI note input
  - [ ] Test pitch accuracy across range
  - [ ] Test preset save/load
  - [ ] Test parameter page navigation
  - [ ] Test OLED display updates

- [ ] Task 6: CPU profiling (AC: 6)
  - [ ] Deploy to disting NT hardware
  - [ ] Measure CPU usage with all parameters active
  - [ ] Compare to pre-Epic 4 baseline
  - [ ] Document CPU impact of new parameters

- [ ] Task 7: Document findings
  - [ ] Create validation report
  - [ ] List any behavioral differences
  - [ ] Note any issues discovered

## Dev Notes

- This story validates all work from Epic 4 stories 4.1-4.6 and 4.8-4.9
- Audible Instruments Modal Synthesizer is based on same Mutable Instruments Elements source
- CPU budget target: < 30% at 32kHz
- All parameters should function identically to reference implementation

### Test Procedures

**Level Matching Test:**
1. Set identical patch on both modules
2. Send same MIDI note
3. Compare peak output levels
4. Should be within ±1dB

**Parameter Sweep Test:**
1. Set parameter to minimum on both
2. Gradually increase to maximum
3. Listen for timbral changes matching

**CPU Test:**
1. Load on hardware NT
2. Note baseline CPU %
3. Activate all new parameters
4. Verify still < 30%

### Project Structure Notes

- No code changes in this story
- Testing and documentation only
- May create validation report document

### References

- [Source: docs/epic-4-audio-quality-and-parameters.md#Story 4.7]
- [Source: docs/architecture.md#Audio Quality]
- [Source: docs/architecture.md#Performance Considerations]
- Prerequisites: All previous Epic 4 stories (4.1-4.6, 4.8-4.9)

## Dev Agent Record

### Context Reference

- docs/stories/4-7-validation-and-ab-testing.context.md

### Agent Model Used

### Debug Log References

### Completion Notes List

### File List
