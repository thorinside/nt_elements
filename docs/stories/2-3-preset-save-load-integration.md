# Story 2.3: Preset Save/Load Integration

Status: ready-for-dev

## Story

As a user,
I want to save and load my patches using NT's preset system,
So that I can recall my favorite sounds quickly.

## Acceptance Criteria

1. All parameter states serialize correctly to NT preset format
2. Presets save via NT's standard save mechanism
3. Presets load and restore all parameters accurately
4. Preset switching does not cause audio glitches
5. Sample rate mode (32/48kHz) saved with preset
6. Preset names display in NT interface

## Tasks / Subtasks

- [ ] Research NT preset system (AC: #1-3, #6)
  - [ ] Study distingNT_API preset documentation
  - [ ] Identify preset save/load callbacks or mechanisms
  - [ ] Understand parameter serialization format
  - [ ] Document NT preset system architecture
  - [ ] Note any special handling required

- [ ] Verify automatic parameter serialization (AC: #1, #2)
  - [ ] Confirm NT framework auto-serializes parameter array
  - [ ] Test saving preset via NT save mechanism
  - [ ] Verify all parameters included in saved preset
  - [ ] Check preset file format (if accessible)
  - [ ] Document what gets saved automatically

- [ ] Implement preset load callback (AC: #3)
  - [ ] Add preset_loaded() callback if required by NT API
  - [ ] Reinitialize Elements DSP with loaded parameters
  - [ ] Apply all parameter values to Elements Patch
  - [ ] Update display to reflect loaded parameters
  - [ ] Verify complete state restoration

- [ ] Implement glitch-free preset switching (AC: #4)
  - [ ] Mute output briefly during preset load
  - [ ] Apply all parameters atomically
  - [ ] Reinitialize any stateful components
  - [ ] Fade output back in smoothly
  - [ ] Test rapid preset switching (no clicks/pops)

- [ ] Verify sample rate mode persistence (AC: #5)
  - [ ] Save preset in 48kHz mode
  - [ ] Switch to 32kHz mode
  - [ ] Load preset (should restore 48kHz mode)
  - [ ] Verify mode parameter included in serialization
  - [ ] Test with multiple presets in different modes

- [ ] Test preset save functionality (AC: #1, #2)
  - [ ] Configure unique parameter settings
  - [ ] Save preset via NT interface
  - [ ] Verify preset saved successfully
  - [ ] Change parameters to different values
  - [ ] Verify preset file contains correct data (if accessible)

- [ ] Test preset load functionality (AC: #3, #6)
  - [ ] Load previously saved preset
  - [ ] Verify all parameters restored correctly
  - [ ] Listen to synthesis (should match saved state)
  - [ ] Verify display shows correct parameter values
  - [ ] Verify preset name displays in NT interface

- [ ] Test preset switching scenarios (AC: #4)
  - [ ] Create multiple presets with different settings
  - [ ] Rapidly switch between presets
  - [ ] Listen for audio glitches or artifacts
  - [ ] Verify smooth transitions
  - [ ] Test preset switching during note-on (held note)

- [ ] Create test preset library (AC: #1-6)
  - [ ] Create 3-5 test presets with distinct settings
  - [ ] Document expected parameter values for each
  - [ ] Use test presets for validation
  - [ ] Save test presets for factory preset reference (Story 2.4)
  - [ ] Document test preset characteristics

- [ ] Desktop testing in nt_emu (AC: #1-6)
  - [ ] Test preset save/load in nt_emu (if supported)
  - [ ] Verify all acceptance criteria in desktop environment
  - [ ] Document any nt_emu preset limitations
  - [ ] Prepare for hardware validation

- [ ] Hardware testing (AC: #1-6)
  - [ ] Deploy to disting NT hardware
  - [ ] Test preset save/load on actual hardware
  - [ ] Verify preset persistence across power cycles
  - [ ] Test preset management via NT interface
  - [ ] Document hardware preset validation results

## Dev Notes

### disting NT Preset System

**Automatic Parameter Serialization:**
The disting NT framework automatically serializes the parameter array (`_NT_algorithm::v[]`) when saving presets. No manual serialization code required for basic parameters.

**What Gets Saved:**
- All parameters in the `v[]` array (automatically)
- Parameter indices and values
- Plugin identifier (for loading correct plugin)

**What Doesn't Get Saved (Unless Custom):**
- Internal algorithm state (buffer contents, temporary values)
- Non-parameter configuration (these should be parameters if they need saving)

**Preset Callbacks (if provided by NT API):**
```cpp
void preset_saved(_NT_algorithm* self, const char* preset_name);
void preset_loaded(_NT_algorithm* self, const char* preset_name);
```

**Note:** Exact callback names and signatures depend on distingNT_API version. Consult documentation.

### Parameter-Based State Management

**Key Principle:**
All user-configurable state should be stored in parameters. This ensures automatic preset save/load.

**Verification:**
- Sample rate mode: Stored in kParamSampleRateMode (parameter)
- Page state: Internal UI state, doesn't need saving
- Parameter values: All in `v[]` array (auto-saved)
- Elements Patch: Derived from parameters (regenerated on load)

**If something doesn't persist in presets, make it a parameter.**

### Preset Load Handling

**Preset Load Sequence:**
1. NT framework loads preset file
2. NT framework updates all parameter values in `v[]` array
3. NT framework calls preset_loaded() callback (if implemented)
4. Plugin updates internal state from new parameter values
5. Display updates to reflect new parameters

**Implementation:**
```cpp
void preset_loaded(_NT_algorithm* self, const char* preset_name) {
    _nt_elementsAlgorithm* pThis = (_nt_elementsAlgorithm*)self;

    // 1. Mute output
    bool previous_gate = pThis->patch.gate;
    pThis->patch.gate = false;

    // 2. Apply all parameters to Elements Patch
    for (int i = 0; i < kNumParameters; ++i) {
        parameterChanged(self, i);
    }

    // 3. Update display
    pThis->display_dirty = true;

    // 4. Restore gate (unmute)
    pThis->patch.gate = previous_gate;
}
```

**Glitch Prevention:**
- Briefly mute during parameter updates
- Apply all parameters before unmuting
- Avoid partial state updates (all-or-nothing)

### Glitch-Free Preset Switching

**Challenge:**
Changing many parameters at once can cause audio artifacts if not handled carefully.

**Solutions:**

**Option A: Brief Mute**
- Set gate = false during preset load
- Apply all parameter changes
- Set gate = true after updates
- Simple, effective, brief silence acceptable

**Option B: Smooth Crossfade**
- Fade out current sound
- Apply parameter changes at zero output
- Fade in new sound
- More complex, smoother transition

**Recommendation:**
Option A for simplicity. Brief mute (< 10ms) is imperceptible and avoids complexity.

### Sample Rate Mode Persistence

**Implementation:**
Sample rate mode stored as parameter (kParamSampleRateMode). NT framework automatically saves/loads this parameter with presets.

**Testing:**
1. Set mode to 48kHz, save preset "Test48"
2. Set mode to 32kHz, save preset "Test32"
3. Load "Test48" → verify mode switches to 48kHz
4. Load "Test32" → verify mode switches to 32kHz

**Expected Behavior:**
Each preset remembers its sample rate mode. Loading preset restores mode.

### Preset Name Display

**NT Interface:**
Preset names displayed by NT framework in preset browser. Plugin doesn't need to implement preset name display.

**Preset Naming:**
User names presets via NT interface. Plugin receives preset name in preset_loaded() callback (informational only).

### Testing Strategy

**Test Preset Library:**
Create diverse presets for validation:

1. **Bell Strike** - High brightness, short damping, strike exciter
2. **Bowed String** - Bow exciter, medium geometry, long damping
3. **Blown Pipe** - Blow exciter, tube geometry, medium brightness
4. **Deep Resonance** - Low tuning, long damping, high reverb

**Validation Procedure:**
For each test preset:
1. Configure parameters manually
2. Save preset
3. Change parameters to different values
4. Load preset
5. Verify all parameters restored correctly
6. Listen to synthesis (should sound as expected)

**Edge Cases:**
- Load preset during note-on (held note)
- Rapid preset switching
- Load preset in different sample rate mode
- Power cycle and reload (persistence test)

### Common Preset Issues

**Parameters Don't Restore:**
- Verify parameter is in `v[]` array
- Check parameter index is correct
- Ensure parameterChanged() handles parameter

**Audio Glitches on Load:**
- Implement mute during load
- Apply parameters atomically
- Clear audio buffers if needed

**Preset Names Don't Show:**
- NT framework handles display
- Check NT preset browser functionality
- Verify preset file format

**Sample Rate Mode Not Persistent:**
- Verify kParamSampleRateMode in parameter array
- Check parameterChanged() handles mode switch
- Test mode parameter save/load explicitly

### Project Structure

**Files Modified:**
- src/nt_elements.cpp - Add preset_loaded() callback (if required)
- src/parameter_adapter.h - Verify all state in parameters

**Files Created:**
- docs/testing/preset-test-library.md (optional) - Document test presets

### References

- [Source: docs/epics.md#Story 2.3] - Full story specification and acceptance criteria
- [Source: docs/PRD.md#FR012] - Preset save/load functional requirement
- [Source: docs/architecture.md#Preset System] - Serialization and factory presets
- [Source: docs/architecture.md#Data Architecture] - NT parameter array structure
- [Source: docs/PRD.md#NFR003] - Reliability requirement (no glitches)

## Dev Agent Record

### Context Reference

- [Story Context](./2-3-preset-save-load-integration.context.md)

### Agent Model Used

<!-- Model information will be filled in during implementation -->

### Debug Log References

### Completion Notes List

### File List
