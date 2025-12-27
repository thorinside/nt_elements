# Story 4.9: Add Emilie Gillet Attribution

Status: ready-for-dev

## Story

As a user,
I want to see "By Emilie Gillet" displayed on the OLED screen,
So that the original creator of Elements receives proper credit.

## Acceptance Criteria

1. Text "By Emilie Gillet" displayed below the main title text
2. Uses small/subtle font size (not competing with page title)
3. Visible on all parameter pages
4. Properly positioned and readable

## Tasks / Subtasks

- [ ] Task 1: Locate display rendering code (AC: 1-4)
  - [ ] Open `src/oled_display.cpp`
  - [ ] Find `renderDisplay()` or equivalent function
  - [ ] Identify where title/header is rendered

- [ ] Task 2: Choose font and positioning (AC: 2, 4)
  - [ ] Review available NT OLED fonts
  - [ ] Select small font that is readable but subtle
  - [ ] Determine Y position below title

- [ ] Task 3: Add attribution text (AC: 1)
  - [ ] Add draw call for "By Emilie Gillet" text
  - [ ] Position below main title
  - [ ] Use selected small font

- [ ] Task 4: Ensure visibility on all pages (AC: 3)
  - [ ] Verify attribution is part of common header section
  - [ ] Not page-specific but always visible
  - [ ] Test all 4 parameter pages

- [ ] Task 5: Verify readability (AC: 4)
  - [ ] Test in nt_emu
  - [ ] Check text is legible
  - [ ] Verify no overlap with other UI elements

## Dev Notes

Emilie Gillet is the creator of Mutable Instruments and designed the original Elements module. This attribution honors her exceptional work in synthesis algorithm design.

The Elements firmware and DSP code are released under MIT license, which requires attribution. This visible credit goes beyond the license requirement to properly acknowledge her contribution.

### Display Layout Suggestion

```
+----------------------+
|  ELEMENTS            |
|  By Emilie Gillet    |
+----------------------+
|  PAGE: EXCITER       |
|  Contour   Bow  Blow |
|   50%      70%   30% |
+----------------------+
```

### Project Structure Notes

- Single file modification: `src/oled_display.cpp`
- Add string constant for attribution text
- No parameter or logic changes

### References

- [Source: docs/epic-4-audio-quality-and-parameters.md#Story 4.9]
- [Source: src/oled_display.cpp] - Display rendering functions
- Mutable Instruments: https://mutable-instruments.net/
- Elements Repository: https://github.com/pichenettes/eurorack

## Dev Agent Record

### Context Reference

- docs/stories/4-9-add-emilie-gillet-attribution.context.md

### Agent Model Used

### Debug Log References

### Completion Notes List

### File List
