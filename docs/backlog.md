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
| 2025-11-02 | 2.1 | 2 | Bug | High | Dev Agent | Open | Implement pot/encoder routing callbacks (`potChanged`, `encoderChanged`) in `src/nt_elements.cpp` to route physical controls to parameters based on `current_page` state (AC #7, critical gap). |
| 2025-11-02 | 2.1 | 2 | TechDebt | High | Dev Agent | Open | Research NT framework parameter routing model - clarify if `_NT_parameterPages` provides automatic routing or if manual callbacks required (`src/nt_elements.cpp`, architecture.md). |
| 2025-11-02 | 2.1 | 2 | TechDebt | Medium | Dev Agent | Closed | Update dev notes section to match actual implementation - remove aspirational code examples for pot/encoder routing that don't exist in codebase (`docs/stories/2-1-complete-parameter-page-system.md` lines 176-244). Resolved 2025-11-02 with post-story documentation update. |
| 2025-11-02 | 2.1 | 2 | Enhancement | Low | Dev Agent | Open | Add basic OLED `draw()` callback to show current page name on button press (AC #6 partial) or document explicit deferral to Story 2.2 (`src/nt_elements.cpp`). |
| 2025-11-02 | 2.1 | 2 | Enhancement | High | Dev Agent | Closed | Implement stereo output routing for Elements reverb - added Main Output (bus 13) and Aux Output (bus 14) with independent mix/replace modes. Completed 2025-11-02. |
| 2025-11-02 | 2.1 | 2 | Feature | Medium | Dev Agent | Closed | Add essential CV inputs for common modulation targets: FM CV, Brightness CV, Expression CV. Enables expressive performance without manual CV mapping. Completed 2025-11-02. |
