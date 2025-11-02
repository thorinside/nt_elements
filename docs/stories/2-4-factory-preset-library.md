# Story 2.4: Factory Preset Library

Status: ready-for-dev

## Story

As a user,
I want a curated set of factory presets demonstrating different sounds,
So that I can quickly explore nt_elements' capabilities and learn the synthesis.

## Acceptance Criteria

1. Minimum 8 factory presets covering diverse timbres:
   - Bell Strike (metallic resonance)
   - Bowed String (smooth sustain)
   - Blown Pipe (airy, breathy)
   - Wooden Mallet (percussive, warm)
   - Glass Chime (bright, crystalline)
   - Bass Drum (deep, resonant)
   - Ambient Pad (evolving texture)
   - Plucked String (attack + decay)
2. Each preset named descriptively
3. Presets ship with plugin
4. Brief preset descriptions documented

## Tasks / Subtasks

- [ ] Design factory preset library (AC: #1, #2)
  - [ ] Plan 8+ presets covering diverse timbres
  - [ ] Ensure presets demonstrate different synthesis modes
  - [ ] Cover exciter types (bow, blow, strike)
  - [ ] Cover resonator geometries (string, tube, mixed)
  - [ ] Plan preset names (clear, descriptive)

- [ ] Create Bell Strike preset (AC: #1)
  - [ ] Configure strike exciter (high level)
  - [ ] Set metallic geometry
  - [ ] Set high brightness
  - [ ] Set medium-short damping
  - [ ] Test and refine sound
  - [ ] Document parameter values

- [ ] Create Bowed String preset (AC: #1)
  - [ ] Configure bow exciter (high level)
  - [ ] Set string geometry
  - [ ] Set medium brightness
  - [ ] Set long damping (sustained)
  - [ ] Test and refine sound
  - [ ] Document parameter values

- [ ] Create Blown Pipe preset (AC: #1)
  - [ ] Configure blow exciter (high level)
  - [ ] Set tube geometry
  - [ ] Set medium brightness
  - [ ] Set medium damping
  - [ ] Test and refine sound
  - [ ] Document parameter values

- [ ] Create Wooden Mallet preset (AC: #1)
  - [ ] Configure strike exciter (medium level)
  - [ ] Set warm geometry (mid-range)
  - [ ] Set medium-low brightness
  - [ ] Set medium damping
  - [ ] Test and refine sound
  - [ ] Document parameter values

- [ ] Create Glass Chime preset (AC: #1)
  - [ ] Configure strike exciter (light touch)
  - [ ] Set bright/crystalline geometry
  - [ ] Set high brightness
  - [ ] Set medium-long damping
  - [ ] Add reverb for spaciousness
  - [ ] Test and refine sound
  - [ ] Document parameter values

- [ ] Create Bass Drum preset (AC: #1)
  - [ ] Configure strike exciter (heavy hit)
  - [ ] Set low geometry (deep resonance)
  - [ ] Set low brightness (dark)
  - [ ] Set medium damping
  - [ ] Set low tuning (coarse tune down)
  - [ ] Test and refine sound
  - [ ] Document parameter values

- [ ] Create Ambient Pad preset (AC: #1)
  - [ ] Configure bow + blow exciters (mixed)
  - [ ] Set evolving geometry
  - [ ] Set medium-high brightness
  - [ ] Set long damping
  - [ ] Add significant reverb
  - [ ] Test and refine sound
  - [ ] Document parameter values

- [ ] Create Plucked String preset (AC: #1)
  - [ ] Configure strike exciter (quick attack)
  - [ ] Set string geometry
  - [ ] Set medium-high brightness
  - [ ] Set medium damping (natural decay)
  - [ ] Test and refine sound
  - [ ] Document parameter values

- [ ] Implement preset data structure (AC: #3)
  - [ ] Define factory preset structure in code
  - [ ] Store parameter values for each preset
  - [ ] Store preset names
  - [ ] Implement preset initialization function
  - [ ] Document preset data format

- [ ] Integrate factory presets with NT system (AC: #3)
  - [ ] Research NT factory preset mechanism
  - [ ] Implement factory preset loading
  - [ ] Ensure presets appear in NT preset browser
  - [ ] Verify factory presets ship with plugin
  - [ ] Test preset loading from factory set

- [ ] Create preset documentation (AC: #4)
  - [ ] Document each preset with description
  - [ ] Note key parameter settings for each
  - [ ] Explain synthesis technique used
  - [ ] Provide usage suggestions
  - [ ] Add to README or user guide

- [ ] Test all factory presets (AC: #1-4)
  - [ ] Load each preset individually
  - [ ] Play MIDI notes (C2, C4, C6) for each
  - [ ] Verify preset sounds as intended
  - [ ] Check preset names display correctly
  - [ ] Test on both desktop and hardware

- [ ] Refine presets based on testing (AC: #1)
  - [ ] Adjust parameter values for best sound
  - [ ] Ensure presets are musically useful
  - [ ] Verify diverse timbre coverage
  - [ ] Test presets with different MIDI velocities (if supported)
  - [ ] Finalize preset library

## Dev Notes

### Factory Preset Design Goals

**Diversity:**
Cover wide range of synthesis capabilities:
- Different exciter types (bow, blow, strike)
- Different resonator geometries (string, tube, mixed)
- Different brightness levels (dark to bright)
- Different damping times (short percussive to long sustained)
- With and without reverb

**Educational Value:**
Each preset demonstrates specific synthesis technique:
- Bell Strike: High brightness + metallic geometry
- Bowed String: Bow exciter + long sustain
- Blown Pipe: Blow exciter + tube geometry
- Etc.

**Musical Utility:**
Presets should be immediately useful in music production:
- Recognizable timbres (bell, string, pipe, drum)
- Playable across MIDI range
- Responsive to performance gestures
- Starting points for user customization

### Preset Parameter Templates

**Bell Strike:**
```
Page 1 (Exciter):
- Bow Level: 0%
- Blow Level: 0%
- Strike Level: 90%
- Bow Timbre: 50%
- Blow Timbre: 50%

Page 2 (Resonator):
- Geometry: 80% (metallic)
- Brightness: 75% (bright)
- Damping: 35% (medium-short)
- Position: 50%
- Inharmonicity: 60% (bell-like)

Page 3 (Space):
- Reverb Amount: 25%
- Reverb Size: 50%
- Reverb Damping: 40%

Page 4 (Performance):
- Coarse Tune: 50% (0 semitones)
- Fine Tune: 50% (0 cents)
- Output Level: 75%
- FM Amount: 0%
- Exciter Contour: 50%
```

**Bowed String:**
```
Page 1 (Exciter):
- Bow Level: 85%
- Blow Level: 0%
- Strike Level: 0%
- Bow Timbre: 60%
- Blow Timbre: 50%

Page 2 (Resonator):
- Geometry: 15% (string)
- Brightness: 55% (medium)
- Damping: 75% (long sustain)
- Position: 40%
- Inharmonicity: 10% (harmonic)

Page 3 (Space):
- Reverb Amount: 20%
- Reverb Size: 45%
- Reverb Damping: 50%

Page 4 (Performance):
- Coarse Tune: 50%
- Fine Tune: 50%
- Output Level: 70%
- FM Amount: 0%
- Exciter Contour: 60%
```

**Note:** Full parameter specifications for all 8 presets should be documented during implementation.

### Preset Data Structure

**Code Representation:**
```cpp
struct FactoryPreset {
    const char* name;
    float parameters[kNumParameters];
};

const FactoryPreset factory_presets[] = {
    {
        "Bell Strike",
        {
            // Parameter values in order
            [kParamBowLevel] = 0.0f,
            [kParamBlowLevel] = 0.0f,
            [kParamStrikeLevel] = 90.0f,
            // ... all parameters
        }
    },
    {
        "Bowed String",
        {
            // Parameter values
        }
    },
    // ... more presets
};

const int num_factory_presets = sizeof(factory_presets) / sizeof(FactoryPreset);
```

**Loading Function:**
```cpp
void load_factory_preset(int preset_index) {
    if (preset_index < 0 || preset_index >= num_factory_presets) {
        return;  // Invalid index
    }

    const FactoryPreset* preset = &factory_presets[preset_index];

    // Copy parameters
    for (int i = 0; i < kNumParameters; ++i) {
        algorithm->v[i] = preset->parameters[i];
        parameterChanged(algorithm, i);
    }

    // Update display
    algorithm->display_dirty = true;
}
```

### NT Factory Preset Integration

**Implementation Options:**

**Option A: Preinstalled User Presets**
- Save factory presets as regular presets on SD card
- Ship with plugin (in dedicated folder)
- User can load like any preset
- Simple, no special code needed

**Option B: Embedded Factory Presets**
- Presets stored in plugin code (const data)
- Special factory preset browser in NT interface
- Requires NT API support for factory presets
- More elegant, always available

**Recommendation:**
Check NT API for factory preset support. If available, use Option B. Otherwise, use Option A (preinstalled files).

### Preset Documentation Format

**Documentation Structure:**
```markdown
## Factory Presets

### Bell Strike
**Description:** Bright metallic resonance with quick strike excitation. Ideal for bell-like tones, chimes, and percussive metallic sounds.

**Synthesis Technique:** High strike level, metallic geometry (80%), high brightness (75%), medium-short damping for characteristic bell decay.

**Usage:** Play higher notes (C4-C6) for chime-like tones. Lower notes (C2-C3) for large bell sounds. Adjust damping for longer/shorter ring.

**Key Parameters:**
- Strike Level: 90%
- Geometry: 80% (Metallic)
- Brightness: 75%
- Damping: 35%

---

### Bowed String
[Similar documentation]

---

[Continue for all 8 presets]
```

### Testing Strategy

**Per-Preset Testing:**
For each factory preset:
1. Load preset
2. Play MIDI note C4 (middle C)
3. Verify expected timbre
4. Play MIDI notes C2, C4, C6 (test range)
5. Verify preset works across range
6. Adjust if needed and re-save

**Comparative Testing:**
- Load all presets sequentially
- Verify distinct character for each
- Ensure no duplicate timbres
- Check for musical utility

**User Testing:**
- Ask beta testers to try factory presets
- Gather feedback on sound quality and utility
- Refine based on feedback

### Preset Refinement Process

**Iterative Development:**
1. Create initial preset based on template
2. Test preset with various MIDI notes
3. Adjust parameters for best sound
4. Compare to reference (hardware Elements if available)
5. Refine until preset sounds excellent
6. Document final parameter values

**Refinement Criteria:**
- Sound matches intended character (e.g., bell sounds bell-like)
- Preset is musically useful across range
- Parameters are well-balanced (no clipping, no silence)
- Preset demonstrates specific synthesis technique

### Project Structure

**Files Created:**
- src/factory_presets.h (new) - Factory preset data and loading
- docs/presets/factory-presets.md (new) - Preset documentation

**Files Modified:**
- src/nt_elements.cpp - Integrate factory preset loading

### References

- [Source: docs/epics.md#Story 2.4] - Full story specification and acceptance criteria
- [Source: docs/PRD.md#FR013] - Factory presets functional requirement
- [Source: docs/architecture.md#Preset System] - Factory preset implementation details
- [Source: docs/PRD.md#User Journeys] - User journey using factory presets

## Dev Agent Record

### Context Reference

- docs/stories/2-4-factory-preset-library.context.md

### Agent Model Used

<!-- Model information will be filled in during implementation -->

### Debug Log References

### Completion Notes List

### File List
