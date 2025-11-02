# Story 2.1: Complete Parameter Page System

Status: ready-for-dev

## Story

As a user,
I want all 4 parameter pages (Exciter, Resonator, Space, Performance) fully implemented,
So that I can access all Elements synthesis parameters via the NT interface.

## Acceptance Criteria

1. Page 1 (Exciter): All exciter parameters mapped (bow/blow balance, strike, contour, timbres)
2. Page 2 (Resonator): All resonator parameters mapped (geometry, brightness, damping, position, inharmonicity)
3. Page 3 (Space): Reverb parameters mapped (amount, size, damping)
4. Page 4 (Performance): Global parameters mapped (tuning, FM, output level)
5. Button navigation cycles through pages smoothly
6. OLED display shows current page and parameter values clearly
7. All 3 pots + 2 encoders functional on each page

## Tasks / Subtasks

- [ ] Expand parameter enumeration (AC: #1-4)
  - [ ] Add all Page 1 (Exciter) parameters to enum
  - [ ] Add all Page 2 (Resonator) parameters to enum
  - [ ] Add all Page 3 (Space) parameters to enum
  - [ ] Add all Page 4 (Performance) parameters to enum
  - [ ] Update kNumParameters count
  - [ ] Document complete parameter list

- [ ] Create parameter page structure (AC: #1-4, #7)
  - [ ] Create src/parameter_pages.h
  - [ ] Define 4-page structure with parameter mappings
  - [ ] Map each pot/encoder to parameter per page
  - [ ] Define page names and display labels
  - [ ] Document parameter organization

- [ ] Implement Page 1 (Exciter) parameter mappings (AC: #1)
  - [ ] Pot 1: Bow Level (bow/blow balance)
  - [ ] Pot 2: Blow Level
  - [ ] Pot 3: Strike Level
  - [ ] Encoder 1: Bow Timbre
  - [ ] Encoder 2: Blow Timbre
  - [ ] Map to Elements Patch exciter fields
  - [ ] Test exciter parameter changes

- [ ] Implement Page 2 (Resonator) parameter mappings (AC: #2)
  - [ ] Pot 1: Geometry (material type)
  - [ ] Pot 2: Brightness (filter cutoff)
  - [ ] Pot 3: Damping (decay rate)
  - [ ] Encoder 1: Position (strike position)
  - [ ] Encoder 2: Inharmonicity
  - [ ] Map to Elements Patch resonator fields
  - [ ] Test resonator parameter changes

- [ ] Implement Page 3 (Space) parameter mappings (AC: #3)
  - [ ] Pot 1: Reverb Amount
  - [ ] Pot 2: Reverb Size
  - [ ] Pot 3: Reverb Damping
  - [ ] Encoder 1: (future use or bypass toggle)
  - [ ] Encoder 2: (future use or reverb type)
  - [ ] Map to Elements Patch reverb fields
  - [ ] Test reverb parameter changes

- [ ] Implement Page 4 (Performance) parameter mappings (AC: #4)
  - [ ] Pot 1: Coarse Tune (octave/semitone)
  - [ ] Pot 2: Fine Tune (cents)
  - [ ] Pot 3: Output Level
  - [ ] Encoder 1: FM Amount
  - [ ] Encoder 2: Exciter Contour (envelope)
  - [ ] Implement tuning offset calculation
  - [ ] Implement output level scaling
  - [ ] Test performance parameter changes

- [ ] Implement page navigation (AC: #5)
  - [ ] Track current_page state variable
  - [ ] Implement button handler for page switching
  - [ ] Cycle through pages: 1 → 2 → 3 → 4 → 1
  - [ ] Update OLED display on page change
  - [ ] Verify smooth page transitions

- [ ] Implement pot/encoder routing per page (AC: #7)
  - [ ] Route pot/encoder inputs to correct parameter based on current page
  - [ ] Handle pot value changes
  - [ ] Handle encoder value changes
  - [ ] Handle encoder button presses (page navigation)
  - [ ] Verify all controls functional on all pages

- [ ] Update parameterChanged() for all parameters (AC: #1-4)
  - [ ] Expand switch statement for all new parameters
  - [ ] Implement conversion for each parameter (NT → Elements)
  - [ ] Add special handling for tuning (offset calculation)
  - [ ] Add special handling for output level (scaling)
  - [ ] Verify immediate parameter response

- [ ] Test each page independently (AC: #1-4, #7)
  - [ ] Test Page 1: Change all 5 controls, verify exciter behavior
  - [ ] Test Page 2: Change all 5 controls, verify resonator behavior
  - [ ] Test Page 3: Change all 3 controls, verify reverb behavior
  - [ ] Test Page 4: Change all 5 controls, verify performance changes
  - [ ] Document parameter ranges and effects

- [ ] Test page navigation (AC: #5, #6)
  - [ ] Navigate through all pages in sequence
  - [ ] Verify current page displayed correctly on OLED
  - [ ] Verify parameter values update on OLED
  - [ ] Test rapid page switching (no glitches)
  - [ ] Verify parameter states persist across page switches

- [ ] Desktop validation in nt_emu (AC: #1-7)
  - [ ] Load plugin in nt_emu
  - [ ] Test all parameters on all pages
  - [ ] Verify OLED display updates (if nt_emu supports display emulation)
  - [ ] Test page navigation
  - [ ] Document any issues discovered

## Dev Notes

### Parameter Page Organization

**Page 1 - Exciter:**
Physical modeling synthesis begins with excitation. This page controls how the resonator is excited (bowed, blown, struck).

**Controls:**
- Pot 1: Bow Level (0-100%) → patch.exciter_bow_level
- Pot 2: Blow Level (0-100%) → patch.exciter_blow_level
- Pot 3: Strike Level (0-100%) → patch.exciter_strike_level
- Encoder 1: Bow Timbre (0-100%) → patch.exciter_bow_timbre
- Encoder 2: Blow Timbre (0-100%) → patch.exciter_blow_timbre

**Design Rationale:**
Exciter levels control the balance between three excitation types. Timbres control the color/character of each exciter. This matches hardware Elements' exciter section.

**Page 2 - Resonator:**
The resonator is the core of Elements synthesis. Modal filters create the physical modeling character.

**Controls:**
- Pot 1: Geometry (0-100%) → patch.resonator_geometry
- Pot 2: Brightness (0-100%) → patch.resonator_brightness
- Pot 3: Damping (0-100%) → patch.resonator_damping
- Encoder 1: Position (0-100%) → patch.resonator_position
- Encoder 2: Inharmonicity (0-100%) → patch.resonator_modulation

**Design Rationale:**
These five parameters define the resonator's character. Geometry selects material type (string/tube/etc), brightness controls harmonic content, damping controls decay time, position affects timbre (where resonator is struck/bowed), inharmonicity adds metallic/bell-like character.

**Page 3 - Space (Reverb):**
Reverb section adds spatial dimension and ambience.

**Controls:**
- Pot 1: Reverb Amount (0-100%) → patch.reverb_amount
- Pot 2: Reverb Size (0-100%) → patch.reverb_time
- Pot 3: Reverb Damping (0-100%) → patch.reverb_diffusion
- Encoder 1: (reserved for future use or bypass)
- Encoder 2: (reserved for future use or reverb type)

**Design Rationale:**
Three primary reverb controls. Amount = wet/dry mix, Size = reverb time/room size, Damping = high-frequency damping (bright vs dark reverb).

**Page 4 - Performance:**
Global parameters affecting overall synthesis behavior.

**Controls:**
- Pot 1: Coarse Tune (-12 to +12 semitones) → tuning offset
- Pot 2: Fine Tune (-50 to +50 cents) → tuning offset
- Pot 3: Output Level (0-100%) → output scaling
- Encoder 1: FM Amount (0-100%) → patch frequency modulation
- Encoder 2: Exciter Contour (0-100%) → patch.exciter_envelope_shape

**Design Rationale:**
Performance controls don't fit into the three Elements sections. Tuning allows transposition without MIDI pitch changes. Output level controls final volume. FM and contour provide additional expressiveness.

### Page Navigation Implementation

**State Management:**
```cpp
struct _nt_elementsAlgorithm {
    // ...
    int current_page;  // 0-3 for pages 1-4
    // ...
};
```

**Button Handler:**
```cpp
void buttonPressed(_NT_algorithm* self, int button_index) {
    _nt_elementsAlgorithm* pThis = (_nt_elementsAlgorithm*)self;

    if (button_index == BUTTON_PAGE_NEXT) {
        pThis->current_page = (pThis->current_page + 1) % 4;
        update_oled_display(pThis);
    }
}
```

**Parameter Routing:**
```cpp
void potChanged(_NT_algorithm* self, int pot_index, float value) {
    _nt_elementsAlgorithm* pThis = (_nt_elementsAlgorithm*)self;

    int param_index;
    switch (pThis->current_page) {
        case 0: // Page 1 - Exciter
            param_index = PAGE1_POT_MAPPING[pot_index];
            break;
        case 1: // Page 2 - Resonator
            param_index = PAGE2_POT_MAPPING[pot_index];
            break;
        // ... etc
    }

    pThis->v[param_index] = value;
    parameterChanged(self, param_index);
}
```

### Parameter Mapping Tables

**Page Mapping Structure:**
```cpp
// In parameter_pages.h
enum PageIndex {
    kPageExciter = 0,
    kPageResonator = 1,
    kPageSpace = 2,
    kPagePerformance = 3,
    kNumPages = 4
};

// Pot mappings per page
const int PAGE1_POT_MAPPING[3] = {
    kParamBowLevel,
    kParamBlowLevel,
    kParamStrikeLevel
};

const int PAGE2_POT_MAPPING[3] = {
    kParamGeometry,
    kParamBrightness,
    kParamDamping
};

// ... etc for all pages and controls
```

### Tuning Implementation

**Tuning Offset Calculation:**
Tuning parameters adjust the base MIDI note frequency.

**Implementation:**
```cpp
// Coarse tune: semitone offset (-12 to +12)
float coarse_semitones = (pThis->v[kParamCoarseTune] - 50.0f) * 0.24f;  // 0-100% → -12 to +12

// Fine tune: cent offset (-50 to +50)
float fine_cents = (pThis->v[kParamFineTune] - 50.0f) * 1.0f;  // 0-100% → -50 to +50

// Total tuning offset in semitones
float total_semitones = coarse_semitones + (fine_cents / 100.0f);

// Apply to MIDI frequency
float tuned_frequency = base_frequency * powf(2.0f, total_semitones / 12.0f);
pThis->patch.note = tuned_frequency;
```

**Use Cases:**
- Transpose without changing MIDI input
- Detune for chorus/ensemble effects
- Microtuning for alternate scales

### Output Level Scaling

**Output Level Implementation:**
```cpp
// In step() callback, after Elements processing:
float output_level = pThis->v[kParamOutputLevel] / 100.0f;  // 0-100% → 0.0-1.0

for (int i = 0; i < numFrames; ++i) {
    pThis->temp_output[i] *= output_level;
}
```

**Purpose:**
- Master volume control for nt_elements
- Prevents clipping when layering with other algorithms
- Allows balancing in complex patches

### Parameter Count and Memory

**Total Parameters:**
- System: 3 (input bus, output bus, output mode)
- Sample rate: 1 (32kHz/48kHz mode)
- Page 1 (Exciter): 5 parameters
- Page 2 (Resonator): 5 parameters
- Page 3 (Space): 3 parameters
- Page 4 (Performance): 5 parameters
- **Total: ~22 parameters**

**Memory Impact:**
Each parameter is a float (4 bytes). 22 parameters = 88 bytes (negligible).

### OLED Display Integration

OLED display implementation is primarily in Story 2.2, but this story lays groundwork.

**Basic Display Updates:**
- Show current page name
- Show current parameter values (simplified)
- Update on page change and parameter change

**Full implementation deferred to Story 2.2.**

### Testing Strategy

**Per-Page Testing:**
For each page, systematically test each control:
1. Set control to 0% → observe synthesis change
2. Set control to 50% → observe synthesis change
3. Set control to 100% → observe synthesis change
4. Verify expected behavior based on parameter function

**Integration Testing:**
- Navigate through all pages while synthesizing
- Change parameters on each page
- Verify parameter states persist when switching pages
- Test rapid page switching

**Edge Cases:**
- Page switching during note-on (should not glitch)
- Rapid parameter changes (should not crash)
- Extreme parameter values (should remain stable)

### Project Structure

**Files Created:**
- src/parameter_pages.h (new) - Page structure and mappings

**Files Modified:**
- src/parameter_adapter.h - Add all new parameter enums
- src/nt_elements.cpp - Implement page navigation and routing

### References

- [Source: docs/epics.md#Story 2.1] - Full story specification and acceptance criteria
- [Source: docs/PRD.md#FR006] - 4 parameter pages functional requirement
- [Source: docs/PRD.md#FR007] - Parameter mapping to NT interface
- [Source: docs/PRD.md#User Interface Design Goals] - Page organization and controls
- [Source: docs/architecture.md#Parameters] - Parameter count, mapping strategy, page navigation
- [Source: docs/architecture.md#Data Architecture] - NT parameter array and Elements Patch structure

## Dev Agent Record

### Context Reference

- docs/stories/2-1-complete-parameter-page-system.context.md

### Agent Model Used

<!-- Model information will be filled in during implementation -->

### Debug Log References

### Completion Notes List

### File List
