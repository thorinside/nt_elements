<story-context id="bmad/bmm/workflows/4-implementation/story-context/template" v="1.0">
  <metadata>
    <epicId>2</epicId>
    <storyId>3</storyId>
    <title>Preset Save/Load Integration</title>
    <status>drafted</status>
    <generatedAt>2025-10-26</generatedAt>
    <generator>BMAD Story Context Workflow</generator>
    <sourceStoryPath>docs/stories/2-3-preset-save-load-integration.md</sourceStoryPath>
  </metadata>

  <story>
    <asA>a user</asA>
    <iWant>to save and load my patches using NT's preset system</iWant>
    <soThat>I can recall my favorite sounds quickly</soThat>
    <tasks>
- Research NT preset system (AC: #1-3, #6)
  - Study distingNT_API preset documentation
  - Identify preset save/load callbacks or mechanisms
  - Understand parameter serialization format
  - Document NT preset system architecture
  - Note any special handling required

- Verify automatic parameter serialization (AC: #1, #2)
  - Confirm NT framework auto-serializes parameter array
  - Test saving preset via NT save mechanism
  - Verify all parameters included in saved preset
  - Check preset file format (if accessible)
  - Document what gets saved automatically

- Implement preset load callback (AC: #3)
  - Add preset_loaded() callback if required by NT API
  - Reinitialize Elements DSP with loaded parameters
  - Apply all parameter values to Elements Patch
  - Update display to reflect loaded parameters
  - Verify complete state restoration

- Implement glitch-free preset switching (AC: #4)
  - Mute output briefly during preset load
  - Apply all parameters atomically
  - Reinitialize any stateful components
  - Fade output back in smoothly
  - Test rapid preset switching (no clicks/pops)

- Verify sample rate mode persistence (AC: #5)
  - Save preset in 48kHz mode
  - Switch to 32kHz mode
  - Load preset (should restore 48kHz mode)
  - Verify mode parameter included in serialization
  - Test with multiple presets in different modes

- Test preset save functionality (AC: #1, #2)
  - Configure unique parameter settings
  - Save preset via NT interface
  - Verify preset saved successfully
  - Change parameters to different values
  - Verify preset file contains correct data (if accessible)

- Test preset load functionality (AC: #3, #6)
  - Load previously saved preset
  - Verify all parameters restored correctly
  - Listen to synthesis (should match saved state)
  - Verify display shows correct parameter values
  - Verify preset name displays in NT interface

- Test preset switching scenarios (AC: #4)
  - Create multiple presets with different settings
  - Rapidly switch between presets
  - Listen for audio glitches or artifacts
  - Verify smooth transitions
  - Test preset switching during note-on (held note)

- Create test preset library (AC: #1-6)
  - Create 3-5 test presets with distinct settings
  - Document expected parameter values for each
  - Use test presets for validation
  - Save test presets for factory preset reference (Story 2.4)
  - Document test preset characteristics

- Desktop testing in nt_emu (AC: #1-6)
  - Test preset save/load in nt_emu (if supported)
  - Verify all acceptance criteria in desktop environment
  - Document any nt_emu preset limitations
  - Prepare for hardware validation

- Hardware testing (AC: #1-6)
  - Deploy to disting NT hardware
  - Test preset save/load on actual hardware
  - Verify preset persistence across power cycles
  - Test preset management via NT interface
  - Document hardware preset validation results
    </tasks>
  </story>

  <acceptanceCriteria>
1. All parameter states serialize correctly to NT preset format
2. Presets save via NT's standard save mechanism
3. Presets load and restore all parameters accurately
4. Preset switching does not cause audio glitches
5. Sample rate mode (32/48kHz) saved with preset
6. Preset names display in NT interface
  </acceptanceCriteria>

  <artifacts>
    <docs>
- path: docs/PRD.md
  title: Product Requirements Document
  section: FR012 - Preset save/load requirement
  snippet: "Support preset save/load via NT's preset management system"

- path: docs/PRD.md
  title: Product Requirements Document
  section: NFR003 - Reliability requirement
  snippet: "Plugin must load and operate stably on disting NT hardware without crashes, undefined behavior, or memory leaks. All parameters must respond correctly and predictably. System must handle edge cases gracefully (extreme parameter values, rapid parameter changes, MIDI input bursts)."

- path: docs/architecture.md
  title: Decision Architecture
  section: Preset System - Serialization
  snippet: "NT automatic parameter save/load via standard NT preset system. All parameters in the v[] array are automatically serialized when saving presets."

- path: docs/architecture.md
  title: Decision Architecture
  section: Preset System - Factory Presets
  snippet: "8 presets demonstrating timbres (Bell, String, Pipe, etc.) will be created in Story 2.4"

- path: docs/architecture.md
  title: Decision Architecture
  section: Data Architecture - NT Parameter Array
  snippet: "NT Parameter Array maintains parameter state with indices for all user-configurable parameters including kParamSampleRateMode, kParamBrightness, etc. All stored in v[] array."

- path: docs/epics.md
  title: Epic Breakdown
  section: Story 2.3 - Preset Save/Load Integration
  snippet: "All parameter states serialize correctly to NT preset format. Presets save via NT's standard save mechanism. Presets load and restore all parameters accurately. Preset switching does not cause audio glitches. Sample rate mode (32/48kHz) saved with preset."

- path: docs/stories/2-3-preset-save-load-integration.md
  title: Story 2.3 Full Specification
  section: Dev Notes - disting NT Preset System
  snippet: "The disting NT framework automatically serializes the parameter array (_NT_algorithm::v[]) when saving presets. No manual serialization code required for basic parameters."

- path: docs/stories/2-3-preset-save-load-integration.md
  title: Story 2.3 Full Specification
  section: Dev Notes - Parameter-Based State Management
  snippet: "All user-configurable state should be stored in parameters. This ensures automatic preset save/load. Sample rate mode: Stored in kParamSampleRateMode (parameter)."

- path: docs/stories/2-3-preset-save-load-integration.md
  title: Story 2.3 Full Specification
  section: Dev Notes - Preset Load Handling
  snippet: "Preset Load Sequence: 1) NT framework loads preset file, 2) NT framework updates all parameter values in v[] array, 3) NT framework calls preset_loaded() callback (if implemented), 4) Plugin updates internal state from new parameter values, 5) Display updates to reflect new parameters."

- path: docs/stories/2-3-preset-save-load-integration.md
  title: Story 2.3 Full Specification
  section: Dev Notes - Glitch-Free Preset Switching
  snippet: "Option A: Brief Mute - Set gate = false during preset load, apply all parameter changes, set gate = true after updates. Simple, effective, brief silence acceptable."

- path: docs/stories/2-3-preset-save-load-integration.md
  title: Story 2.3 Full Specification
  section: Dev Notes - Sample Rate Mode Persistence
  snippet: "Sample rate mode stored as parameter (kParamSampleRateMode). NT framework automatically saves/loads this parameter with presets."

- path: docs/stories/2-3-preset-save-load-integration.md
  title: Story 2.3 Full Specification
  section: Dev Notes - Testing Strategy
  snippet: "Create diverse presets for validation: Bell Strike (high brightness, short damping, strike exciter), Bowed String (bow exciter, medium geometry, long damping), Blown Pipe (blow exciter, tube geometry, medium brightness), Deep Resonance (low tuning, long damping, high reverb)."

- path: docs/stories/1-11-sample-rate-mode-selection.md
  title: Story 1.11 - Sample Rate Mode Selection
  section: Preset System Integration
  snippet: "Sample rate mode parameter is automatically saved/loaded with presets via NT preset system. Each preset remembers its sample rate mode."
    </docs>
    <code>
- path: (To be created) src/nt_elements.cpp
  kind: plugin main file
  symbol: preset_loaded() callback
  lines: TBD
  reason: Callback function to handle preset load events from NT framework. Will reinitialize Elements DSP with loaded parameters and update display.

- path: (To be created) src/parameter_adapter.h
  kind: header file
  symbol: Parameter mapping structures
  lines: TBD
  reason: Defines NT parameter indices (kParamSampleRateMode, kParamBrightness, etc.) that are automatically serialized by NT framework.

- path: (To be created) src/nt_elements.cpp
  kind: plugin main file
  symbol: parameterChanged() callback
  lines: TBD
  reason: Handles individual parameter updates. Will be used during preset load to apply all parameters to Elements Patch structure.

- path: distingNT_API/include/distingNT.h
  kind: API header
  symbol: _NT_algorithm structure
  lines: TBD
  reason: Defines the NT plugin interface including parameter array v[] that is automatically serialized for presets.

- path: (To be created) src/nt_elements.cpp
  kind: plugin main file
  symbol: Elements Part instance
  lines: TBD
  reason: The Elements synthesis engine that needs to be reinitialized when presets are loaded to ensure clean state.
    </code>
    <dependencies>
- C++11 - Language for Elements DSP and NT plugin adapter
- ARM GCC (arm-none-eabi-gcc) - Cross-compiler for ARM Cortex-M7
- distingNT API v9 - Expert Sleepers plugin API providing preset system
- Elements DSP - Mutable Instruments modal synthesis engine (MIT license)
- VCV Rack + nt_emu module - Desktop testing environment (if preset testing supported)
    </dependencies>
  </artifacts>

  <constraints>
- All memory must be allocated via NT_globals.getMemory() - never use malloc/new
- No exceptions allowed - compiler flag -fno-exceptions enforced
- Parameters must be stored in v[] array to enable automatic serialization
- Preset load must not cause audio glitches (brief muting acceptable)
- All parameter state must be restorable from v[] array alone
- Follow Elements C++ style for naming conventions
- Code must work on both desktop (nt_emu) and hardware targets
- Clean separation between adapter code and Elements DSP source (read-only submodule)
- Sample rate mode parameter must persist with presets
- No forward dependencies - build only on completed Epic 1 and Stories 2.1-2.2
  </constraints>

  <interfaces>
- name: NT Preset System Callbacks
  kind: C++ callback functions
  signature: void preset_loaded(_NT_algorithm* self, const char* preset_name)
  path: distingNT_API/include/distingNT.h
  description: Optional callback invoked by NT framework after preset parameters are loaded. Used to reinitialize plugin state.

- name: NT Parameter System
  kind: C++ parameter array
  signature: float v[kNumParameters] in _NT_algorithm structure
  path: distingNT_API/include/distingNT.h
  description: Array of parameter values automatically serialized/deserialized by NT framework during preset save/load.

- name: parameterChanged callback
  kind: C++ callback function
  signature: void parameterChanged(_NT_algorithm* self, int p)
  path: distingNT_API/include/distingNT.h
  description: Called by NT framework when parameter values change. Adapter converts NT parameters to Elements Patch fields.

- name: Elements Patch Structure
  kind: C++ struct
  signature: struct Patch { float note; bool gate; float resonator_brightness; float resonator_damping; ... }
  path: external/mutable-instruments/elements/dsp/patch.h
  description: Elements DSP patch structure containing all synthesis parameters. Populated from NT parameter array during preset load.

- name: Elements Part::Init
  kind: C++ member function
  signature: void Init(float sample_rate)
  path: external/mutable-instruments/elements/dsp/part.h
  description: Reinitializes Elements synthesis engine with specified sample rate. Called during preset load if sample rate mode changes.
  </interfaces>

  <tests>
    <standards>
Testing follows desktop-first workflow using VCV Rack + nt_emu module for rapid iteration, followed by hardware validation on disting NT. No formal unit test framework - validation is functional testing on both platforms. Performance testing uses NT's built-in CPU display. Stability testing includes extended operation (1-hour minimum) and stress tests with rapid parameter changes.

Desktop testing in nt_emu allows faster iteration with debugging capabilities. Hardware testing validates real-world performance, CPU usage, and preset persistence across power cycles. All acceptance criteria must pass on hardware before story completion.

Testing emphasizes defensive coding with bounds checking, clamping parameter values, and graceful error handling. No exceptions allowed due to embedded constraints.
    </standards>
    <locations>
- VCV Rack with nt_emu module - Desktop testing environment
- disting NT hardware - Final validation platform
- Build outputs: plugins/nt_elements.dylib (desktop), plugins/nt_elements.o (hardware)
    </locations>
    <ideas>
- AC#1: Test that all 20+ user parameters serialize correctly by creating preset with unique values, saving, changing all parameters, then loading to verify restoration

- AC#2: Verify presets save via NT's standard save mechanism by using NT interface to save preset and confirming file creation (if accessible)

- AC#3: Test preset load accuracy by creating multiple presets with different parameter combinations and verifying exact restoration of all values

- AC#4: Validate glitch-free preset switching by rapidly switching between presets during audio playback and listening for clicks/pops

- AC#5: Verify sample rate mode persistence by creating presets in both 32kHz and 48kHz modes and confirming mode switches correctly on preset load

- AC#6: Confirm preset names display in NT interface by saving presets with distinct names and verifying visibility in preset browser

- Edge case: Test preset loading during note-on (held note) to verify glitch-free transition even with active synthesis

- Edge case: Test rapid consecutive preset loads to ensure no parameter value corruption or state errors

- Integration: Create test preset library (Bell Strike, Bowed String, Blown Pipe, Deep Resonance) for validation and reference for Story 2.4

- Performance: Measure any CPU overhead from preset load operations to ensure it doesn't impact real-time audio

- Persistence: Test preset survival across power cycles on hardware to verify NT's preset storage mechanism

- Desktop vs Hardware: Document any differences in preset behavior between nt_emu and hardware platforms
    </ideas>
  </tests>
</story-context>
