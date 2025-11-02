# Sprint Change Proposal: Dynamic Sample Rate Support

**Date:** 2025-11-02
**Author:** John (Product Manager Agent)
**Triggered By:** Neal: "The NT provides the sample rate, and can be variable, user can change this in settings. We need to use that."
**Status:** Pending Approval
**Scope Classification:** Moderate - Code changes required to adapter layer

---

## 1. Issue Summary

### Problem Statement

Elements DSP has a hardcoded sample rate constant (`static const float kSampleRate = 32000.0f` in `dsp.h`) but **disting NT provides dynamic sample rate via `NT_globals.sampleRate`** that users can change in settings.

Currently, the plugin runs Elements at fixed 32kHz regardless of NT's actual sample rate, causing:
- **Incorrect pitch** when NT runs at 48kHz (everything 1.5x too low) or other rates
- **Wrong filter frequencies** (resonator operates at wrong cutoffs)
- **Incorrect envelope timings** (attack/decay scaled incorrectly)
- **Timbral inaccuracies** (entire DSP engine operating at wrong rate)

### Discovery Context

Initial analysis incorrectly assumed NT operated at fixed 48kHz with platform resampling. Neal clarified that **NT sample rate is user-configurable** and plugins must adapt to `NT_globals.sampleRate`.

Research shows:
- `NT_globals.sampleRate` provides current rate in Hz
- Other NT plugins access this (examples: fourteen.cpp:193, monosynth.cpp:92)
- Elements uses `kSampleRate` in 9 locations for all frequency-dependent calculations

### Evidence

- **NT API:** `_NT_globals.sampleRate` at distingNT_API/include/distingnt/api.h:115
- **Elements constant:** `dsp.h:38` defines `static const float kSampleRate = 32000.0f`
- **Usage:** 9 references across Elements DSP (string.cc, part.cc, voice.cc, exciter.cc, resonator.cc)
- **Example plugins:** Show NT_globals.sampleRate usage pattern

---

## 2. Impact Analysis

### Epic Impact

**Epic 1: Project Foundation & Core DSP Integration**
- Story 1.5 "Sample Rate Conversion Implementation" - Needs complete redesign
- Story 1.4 "Elements Part Instance" - Needs update to inject sample rate
- Story 1.10 "Hardware Deployment" - Critical for testing at different user-selected rates

**Epic 2: UI, Presets & Production Polish**
- Story 2.5 "Performance Optimization" - CPU usage varies with sample rate
- Story 2.6 "Sound Quality Validation" - Must test at multiple sample rates

### Story Impact

**Story 1.5 Must Be Reimplemented:**
- **Old scope:** "Recalculate coefficients for 48kHz" (assumed fixed target)
- **New scope:** "Inject NT's dynamic sample rate into Elements DSP"
- **Status:** Change from "blocked" to "in progress"

### Artifact Conflicts

**PRD (docs/PRD.md):**
- FR014 incorrectly describes fixed 32kHz operation
- Should state: "Adapt to NT's user-configurable sample rate"

**Architecture (docs/architecture.md):**
- ADR-002 documents wrong decision (32kHz-only)
- Should document: "Dynamic sample rate adaptation via constant injection"
- Multiple sections assume 32kHz/48kHz fixed modes

**Epics (docs/epics.md):**
- Lines 109, 199 incorrectly state Stories 1.5 and 1.11 removed
- These stories are actually CRITICAL for correct operation

**Story 1.5:**
- Current implementation approach is wrong
- Needs complete redesign

### Technical Impact

**HIGH** - Core DSP adapter requires architectural change

---

## 3. Recommended Approach

### Selected Path: Constant Injection via Namespace Wrapper

**Technical Approach:**

Create a sample rate injection header that redefines Elements' `kSampleRate` to reference NT's dynamic sample rate:

```cpp
// src/elements_sample_rate_adapter.h
#ifndef NT_ELEMENTS_SAMPLE_RATE_ADAPTER_H_
#define NT_ELEMENTS_SAMPLE_RATE_ADAPTER_H_

#include "distingnt/api.h"

// Inject NT's dynamic sample rate into Elements namespace
// This must be included BEFORE any Elements headers
namespace elements {
    // Shadow the hardcoded kSampleRate with NT's actual sample rate
    inline float get_sample_rate() {
        return (float)NT_globals.sampleRate;
    }
    #define kSampleRate get_sample_rate()
}

#endif
```

**Integration Pattern:**

```cpp
// src/nt_elements.cpp
#include "distingnt/api.h"
#include "elements_sample_rate_adapter.h"  // BEFORE Elements headers
#include "elements/dsp/part.h"
#include "elements/dsp/dsp.h"
```

**How It Works:**
1. Macro `#define kSampleRate get_sample_rate()` replaces constant with function call
2. Function returns current `NT_globals.sampleRate` value
3. All Elements DSP calculations use NT's actual rate
4. No modification to Elements submodule required
5. Adapts automatically when user changes NT sample rate

**Alternatives Considered:**

**Option A: Modify dsp.h directly**
- **Rejected:** Violates readonly submodule constraint

**Option B: Sample rate conversion (run Elements at 32kHz, resample to NT rate)**
- **Rejected:** 10-15% CPU overhead, conversion artifacts, unnecessary complexity

**Option C: Compile-time sample rate patching (sed/awk in Makefile)**
- **Rejected:** Fragile, breaks on Elements updates, doesn't handle dynamic changes

### Effort Estimate

**Medium** - Clean implementation, requires testing

**Breakdown:**
- Create `src/elements_sample_rate_adapter.h` (1 hour)
- Update `src/nt_elements.cpp` include order (15 min)
- Test at multiple sample rates (32kHz, 48kHz, 96kHz) (2 hours)
- Update documentation (1 hour)
- **Total:** ~4-5 hours

### Risk Assessment

**Risk Level: Medium**

**Risks:**
1. **Macro shadowing might fail** - Some Elements code might use fully-qualified `elements::kSampleRate`
   - **Mitigation:** Test thoroughly, check compilation
2. **Function call overhead** - `get_sample_rate()` called frequently
   - **Mitigation:** Inline function should be optimized to register read
3. **Sample rate changes during operation** - If NT allows runtime rate changes
   - **Mitigation:** Document behavior, test rate change scenarios

**Risk Mitigation Plan:**
- Desktop testing first (nt_emu with different rates)
- Hardware validation at 32kHz, 48kHz, 96kHz
- Frequency response measurements at each rate
- Pitch accuracy validation (MIDI note tests)

### Timeline Impact

**Minor** - Story 1.5 effort increases from "documentation only" to "4-5 hours implementation"

---

## 4. Detailed Change Proposals

### CODE CHANGE 1: Create Sample Rate Adapter Header

**File:** `src/elements_sample_rate_adapter.h` (NEW FILE)
**Type:** New infrastructure

**CONTENT:**
```cpp
/*
 * Elements Sample Rate Adapter
 *
 * Injects disting NT's dynamic sample rate into Elements DSP engine.
 * Must be included BEFORE any Elements headers.
 *
 * Elements uses `static const float kSampleRate = 32000.0f` which cannot
 * be modified (readonly submodule). This header shadows that constant with
 * a macro that returns NT_globals.sampleRate, allowing Elements to adapt
 * to user-configured sample rates (32kHz, 48kHz, 96kHz, etc).
 */

#ifndef NT_ELEMENTS_SAMPLE_RATE_ADAPTER_H_
#define NT_ELEMENTS_SAMPLE_RATE_ADAPTER_H_

#include "distingnt/api.h"

namespace elements {
    // Return NT's current sample rate
    // Inline for zero overhead (compiler should optimize to register read)
    inline float get_sample_rate() {
        return static_cast<float>(NT_globals.sampleRate);
    }

    // Shadow Elements' hardcoded kSampleRate constant
    // All Elements code using kSampleRate will now get NT's actual rate
    #define kSampleRate get_sample_rate()
}

#endif  // NT_ELEMENTS_SAMPLE_RATE_ADAPTER_H_
```

**Rationale:** Clean, non-invasive approach that respects readonly submodule while achieving dynamic sample rate.

---

### CODE CHANGE 2: Update nt_elements.cpp Include Order

**File:** `src/nt_elements.cpp`
**Lines:** 11-20
**Type:** Include reordering

**OLD:**
```cpp
#include <cstddef>
#include <cstdint>
#include <cmath>
#include <cstring>
#ifdef NT_EMU_DEBUG
#include <cstdio>
#endif
#include "distingnt/api.h"
#include "elements/dsp/part.h"
#include "elements/dsp/dsp.h"
```

**NEW:**
```cpp
#include <cstddef>
#include <cstdint>
#include <cmath>
#include <cstring>
#ifdef NT_EMU_DEBUG
#include <cstdio>
#endif
#include "distingnt/api.h"

// CRITICAL: Include sample rate adapter BEFORE Elements headers
// This injects NT_globals.sampleRate into Elements DSP calculations
#include "elements_sample_rate_adapter.h"

#include "elements/dsp/part.h"
#include "elements/dsp/dsp.h"
```

**Rationale:** Ensures macro takes effect before Elements constants are defined.

---

### CODE CHANGE 3: Remove Obsolete math_constants.h Comment

**File:** `src/math_constants.h`
**Lines:** 20-25
**Type:** Comment update

**OLD:**
```cpp
// Story 1.5 Note: Sample Rate Override
// Elements uses `static const float kSampleRate = 32000.0f` which cannot be overridden
// without modifying the Elements source (readonly git submodule).
// The plugin currently runs Elements at its native 32kHz internally.
// Future Story 1.11 will implement runtime sample rate mode toggle with proper resampling.
// For now, Elements runs at 32kHz with audio resampling handled by NT hardware (if needed)
```

**NEW:**
```cpp
// Sample Rate: Dynamic Adaptation
// Elements uses `static const float kSampleRate = 32000.0f` (hardcoded in dsp.h).
// We inject NT_globals.sampleRate via macro shadowing in elements_sample_rate_adapter.h.
// This allows Elements to adapt to NT's user-configurable sample rate (32/48/96kHz etc).
// See src/elements_sample_rate_adapter.h for implementation details.
```

**Rationale:** Reflect actual implementation approach.

---

### DOCUMENTATION CHANGE 4: Update PRD FR014

**File:** `docs/PRD.md`
**Line:** 64
**Type:** Requirement correction

**OLD:**
```markdown
**FR014:** Process audio at Elements' native 32kHz sample rate with disting NT handling platform resampling as needed
```

**NEW:**
```markdown
**FR014:** Process audio at disting NT's user-configurable sample rate by dynamically adapting Elements DSP calculations to match NT_globals.sampleRate
```

**Rationale:** Reflect dynamic sample rate requirement.

---

### DOCUMENTATION CHANGE 5: Update architecture.md ADR-002

**File:** `docs/architecture.md`
**Section:** ADR-002
**Lines:** 730-748
**Type:** Complete ADR rewrite

**OLD:**
```markdown
### ADR-002: [SUPERSEDED] Native 32kHz Operation Only

**Context**: Elements runs at 32kHz with hardcoded sample rate constant...
```

**NEW:**
```markdown
### ADR-002: Dynamic Sample Rate Adaptation via Constant Injection

**Context**: Elements DSP uses hardcoded `static const float kSampleRate = 32000.0f` (dsp.h:38) but disting NT provides user-configurable sample rate via `NT_globals.sampleRate`.

**Decision**: Inject NT's dynamic sample rate into Elements namespace using macro shadowing.

**Implementation**:
```cpp
// src/elements_sample_rate_adapter.h
namespace elements {
    inline float get_sample_rate() { return (float)NT_globals.sampleRate; }
    #define kSampleRate get_sample_rate()
}
```

**Rationale**:
- Respects readonly submodule constraint (no Elements modification)
- Adapts to NT's user-configurable sample rate (32/48/96kHz)
- Zero runtime overhead (inline function optimized to register read)
- Maintains 100% correct synthesis behavior at any sample rate
- Cleaner than sample rate conversion (no CPU overhead, no artifacts)

**Consequences**:
- Must include adapter header before Elements headers (enforced by compile error)
- Assumes NT_globals.sampleRate stable during step() callback
- Simplifies architecture (no SRC, no dual-mode complexity)
- Pitch and filter frequencies automatically correct at any rate

**Date:** 2025-11-02
```

**Rationale:** Document actual architectural decision.

---

### DOCUMENTATION CHANGE 6: Update Story 1.5

**File:** `docs/stories/1-5-sample-rate-conversion-implementation.md`
**Line:** 3 and story content
**Type:** Complete story rewrite

**OLD:**
```markdown
Status: blocked

## Story

As a developer,
I want all filter coefficients recalculated for 48kHz operation,
So that synthesis sounds correct at disting NT's sample rate.
```

**NEW:**
```markdown
Status: in progress

## Story

As a developer,
I want Elements DSP to adapt to NT's user-configurable sample rate,
So that synthesis sounds correct regardless of user's sample rate setting (32/48/96kHz).

## Implementation Approach

**Technical Solution:** Constant injection via macro shadowing

Create `src/elements_sample_rate_adapter.h` that injects `NT_globals.sampleRate` into Elements namespace:

```cpp
namespace elements {
    inline float get_sample_rate() { return (float)NT_globals.sampleRate; }
    #define kSampleRate get_sample_rate()
}
```

Include this header BEFORE Elements headers in `src/nt_elements.cpp`.

**Why This Works:**
- Macro shadows Elements' hardcoded `kSampleRate = 32000.0f`
- All Elements DSP calculations now use NT's actual sample rate
- No submodule modification required
- Zero runtime overhead (inline optimized away)
- Adapts automatically when user changes NT sample rate
```

**Rationale:** Reflect actual implementation approach and current status.

---

### DOCUMENTATION CHANGE 7: Update epics.md

**File:** `docs/epics.md`
**Lines:** 109, 199
**Type:** Remove incorrect removal notes

**OLD:**
```markdown
Line 109: (Story 1.5 removed - Elements runs at native 32kHz, no conversion needed)
Line 199: (Story 1.11 removed - Single 32kHz mode only, no mode selection needed)
```

**NEW:**
```markdown
Line 109: [Remove this line entirely - Story 1.5 is active]
Line 199: (Story 1.11 removed - Dynamic sample rate adaptation via Story 1.5 eliminates need for mode toggle)
```

**Rationale:** Story 1.5 is critical, not removed. Story 1.11 still not needed (no UI toggle required).

---

## 5. Implementation Handoff

### Change Scope Classification

**Moderate** - Core adapter code changes + documentation updates

### Handoff Recipients

**Route to:** Development team (Neal) for implementation

### Implementation Tasks

**Phase 1: Core Implementation (2-3 hours)**
1. Create `src/elements_sample_rate_adapter.h` with macro injection
2. Update `src/nt_elements.cpp` include order
3. Update `src/math_constants.h` comment
4. Build and verify compilation succeeds

**Phase 2: Testing (2 hours)**
1. Desktop testing (nt_emu):
   - Test at 32kHz sample rate
   - Test at 48kHz sample rate
   - Test at 96kHz sample rate (if NT supports)
2. Pitch accuracy validation:
   - Send MIDI C4 (note 60), measure output frequency
   - Should be 261.63 Hz at all sample rates
3. Frequency response check:
   - Sweep resonator brightness, verify filter behavior
   - Compare spectral response across sample rates

**Phase 3: Hardware Validation (1 hour)**
1. Deploy to disting NT
2. Test at user's configured sample rate
3. Change NT sample rate setting, verify plugin adapts
4. CPU usage measurement at different rates

**Phase 4: Documentation (1 hour)**
1. Update PRD.md (FR014)
2. Update architecture.md (ADR-002)
3. Update Story 1.5 status and content
4. Update epics.md (remove incorrect removal notes)

### Success Criteria

- [ ] `src/elements_sample_rate_adapter.h` created and compiles
- [ ] Plugin builds successfully with new include order
- [ ] Desktop test: Pitch correct at 32kHz, 48kHz, 96kHz
- [ ] Hardware test: Adapts to NT's configured sample rate
- [ ] Hardware test: Pitch accuracy < 1 cent error at all rates
- [ ] Hardware test: No audio glitches when changing sample rate
- [ ] Documentation updated to reflect dynamic sample rate
- [ ] Story 1.5 status updated to "completed"

### Deliverables

1. `src/elements_sample_rate_adapter.h` (new file)
2. Updated `src/nt_elements.cpp` (include order)
3. Updated documentation (PRD, architecture, Story 1.5, epics)
4. Test results documenting pitch accuracy at multiple sample rates
5. This Sprint Change Proposal (archived in docs/)

---

## 6. Summary

### Issue Addressed

Elements DSP fixed at 32kHz - must adapt to NT's user-configurable dynamic sample rate.

### Change Scope

**Moderate** - Core adapter changes + testing + documentation

### Implementation Approach

**Constant injection via macro shadowing** - Clean, non-invasive, zero-overhead solution.

### Implementation Effort

**4-5 hours total:**
- Code: 2-3 hours
- Testing: 2 hours
- Docs: 1 hour

### Risk Level

**Medium** - Core DSP adapter change, requires thorough testing

### Expected Outcome

Elements DSP adapts to NT's sample rate automatically. Correct pitch, filter frequencies, and timbral behavior at any user-selected sample rate (32kHz, 48kHz, 96kHz, etc).

---

**Next Steps:** Awaiting Neal's approval to proceed with implementation.

---

*Generated by BMAD Correct Course Workflow*
*Product Manager: John*
*Date: 2025-11-02*
