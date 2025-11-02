# Story 2.2: OLED Display Implementation

Status: ready-for-dev

## Story

As a user,
I want clear visual feedback on the OLED display,
So that I always know which page I'm on and current parameter values.

## Acceptance Criteria

1. Page title displayed prominently (e.g., "EXCITER", "RESONATOR")
2. Active parameter names and values shown for pots/encoders
3. Display updates immediately when parameters change
4. Value representation appropriate (percentages, descriptive text, numeric)
5. Display remains readable in typical Eurorack lighting
6. No flicker or visual artifacts during updates

## Tasks / Subtasks

- [ ] Create OLED display header (AC: #1-4)
  - [ ] Create src/oled_display.h
  - [ ] Define display layout constants (positions, sizes)
  - [ ] Define font sizes for headers and values
  - [ ] Declare rendering functions
  - [ ] Document display architecture

- [ ] Implement page title rendering (AC: #1)
  - [ ] Create render_page_title() function
  - [ ] Display current page name (EXCITER/RESONATOR/SPACE/PERF)
  - [ ] Use large/bold font for visibility
  - [ ] Position at top of display
  - [ ] Test all four page titles

- [ ] Implement parameter name rendering (AC: #2)
  - [ ] Create render_parameter_names() function
  - [ ] Display names for active pots/encoders on current page
  - [ ] Use abbreviated names if needed (fit display width)
  - [ ] Position below page title
  - [ ] Test all parameter name displays

- [ ] Implement parameter value rendering (AC: #2, #4)
  - [ ] Create render_parameter_values() function
  - [ ] Display numeric values aligned with parameter names
  - [ ] Format percentages (0-100%)
  - [ ] Format tuning values (+/-12 semitones, +/-50 cents)
  - [ ] Format descriptive text for geometry/mode parameters
  - [ ] Test all value formats

- [ ] Implement display update logic (AC: #3)
  - [ ] Add draw() callback to NT_algorithm structure
  - [ ] Call rendering functions on parameter change
  - [ ] Call rendering functions on page change
  - [ ] Implement dirty flag to minimize redraws
  - [ ] Verify immediate updates (no lag)

- [ ] Create display layouts for each page (AC: #1-2)
  - [ ] Page 1 layout: "EXCITER" + 5 parameter name/value pairs
  - [ ] Page 2 layout: "RESONATOR" + 5 parameter name/value pairs
  - [ ] Page 3 layout: "SPACE" + 3 parameter name/value pairs
  - [ ] Page 4 layout: "PERF" + 5 parameter name/value pairs
  - [ ] Test visual layout on each page

- [ ] Implement value formatting helpers (AC: #4)
  - [ ] Format percentage: "50%"
  - [ ] Format semitones: "+2.0" or "-5.5"
  - [ ] Format cents: "+25c"
  - [ ] Format geometry: "String", "Tube", "Mixed"
  - [ ] Format mode: "32kHz", "48kHz"
  - [ ] Test all formatters

- [ ] Optimize display performance (AC: #6)
  - [ ] Implement dirty flag (only redraw on change)
  - [ ] Minimize full-screen clears
  - [ ] Use partial updates when possible
  - [ ] Avoid expensive operations in draw()
  - [ ] Verify no flicker or artifacts

- [ ] Test display readability (AC: #5)
  - [ ] View display in normal lighting
  - [ ] View display in dim lighting (Eurorack typical)
  - [ ] Verify contrast is sufficient
  - [ ] Check font sizes are readable from normal viewing distance
  - [ ] Adjust contrast/brightness if needed

- [ ] Test display updates (AC: #3, #6)
  - [ ] Change parameters, verify immediate display update
  - [ ] Switch pages, verify display updates correctly
  - [ ] Rapid parameter changes, verify no flicker
  - [ ] Sustained operation, verify no display artifacts
  - [ ] Monitor for visual glitches

- [ ] Desktop testing in nt_emu (AC: #1-6)
  - [ ] Load plugin in nt_emu
  - [ ] If nt_emu supports display emulation, test all criteria
  - [ ] If not, validate logic and defer visual testing to hardware
  - [ ] Document testing limitations

- [ ] Hardware testing (AC: #1-6)
  - [ ] Deploy to disting NT hardware
  - [ ] Verify all display functionality on actual OLED
  - [ ] Test readability in Eurorack case
  - [ ] Verify no performance impact from display updates
  - [ ] Document final display validation

## Dev Notes

### OLED Display Architecture

**disting NT Display Specifications:**
- Resolution: Typically 128x64 pixels (monochrome)
- Interface: NT API provides drawing primitives
- Update: draw() callback called when display needs refresh
- Performance: Minimize updates to avoid CPU overhead

**Display Constraints:**
- Limited resolution (must fit information clearly)
- Monochrome (no color coding)
- Small physical size
- Must be readable from ~1 foot distance in dim lighting

### Display Layout Design

**Layout Structure:**
```
┌──────────────────────────┐
│ PAGE TITLE               │  Line 1: Large font, page name
├──────────────────────────┤
│ Pot1: Name     Value%    │  Line 2: Pot 1 name and value
│ Pot2: Name     Value%    │  Line 3: Pot 2 name and value
│ Pot3: Name     Value%    │  Line 4: Pot 3 name and value
│ Enc1: Name     Value%    │  Line 5: Encoder 1 name and value
│ Enc2: Name     Value%    │  Line 6: Encoder 2 name and value
└──────────────────────────┘
```

**Example Page 1 (Exciter):**
```
┌──────────────────────────┐
│ EXCITER                  │
├──────────────────────────┤
│ Bow:          45%        │
│ Blow:         30%        │
│ Strike:       75%        │
│ BowTim:       50%        │
│ BlowTim:      60%        │
└──────────────────────────┘
```

**Example Page 2 (Resonator):**
```
┌──────────────────────────┐
│ RESONATOR                │
├──────────────────────────┤
│ Geom:       String       │
│ Bright:       68%        │
│ Damp:         55%        │
│ Pos:          40%        │
│ Inharm:       20%        │
└──────────────────────────┘
```

### NT Display API

**Drawing Primitives (typical NT API):**
```cpp
void draw(_NT_algorithm* self, /* display context */) {
    _nt_elementsAlgorithm* pThis = (_nt_elementsAlgorithm*)self;

    // Clear display or prepare for drawing
    // NT_display_clear();

    // Draw text at position
    // NT_display_text(x, y, "EXCITER", FONT_LARGE);

    // Draw parameter name/value pair
    // NT_display_text(0, line_y, "Bow:", FONT_SMALL);
    // NT_display_text(60, line_y, "45%", FONT_SMALL);
}
```

**Note:** Exact NT display API may differ. Consult distingNT_API documentation for actual functions and signatures.

### Display Update Strategy

**Event-Driven Updates:**
Only redraw when something changes (parameter, page, mode).

**Implementation:**
```cpp
struct _nt_elementsAlgorithm {
    // ...
    bool display_dirty;  // Flag for redraw needed
    // ...
};

// Set dirty flag on changes
void parameterChanged(_NT_algorithm* self, int p) {
    _nt_elementsAlgorithm* pThis = (_nt_elementsAlgorithm*)self;
    // ... parameter logic
    pThis->display_dirty = true;
}

void page_changed(_NT_algorithm* self) {
    _nt_elementsAlgorithm* pThis = (_nt_elementsAlgorithm*)self;
    pThis->display_dirty = true;
}

// Check dirty flag in draw()
void draw(_NT_algorithm* self, /* context */) {
    _nt_elementsAlgorithm* pThis = (_nt_elementsAlgorithm*)self;

    if (!pThis->display_dirty) {
        return;  // No update needed
    }

    // Render display
    render_current_page(pThis);

    pThis->display_dirty = false;
}
```

**Benefits:**
- Minimal CPU overhead (no unnecessary redraws)
- No flicker from constant refreshing
- Efficient use of display bandwidth

### Value Formatting

**Percentage Values:**
```cpp
char buffer[16];
snprintf(buffer, sizeof(buffer), "%d%%", (int)value);
```

**Tuning Values:**
```cpp
// Coarse tune: -12 to +12 semitones
char buffer[16];
float semitones = (value - 50.0f) * 0.24f;
snprintf(buffer, sizeof(buffer), "%+.1f", semitones);

// Fine tune: -50 to +50 cents
float cents = (value - 50.0f) * 1.0f;
snprintf(buffer, sizeof(buffer), "%+dc", (int)cents);
```

**Geometry Values (Descriptive):**
```cpp
const char* get_geometry_name(float geometry_value) {
    // Map 0-100% to descriptive names
    if (geometry_value < 25.0f) return "String";
    if (geometry_value < 50.0f) return "Tube";
    if (geometry_value < 75.0f) return "Mixed";
    return "Other";
}
```

**Mode Values:**
```cpp
const char* mode_name = (mode == MODE_32KHZ) ? "32kHz" : "48kHz";
```

### Readability Optimization

**Font Size Guidelines:**
- Page title: Large/bold font (high visibility)
- Parameter names: Small font (fit more info)
- Parameter values: Small font, right-aligned

**Contrast:**
- Use maximum contrast (white on black or black on white)
- Avoid gray levels (monochrome OLED)

**Spacing:**
- Adequate line spacing (avoid cramped text)
- Align parameter names and values in columns
- Use consistent layout across pages

**Abbreviations:**
Abbreviate parameter names to fit display width:
- "Bow Timbre" → "BowTim"
- "Brightness" → "Bright"
- "Inharmonicity" → "Inharm"
- "Position" → "Pos"
- "Damping" → "Damp"

### Performance Considerations

**Display Update Cost:**
OLED updates can consume 1-5% CPU depending on complexity. Minimize updates:
- Use dirty flag (only redraw on change)
- Avoid animations or constant refreshing
- Keep draw() logic simple

**Profiling:**
Measure CPU impact of display updates on hardware. If > 2% overhead, optimize further.

### Testing Strategy

**Desktop Testing (nt_emu):**
If nt_emu supports display emulation:
- Verify all layouts render correctly
- Test parameter value formatting
- Test page transitions
- Check for visual glitches

If nt_emu doesn't support display:
- Validate display logic in code
- Test formatting functions with unit tests
- Defer visual validation to hardware

**Hardware Testing:**
- Deploy to disting NT
- Verify all pages display correctly
- Test readability in actual Eurorack case lighting
- Verify no flicker or artifacts
- Measure CPU impact of display updates

**Edge Cases:**
- Rapid parameter changes (should not flicker)
- Rapid page switching (should update cleanly)
- Extreme parameter values (should format correctly)
- Long parameter names (should truncate gracefully)

### Project Structure

**Files Created:**
- src/oled_display.h (new) - Display rendering interface
- src/oled_display.cpp (new) - Display rendering implementation

**Files Modified:**
- src/nt_elements.cpp - Add draw() callback

### References

- [Source: docs/epics.md#Story 2.2] - Full story specification and acceptance criteria
- [Source: docs/PRD.md#FR008] - Display parameter values and page information
- [Source: docs/PRD.md#User Interface Design Goals] - OLED display design constraints
- [Source: docs/architecture.md#UI/Display] - OLED strategy and display updates
- [Source: docs/architecture.md#ADR-006] - Four-page parameter organization

## Dev Agent Record

### Context Reference

- docs/stories/2-2-oled-display-implementation.context.md

### Agent Model Used

<!-- Model information will be filled in during implementation -->

### Debug Log References

### Completion Notes List

### File List
