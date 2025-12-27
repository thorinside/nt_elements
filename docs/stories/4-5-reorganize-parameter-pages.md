# Story 4.5: Reorganize Parameter Pages (Manual-Aligned Layout)

Status: review

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

- [x] Task 1: Document current page layout (AC: 1-4)
  - [x] Read `src/parameter_pages.h`
  - [x] List current page structure
  - [x] Identify parameters to move

- [x] Task 2: Redesign Page 1 - Exciter (AC: 1, 6)
  - [x] Pot 1: Contour (envelope shape for bow/blow)
  - [x] Pot 2: Bow (bow exciter level)
  - [x] Pot 3: Blow (blow exciter level)
  - [x] Encoder 1: Strike (strike exciter level)
  - [x] Encoder 2: Strength (excitation intensity)

- [x] Task 3: Redesign Page 2 - Timbre (AC: 2)
  - [x] Pot 1: Bow Timbre (bow smoothness/granularity)
  - [x] Pot 2: Blow Timbre (blow pitch/granulation rate)
  - [x] Pot 3: Strike Timbre - placeholder until Story 4-6
  - [x] Encoder 1: Flow (blow meta - air flow character)
  - [x] Encoder 2: Mallet (strike meta - mallet type)

- [x] Task 4: Redesign Page 3 - Resonator (AC: 3)
  - [x] Pot 1: Geometry (structure shape)
  - [x] Pot 2: Brightness (high frequency damping)
  - [x] Pot 3: Damping (energy dissipation)
  - [x] Encoder 1: Position (excitation point)
  - [x] Encoder 2: Space (reverb amount)

- [x] Task 5: Redesign Page 4 - Tuning (AC: 4)
  - [x] Pot 1: Coarse (semitone tuning)
  - [x] Pot 2: Fine (cent tuning)
  - [x] Pot 3: FM Amount (frequency modulation depth)
  - [x] Encoder 1: Output Level (master volume)
  - [x] Encoder 2: Reverb Size

- [x] Task 6: Update parameter_pages.h (AC: 1-4)
  - [x] Implement new page structure with enum names (not hardcoded indices)
  - [x] Renamed pages: kPageExciter, kPageTimbre, kPageResonator, kPageTuning
  - [x] Build verified successful

- [x] Task 7: Update OLED display labels (AC: 5)
  - [x] Page titles: "EXCITER", "TIMBRE", "RESONATOR", "TUNING"
  - [x] Parameter names from parameter definitions used automatically

- [x] Task 8: Verify routing via NT firmware (AC: 7)
  - [x] Routing parameters remain in parameter definitions array
  - [x] Accessible via standard NT firmware UI (not on custom pages)

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

- Rewrote parameter_pages.h with new 4-page structure
- Renamed pages from Exciter/Resonator/Space/Performance to Exciter/Timbre/Resonator/Tuning
- Changed from hardcoded indices to enum names for maintainability
- Added page_timbre namespace for new Timbre page

### Completion Notes List

- Complete page reorganization matching Elements hardware layout
- Page 1 (Exciter): Contour, Bow, Blow on pots; Strike, Strength on encoders
- Page 2 (Timbre): Bow Timbre, Blow Timbre, placeholder for Strike Timbre; Flow, Mallet on encoders
- Page 3 (Resonator): Geometry, Brightness, Damping on pots; Position, Space on encoders
- Page 4 (Tuning): Coarse, Fine, FM Amount on pots; Output Level, Reverb Size on encoders
- Note: Strike Timbre parameter will be added in Story 4-6 (currently using Bow Timbre as placeholder)
- Build verified successful

### File List

- src/parameter_pages.h (complete rewrite with new page structure)
- src/parameter_adapter.h (updated comments to reflect new organization)
