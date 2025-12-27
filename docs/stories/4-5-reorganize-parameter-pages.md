# Story 4.5: Reorganize Parameter Pages (Manual-Aligned Layout)

Status: ready-for-dev

## Story

As a user,
I want parameter pages organized to match the original Elements layout,
So that I can access all sound design controls intuitively.

## Acceptance Criteria

1. **Page 1 (Exciter)**: Contour, Bow, Blow on pots; Strike, Strength on encoders
2. **Page 2 (Timbre)**: Bow Timbre, Blow Timbre, Strike Timbre on pots; Flow, Mallet on encoders
3. **Page 3 (Resonator)**: Geometry, Brightness, Damping on pots; Position, Space on encoders
4. **Page 4 (Tuning)**: Coarse, Fine, FM Amount on pots; Output Level on encoder
5. OLED display shows page names and parameter labels matching manual terminology
6. Contour prominently placed as first pot on Exciter page (matches hardware layout)
7. Routing parameters (buses, MIDI, CV) remain accessible via NT firmware UI

## Tasks / Subtasks

- [ ] Task 1: Document current page layout (AC: 1-4)
  - [ ] Read `src/parameter_pages.h`
  - [ ] List current page structure
  - [ ] Identify parameters to move

- [ ] Task 2: Redesign Page 1 - Exciter (AC: 1, 6)
  - [ ] Pot 1: Contour (envelope shape for bow/blow)
  - [ ] Pot 2: Bow (bow exciter level)
  - [ ] Pot 3: Blow (blow exciter level)
  - [ ] Encoder 1: Strike (strike exciter level)
  - [ ] Encoder 2: Strength (excitation intensity)

- [ ] Task 3: Redesign Page 2 - Timbre (AC: 2)
  - [ ] Pot 1: Bow Timbre (bow smoothness/granularity)
  - [ ] Pot 2: Blow Timbre (blow pitch/granulation rate)
  - [ ] Pot 3: Strike Timbre (strike brightness/speed)
  - [ ] Encoder 1: Flow (blow meta - air flow character)
  - [ ] Encoder 2: Mallet (strike meta - mallet type)

- [ ] Task 4: Redesign Page 3 - Resonator (AC: 3)
  - [ ] Pot 1: Geometry (structure shape)
  - [ ] Pot 2: Brightness (high frequency damping)
  - [ ] Pot 3: Damping (energy dissipation)
  - [ ] Encoder 1: Position (excitation point)
  - [ ] Encoder 2: Space (stereo width + reverb)

- [ ] Task 5: Redesign Page 4 - Tuning (AC: 4)
  - [ ] Pot 1: Coarse (semitone tuning)
  - [ ] Pot 2: Fine (cent tuning)
  - [ ] Pot 3: FM Amount (frequency modulation depth)
  - [ ] Encoder 1: Output Level (master volume)
  - [ ] Encoder 2: Reserved or additional param

- [ ] Task 6: Update parameter_pages.h (AC: 1-4)
  - [ ] Implement new page structure
  - [ ] Verify all parameter indices match
  - [ ] Test page navigation

- [ ] Task 7: Update OLED display labels (AC: 5)
  - [ ] Page titles: "EXCITER", "TIMBRE", "RESONATOR", "TUNING"
  - [ ] Parameter names matching Elements manual terminology
  - [ ] Abbreviations if needed for display width

- [ ] Task 8: Verify routing via NT firmware (AC: 7)
  - [ ] Confirm buses, MIDI, CV accessible via standard NT UI
  - [ ] Document how users access routing settings

## Dev Notes

This organization mirrors the original Elements hardware panel layout:
- Contour affects BOW and BLOW envelope shapes
- Exciter levels (Bow/Blow/Strike) are the main mixing controls
- Timbre parameters shape the character of each exciter type
- Flow and Mallet are the "meta" parameters for blow and strike
- Resonator page groups all modal filter bank controls
- Space combines stereo width and reverb (at max + high CV = freeze)

Current page structure needs reorganization from existing layout to 4 custom pages in `parameter_pages.h`.

### Project Structure Notes

- Primary file: `src/parameter_pages.h` - Complete restructure
- Secondary file: `src/oled_display.cpp` - Update page/parameter labels
- Verify: All parameter indices still valid after reorganization

### References

- [Source: docs/epic-4-audio-quality-and-parameters.md#Story 4.5]
- [Source: docs/epic-4-audio-quality-and-parameters.md#Page Organization]
- [Source: docs/architecture.md#ADR-006] - Four-Page Parameter Organization
- Elements Hardware Manual - Panel layout descriptions

## Dev Agent Record

### Context Reference

- docs/stories/4-5-reorganize-parameter-pages.context.md

### Agent Model Used

### Debug Log References

### Completion Notes List

### File List
