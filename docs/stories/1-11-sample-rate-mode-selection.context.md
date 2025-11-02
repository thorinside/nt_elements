<story-context id="bmad/bmm/workflows/4-implementation/story-context/template" v="1.0">
  <metadata>
    <epicId>1</epicId>
    <storyId>11</storyId>
    <title>Sample Rate Mode Selection</title>
    <status>drafted</status>
    <generatedAt>2025-10-26</generatedAt>
    <generator>BMAD Story Context Workflow</generator>
    <sourceStoryPath>docs/stories/1-11-sample-rate-mode-selection.md</sourceStoryPath>
  </metadata>

  <story>
    <asA>user</asA>
    <iWant>to choose between 48kHz (converted) and 32kHz (authentic) operation modes</iWant>
    <soThat>I can optimize for either performance or compatibility with other NT algorithms</soThat>
    <tasks>
      - Add sample rate mode parameter (AC: #1)
        - Define kParamSampleRateMode enum value
        - Add parameter to NT parameter array
        - Set default mode (48kHz recommended for NT integration)
        - Define mode values (0 = 32kHz, 1 = 48kHz)
        - Add parameter metadata (name, range, display)

      - Implement mode switching logic (AC: #2, #3)
        - Add mode state variable to algorithm structure
        - Implement parameterChanged() handler for mode parameter
        - Call elements_part->Init(32000) for 32kHz mode
        - Call elements_part->Init(48000) for 48kHz mode
        - Verify Elements reinitializes correctly on mode change

      - Handle sample rate conversion in 32kHz mode (AC: #2)
        - Implement sample rate conversion for input buffer (48kHz → 32kHz)
        - Implement sample rate conversion for output buffer (32kHz → 48kHz)
        - Use simple decimation/interpolation or skip for now
        - Alternative: Disable 32kHz mode in Story 1.11, enable in Epic 2
        - Document conversion approach or mode limitation

      - Implement glitch-free mode switching (AC: #4)
        - Mute output briefly during mode switch
        - Reinitialize Elements DSP
        - Clear audio buffers
        - Fade output back in smoothly
        - Verify no clicks or pops during switch

      - Test mode switching in nt_emu (AC: #1, #4)
        - Load plugin in nt_emu
        - Start synthesis in 48kHz mode
        - Switch to 32kHz mode while note playing
        - Listen for glitches or artifacts
        - Switch back to 48kHz mode
        - Verify smooth transitions

      - Measure CPU usage for both modes (AC: #5)
        - Deploy to hardware (disting NT)
        - Measure CPU % in 48kHz mode (idle and synthesis)
        - Measure CPU % in 32kHz mode (idle and synthesis)
        - Calculate CPU savings: (48kHz - 32kHz) / 48kHz
        - Verify ~15-20% savings in 32kHz mode

      - Validate audio quality in both modes (AC: #2, #3)
        - Record synthesis in 48kHz mode
        - Record synthesis in 32kHz mode (same patch)
        - Perform spectral analysis comparison
        - Verify 32kHz mode sounds authentic (native Elements)
        - Verify 48kHz mode sounds identical (proper conversion)

      - Create user documentation (AC: #6)
        - Document mode selection parameter
        - Explain 32kHz mode benefits (lower CPU, authentic)
        - Explain 48kHz mode benefits (NT integration, compatibility)
        - Provide guidance on when to use each mode
        - Add to README or user guide

      - Test mode persistence in presets (AC: #1)
        - Save preset in 48kHz mode
        - Switch to 32kHz mode
        - Load preset (should restore 48kHz mode)
        - Verify preset system saves/restores mode correctly
        - Test with multiple presets
    </tasks>
  </story>

  <acceptanceCriteria>
    1. Mode selection parameter added (48kHz/32kHz toggle)
    2. In 32kHz mode, Elements runs at native sample rate (no conversion overhead)
    3. In 48kHz mode, converted coefficients used (from Story 1.5)
    4. Mode switch does not cause audio glitches or pops
    5. CPU usage in 32kHz mode measured and compared to 48kHz (expect ~15-20% savings)
    6. User documentation notes trade-offs: 32kHz = lower CPU, 48kHz = better NT integration
  </acceptanceCriteria>

  <artifacts>
    <docs>
      <doc>
        <path>docs/PRD.md</path>
        <title>Product Requirements Document</title>
        <section>FR014: Sample Rate Processing</section>
        <snippet>Process audio at 48kHz sample rate with proper filter coefficient conversion from Elements' 32kHz. Hardware Elements runs at 32kHz, disting NT runs at 48kHz.</snippet>
      </doc>
      <doc>
        <path>docs/PRD.md</path>
        <title>Product Requirements Document</title>
        <section>NFR001: Performance</section>
        <snippet>CPU usage must remain below 30% under typical operating conditions to allow algorithm chaining. Target 20-30% with all features active. No audio dropouts or glitches during sustained operation.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>ADR-002: Runtime Sample Rate Mode Toggle</section>
        <snippet>Decision: Runtime parameter toggle between 32kHz and 48kHz modes. User can A/B test modes without reloading plugin. Mode switch happens at Init() (not per-sample overhead). Aligns with PRD requirement (Story 1.11).</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Sample Rate - Dual Mode Support</section>
        <snippet>Decision: Dual Mode Support - Runtime toggle: 32kHz/48kHz (Epic 1 Story 1.5, 1.11). Implementation: Runtime parameter with Init(). Elements Init() accepts sample rate parameter for dynamic switching.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Performance Considerations - CPU Budget</section>
        <snippet>Target: < 30% CPU usage @ 48kHz. Expected breakdown: Elements DSP core 15-20%, Reverb 5-10%, Sample rate conversion 2-3% (if 48kHz mode), Adapter overhead < 1%. Sample rate mode: 32kHz mode saves ~15-20% vs 48kHz.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Parameters - Sample Rate Toggle</section>
        <snippet>Decision: Pot 3 press for sample rate mode toggle. Quick access to mode switch. Affects Epic 1 (Story 1.11).</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Parameter Update Pattern</section>
        <snippet>Parameters update via parameterChanged() callback. For kParamSampleRateMode: Handle sample rate mode switch by calling elements_part->Init(48000) for 48kHz or Init(32000) for 32kHz.</snippet>
      </doc>
      <doc>
        <path>docs/epics.md</path>
        <title>Epic Breakdown</title>
        <section>Story 1.11: Sample Rate Mode Selection</section>
        <snippet>AC: 1) Mode selection parameter (48kHz/32kHz), 2) 32kHz mode runs at native rate, 3) 48kHz mode uses converted coefficients, 4) Mode switch without glitches, 5) CPU usage measured (~15-20% savings), 6) User documentation on trade-offs.</snippet>
      </doc>
      <doc>
        <path>docs/epics.md</path>
        <title>Epic Breakdown</title>
        <section>Story 1.5: Sample Rate Conversion Implementation</section>
        <snippet>All Elements filter coefficients scaled from 32kHz to 48kHz. Frequency response measurements match expected values. Pitch tracking accurate across MIDI note range. Story 1.11 builds on this for dual-mode support.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Technology Stack - Hardware Platform</section>
        <snippet>disting NT: ARM Cortex-M7 @ 480MHz (STM32H7). RAM: ~128KB SRAM, ~8MB DRAM, ~64KB DTC, ~64KB ITC. Audio: 48kHz sample rate (primary), 32kHz (compatibility mode). Interface: 3 pots, 2 encoders, 4 buttons, OLED display.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Technology Stack - DSP Source</section>
        <snippet>Mutable Instruments Elements modal synthesis engine. Repository: github.com/pichenettes/eurorack. License: MIT. Native sample rate: 32kHz. Git submodule integration preserves original source.</snippet>
      </doc>
      <doc>
        <path>docs/stories/1-11-sample-rate-mode-selection.md</path>
        <title>Story 1.11 Dev Notes</title>
        <section>Sample Rate Mode Architecture</section>
        <snippet>48kHz Mode (Default): Elements at 48kHz, filter coefficients scaled, best NT compatibility, slightly higher CPU. 32kHz Mode (Performance): Elements at 32kHz native rate, no conversion overhead, 100% authentic sound, lower CPU (~15-20% savings), requires sample rate conversion for NT I/O.</snippet>
      </doc>
      <doc>
        <path>docs/stories/1-11-sample-rate-mode-selection.md</path>
        <title>Story 1.11 Dev Notes</title>
        <section>Elements Reinitialization</section>
        <snippet>Mode switch procedure: 1) Mute output (set gate = false), 2) Reinitialize Elements at new sample rate (Init with 32000 or 48000), 3) Clear buffers to avoid artifacts, 4) Restore gate state (unmute), 5) Update mode state variable.</snippet>
      </doc>
      <doc>
        <path>docs/stories/1-11-sample-rate-mode-selection.md</path>
        <title>Story 1.11 Dev Notes</title>
        <section>CPU Usage Comparison</section>
        <snippet>48kHz Mode: Filter ~18-22%, Reverb ~8-10%, Total ~30-35% CPU. 32kHz Mode: Filter ~12-15%, Reverb ~5-7%, SRC ~1-2%, Total ~20-27% CPU. CPU Savings: ~10% absolute, ~30% relative. Measure on hardware using built-in CPU display.</snippet>
      </doc>
      <doc>
        <path>docs/stories/1-11-sample-rate-mode-selection.md</path>
        <title>Story 1.11 Dev Notes</title>
        <section>Mode Selection Trade-offs</section>
        <snippet>32kHz: Lower CPU, authentic Elements sound, matches hardware sample rate, better standalone. 48kHz: Native NT rate, best integration, no sample rate conversion, better for complex patching. Default to 48kHz for most users. Offer 32kHz as power-user option.</snippet>
      </doc>
      <doc>
        <path>docs/stories/1-11-sample-rate-mode-selection.md</path>
        <title>Story 1.11 Dev Notes</title>
        <section>Sample Rate Conversion Challenge</section>
        <snippet>Options: A) Simple decimation/interpolation (low CPU but potential aliasing), B) Proper sample rate conversion (high quality but higher CPU overhead), C) Disable 32kHz mode for v1.0. Recommended: Option C (48kHz only) or Option A (simple conversion).</snippet>
      </doc>
    </docs>

    <code>
      <artifact>
        <path>src/parameter_adapter.h</path>
        <kind>header</kind>
        <symbol>kParamSampleRateMode</symbol>
        <lines>N/A (to be added)</lines>
        <reason>Parameter enumeration needs new entry for sample rate mode selection. Defines the parameter index for 32kHz/48kHz toggle.</reason>
      </artifact>
      <artifact>
        <path>src/nt_elements.cpp</path>
        <kind>implementation</kind>
        <symbol>parameterChanged</symbol>
        <lines>N/A (to be modified)</lines>
        <reason>Callback implementation must handle kParamSampleRateMode parameter changes and trigger Elements reinitialization at appropriate sample rate (32kHz or 48kHz).</reason>
      </artifact>
      <artifact>
        <path>external/mutable-instruments/elements/dsp/part.h</path>
        <kind>header</kind>
        <symbol>Part::Init</symbol>
        <lines>N/A (submodule)</lines>
        <reason>Elements Part::Init() method accepts sample rate parameter. Called during mode switching with either 32000 or 48000 to reinitialize DSP at target sample rate.</reason>
      </artifact>
      <artifact>
        <path>src/nt_elements.cpp</path>
        <kind>implementation</kind>
        <symbol>construct</symbol>
        <lines>N/A (to be modified)</lines>
        <reason>Initialization must set default sample rate mode (48kHz recommended). Mode state variable added to algorithm structure for tracking current mode.</reason>
      </artifact>
      <artifact>
        <path>src/nt_elements.cpp</path>
        <kind>implementation</kind>
        <symbol>step</symbol>
        <lines>N/A (to be modified)</lines>
        <reason>Audio processing callback may need sample rate conversion logic if 32kHz mode is enabled. Decimation/interpolation between NT's 48kHz bus and Elements' 32kHz processing.</reason>
      </artifact>
    </code>

    <dependencies>
      <cpp>
        <item>arm-none-eabi-gcc</item>
        <description>ARM cross-compiler toolchain for Cortex-M7 with hardware floating-point support</description>
      </cpp>
      <submodules>
        <item>distingNT_API</item>
        <description>Expert Sleepers disting NT C++ plugin API (git submodule). Provides parameter system and 48kHz audio bus infrastructure.</description>
      </submodules>
      <submodules>
        <item>external/mutable-instruments</item>
        <description>Mutable Instruments Elements DSP source code (git submodule, MIT licensed). Native 32kHz operation, supports runtime sample rate via Init() method.</description>
      </submodules>
      <tools>
        <item>VCV Rack + nt_emu</item>
        <description>Desktop testing environment for NT plugin development. Allows rapid iteration and mode switching testing before hardware deployment.</description>
      </tools>
      <hardware>
        <item>disting NT</item>
        <description>Target hardware platform for CPU measurement. Built-in CPU display shows usage percentage for performance validation.</description>
      </hardware>
    </dependencies>
  </artifacts>

  <constraints>
    - Default to 48kHz mode for best NT integration and compatibility with other algorithms
    - Sample rate mode parameter must be serialized with presets (automatic via NT parameter system)
    - Mode switching must not cause audio glitches - require brief muting during transition
    - 32kHz mode requires sample rate conversion (48kHz NT bus ↔ 32kHz Elements processing)
    - Consider deferring 32kHz mode to Epic 2 if sample rate conversion adds too much complexity
    - Story depends on Story 1.5 (Sample Rate Conversion) and Story 1.10 (Hardware Deployment)
    - CPU measurement must be done on actual hardware (desktop measurements not accurate for embedded ARM)
    - Elements Part::Init() must be called with correct sample rate (32000 or 48000) - no other values
    - Clear audio buffers during mode switch to prevent residual artifacts from previous mode
    - Follow parameter update pattern: use parameterChanged() callback for mode switching logic
    - Mode state must be tracked in algorithm structure for proper audio buffer handling in step()
    - If 32kHz mode implemented: simple decimation/interpolation acceptable (quality resampling in Epic 2+)
  </constraints>

  <interfaces>
    <interface>
      <name>Part::Init</name>
      <kind>method</kind>
      <signature>void Init(uint32_t sample_rate)</signature>
      <path>external/mutable-instruments/elements/dsp/part.h</path>
      <description>Elements Part initialization method accepting sample rate parameter. Called during plugin construction and mode switching. Valid values: 32000 or 48000. Recalculates filter coefficients and resets DSP state.</description>
    </interface>
    <interface>
      <name>parameterChanged</name>
      <kind>callback function</kind>
      <signature>void parameterChanged(_NT_algorithm* self, int p)</signature>
      <path>distingNT_API/include/distingNT.h</path>
      <description>NT framework callback for parameter changes. Implements mode switching logic when p == kParamSampleRateMode. Must handle glitch-free transition by muting, reinitializing, and clearing buffers.</description>
    </interface>
    <interface>
      <name>NT 48kHz audio bus</name>
      <kind>audio interface</kind>
      <signature>float* busFrames (28 buses × numFrames samples)</signature>
      <path>distingNT_API/include/distingNT.h (step callback)</path>
      <description>disting NT audio bus system operates at fixed 48kHz. In 32kHz mode, requires sample rate conversion: decimation for input (48k→32k), interpolation for output (32k→48k).</description>
    </interface>
    <interface>
      <name>NT preset serialization</name>
      <kind>system interface</kind>
      <signature>Automatic via parameter array v[]</signature>
      <path>distingNT_API preset system</path>
      <description>NT automatically saves/restores all parameter values including sample rate mode. No custom serialization needed. Mode persists across preset save/load cycles.</description>
    </interface>
  </interfaces>

  <tests>
    <standards>
      This is an embedded C++ audio plugin project with no traditional unit testing framework. Testing follows a desktop-first manual validation approach with hardware validation for performance:
      1) Desktop testing in nt_emu (VCV Rack) for mode switching functionality and audio quality
      2) Hardware testing on disting NT for CPU usage measurement and performance validation
      3) Audio quality verification via listening tests and spectral analysis comparing both modes
      4) Preset persistence testing to verify mode saves/loads correctly
      5) Stress testing: rapid mode switching, mode changes during synthesis

      Validation criteria: smooth mode transitions (no glitches), correct CPU usage differential (~15-20% savings in 32kHz), authentic sound in both modes, preset system preserves mode setting.
    </standards>

    <locations>
      No automated test files - manual testing approach:
      - Desktop testing: VCV Rack with nt_emu module (load plugins/nt_elements.dylib)
      - Hardware testing: disting NT hardware (deploy plugins/nt_elements.o to SD card)
      - CPU profiling: Use disting NT built-in CPU display (visible on main screen)
      - Spectral analysis: External tools for frequency response comparison
      - Validation documented in story completion notes within story file
    </locations>

    <ideas>
      - AC #1 Test: Add kParamSampleRateMode to parameter enumeration. Verify parameter appears in NT interface. Check default value is 48kHz mode. Test toggle between 32kHz (0) and 48kHz (1).

      - AC #2 Test: Set mode to 32kHz. Verify Elements Part::Init(32000) called. Play MIDI note, record output. Confirm authentic 32kHz Elements sound (no coefficient conversion artifacts). Check CPU usage is lower than 48kHz mode.

      - AC #3 Test: Set mode to 48kHz. Verify Elements Part::Init(48000) called. Play identical MIDI note/patch as AC #2 test. Confirm sound uses converted coefficients from Story 1.5. Verify proper 48kHz operation.

      - AC #4 Test: Play sustained MIDI note in 48kHz mode. While note playing, switch to 32kHz mode. Listen for clicks, pops, or glitches during transition. Verify smooth crossover. Repeat switching back to 48kHz. Test rapid mode toggling (stress test).

      - AC #5 Test: Deploy to hardware disting NT. Measure CPU % in 48kHz mode (idle and during synthesis). Switch to 32kHz mode, measure CPU % again. Calculate savings: (48kHz_CPU - 32kHz_CPU) / 48kHz_CPU. Verify ~15-20% relative savings. Document actual measurements.

      - AC #6 Test: Create or update README/user guide documentation section explaining: 1) Mode selection parameter location, 2) 32kHz benefits (lower CPU, authentic sound, matches hardware Elements), 3) 48kHz benefits (NT integration, compatibility, no SRC artifacts), 4) When to use each mode, 5) How to switch modes.

      - Preset Persistence Test: Save preset in 48kHz mode. Switch global mode to 32kHz. Load saved preset. Verify mode switches back to 48kHz (preset overrides global setting). Save preset in 32kHz mode. Load it. Verify 32kHz mode restored.

      - Sample Rate Conversion Test (if 32kHz mode implemented): In 32kHz mode, verify input buffer decimation (48kHz → 32kHz) and output interpolation (32kHz → 48kHz) work correctly. Check for aliasing artifacts or quality degradation.

      - Buffer Clearing Test: During mode switch, verify audio buffers are cleared (memset to zero). Prevents residual samples from previous mode causing artifacts in new mode.

      - Simplification Option Test: If sample rate conversion proves too complex, verify 48kHz-only operation (defer 32kHz to Epic 2). Document decision and rationale in story completion notes.
    </ideas>
  </tests>
</story-context>
