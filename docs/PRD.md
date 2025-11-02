# nt_elements Product Requirements Document (PRD)

**Author:** Neal
**Date:** 2025-10-26
**Project Level:** 2
**Target Scale:** Medium project - 5-15 stories across 2-3 epics

---

## Goals and Background Context

### Goals

1. **Deliver production-ready nt_elements v1.0 within 6 weeks** - Complete C++ plugin with authentic Elements sound
2. **Achieve < 30% CPU usage** - Maintain performance headroom for algorithm chaining on disting NT
3. **Preserve 100% sound authenticity** - Indistinguishable from hardware Elements in A/B testing
4. **Reach 100+ downloads in first month** - Demonstrate community adoption and value
5. **Establish MI porting methodology** - Create repeatable process for future Mutable Instruments ports (Rings, Plaits, Braids)
6. **Respond to community demand** - Build 80%+ positive sentiment in Discord/forums
7. **Enable NT platform growth** - Position disting NT as destination for premium synthesis algorithms

### Background Context

**The Problem:** disting NT users seeking modal synthesis - particularly the distinctive sound of Mutable Instruments Elements - face limited options. Hardware Elements modules are discontinued, expensive ($500+), and scarce on the used market. The NT platform lacks high-quality physical modeling synthesis, creating both unmet user demand and an opportunity to expand the platform's value proposition.

**Community Demand:** Active requests from the disting NT Discord community validate market need. Elements is consistently mentioned as a desired synthesis option, demonstrating clear demand for this capability.

**Technical Validation:** Detailed research confirms this is achievable with low risk. Direct C++ port approach preserves 100% sound authenticity while leveraging disting NT's 3-5x performance advantage (480MHz ARM Cortex-M7 vs Elements' 168MHz M4). Multiple successful precedents (VCV Rack, Max/MSP, Daisy Seed) validate the technical approach with 4-6 week timeline.

**Strategic Context:** This project establishes a repeatable methodology for porting Mutable Instruments modules to disting NT. Success enables future ports (Rings, Plaits, Braids) and positions NT as the platform for premium, rare, or discontinued module sounds. Elements' MIT license permits this authentic port with proper attribution.

---

## Requirements

### Functional Requirements

**FR001:** Port complete Elements DSP engine including all three exciter modes (Bow, Blow, Strike)

**FR002:** Implement full 64-filter modal resonator with geometry, brightness, damping, and position controls

**FR003:** Include string and tube modeling algorithms from Elements codebase

**FR004:** Integrate multistage envelope generators for exciter and resonator control

**FR005:** Implement reverb section with amount, size, and damping parameters

**FR006:** Create 4 parameter pages organized as: Exciter, Resonator, Space (reverb), Performance

**FR007:** Map all parameters to disting NT's 3 pots + 2 encoders + 4 buttons interface

**FR008:** Display parameter values and page information on OLED screen

**FR009:** Support mono/stereo audio input with external audio routing to resonator

**FR010:** Output stereo audio integrated with NT's 28-bus routing system

**FR011:** Implement MIDI note input for pitch control with note-on/note-off and pitch bend

(FR012 removed - NT firmware handles preset management automatically)

(FR013 removed - Users create presets via NT firmware, default parameters provide good initial sound)

**FR014:** Process audio at Elements' native 32kHz sample rate with disting NT handling platform resampling as needed

**FR015:** Implement smooth parameter changes without zipper noise or audio artifacts

### Non-Functional Requirements

**NFR001: Performance** - CPU usage must remain below 30% under typical operating conditions to allow algorithm chaining. Target 20-30% with all features active. No audio dropouts or glitches during sustained operation (validated via 1-hour stress test).

**NFR002: Sound Quality** - Audio output must be indistinguishable from hardware Elements module in A/B comparison testing. Frequency response, timbral characteristics, and synthesis behavior must match original within measurable tolerances. Verified via spectral analysis and blind listening tests with Elements owners.

**NFR003: Reliability** - Plugin must load and operate stably on disting NT hardware without crashes, undefined behavior, or memory leaks. All parameters must respond correctly and predictably. System must handle edge cases gracefully (extreme parameter values, rapid parameter changes, MIDI input bursts).

---

## User Journeys

### Journey 1: First-Time User Creating Modal Synthesis Patch

**Actor:** Sarah, an intermediate disting NT user exploring modal synthesis

**Goal:** Load nt_elements and create a resonant bell-like sound

**Steps:**

1. **Discovery & Download**
   - Sarah sees nt_elements announced in disting NT Discord
   - Downloads .o plugin file from NT plugin gallery
   - Copies file to disting NT SD card

2. **Initial Load**
   - Powers up disting NT in Eurorack system
   - Navigates to plugin loader
   - Selects nt_elements from list
   - Plugin loads, displays "Page 1: Exciter" on OLED

3. **Exploration**
   - Tries factory preset "Bell Strike"
   - Sends MIDI note from keyboard, hears resonant bell tone
   - Realizes this is the Elements sound she's heard in demos

4. **Parameter Navigation**
   - Presses button to switch to Page 2: Resonator
   - Adjusts Pot 1 (Geometry) - hears material change from metal to wood
   - Adjusts Pot 2 (Brightness) - controls harmonic content
   - Switches to Page 3: Space, adds reverb with Pot 1

5. **Refinement**
   - Returns to Page 1: Exciter
   - Adjusts strike level and contour to shape attack
   - Tests with different MIDI velocities, hears dynamic response
   - Creates evolving texture by modulating brightness with CV

6. **Save & Use**
   - Saves patch as "My Bell" using NT preset system
   - Integrates into larger Eurorack patch
   - Routes nt_elements output through external filter
   - Records performance, shares on social media

**Success Criteria:**
- Loads plugin within 2 minutes
- Creates first sound within 5 minutes
- Understands parameter organization intuitively
- Successfully saves and recalls preset
- Feels the sound is authentic Elements character

---

## UX Design Principles

1. **Familiar Mental Model** - Parameter organization mirrors hardware Elements' three-section layout (Exciter/Resonator/Space), making it intuitive for users familiar with Elements

2. **Immediate Tactile Control** - All essential parameters accessible via hardware controls (3 pots, 2 encoders) without menu diving

3. **Clear Context Indication** - OLED display always shows current page and parameter assignments, eliminating confusion about what controls do

4. **Progressive Disclosure** - 4-page organization allows focused control over each synthesis section without overwhelming users

---

## User Interface Design Goals

**Platform:** Hardware-based interface on disting NT Eurorack module
- 3 continuous rotation potentiometers
- 2 rotary encoders (with push)
- 4 buttons for navigation
- Small OLED display (128x64 pixels typical)

**Core Screens/Pages:**

**Page 1 - Exciter:**
- Pot 1: Bow/Blow Balance
- Pot 2: Strike Level
- Pot 3: Contour (envelope)
- Encoder 1: Bow Timbre
- Encoder 2: Blow Timbre
- Display: "EXCITER | Bow: 50% Strike: 30%"

**Page 2 - Resonator:**
- Pot 1: Geometry (material type)
- Pot 2: Brightness (filter cutoff)
- Pot 3: Damping (decay)
- Encoder 1: Position (strike position)
- Encoder 2: Inharmonicity
- Display: "RESONATOR | Geom: String Bright: 45%"

**Page 3 - Space (Reverb):**
- Pot 1: Reverb Amount
- Pot 2: Reverb Size
- Pot 3: Reverb Damping
- Display: "SPACE | Amt: 35% Size: Large"

**Page 4 - Performance:**
- Pot 1: Coarse Tune
- Pot 2: Fine Tune
- Pot 3: FM Amount
- Encoder 1: Output Level
- Display: "PERF | Tune: +2.0 FM: 20%"

**Navigation:**
- Buttons cycle through pages
- Display updates immediately to show current context
- Parameter values shown as percentage or descriptive text

**Design Constraints:**
- Fixed hardware interface (cannot add controls)
- Limited display real estate
- Must work with NT's existing UI framework
- Monochrome OLED display

---

## Epic List

### Epic 1: Project Foundation & Core DSP Integration
**Timeline:** Weeks 1-4
**Goal:** Establish development environment, build system, and integrate Elements DSP core at native 32kHz with basic audio I/O and parameter control on disting NT platform
**Estimated Stories:** 9

**Key Deliverables:**
- Working build pipeline (ARM toolchain, nt_emu testing)
- Elements DSP modules integrated and compiling at native 32kHz
- Basic audio processing functional
- Parameter adapter layer operational
- MIDI and audio I/O integration complete

### Epic 2: UI, Presets & Production Polish
**Timeline:** Weeks 5-6
**Goal:** Complete 4-page parameter interface, performance optimization, and final testing for production release
**Estimated Stories:** 5

**Key Deliverables:**
- All 4 parameter pages implemented
- Performance validated (< 30% CPU)
- A/B sound validation complete
- Production-ready .o plugin file

> **Note:** Detailed epic breakdown with full story specifications is available in [epics.md](./epics.md)

---

## Out of Scope

**Explicitly Excluded from v1.0:**

1. **Polyphony** - Single voice only (hardware Elements is monophonic)
2. **Extended modulation routing** - Beyond basic MIDI and NT parameter CV
3. **Custom visualizations** - Basic OLED display only, no elaborate graphics
4. **Alternate algorithms or variations** - Faithful Elements port only, no experimental modes
5. **Stereo resonator processing** - Mono processing like hardware
6. **Additional effects beyond reverb** - Only Elements' built-in reverb
7. **Desktop plugin versions** - disting NT hardware only (no VST/AU)
8. **Advanced calibration** - Use NT's existing calibration system
9. **Legacy hardware Elements support** - No hardware interface or I2C communication
10. **Extensive documentation** - Minimal docs, assumes Elements knowledge

**Deferred to Future Versions:**

- Performance optimizations beyond 30% CPU target
- Extended modulation matrix (v1.1+)
- Additional exciter models or resonator modes (v1.2+)
- Limited polyphony if CPU allows (v2.0+)
- Community preset library integration (post-release)
- Porting to other embedded platforms (Daisy, Teensy) - separate projects

**Platform Boundaries:**

- disting NT C++ plugin only
- No standalone hardware version
- No integration with other Eurorack control standards (I2C, USB host) in v1.0
- No editor software or companion apps
