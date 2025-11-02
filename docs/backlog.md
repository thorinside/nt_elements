# Engineering Backlog

This backlog collects cross-cutting or future action items that emerge from reviews and planning.

Routing guidance:

- Use this file for non-urgent optimizations, refactors, or follow-ups that span multiple stories/epics.
- Must-fix items to ship a story belong in that story’s `Tasks / Subtasks`.
- Same-epic improvements may also be captured under the epic Tech Spec `Post-Review Follow-ups` section.

| Date | Story | Epic | Type | Severity | Owner | Status | Notes |
| ---- | ----- | ---- | ---- | -------- | ----- | ------ | ----- |
| 2025-10-26 | 1.2 | 1 | Bug | High | TBD | Closed | Separate Elements main/aux buffers and handle mono mix in adapter (`src/nt_elements.cpp`). Fix merged 2025-10-26. |
| 2025-10-26 | 1.2 | 1 | Bug | High | TBD | Closed | Feed Elements `PerformanceState::note` with MIDI semitone values and reset default pitch domain (`src/nt_elements.cpp`). Fix merged 2025-10-26. |
| 2025-10-26 | 1.2 | 1 | Bug | Medium | TBD | Closed | Clamp or reject host buffer sizes that exceed 512-frame scratch allocation to avoid overflow (`src/nt_elements.cpp`). Fix merged 2025-10-26. |
| 2025-10-26 | 1.2 | 1 | Enhancement | Low | TBD | Planned | Add nt_emu regression coverage for audio output and MIDI pitch sanity checks (scheduled for Story 1.9). |
| 2025-10-26 | 1.7 | 1 | Bug | High | TBD | Closed | Added `src/parameter_adapter.h` and updated `src/nt_elements.cpp` to use the shared adapter API so downstream stories can reuse it. |
| 2025-10-26 | 1.8 | 1 | Enhancement | Low | Dev Agent | Open | Add brief validation report to story completion notes documenting nt_emu manual test results (AC3-AC5 validation). |
| 2025-10-26 | 1.8 | 1 | TechDebt | Low | Future maintainer | Open | Add inline comments to bus offset calculation logic (`src/nt_elements.cpp:265-266`) explaining 1-based to 0-based conversion. |
| 2025-10-26 | 1.8 | 1 | Enhancement | Low | Team decision | Open | Consider adding unit test infrastructure for future stories (Epic 2) to reduce reliance on manual validation. |
