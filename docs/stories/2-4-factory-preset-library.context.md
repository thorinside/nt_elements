<story-context id="2-4-factory-preset-library" v="1.0">
  <metadata>
    <epicId>2</epicId>
    <storyId>4</storyId>
    <title>Factory Preset Library</title>
    <status>drafted</status>
    <generatedAt>2025-10-26</generatedAt>
    <generator>BMAD Story Context Workflow</generator>
    <sourceStoryPath>docs/stories/2-4-factory-preset-library.md</sourceStoryPath>
  </metadata>

  <story>
    <asA>a user</asA>
    <iWant>a curated set of factory presets demonstrating different sounds</iWant>
    <soThat>I can quickly explore nt_elements' capabilities and learn the synthesis</soThat>
    <tasks>
      - Design factory preset library (AC: #1, #2)
        - Plan 8+ presets covering diverse timbres
        - Ensure presets demonstrate different synthesis modes
        - Cover exciter types (bow, blow, strike)
        - Cover resonator geometries (string, tube, mixed)
        - Plan preset names (clear, descriptive)

      - Create Bell Strike preset (AC: #1)
        - Configure strike exciter (high level)
        - Set metallic geometry
        - Set high brightness
        - Set medium-short damping
        - Test and refine sound
        - Document parameter values

      - Create Bowed String preset (AC: #1)
        - Configure bow exciter (high level)
        - Set string geometry
        - Set medium brightness
        - Set long damping (sustained)
        - Test and refine sound
        - Document parameter values

      - Create Blown Pipe preset (AC: #1)
        - Configure blow exciter (high level)
        - Set tube geometry
        - Set medium brightness
        - Set medium damping
        - Test and refine sound
        - Document parameter values

      - Create Wooden Mallet preset (AC: #1)
        - Configure strike exciter (medium level)
        - Set warm geometry (mid-range)
        - Set medium-low brightness
        - Set medium damping
        - Test and refine sound
        - Document parameter values

      - Create Glass Chime preset (AC: #1)
        - Configure strike exciter (light touch)
        - Set bright/crystalline geometry
        - Set high brightness
        - Set medium-long damping
        - Add reverb for spaciousness
        - Test and refine sound
        - Document parameter values

      - Create Bass Drum preset (AC: #1)
        - Configure strike exciter (heavy hit)
        - Set low geometry (deep resonance)
        - Set low brightness (dark)
        - Set medium damping
        - Set low tuning (coarse tune down)
        - Test and refine sound
        - Document parameter values

      - Create Ambient Pad preset (AC: #1)
        - Configure bow + blow exciters (mixed)
        - Set evolving geometry
        - Set medium-high brightness
        - Set long damping
        - Add significant reverb
        - Test and refine sound
        - Document parameter values

      - Create Plucked String preset (AC: #1)
        - Configure strike exciter (quick attack)
        - Set string geometry
        - Set medium-high brightness
        - Set medium damping (natural decay)
        - Test and refine sound
        - Document parameter values

      - Implement preset data structure (AC: #3)
        - Define factory preset structure in code
        - Store parameter values for each preset
        - Store preset names
        - Implement preset initialization function
        - Document preset data format

      - Integrate factory presets with NT system (AC: #3)
        - Research NT factory preset mechanism
        - Implement factory preset loading
        - Ensure presets appear in NT preset browser
        - Verify factory presets ship with plugin
        - Test preset loading from factory set

      - Create preset documentation (AC: #4)
        - Document each preset with description
        - Note key parameter settings for each
        - Explain synthesis technique used
        - Provide usage suggestions
        - Add to README or user guide

      - Test all factory presets (AC: #1-4)
        - Load each preset individually
        - Play MIDI notes (C2, C4, C6) for each
        - Verify preset sounds as intended
        - Check preset names display correctly
        - Test on both desktop and hardware

      - Refine presets based on testing (AC: #1)
        - Adjust parameter values for best sound
        - Ensure presets are musically useful
        - Verify diverse timbre coverage
        - Test presets with different MIDI velocities (if supported)
        - Finalize preset library
    </tasks>
  </story>

  <acceptanceCriteria>
    1. Minimum 8 factory presets covering diverse timbres:
       - Bell Strike (metallic resonance)
       - Bowed String (smooth sustain)
       - Blown Pipe (airy, breathy)
       - Wooden Mallet (percussive, warm)
       - Glass Chime (bright, crystalline)
       - Bass Drum (deep, resonant)
       - Ambient Pad (evolving texture)
       - Plucked String (attack + decay)
    2. Each preset named descriptively
    3. Presets ship with plugin
    4. Brief preset descriptions documented
  </acceptanceCriteria>

  <artifacts>
    <docs>
      <doc>
        <path>docs/PRD.md</path>
        <title>Product Requirements Document</title>
        <section>FR013: Factory Presets</section>
        <snippet>Include 5-10 factory presets demonstrating different timbres and synthesis modes. Presets should showcase Elements' exciter types (bow, blow, strike), resonator geometries, and space processing.</snippet>
      </doc>
      <doc>
        <path>docs/PRD.md</path>
        <title>Product Requirements Document</title>
        <section>User Journey 1: First-Time User</section>
        <snippet>Sarah tries factory preset "Bell Strike", sends MIDI note from keyboard, hears resonant bell tone, realizes this is the Elements sound she's heard in demos. This demonstrates the critical role of factory presets in user onboarding.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Preset System: Factory Presets</section>
        <snippet>Decision: 8 curated presets demonstrating timbres (Bell, String, Pipe, etc.). Option A: Preinstalled user presets on SD card (simple). Option B: Embedded factory presets in code (elegant, requires NT API support). Check NT API for factory preset support.</snippet>
      </doc>
      <doc>
        <path>docs/epics.md</path>
        <title>Epic Breakdown</title>
        <section>Story 2.4: Factory Preset Library</section>
        <snippet>As a user, I want a curated set of factory presets demonstrating different sounds, so that I can quickly explore nt_elements' capabilities and learn the synthesis. Minimum 8 factory presets covering diverse timbres.</snippet>
      </doc>
      <doc>
        <path>docs/stories/2-3-preset-save-load-integration.md</path>
        <title>Story 2.3: Preset Save/Load Integration</title>
        <section>Parameter-Based State Management</section>
        <snippet>All user-configurable state should be stored in parameters. This ensures automatic preset save/load. Sample rate mode, parameter values all in v[] array (auto-saved). Elements Patch derived from parameters (regenerated on load).</snippet>
      </doc>
      <doc>
        <path>docs/stories/2-3-preset-save-load-integration.md</path>
        <title>Story 2.3: Preset Save/Load Integration</title>
        <section>disting NT Preset System</section>
        <snippet>The disting NT framework automatically serializes the parameter array (_NT_algorithm::v[]) when saving presets. No manual serialization code required for basic parameters. All parameters in v[] array automatically saved.</snippet>
      </doc>
      <doc>
        <path>docs/stories/2-4-factory-preset-library.md</path>
        <title>Story 2.4: Factory Preset Library</title>
        <section>Factory Preset Design Goals</section>
        <snippet>Diversity: Cover wide range of synthesis capabilities (different exciter types, resonator geometries, brightness levels, damping times). Educational Value: Each preset demonstrates specific synthesis technique. Musical Utility: Presets should be immediately useful in music production.</snippet>
      </doc>
      <doc>
        <path>docs/stories/2-4-factory-preset-library.md</path>
        <title>Story 2.4: Factory Preset Library</title>
        <section>Preset Parameter Templates</section>
        <snippet>Detailed parameter specifications for Bell Strike and Bowed String presets, including Page 1 (Exciter), Page 2 (Resonator), Page 3 (Space), Page 4 (Performance) with specific percentage values for all parameters.</snippet>
      </doc>
      <doc>
        <path>docs/stories/2-4-factory-preset-library.md</path>
        <title>Story 2.4: Factory Preset Library</title>
        <section>NT Factory Preset Integration</section>
        <snippet>Option A: Preinstalled User Presets (save as regular presets on SD card, simple). Option B: Embedded Factory Presets (stored in plugin code as const data, requires NT API support). Recommendation: Check NT API for factory preset support.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Parameters: Parameter Count</section>
        <snippet>20 user parameters + 3 system parameters. 4 pages × 5 controls (pots + encoders). Parameters organized as: Page 1 (Exciter), Page 2 (Resonator), Page 3 (Space), Page 4 (Performance).</snippet>
      </doc>
    </docs>
    <code>
      <artifact>
        <path>docs/stories/2-4-factory-preset-library.md</path>
        <kind>documentation</kind>
        <symbol>Preset Data Structure</symbol>
        <lines>226-276</lines>
        <reason>Code examples for FactoryPreset struct, factory_presets array, and load_factory_preset() function showing implementation pattern</reason>
      </artifact>
      <artifact>
        <path>docs/architecture.md</path>
        <kind>architecture</kind>
        <symbol>NT Parameter Array</symbol>
        <lines>502-539</lines>
        <reason>Defines parameter indices enum (kParamBowLevel, kParamGeometry, etc.) that factory presets will reference</reason>
      </artifact>
      <artifact>
        <path>docs/architecture.md</path>
        <kind>architecture</kind>
        <symbol>Elements Patch Structure</symbol>
        <lines>472-499</lines>
        <reason>Shows Elements DSP Patch structure that factory presets will populate (exciter_bow_level, resonator_geometry, reverb_amount, etc.)</reason>
      </artifact>
    </code>
    <dependencies>
      <cpp>
        <framework>Elements DSP (Mutable Instruments)</framework>
        <version>Git submodule (fixed commit)</version>
        <usage>Modal synthesis engine that factory presets will configure</usage>
      </cpp>
      <cpp>
        <framework>distingNT API v9</framework>
        <version>Git submodule</version>
        <usage>NT plugin framework for preset system integration</usage>
      </cpp>
      <build>
        <tool>ARM GCC Toolchain</tool>
        <version>arm-none-eabi-gcc (latest)</version>
        <usage>Cross-compiler for ARM Cortex-M7 hardware target</usage>
      </build>
      <testing>
        <tool>VCV Rack + nt_emu</tool>
        <version>Latest stable</version>
        <usage>Desktop testing environment for preset validation</usage>
      </testing>
    </dependencies>
  </artifacts>

  <constraints>
    - Factory presets must ship with plugin (embedded or preinstalled)
    - Each preset must demonstrate unique timbre and synthesis technique
    - Presets must be musically useful across MIDI note range
    - Parameter values must be within valid ranges (0-100% or equivalent)
    - Preset names must be descriptive and follow naming conventions
    - All presets must work correctly at both 32kHz and 48kHz sample rates
    - Presets should showcase Elements' three exciter types (bow, blow, strike)
    - Coverage of different resonator geometries (string, tube, mixed)
    - Presets must be compatible with NT preset system architecture
    - No code changes to Elements DSP source (read-only submodule)
    - All state stored in NT parameter array (v[]) for automatic serialization
    - Follow architecture decision: Check NT API for factory preset support first
    - If NT API lacks factory preset support, use preinstalled user preset approach
  </constraints>

  <interfaces>
    <interface>
      <name>NT Parameter Array</name>
      <kind>Data structure</kind>
      <signature>float v[kNumParameters]</signature>
      <path>docs/architecture.md#NT Parameter Array</path>
      <description>Parameter storage array that factory presets will populate. Includes 20 user parameters organized across 4 pages: Exciter (kParamBowLevel, kParamBlowLevel, kParamStrikeLevel, etc.), Resonator (kParamGeometry, kParamBrightness, kParamDamping, etc.), Space (kParamReverbAmount, kParamReverbSize, kParamReverbDamping), Performance (kParamCoarseTune, kParamFineTune, kParamOutputLevel, kParamFMAmount).</description>
    </interface>
    <interface>
      <name>Elements Patch Structure</name>
      <kind>Data structure</kind>
      <signature>struct Patch { float note; bool gate; float exciter_bow_level; float resonator_geometry; float reverb_amount; ... }</signature>
      <path>external/mutable-instruments/elements/dsp/patch.h</path>
      <description>Elements DSP configuration structure. Factory presets define parameter values that adapter layer converts to this Patch format. All Patch fields range 0.0-1.0, derived from NT parameters (0-100%).</description>
    </interface>
    <interface>
      <name>parameterChanged callback</name>
      <kind>Function callback</kind>
      <signature>void parameterChanged(_NT_algorithm* self, int p)</signature>
      <path>docs/architecture.md#Parameter Update Pattern</path>
      <description>NT callback invoked when parameter changes. Factory preset loading will trigger this for each parameter to update Elements Patch. Converts NT parameter (0-100%) to Elements Patch field (0.0-1.0).</description>
    </interface>
    <interface>
      <name>preset_loaded callback</name>
      <kind>Function callback (optional)</kind>
      <signature>void preset_loaded(_NT_algorithm* self, const char* preset_name)</signature>
      <path>docs/stories/2-3-preset-save-load-integration.md#Preset Load Handling</path>
      <description>Optional NT callback invoked when preset loads. Factory preset system may use this to reinitialize Elements DSP, apply all parameters atomically, update display. Ensures glitch-free preset loading.</description>
    </interface>
    <interface>
      <name>Factory Preset Structure</name>
      <kind>Data structure (proposed)</kind>
      <signature>struct FactoryPreset { const char* name; float parameters[kNumParameters]; }</signature>
      <path>docs/stories/2-4-factory-preset-library.md#Preset Data Structure</path>
      <description>Proposed structure for factory presets. Array of FactoryPreset instances stored as const data. Each preset contains name and complete parameter array matching NT parameter indices.</description>
    </interface>
  </interfaces>

  <tests>
    <standards>
      Testing for nt_elements follows embedded hardware testing patterns with desktop-first development. Primary testing environments: (1) Desktop nt_emu (VCV Rack module) for rapid iteration with debugging, (2) Hardware disting NT for performance and final validation. For factory presets, testing focuses on: Per-preset validation (load, play MIDI C2/C4/C6, verify expected timbre), Comparative testing (load all presets sequentially, verify distinct character, ensure no duplicate timbres), User testing (beta testers try factory presets, gather feedback on sound quality and utility). Test framework: Manual functional testing with documented test procedures. No automated unit tests for presets (subjective sound quality). Performance testing: CPU usage measured via NT built-in display. Sound quality validation: A/B comparison with hardware Elements (Story 2.6). Test documentation: Parameter specifications and expected characteristics for each preset.
    </standards>
    <locations>
      - Desktop testing: VCV Rack + nt_emu module
      - Hardware testing: disting NT Eurorack module
      - Documentation: docs/stories/2-4-factory-preset-library.md (Dev Notes sections)
      - Test presets: Created during Story 2.3 (Preset Save/Load Integration)
    </locations>
    <ideas>
      <test id="2.4-MANUAL-001" ac="1,2" priority="P0">
        <description>Verify all 8 factory presets load and produce expected timbres</description>
        <approach>For each preset: Load via NT interface, play MIDI notes C2/C4/C6, verify timbre matches description (Bell Strike sounds metallic, Bowed String sounds smooth sustained, etc.), check preset name displays correctly</approach>
      </test>
      <test id="2.4-MANUAL-002" ac="1" priority="P0">
        <description>Verify preset diversity coverage</description>
        <approach>Load all 8 presets in sequence, verify distinct character for each (no duplicate timbres), confirm coverage of exciter types (bow: Bowed String/Ambient Pad, blow: Blown Pipe/Ambient Pad, strike: Bell/Mallet/Chime/Drum/Plucked), confirm coverage of resonator geometries (string: Bowed/Plucked, tube: Blown, metallic: Bell/Chime, mixed: varied)</approach>
      </test>
      <test id="2.4-MANUAL-003" ac="3" priority="P0">
        <description>Verify factory presets ship with plugin</description>
        <approach>Fresh plugin installation on hardware NT (or nt_emu), load plugin for first time, verify factory presets immediately available in NT preset browser without manual installation, test preset loading works on first use</approach>
      </test>
      <test id="2.4-MANUAL-004" ac="1,4" priority="P1">
        <description>Verify presets work across MIDI range and sample rate modes</description>
        <approach>For each preset: Test in 32kHz mode (play MIDI C1-C6), test in 48kHz mode (play MIDI C1-C6), verify preset sounds musical across range, verify no pitch tracking issues or artifacts, confirm preset descriptions accurately reflect sound characteristics</approach>
      </test>
      <test id="2.4-MANUAL-005" ac="1" priority="P1">
        <description>Validate preset parameter specifications</description>
        <approach>For each preset: Load preset, inspect parameter values (if accessible via NT display/interface), compare to documented specifications in Dev Notes, verify key parameters match design (e.g., Bell Strike: Strike Level 90%, Geometry 80%, Brightness 75%)</approach>
      </test>
      <test id="2.4-MANUAL-006" ac="2,4" priority="P2">
        <description>Beta user testing for musical utility</description>
        <approach>Share plugin with 3-5 beta testers, ask them to try all factory presets, gather feedback on: sound quality, preset naming clarity, musical usefulness, educational value (do presets help understand synthesis?), suggested improvements</approach>
      </test>
    </ideas>
  </tests>
</story-context>
