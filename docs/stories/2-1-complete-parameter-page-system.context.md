<story-context id="bmad/bmm/workflows/4-implementation/story-context/template" v="1.0">
  <metadata>
    <epicId>2</epicId>
    <storyId>1</storyId>
    <title>Complete Parameter Page System</title>
    <status>drafted</status>
    <generatedAt>2025-10-26</generatedAt>
    <generator>BMAD Story Context Workflow</generator>
    <sourceStoryPath>docs/stories/2-1-complete-parameter-page-system.md</sourceStoryPath>
  </metadata>

  <story>
    <asA>user</asA>
    <iWant>all 4 parameter pages (Exciter, Resonator, Space, Performance) fully implemented</iWant>
    <soThat>I can access all Elements synthesis parameters via the NT interface</soThat>
    <tasks>
      - Expand parameter enumeration (AC: #1-4)
        - Add all Page 1 (Exciter) parameters to enum
        - Add all Page 2 (Resonator) parameters to enum
        - Add all Page 3 (Space) parameters to enum
        - Add all Page 4 (Performance) parameters to enum
        - Update kNumParameters count
        - Document complete parameter list

      - Create parameter page structure (AC: #1-4, #7)
        - Create src/parameter_pages.h
        - Define 4-page structure with parameter mappings
        - Map each pot/encoder to parameter per page
        - Define page names and display labels
        - Document parameter organization

      - Implement Page 1 (Exciter) parameter mappings (AC: #1)
        - Pot 1: Bow Level (bow/blow balance)
        - Pot 2: Blow Level
        - Pot 3: Strike Level
        - Encoder 1: Bow Timbre
        - Encoder 2: Blow Timbre
        - Map to Elements Patch exciter fields
        - Test exciter parameter changes

      - Implement Page 2 (Resonator) parameter mappings (AC: #2)
        - Pot 1: Geometry (material type)
        - Pot 2: Brightness (filter cutoff)
        - Pot 3: Damping (decay rate)
        - Encoder 1: Position (strike position)
        - Encoder 2: Inharmonicity
        - Map to Elements Patch resonator fields
        - Test resonator parameter changes

      - Implement Page 3 (Space) parameter mappings (AC: #3)
        - Pot 1: Reverb Amount
        - Pot 2: Reverb Size
        - Pot 3: Reverb Damping
        - Encoder 1: (future use or bypass toggle)
        - Encoder 2: (future use or reverb type)
        - Map to Elements Patch reverb fields
        - Test reverb parameter changes

      - Implement Page 4 (Performance) parameter mappings (AC: #4)
        - Pot 1: Coarse Tune (octave/semitone)
        - Pot 2: Fine Tune (cents)
        - Pot 3: Output Level
        - Encoder 1: FM Amount
        - Encoder 2: Exciter Contour (envelope)
        - Implement tuning offset calculation
        - Implement output level scaling
        - Test performance parameter changes

      - Implement page navigation (AC: #5)
        - Track current_page state variable
        - Implement button handler for page switching
        - Cycle through pages: 1 → 2 → 3 → 4 → 1
        - Update OLED display on page change
        - Verify smooth page transitions

      - Implement pot/encoder routing per page (AC: #7)
        - Route pot/encoder inputs to correct parameter based on current page
        - Handle pot value changes
        - Handle encoder value changes
        - Handle encoder button presses (page navigation)
        - Verify all controls functional on all pages

      - Update parameterChanged() for all parameters (AC: #1-4)
        - Expand switch statement for all new parameters
        - Implement conversion for each parameter (NT → Elements)
        - Add special handling for tuning (offset calculation)
        - Add special handling for output level (scaling)
        - Verify immediate parameter response

      - Test each page independently (AC: #1-4, #7)
        - Test Page 1: Change all 5 controls, verify exciter behavior
        - Test Page 2: Change all 5 controls, verify resonator behavior
        - Test Page 3: Change all 3 controls, verify reverb behavior
        - Test Page 4: Change all 5 controls, verify performance changes
        - Document parameter ranges and effects

      - Test page navigation (AC: #5, #6)
        - Navigate through all pages in sequence
        - Verify current page displayed correctly on OLED
        - Verify parameter values update on OLED
        - Test rapid page switching (no glitches)
        - Verify parameter states persist across page switches

      - Desktop validation in nt_emu (AC: #1-7)
        - Load plugin in nt_emu
        - Test all parameters on all pages
        - Verify OLED display updates (if nt_emu supports display emulation)
        - Test page navigation
        - Document any issues discovered
    </tasks>
  </story>

  <acceptanceCriteria>
    1. Page 1 (Exciter): All exciter parameters mapped (bow/blow balance, strike, contour, timbres)
    2. Page 2 (Resonator): All resonator parameters mapped (geometry, brightness, damping, position, inharmonicity)
    3. Page 3 (Space): Reverb parameters mapped (amount, size, damping)
    4. Page 4 (Performance): Global parameters mapped (tuning, FM, output level)
    5. Button navigation cycles through pages smoothly
    6. OLED display shows current page and parameter values clearly
    7. All 3 pots + 2 encoders functional on each page
  </acceptanceCriteria>

  <artifacts>
    <docs>
      <doc>
        <path>docs/PRD.md</path>
        <title>Product Requirements Document</title>
        <section>FR006: 4 Parameter Pages</section>
        <snippet>Create 4 parameter pages organized as: Exciter, Resonator, Space (reverb), Performance. Map all parameters to disting NT's 3 pots + 2 encoders + 4 buttons interface.</snippet>
      </doc>
      <doc>
        <path>docs/PRD.md</path>
        <title>Product Requirements Document</title>
        <section>FR007: Parameter Mapping</section>
        <snippet>Map all parameters to disting NT's 3 pots + 2 encoders + 4 buttons interface. Each page provides focused control over synthesis section without overwhelming users.</snippet>
      </doc>
      <doc>
        <path>docs/PRD.md</path>
        <title>Product Requirements Document</title>
        <section>UI Design - Page 1 Exciter</section>
        <snippet>Pot 1: Bow/Blow Balance, Pot 2: Strike Level, Pot 3: Contour (envelope), Encoder 1: Bow Timbre, Encoder 2: Blow Timbre. Display: "EXCITER | Bow: 50% Strike: 30%"</snippet>
      </doc>
      <doc>
        <path>docs/PRD.md</path>
        <title>Product Requirements Document</title>
        <section>UI Design - Page 2 Resonator</section>
        <snippet>Pot 1: Geometry (material type), Pot 2: Brightness (filter cutoff), Pot 3: Damping (decay), Encoder 1: Position (strike position), Encoder 2: Inharmonicity. Display: "RESONATOR | Geom: String Bright: 45%"</snippet>
      </doc>
      <doc>
        <path>docs/PRD.md</path>
        <title>Product Requirements Document</title>
        <section>UI Design - Page 3 Space</section>
        <snippet>Pot 1: Reverb Amount, Pot 2: Reverb Size, Pot 3: Reverb Damping. Display: "SPACE | Amt: 35% Size: Large"</snippet>
      </doc>
      <doc>
        <path>docs/PRD.md</path>
        <title>Product Requirements Document</title>
        <section>UI Design - Page 4 Performance</section>
        <snippet>Pot 1: Coarse Tune, Pot 2: Fine Tune, Pot 3: FM Amount, Encoder 1: Output Level. Display: "PERF | Tune: +2.0 FM: 20%"</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Parameters - Parameter Count</section>
        <snippet>20 user parameters + 3 system (total ~22 parameters). 4 pages × 5 controls (pots + encoders). Affects Epic 2.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Parameters - Page Navigation</section>
        <snippet>Encoder press: prev/next page. Encoder 1 = prev, Encoder 2 = next. Affects Epic 2.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Data Architecture - NT Parameter Array</section>
        <snippet>Parameter enumeration includes: kParamInputBus, kParamOutputBus, kParamOutputMode, kParamSampleRateMode, Page 1 (Exciter): kParamBowLevel, kParamBlowLevel, kParamStrikeLevel, kParamBowTimbre, kParamBlowTimbre, Page 2 (Resonator): kParamGeometry, kParamBrightness, kParamDamping, kParamResonatorModel, kParamInharmonicity, Page 3 (Space): kParamReverbAmount, kParamReverbSize, kParamReverbDamping, kParamReverbType, Page 4 (Performance): kParamCoarseTune, kParamFineTune, kParamOutputLevel, kParamFMAmount, kParamExciterContour, kNumParameters</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Data Architecture - Elements Patch Structure</section>
        <snippet>Elements Patch contains: note (frequency Hz), gate (on/off), exciter_bow_level, exciter_bow_timbre, exciter_blow_level, exciter_blow_timbre, exciter_strike_level, exciter_strike_timbre, resonator_geometry, resonator_brightness, resonator_damping, resonator_position, resonator_modulation, reverb_amount, reverb_time, reverb_diffusion</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>ADR-006: Four-Page Parameter Organization</section>
        <snippet>Decision: 4 pages (Exciter, Resonator, Space, Performance). Rationale: Matches hardware Elements' three-section layout (familiar mental model), logical grouping by synthesis function, 5 controls per page fits NT hardware, encoder press navigation intuitive. Affects Epic 2.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Project Structure</section>
        <snippet>src/ contains: nt_elements.cpp (main plugin, factory, callbacks, adapter), parameter_adapter.h (NT parameters → Elements Patch mapping), parameter_pages.h (4-page UI definition), oled_display.h (OLED rendering functions)</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Epic to Architecture Mapping - Story 2.1</section>
        <snippet>Story 2.1 implements 4-page structure, 20 parameters, encoder navigation using src/parameter_pages.h and page navigation logic.</snippet>
      </doc>
      <doc>
        <path>docs/epics.md</path>
        <title>Epic Breakdown</title>
        <section>Story 2.1: Complete Parameter Page System</section>
        <snippet>AC: 1) Page 1 (Exciter): All exciter parameters mapped, 2) Page 2 (Resonator): All resonator parameters mapped, 3) Page 3 (Space): Reverb parameters mapped, 4) Page 4 (Performance): Global parameters mapped, 5) Button navigation cycles smoothly, 6) OLED shows current page/values clearly, 7) All 3 pots + 2 encoders functional on each page</snippet>
      </doc>
      <doc>
        <path>docs/stories/2-1-complete-parameter-page-system.md</path>
        <title>Story 2.1 Details</title>
        <section>Dev Notes - Parameter Page Organization</section>
        <snippet>Page 1 - Exciter controls bow/blow/strike excitation. Page 2 - Resonator defines material and resonance character. Page 3 - Space adds reverb. Page 4 - Performance has global tuning/FM/output controls. Complete parameter mappings documented with rationale.</snippet>
      </doc>
      <doc>
        <path>docs/stories/2-1-complete-parameter-page-system.md</path>
        <title>Story 2.1 Details</title>
        <section>Dev Notes - Page Navigation Implementation</section>
        <snippet>State management via current_page (0-3). Button handler cycles pages. Parameter routing uses page mapping tables (PAGE1_POT_MAPPING, etc). Example code provided.</snippet>
      </doc>
      <doc>
        <path>docs/stories/2-1-complete-parameter-page-system.md</path>
        <title>Story 2.1 Details</title>
        <section>Dev Notes - Tuning Implementation</section>
        <snippet>Coarse tune: semitone offset (-12 to +12). Fine tune: cent offset (-50 to +50). Total tuning offset applied to MIDI frequency via powf(2.0f, total_semitones / 12.0f).</snippet>
      </doc>
      <doc>
        <path>docs/stories/2-1-complete-parameter-page-system.md</path>
        <title>Story 2.1 Details</title>
        <section>Dev Notes - Output Level Scaling</section>
        <snippet>Output level (0-100%) → 0.0-1.0 scaling. Applied in step() callback after Elements processing. Prevents clipping, allows balancing with other NT algorithms.</snippet>
      </doc>
    </docs>

    <code>
      <artifact>
        <path>src/parameter_pages.h</path>
        <kind>header</kind>
        <symbol>PageIndex, PAGE1_POT_MAPPING</symbol>
        <lines>N/A (to be created)</lines>
        <reason>Core deliverable of this story. Defines 4-page structure, page indices enum, and parameter mapping tables for each page (pots and encoders). Central to AC #1-4, #7.</reason>
      </artifact>
      <artifact>
        <path>src/parameter_adapter.h</path>
        <kind>header</kind>
        <symbol>ParameterIndices</symbol>
        <lines>N/A (to be expanded)</lines>
        <reason>Must be expanded to include all 20 user parameters across 4 pages. Complete parameter enumeration is prerequisite for page system implementation.</reason>
      </artifact>
      <artifact>
        <path>src/nt_elements.cpp</path>
        <kind>implementation</kind>
        <symbol>parameterChanged</symbol>
        <lines>N/A (to be expanded)</lines>
        <reason>Must expand switch statement to handle all new parameters. Implement tuning offset calculation and output level scaling for Performance page.</reason>
      </artifact>
      <artifact>
        <path>src/nt_elements.cpp</path>
        <kind>implementation</kind>
        <symbol>potChanged, encoderChanged</symbol>
        <lines>N/A (to be implemented)</lines>
        <reason>New callbacks for pot/encoder input handling. Must route controls to correct parameter based on current_page state. Critical for AC #7.</reason>
      </artifact>
      <artifact>
        <path>src/nt_elements.cpp</path>
        <kind>implementation</kind>
        <symbol>buttonPressed</symbol>
        <lines>N/A (to be implemented)</lines>
        <reason>Page navigation handler. Cycles through pages on button press, updates OLED display. Implements AC #5.</reason>
      </artifact>
      <artifact>
        <path>src/nt_elements.cpp</path>
        <kind>implementation</kind>
        <symbol>_nt_elementsAlgorithm</symbol>
        <lines>N/A (to be modified)</lines>
        <reason>Main algorithm structure must add current_page state variable for tracking which page is active.</reason>
      </artifact>
      <artifact>
        <path>external/mutable-instruments/elements/dsp/patch.h</path>
        <kind>header</kind>
        <symbol>Patch</symbol>
        <lines>N/A (read-only submodule)</lines>
        <reason>Target structure for all parameter conversions. Contains all exciter, resonator, and reverb fields that parameters map to.</reason>
      </artifact>
      <artifact>
        <path>distingNT_API/include/distingNT.h</path>
        <kind>header</kind>
        <symbol>_NT_algorithm</symbol>
        <lines>N/A (submodule)</lines>
        <reason>NT API base structure defining callback signatures (potChanged, encoderChanged, buttonPressed) and parameter array v[].</reason>
      </artifact>
    </code>

    <dependencies>
      <cpp>
        <item>arm-none-eabi-gcc</item>
        <description>ARM cross-compiler toolchain for Cortex-M7</description>
      </cpp>
      <cpp>
        <item>C++ standard library (math.h)</item>
        <description>Required for powf() function in tuning offset calculation</description>
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
    - Parameter conversion: NT uses 0-100% range (float), Elements Patch uses 0.0-1.0 normalized range (float)
    - Follow C++ naming conventions: camelCase for functions, snake_case for variables, k-prefix PascalCase for constants
    - Page enumeration: kPageExciter=0, kPageResonator=1, kPageSpace=2, kPagePerformance=3, kNumPages=4
    - Total parameters: ~22 (3 system + 4 sample rate/page nav + 5×4 pages = 23 approx)
    - Memory impact: 22 parameters × 4 bytes = 88 bytes (negligible)
    - Defensive programming: clamp parameter values, check bounds, no exceptions allowed (-fno-exceptions)
    - Story depends on Story 1.7 (Parameter Adapter Layer) completion
    - Desktop-first development: test in nt_emu before hardware deployment
    - OLED display integration is primarily Story 2.2, but this story must update display on page change
    - Tuning implementation: Coarse (-12 to +12 semitones), Fine (-50 to +50 cents), combined via frequency multiplication
    - Output level scaling: Applied in step() callback after Elements processing, range 0.0-1.0
    - Page navigation: Button press cycles pages, encoder buttons can also be used for prev/next
    - Parameter states persist across page switches (no reset when changing pages)
  </constraints>

  <interfaces>
    <interface>
      <name>potChanged</name>
      <kind>callback function</kind>
      <signature>void potChanged(_NT_algorithm* self, int pot_index, float value)</signature>
      <path>distingNT_API/include/distingNT.h</path>
      <description>NT framework callback invoked when a pot changes. pot_index is 0-2 for the 3 pots. Adapter must route to correct parameter based on current_page.</description>
    </interface>
    <interface>
      <name>encoderChanged</name>
      <kind>callback function</kind>
      <signature>void encoderChanged(_NT_algorithm* self, int encoder_index, int delta)</signature>
      <path>distingNT_API/include/distingNT.h</path>
      <description>NT framework callback invoked when encoder rotates. encoder_index is 0-1 for the 2 encoders. delta is rotation amount. Route to parameter based on current_page.</description>
    </interface>
    <interface>
      <name>buttonPressed</name>
      <kind>callback function</kind>
      <signature>void buttonPressed(_NT_algorithm* self, int button_index)</signature>
      <path>distingNT_API/include/distingNT.h</path>
      <description>NT framework callback invoked when button pressed. Implement page navigation: cycle through 0→1→2→3→0. Update OLED display on page change.</description>
    </interface>
    <interface>
      <name>parameterChanged</name>
      <kind>callback function</kind>
      <signature>void parameterChanged(_NT_algorithm* self, int p)</signature>
      <path>distingNT_API/include/distingNT.h</path>
      <description>NT framework callback invoked when a parameter changes. Must expand to handle all 20 user parameters, including special logic for tuning and output level.</description>
    </interface>
    <interface>
      <name>Elements::Patch structure</name>
      <kind>data structure</kind>
      <signature>struct Patch { float note; bool gate; float exciter_bow_level; float exciter_bow_timbre; float exciter_blow_level; float exciter_blow_timbre; float exciter_strike_level; float exciter_strike_timbre; float resonator_geometry; float resonator_brightness; float resonator_damping; float resonator_position; float resonator_modulation; float reverb_amount; float reverb_time; float reverb_diffusion; }</signature>
      <path>external/mutable-instruments/elements/dsp/patch.h</path>
      <description>Elements DSP parameter container. All NT parameters ultimately convert to these fields. Range: 0.0-1.0 for most parameters.</description>
    </interface>
    <interface>
      <name>Page Mapping Tables</name>
      <kind>data structure</kind>
      <signature>const int PAGE1_POT_MAPPING[3], PAGE1_ENCODER_MAPPING[2], etc.</signature>
      <path>src/parameter_pages.h (to be created)</path>
      <description>Constant arrays mapping pot/encoder indices to parameter indices for each page. Used in potChanged/encoderChanged to route controls based on current_page.</description>
    </interface>
  </interfaces>

  <tests>
    <standards>
      This is an embedded C++ audio plugin project with no traditional unit testing framework. Testing follows a desktop-first manual validation approach:
      1) Desktop testing in nt_emu (VCV Rack module) for rapid iteration with visual/audio feedback
      2) Hardware validation on actual disting NT at key milestones
      3) Manual audio quality verification (listening tests, parameter sweep tests)
      4) Performance profiling using NT built-in CPU display

      Validation criteria: all parameters audibly affect synthesis, smooth page navigation without glitches, parameter states persist across pages, OLED display updates correctly, no crashes during stress tests (rapid page switching, rapid parameter changes), stable operation during extended runs.
    </standards>

    <locations>
      No automated test files - manual testing approach:
      - Desktop testing: VCV Rack with nt_emu module (load plugins/nt_elements.dylib)
      - Hardware testing: disting NT hardware (deploy plugins/nt_elements.o to SD card)
      - Validation documented in story completion notes within story file
    </locations>

    <ideas>
      - AC #1 Test (Page 1 - Exciter): Navigate to Page 1. Adjust Pot 1 (Bow Level) 0→100%, verify patch.exciter_bow_level updates 0.0→1.0 and sound changes. Repeat for all 5 controls on page. Test bow vs blow vs strike excitation balance.

      - AC #2 Test (Page 2 - Resonator): Navigate to Page 2. Adjust Pot 1 (Geometry) through full range, verify material type changes (string→tube→etc). Test brightness (harmonic content), damping (decay time), position (timbre shift), inharmonicity (metallic character).

      - AC #3 Test (Page 3 - Space): Navigate to Page 3. Adjust Pot 1 (Reverb Amount) 0→100%, verify wet/dry mix changes. Test reverb size (room size/time) and damping (brightness). Verify reverb bypass at Amount=0.

      - AC #4 Test (Page 4 - Performance): Navigate to Page 4. Test Coarse Tune: adjust Pot 1, verify pitch shifts by semitones. Test Fine Tune: adjust Pot 2, verify pitch shifts by cents. Test Output Level: verify volume scaling without affecting timbre. Test FM and Exciter Contour controls.

      - AC #5 Test (Page Navigation): Press button repeatedly, verify pages cycle 1→2→3→4→1 correctly. Test encoder buttons for prev/next navigation. Verify current_page state updates correctly. Test rapid page switching (no crashes).

      - AC #6 Test (OLED Display): On each page change, verify OLED shows correct page name ("EXCITER", "RESONATOR", "SPACE", "PERF"). Verify parameter values display. Note: full OLED implementation in Story 2.2, but basic page name display needed here.

      - AC #7 Test (Controls on Each Page): On Page 1, verify all 3 pots + 2 encoders control correct exciter parameters. Repeat for Pages 2, 3, 4. Verify no control cross-talk between pages.

      - Parameter Persistence Test: Set all parameters on Page 1 to specific values. Navigate to Page 2, then back to Page 1. Verify Page 1 parameters unchanged (state persists).

      - Tuning Calculation Test: Set Coarse Tune to +12 semitones (100%), verify pitch exactly 1 octave up. Set to -12 semitones (0%), verify pitch 1 octave down. Test Fine Tune ±50 cents for microtuning.

      - Output Level Scaling Test: Set Output Level to 50%, measure audio output, verify amplitude is half of 100% setting. Verify scaling is linear and affects output gain only (not timbre).

      - Stress Test: Rapidly switch pages while changing parameters. Verify no crashes, glitches, or stuck notes. Test for 5 minutes continuous operation.

      - Mapping Table Test: Code inspection - verify PAGE1_POT_MAPPING[0]=kParamBowLevel, PAGE1_POT_MAPPING[1]=kParamBlowLevel, etc. Verify all 4 pages have correct mappings for all controls.

      - Default Values Test: Load plugin fresh, verify reasonable defaults for all 20 parameters. Navigate through all pages, verify each sounds good with defaults.
    </ideas>
  </tests>
</story-context>
