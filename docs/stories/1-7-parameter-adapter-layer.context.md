<story-context id="bmad/bmm/workflows/4-implementation/story-context/template" v="1.0">
  <metadata>
    <epicId>1</epicId>
    <storyId>7</storyId>
    <title>Parameter Adapter Layer</title>
    <status>drafted</status>
    <generatedAt>2025-10-26</generatedAt>
    <generator>BMAD Story Context Workflow</generator>
    <sourceStoryPath>docs/stories/1-7-parameter-adapter-layer.md</sourceStoryPath>
  </metadata>

  <story>
    <asA>developer</asA>
    <iWant>a clean adapter between NT parameters and Elements patch format</iWant>
    <soThat>NT parameter changes update Elements synthesis correctly</soThat>
    <tasks>
      - Define parameter enumeration (AC: #3)
        - Create ParameterIndices enum in src/parameter_adapter.h
        - Define kParamGeometry, kParamBrightness, kParamDamping
        - Add bus routing parameters (input/output bus, output mode)
        - Add kNumParameters count
        - Document parameter index ranges

      - Create parameter adapter header (AC: #5)
        - Create src/parameter_adapter.h
        - Declare parameter conversion functions
        - Define parameter value ranges (NT: 0-100%, Elements: 0.0-1.0)
        - Add helper functions for range mapping
        - Include Elements Patch structure forward declaration

      - Implement parameterChanged() callback (AC: #1, #2)
        - Add parameterChanged() to NT_algorithm structure
        - Implement switch statement for each parameter index
        - Convert NT parameter value (0-100%) to Elements range (0.0-1.0)
        - Update corresponding Elements Patch field
        - Verify immediate update without buffering

      - Implement core parameter mappings (AC: #3)
        - Map kParamGeometry → patch.resonator_geometry
        - Map kParamBrightness → patch.resonator_brightness
        - Map kParamDamping → patch.resonator_damping
        - Test each parameter independently
        - Verify correct value scaling (0-100% → 0.0-1.0)

      - Initialize default parameter values (AC: #2)
        - Set reasonable defaults in construct() callback
        - Initialize Patch structure with default values
        - Ensure synthesis starts with known good state
        - Document default parameter choices
        - Test that defaults produce stable sound

      - Test parameter smoothness (AC: #4)
        - Sweep brightness from 0-100% while synthesizing
        - Listen for zipper noise or artifacts
        - Verify Elements internal smoothing handles transitions
        - Test rapid parameter changes (stress test)
        - Document any smoothing issues discovered

      - Validate parameter to audio path (AC: #2)
        - Change geometry parameter, verify timbre change
        - Change brightness parameter, verify frequency response change
        - Change damping parameter, verify decay time change
        - Measure latency from parameter change to audio effect
        - Verify parameters update without audio glitches

      - Desktop testing in nt_emu (AC: #1-5)
        - Load plugin in nt_emu
        - Test parameter control via VCV Rack knobs
        - Verify immediate parameter response
        - Test all three core parameters
        - Check for audio artifacts during parameter changes

      - Code separation validation (AC: #5)
        - Verify no modifications to Elements DSP source files
        - Verify all adapter code in src/ directory
        - Review code organization and clarity
        - Add comments explaining parameter mappings
        - Ensure clean architectural boundaries
    </tasks>
  </story>

  <acceptanceCriteria>
    1. Adapter layer converts NT float parameters to Elements patch structure
    2. Parameter changes reflect in synthesis output immediately
    3. At least 3 core parameters exposed (geometry, brightness, damping)
    4. Parameter interpolation smooth (no zipper noise)
    5. Adapter code clearly separated from Elements DSP code
  </acceptanceCriteria>

  <artifacts>
    <docs>
      <doc>
        <path>docs/PRD.md</path>
        <title>Product Requirements Document</title>
        <section>FR015: Smooth Parameter Changes</section>
        <snippet>Implement smooth parameter changes without zipper noise or audio artifacts. Elements DSP includes internal parameter smoothing to prevent audio glitches.</snippet>
      </doc>
      <doc>
        <path>docs/PRD.md</path>
        <title>Product Requirements Document</title>
        <section>UI Design - Page 2 Resonator</section>
        <snippet>Pot 1: Geometry (material type), Pot 2: Brightness (filter cutoff), Pot 3: Damping (decay). Display: "RESONATOR | Geom: String Bright: 45%"</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Parameter Update Pattern</section>
        <snippet>Parameters update via parameterChanged() callback. Convert NT parameter (0-100%) to Elements Patch (0.0-1.0). Elements handles internal smoothing, no external smoothing needed in adapter layer.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Data Architecture - Elements Patch Structure</section>
        <snippet>Elements Patch contains: float note (frequency in Hz), bool gate (note on/off), resonator_geometry (0.0-1.0 material type), resonator_brightness (0.0-1.0 filter cutoff), resonator_damping (0.0-1.0 decay rate).</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Data Architecture - NT Parameter Array</section>
        <snippet>Parameter enumeration includes kParamInputBus, kParamOutputBus, kParamOutputMode, kParamGeometry, kParamBrightness, kParamDamping, and additional parameters organized by page (Exciter, Resonator, Space, Performance).</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Parameters - Mapping Strategy</section>
        <snippet>Decision: Direct copy to Patch struct (Story 1.7). Simple translation, Elements handles smoothing internally. Affects Epic 1.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Consistency Rules - Naming Conventions</section>
        <snippet>Functions: camelCase (parameterChanged, processAudio). Variables: snake_case (input_buffer, sample_rate_mode). Constants: k-prefix PascalCase (kParamBrightness). Parameter naming uses descriptive names matching PRD.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Consistency Rules - Code Organization</section>
        <snippet>Keep adapter code separate from Elements DSP (never modify submodule files). All customization in adapter layer. Elements DSP files in external/mutable-instruments/ are read-only.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Consistency Rules - Error Handling</section>
        <snippet>Defensive programming: Clamp parameter values using std::max/std::min. Check bounds before array access. No exceptions (compiler flag: -fno-exceptions).</snippet>
      </doc>
      <doc>
        <path>docs/epics.md</path>
        <title>Epic Breakdown</title>
        <section>Story 1.7: Parameter Adapter Layer</section>
        <snippet>AC: 1) Adapter converts NT float to Elements patch, 2) Parameter changes reflect immediately, 3) At least 3 core parameters (geometry, brightness, damping), 4) Smooth interpolation (no zipper noise), 5) Adapter code clearly separated.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Project Structure</section>
        <snippet>src/ directory contains: nt_elements.cpp (main plugin, factory, callbacks, adapter), parameter_adapter.h (NT parameters → Elements Patch mapping), parameter_pages.h (4-page UI definition).</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Epic to Architecture Mapping - Story 1.7</section>
        <snippet>Story 1.7 implements NT params → Elements Patch mapping using src/parameter_adapter.h and parameterChanged() callback. Converts parameter values and updates Elements synthesis engine.</snippet>
      </doc>
    </docs>

    <code>
      <artifact>
        <path>external/mutable-instruments/elements/dsp/patch.h</path>
        <kind>header</kind>
        <symbol>Patch</symbol>
        <lines>N/A (not yet created)</lines>
        <reason>Elements Patch structure definition - target data structure for parameter adapter conversion. Contains resonator_geometry, resonator_brightness, resonator_damping fields.</reason>
      </artifact>
      <artifact>
        <path>distingNT_API/include/distingNT.h</path>
        <kind>header</kind>
        <symbol>_NT_algorithm</symbol>
        <lines>N/A (submodule)</lines>
        <reason>distingNT API base structure containing v[] parameter array and parameterChanged() callback definition. Source of NT parameter values to be converted.</reason>
      </artifact>
      <artifact>
        <path>src/parameter_adapter.h</path>
        <kind>header</kind>
        <symbol>ParameterIndices</symbol>
        <lines>N/A (to be created)</lines>
        <reason>Parameter enumeration defining kParamGeometry, kParamBrightness, kParamDamping indices. Core deliverable of this story for organizing parameter mapping.</reason>
      </artifact>
      <artifact>
        <path>src/nt_elements.cpp</path>
        <kind>implementation</kind>
        <symbol>parameterChanged</symbol>
        <lines>N/A (to be modified)</lines>
        <reason>Main callback implementation that performs NT parameter to Elements Patch conversion. Implements the adapter layer logic central to this story.</reason>
      </artifact>
      <artifact>
        <path>src/nt_elements.cpp</path>
        <kind>implementation</kind>
        <symbol>construct</symbol>
        <lines>N/A (to be modified)</lines>
        <reason>Initialization callback where default parameter values are set. Ensures plugin starts with known good state (AC #2).</reason>
      </artifact>
    </code>

    <dependencies>
      <cpp>
        <item>arm-none-eabi-gcc</item>
        <description>ARM cross-compiler toolchain for Cortex-M7</description>
      </cpp>
      <submodules>
        <item>distingNT_API</item>
        <description>Expert Sleepers disting NT C++ plugin API (git submodule)</description>
      </submodules>
      <submodules>
        <item>external/mutable-instruments</item>
        <description>Mutable Instruments Elements DSP source code (git submodule, MIT licensed)</description>
      </submodules>
      <tools>
        <item>VCV Rack + nt_emu</item>
        <description>Desktop testing environment for NT plugin development</description>
      </tools>
    </dependencies>
  </artifacts>

  <constraints>
    - NEVER modify files in external/mutable-instruments/ directory - Elements DSP is read-only submodule
    - All adapter code must be in src/ directory, clearly separated from Elements source
    - Parameter conversion must be simple direct mapping - Elements handles internal smoothing
    - No external smoothing in adapter layer - would cause duplicate smoothing and sluggish response
    - Follow C++ naming conventions: camelCase for functions, snake_case for variables, k-prefix PascalCase for constants
    - Defensive programming: clamp parameter values, check bounds, no exceptions allowed (-fno-exceptions)
    - Parameter values: NT uses 0-100% range (float), Elements Patch uses 0.0-1.0 normalized range (float)
    - Story depends on Story 1.6 (MIDI Note Input Integration) completion
    - Desktop-first development: test in nt_emu before hardware deployment
    - Core parameters for this story: geometry, brightness, damping (minimal viable set, more added in Epic 2)
  </constraints>

  <interfaces>
    <interface>
      <name>parameterChanged</name>
      <kind>callback function</kind>
      <signature>void parameterChanged(_NT_algorithm* self, int p)</signature>
      <path>distingNT_API/include/distingNT.h</path>
      <description>NT framework callback invoked when a parameter changes. Receives parameter index p. Adapter must convert v[p] value to Elements Patch field.</description>
    </interface>
    <interface>
      <name>Elements::Patch structure</name>
      <kind>data structure</kind>
      <signature>struct Patch { float note; bool gate; float resonator_geometry; float resonator_brightness; float resonator_damping; /* ... */ }</signature>
      <path>external/mutable-instruments/elements/dsp/patch.h</path>
      <description>Elements DSP parameter container. Adapter writes converted NT parameters to these fields. Range: 0.0-1.0 for most parameters.</description>
    </interface>
    <interface>
      <name>NT parameter array</name>
      <kind>data structure</kind>
      <signature>float v[kNumParameters]</signature>
      <path>distingNT_API/include/distingNT.h (part of _NT_algorithm)</path>
      <description>NT parameter storage. Indexed by ParameterIndices enum. Source values for conversion. Typical range: 0-100%.</description>
    </interface>
  </interfaces>

  <tests>
    <standards>
      This is an embedded C++ audio plugin project with no traditional unit testing framework. Testing follows a desktop-first manual validation approach:
      1) Desktop testing in nt_emu (VCV Rack module) for rapid iteration with visual/audio feedback
      2) Hardware validation on actual disting NT at key milestones
      3) Manual audio quality verification (listening tests, spectral analysis)
      4) Performance profiling using NT built-in CPU display

      Validation criteria: audible parameter response, smooth transitions (no zipper noise), no crashes during stress tests (rapid parameter changes), stable operation during extended runs.
    </standards>

    <locations>
      No automated test files - manual testing approach:
      - Desktop testing: VCV Rack with nt_emu module (load plugins/nt_elements.dylib)
      - Hardware testing: disting NT hardware (deploy plugins/nt_elements.o to SD card)
      - Validation documented in story completion notes within story file
    </locations>

    <ideas>
      - AC #1 Test: Load plugin in nt_emu, change geometry parameter (0→100%), verify Elements Patch.resonator_geometry updates correctly (0.0→1.0). Repeat for brightness and damping.

      - AC #2 Test: Trigger MIDI note, adjust brightness parameter while note sustains, verify immediate timbre change with no delay or latency. Use oscilloscope or ears to confirm instant response.

      - AC #3 Test: Verify exactly 3 core parameters implemented: geometry (kParamGeometry), brightness (kParamBrightness), damping (kParamDamping). Check parameter_adapter.h enum definition.

      - AC #4 Test: Slowly sweep brightness from 0→100% while playing sustained note. Listen carefully for zipper noise, clicks, or stepping artifacts. Confirm Elements' internal smoothing produces glitch-free transitions.

      - AC #5 Test: Code inspection - verify no modifications to any files in external/mutable-instruments/. Verify all adapter code exists only in src/parameter_adapter.h and src/nt_elements.cpp. Check git submodule status shows clean Elements repository.

      - Stress Test: Rapidly toggle all three parameters (random changes every 10ms) while synthesizing. Confirm no crashes, glitches, or audio dropouts.

      - Default Values Test: Load plugin fresh (no preset), trigger MIDI note without adjusting parameters. Verify pleasant, stable sound output. Defaults should be: Geometry 50%, Brightness 70%, Damping 60%.

      - Boundary Test: Set each parameter to minimum (0%), maximum (100%), and mid-point (50%). Verify clamping works correctly and no undefined behavior at extremes.
    </ideas>
  </tests>
</story-context>
