# nt_elements - Epic Breakdown

**Author:** Neal
**Date:** 2025-10-26
**Project Level:** 2
**Target Scale:** Medium project - 13-17 stories across 2 epics

---

## Overview

This document provides the detailed epic breakdown for nt_elements, expanding on the high-level epic list in the [PRD](./PRD.md).

Each epic includes:

- Expanded goal and value proposition
- Complete story breakdown with user stories
- Acceptance criteria for each story
- Story sequencing and dependencies

**Epic Sequencing Principles:**

- Epic 1 establishes foundational infrastructure and initial functionality
- Subsequent epics build progressively, each delivering significant end-to-end value
- Stories within epics are vertically sliced and sequentially ordered
- No forward dependencies - each story builds only on previous work

---

## Epic 1: Project Foundation & Core DSP Integration

**Timeline:** Weeks 1-4
**Story Count:** 9 stories
**Goal:** Establish the complete development infrastructure and integrate Elements' DSP core with disting NT at native 32kHz sample rate, creating a foundation that enables basic audio synthesis. By epic completion, the plugin loads on disting NT, processes audio through Elements' DSP, and demonstrates authentic modal synthesis sound.

**Value Delivery:** Validates technical approach, proves feasibility, and delivers working audio output that can be iteratively refined in Epic 2.

---

### Story 1.1: Development Environment Setup

As a developer,
I want a working ARM cross-compilation environment with disting NT tooling,
So that I can build and test plugins for the disting NT platform.

**Acceptance Criteria:**
1. ARM toolchain (arm-none-eabi-gcc) installed and verified
2. disting NT API repository cloned as git submodule
3. nt_emu VCV Rack module installed for desktop testing
4. Basic "hello world" plugin compiles for both desktop (.dylib/.so) and hardware (.o)
5. Test plugin loads successfully in nt_emu

**Prerequisites:** None

---

### Story 1.2: Elements Source Integration

As a developer,
I want Elements DSP source code integrated into the project structure,
So that I can access and compile the synthesis algorithms.

**Acceptance Criteria:**
1. Elements repository cloned as git submodule at correct version
2. Elements DSP directory structure (dsp/) copied into project
3. Project builds with Elements source files included
4. MIT license notices preserved in all Elements files
5. Build configuration separates Elements code from adapter code

**Prerequisites:** Story 1.1 (build environment)

---

### Story 1.3: Minimal Plugin Stub with Audio Passthrough

As a developer,
I want a minimal nt_elements plugin that loads and passes audio through,
So that I can verify the plugin interface works before adding DSP.

**Acceptance Criteria:**
1. Plugin implements required disting NT callbacks (construct, destruct, step)
2. Plugin loads in nt_emu without errors
3. Audio input passes through to output unmodified
4. Plugin displays "nt_elements" in NT interface
5. No memory leaks detected during load/unload cycles

**Prerequisites:** Story 1.1, 1.2

---

### Story 1.4: Elements Part Instance Integration

As a developer,
I want to instantiate Elements' Part class and route audio through it,
So that Elements DSP processes audio in the plugin.

**Acceptance Criteria:**
1. Elements::Part instantiated in plugin construct()
2. Audio buffers allocated (SRAM for state, DRAM for reverb)
3. Elements Part::Process() called in step() callback
4. Basic patch parameters initialized to default values
5. Plugin produces modal synthesis sound when MIDI note received
6. No crashes or audio glitches during operation

**Prerequisites:** Story 1.3

---

### Story 1.5: Sample Rate Conversion Implementation

As a developer,
I want Elements DSP to adapt to NT's user-configurable sample rate,
So that synthesis sounds correct regardless of user's sample rate setting (32/48/96kHz).

**Acceptance Criteria:**
1. Elements uses NT_globals.sampleRate instead of hardcoded 32kHz
2. Pitch accuracy correct at all NT sample rates
3. Filter frequencies scale correctly with sample rate
4. No Elements submodule file permissions violated
5. Zero runtime overhead (inline optimization)

**Prerequisites:** Story 1.4 (Elements Part Instance)

**Implementation:** Modified dsp.h to inject NT_globals.sampleRate via inline function + macro

---

### Story 1.6: MIDI Note Input Integration

As a user,
I want to play notes via MIDI input,
So that I can control the pitch of the synthesis.

**Acceptance Criteria:**
1. MIDI note-on messages trigger synthesis
2. MIDI note-off messages stop synthesis
3. MIDI pitch maps correctly to synthesized frequency
4. Pitch bend messages modulate frequency smoothly
5. Multiple rapid note-ons handled gracefully (monophonic, last-note priority)

**Prerequisites:** Story 1.4, 1.5

---

### Story 1.7: Parameter Adapter Layer

As a developer,
I want a clean adapter between NT parameters and Elements patch format,
So that NT parameter changes update Elements synthesis correctly.

**Acceptance Criteria:**
1. Adapter layer converts NT float parameters to Elements patch structure
2. Parameter changes reflect in synthesis output immediately
3. At least 3 core parameters exposed (geometry, brightness, damping)
4. Parameter interpolation smooth (no zipper noise)
5. Adapter code clearly separated from Elements DSP code

**Prerequisites:** Story 1.6

---

### Story 1.8: Bus Routing and Audio I/O

As a user,
I want proper audio input/output through disting NT's bus system,
So that I can integrate nt_elements with other modules.

**Acceptance Criteria:**
1. Mono/stereo audio input supported
2. Stereo output routes to NT buses correctly
3. External audio can excite the resonator
4. Output mode (add/replace) works correctly
5. No audio artifacts at bus boundaries

**Prerequisites:** Story 1.7

---

### Story 1.9: Desktop Testing Validation

As a developer,
I want to validate all functionality works in nt_emu,
So that I can iterate quickly without hardware.

**Acceptance Criteria:**
1. All features from Stories 1.1-1.8 work in nt_emu
2. Performance metrics captured (CPU usage estimate)
3. No crashes during 30-minute stress test
4. Memory usage within expected bounds
5. Test results documented for hardware comparison

**Prerequisites:** All previous Epic 1 stories (1.1-1.8)

---

### Story 1.10: Hardware Deployment and Validation

As a user,
I want to load nt_elements on actual disting NT hardware,
So that I can verify it works in real Eurorack environment.

**Acceptance Criteria:**
1. Plugin (.o file) compiles successfully for ARM hardware
2. Loads on disting NT without errors
3. Audio output sounds identical to nt_emu version
4. CPU usage measured and documented (target < 50% at this stage)
5. Operates stably for 1-hour test
6. MIDI and audio I/O work correctly in hardware

**Prerequisites:** Story 1.9

---

(Story 1.11 removed - Single 32kHz mode only, no mode selection needed)

---

## Epic 2: UI, Presets & Production Polish

**Timeline:** Weeks 5-6
**Story Count:** 5 stories
**Goal:** Complete the user-facing interface with all 4 parameter pages and final production validation. Transform the functional plugin from Epic 1 into a polished, production-ready release with performance optimization, comprehensive testing, and authentic sound validation.

**Value Delivery:** Delivers complete, polished product ready for community release with intuitive interface and validated performance/sound quality.

---

### Story 2.1: Complete Parameter Page System

As a user,
I want all 4 parameter pages (Exciter, Resonator, Space, Performance) fully implemented,
So that I can access all Elements synthesis parameters via the NT interface.

**Acceptance Criteria:**
1. Page 1 (Exciter): All exciter parameters mapped (bow/blow balance, strike, contour, timbres)
2. Page 2 (Resonator): All resonator parameters mapped (geometry, brightness, damping, position, inharmonicity)
3. Page 3 (Space): Reverb parameters mapped (amount, size, damping)
4. Page 4 (Performance): Global parameters mapped (tuning, FM, output level)
5. Button navigation cycles through pages smoothly
6. OLED display shows current page and parameter values clearly
7. All 3 pots + 2 encoders functional on each page

**Prerequisites:** Story 1.7 (parameter adapter)

---

### Story 2.2: OLED Display Implementation

As a user,
I want clear visual feedback on the OLED display,
So that I always know which page I'm on and current parameter values.

**Acceptance Criteria:**
1. Page title displayed prominently (e.g., "EXCITER", "RESONATOR")
2. Active parameter names and values shown for pots/encoders
3. Display updates immediately when parameters change
4. Value representation appropriate (percentages, descriptive text, numeric)
5. Display remains readable in typical Eurorack lighting
6. No flicker or visual artifacts during updates

**Prerequisites:** Story 2.1

---

(Story 2.3 removed - NT firmware handles preset save/load automatically)

---

(Story 2.4 removed - Factory presets not needed, default parameters provide good initial sound)

---

### Story 2.5: Performance Optimization

As a developer,
I want CPU usage optimized to meet < 30% target,
So that users can chain nt_elements with other NT algorithms.

**Acceptance Criteria:**
1. CPU profiling completed on hardware NT
2. Hot paths identified and optimized
3. CPU usage measured < 30% at 32kHz
4. Reverb quality adjusted if needed to meet target
5. Compiler optimizations enabled (-O3, -ffast-math)
6. Performance documented for different configurations

**Prerequisites:** Story 1.10 (hardware validation)

---

### Story 2.6: Sound Quality A/B Validation

As a user,
I want nt_elements to sound identical to hardware Elements,
So that I get authentic modal synthesis.

**Acceptance Criteria:**
1. Side-by-side recordings captured (hardware Elements vs nt_elements)
2. Spectral analysis shows matching frequency response
3. At least 3 beta testers confirm sound authenticity in blind tests
4. Pitch accuracy validated across full MIDI range
5. All exciter modes (bow/blow/strike) behave identically
6. Resonator response matches hardware across geometry settings
7. Any differences documented with explanation

**Prerequisites:** Story 2.1 (parameter pages for testing different synthesis modes)

---

### Story 2.7: Final Testing and Release Preparation

As a developer,
I want comprehensive testing and release artifacts prepared,
So that nt_elements is production-ready for community release.

**Acceptance Criteria:**
1. Extended stability test: 4+ hours continuous operation, no crashes
2. Stress test: Rapid parameter changes, MIDI bursts, no glitches
3. All acceptance criteria from previous stories verified
4. .o plugin file built and validated on hardware
5. MIT license compliance verified (Elements attribution included)
6. README with installation instructions created
7. Known issues documented (if any)
8. Release notes prepared

**Prerequisites:** All previous Epic 1 and Epic 2 stories

---

## Story Guidelines Reference

**Story Format:**

```
**Story [EPIC.N]: [Story Title]**

As a [user type],
I want [goal/desire],
So that [benefit/value].

**Acceptance Criteria:**
1. [Specific testable criterion]
2. [Another specific criterion]
3. [etc.]

**Prerequisites:** [Dependencies on previous stories, if any]
```

**Story Requirements:**

- **Vertical slices** - Complete, testable functionality delivery
- **Sequential ordering** - Logical progression within epic
- **No forward dependencies** - Only depend on previous work
- **AI-agent sized** - Completable in 2-4 hour focused session
- **Value-focused** - Integrate technical enablers into value-delivering stories

---

## Epic 3: Dynamic Sample Loading from SD Card

**Timeline:** Post-release enhancement
**Story Count:** 5 stories
**Goal:** Replace static compiled sample data with dynamic loading from SD card, reducing binary size and enabling user-customizable wavetables.

**See:** [Epic 3 Detail Document](./epic-3-dynamic-sample-loading.md)

---

---

## Epic 4: Audio Quality and Missing Parameters

**Timeline:** Post Epic 3
**Story Count:** 9 stories
**Goal:** Address audio output level issues, expose missing synthesis parameters, and implement dual external input routing to match original Elements hardware.

**See:** [Epic 4 Detail Document](./epic-4-audio-quality-and-parameters.md)

---

**For implementation:** Use the `create-story` workflow to generate individual story implementation plans from this epic breakdown.
