<story-context id="bmad/bmm/workflows/4-implementation/story-context/template" v="1.0">
  <metadata>
    <epicId>1</epicId>
    <storyId>9</storyId>
    <title>Desktop Testing Validation</title>
    <status>drafted</status>
    <generatedAt>2025-10-26</generatedAt>
    <generator>BMAD Story Context Workflow</generator>
    <sourceStoryPath>docs/stories/1-9-desktop-testing-validation.md</sourceStoryPath>
  </metadata>

  <story>
    <asA>a developer</asA>
    <iWant>to validate all functionality works in nt_emu</iWant>
    <soThat>I can iterate quickly without hardware</soThat>
    <tasks>
      - Create test validation checklist (list all features from Stories 1.1-1.8, create test plan for each, define pass/fail criteria, prepare test patch in VCV Rack, document test procedure)
      - Validate Story 1.3: Plugin loads and passes audio (load plugin in nt_emu, verify plugin appears, test audio passthrough, check logs, document load time)
      - Validate Story 1.4: Elements Part instance processes audio (trigger MIDI note-on, verify synthesis sound, verify modal resonance character, test sustained note 10+ seconds, verify no crashes/dropouts)
      - Validate Story 1.5: Sample rate conversion (measure output frequency for MIDI C4 at 261.63 Hz, test chromatic scale C1-C6, verify pitch accuracy within 1 cent, use spectral analyzer, document detuning issues)
      - Validate Story 1.6: MIDI input integration (test note-on triggers synthesis, test note-off stops synthesis, test pitch bend modulates frequency smoothly, test rapid note changes 10+ notes/sec, test legato playing)
      - Validate Story 1.7: Parameter adapter (change geometry/brightness/damping parameters and verify timbre/brightness/decay changes, sweep parameters while synthesizing for zipper noise, test rapid parameter changes for glitches)
      - Validate Story 1.8: Bus routing and audio I/O (test mono input bus routing, test external audio excitation of resonator, test output bus routing, test output mode add vs replace, verify no audio artifacts at bus boundaries)
      - Performance profiling (measure CPU usage during idle/synthesis/parameter changes, estimate ARM Cortex-M7 CPU usage from desktop metrics, document performance baseline for hardware comparison)
      - Stress testing (run synthesis continuously for 30 minutes, send MIDI note bursts 100+ notes, rapidly change parameters throughout test, switch buses and modes during test, monitor for crashes/glitches/memory leaks)
      - Memory usage validation (document DTC/SRAM/DRAM allocation sizes, verify total memory usage less than 128KB SRAM and 2MB DRAM, compare against architecture.md memory budget)
      - Audio quality assessment (record synthesis examples across parameter ranges, listen for authentic Elements character, check for artifacts clicks/pops/glitches, test all exciter modes bow/blow/strike sounds, document sonic characteristics)
      - Create test results document (document all test results pass/fail for each criterion, record performance metrics, record memory usage measurements, note any issues or limitations discovered, create baseline for hardware validation Story 1.10)
      - Fix any issues discovered (address crashes or instability, fix audio artifacts or glitches, resolve parameter mapping issues, optimize performance if needed, re-test after fixes)
    </tasks>
  </story>

  <acceptanceCriteria>
    1. All features from Stories 1.1-1.8 work in nt_emu
    2. Performance metrics captured (CPU usage estimate)
    3. No crashes during 30-minute stress test
    4. Memory usage within expected bounds
    5. Test results documented for hardware comparison
  </acceptanceCriteria>

  <artifacts>
    <docs>
      - path: docs/architecture.md
        title: Decision Architecture
        section: Testing Pattern
        snippet: Desktop-first development workflow - develop on desktop (nt_emu), validate on hardware at milestones. Fast iteration with printf debugging, no hardware deploy cycle during development. Hardware testing reserved for performance and final validation.

      - path: docs/architecture.md
        title: Decision Architecture
        section: Performance Considerations
        snippet: Target less than 30% CPU usage at 48kHz. Expected breakdown - Elements DSP core 15-20%, reverb 5-10% (bypassable), sample rate conversion 2-3%, adapter overhead less than 1%. Total 20-30% depending on mode and reverb. Profile using NT's built-in CPU display.

      - path: docs/architecture.md
        title: Decision Architecture
        section: Memory Layout
        snippet: DTC approximately 4KB for patch structure, parameter state, algorithm state. SRAM approximately 20KB for input buffer 2KB, output buffer 2KB, Elements Part 4KB, resonator state 12KB. DRAM approximately 32KB for reverb buffer. Total approximately 56KB well within hardware limits.

      - path: docs/PRD.md
        title: Product Requirements Document
        section: NFR001 Performance
        snippet: CPU usage must remain below 30% under typical operating conditions to allow algorithm chaining. Target 20-30% with all features active. No audio dropouts or glitches during sustained operation validated via 1-hour stress test.

      - path: docs/PRD.md
        title: Product Requirements Document
        section: NFR002 Sound Quality
        snippet: Audio output must be indistinguishable from hardware Elements module in A/B comparison testing. Frequency response, timbral characteristics, and synthesis behavior must match original within measurable tolerances. Verified via spectral analysis and blind listening tests.

      - path: docs/PRD.md
        title: Product Requirements Document
        section: NFR003 Reliability
        snippet: Plugin must load and operate stably on disting NT hardware without crashes, undefined behavior, or memory leaks. All parameters must respond correctly and predictably. System must handle edge cases gracefully - extreme parameter values, rapid parameter changes, MIDI input bursts.

      - path: docs/epics.md
        title: Epic Breakdown
        section: Story 1.9
        snippet: As a developer I want to validate all functionality works in nt_emu so that I can iterate quickly without hardware. Acceptance criteria - all features from Stories 1.1-1.8 work, performance metrics captured, no crashes during 30-minute stress test, memory usage within expected bounds, test results documented for hardware comparison. Prerequisites all previous Epic 1 stories 1.1-1.8.

      - path: docs/stories/1-3-minimal-plugin-stub-with-audio-passthrough.md
        title: Story 1.3 Minimal Plugin Stub
        section: Acceptance Criteria
        snippet: Plugin implements required disting NT callbacks construct/destruct/step. Plugin loads in nt_emu without errors. Audio input passes through to output unmodified. Plugin displays nt_elements in NT interface. No memory leaks detected during load/unload cycles.

      - path: docs/stories/1-4-elements-part-instance-integration.md
        title: Story 1.4 Elements Part Integration
        section: Acceptance Criteria
        snippet: Elements Part instantiated in plugin construct. Audio buffers allocated SRAM for state DRAM for reverb. Elements Part Process called in step callback. Basic patch parameters initialized to default values. Plugin produces modal synthesis sound when MIDI note received. No crashes or audio glitches during operation.

      - path: docs/stories/1-5-sample-rate-conversion-implementation.md
        title: Story 1.5 Sample Rate Conversion
        section: Acceptance Criteria
        snippet: All Elements filter coefficients scaled from 32kHz to 48kHz. Frequency response measurements match expected values. Pitch tracking accurate across MIDI note range tested C1-C6. Resonator frequencies align with MIDI notes no detuning. A/B comparison shows no unintended timbral changes.

      - path: docs/stories/1-6-midi-note-input-integration.md
        title: Story 1.6 MIDI Integration
        section: Acceptance Criteria
        snippet: MIDI note-on messages trigger synthesis. MIDI note-off messages stop synthesis. MIDI pitch maps correctly to synthesized frequency. Pitch bend messages modulate frequency smoothly. Multiple rapid note-ons handled gracefully monophonic last-note priority.

      - path: docs/stories/1-7-parameter-adapter-layer.md
        title: Story 1.7 Parameter Adapter
        section: Acceptance Criteria
        snippet: Adapter layer converts NT float parameters to Elements patch structure. Parameter changes reflect in synthesis output immediately. At least 3 core parameters exposed geometry brightness damping. Parameter interpolation smooth no zipper noise. Adapter code clearly separated from Elements DSP code.

      - path: docs/stories/1-8-bus-routing-and-audio-i-o.md
        title: Story 1.8 Bus Routing
        section: Acceptance Criteria
        snippet: Mono/stereo audio input supported. Stereo output routes to NT buses correctly. External audio can excite the resonator. Output mode add/replace works correctly. No audio artifacts at bus boundaries.
    </docs>

    <code>
      - path: src/nt_elements.cpp
        kind: plugin main
        symbol: construct, destruct, step
        lines: all
        reason: Main plugin implementation with construct/destruct/step callbacks that must be validated in testing

      - path: src/parameter_adapter.h
        kind: adapter header
        symbol: parameter mapping
        lines: all
        reason: Parameter adapter that converts NT parameters to Elements Patch structure, must validate parameter changes work correctly

      - path: external/mutable-instruments/elements/dsp/part.h
        kind: DSP core
        symbol: Part::Process
        lines: all
        reason: Elements synthesis engine Part class that processes audio, must validate synthesis output and modal character

      - path: external/mutable-instruments/elements/dsp/part.cc
        kind: DSP implementation
        symbol: Part::Init, Part::Process
        lines: all
        reason: Elements DSP implementation that must be validated for correct synthesis behavior at 48kHz

      - path: distingNT_API/include/distingNT.h
        kind: API header
        symbol: NT_algorithm, NT_globals
        lines: all
        reason: distingNT API definitions for callbacks and memory management that must be used correctly
    </code>

    <dependencies>
      - ARM toolchain: arm-none-eabi-gcc (cross-compiler for hardware builds)
      - VCV Rack: v2.x (desktop modular synthesizer for testing)
      - nt_emu module: VCV Rack module that emulates distingNT
      - Git submodules: distingNT_API and mutable-instruments/eurorack
      - Build system: GNU Make with dual targets (hardware and test)
    </dependencies>
  </artifacts>

  <constraints>
    - All memory allocation must use NT_globals.getMemory, never malloc or new
    - NT bus indexing is 1-based (1-28) but array access is 0-based (subtract 1)
    - Frame count parameter numFramesBy4 must be multiplied by 4 to get actual frame count
    - Compiler flags must include -fno-exceptions and -fno-rtti for embedded target
    - Never modify files in git submodules (distingNT_API or external/mutable-instruments)
    - All customization happens in src/ adapter layer only
    - Testing workflow is desktop-first (nt_emu) then hardware validation at milestones
    - Elements DSP expects contiguous input/output buffers, use temporary buffers for NT bus system
    - Output mode must respect add vs replace (replace overwrites, add sums with existing bus contents)
    - Sample rate conversion from 32kHz to 48kHz must maintain pitch accuracy and timbre
  </constraints>

  <interfaces>
    - name: NT_algorithm callbacks
      kind: C function pointers
      signature: construct(_NT_algorithm* self), destruct(_NT_algorithm* self), step(_NT_algorithm* self, float* busFrames, int numFramesBy4), parameterChanged(_NT_algorithm* self, int p), midiMessage(_NT_algorithm* self, uint8_t b0, uint8_t b1, uint8_t b2)
      path: distingNT_API/include/distingNT.h

    - name: Elements Part Process
      kind: C++ class method
      signature: void Part::Process(const Patch& patch, const float* in, float* out, size_t size)
      path: external/mutable-instruments/elements/dsp/part.h

    - name: NT Memory Allocation
      kind: C function
      signature: void* NT_globals.getMemory(size_t size, NT_memoryType type)
      path: distingNT_API/include/distingNT.h

    - name: Elements Part Init
      kind: C++ class method
      signature: void Part::Init(float sample_rate)
      path: external/mutable-instruments/elements/dsp/part.h
  </interfaces>

  <tests>
    <standards>
      Testing follows desktop-first development workflow. Develop and iterate on desktop using VCV Rack with nt_emu module which loads .dylib/.so plugins. Once feature works in nt_emu, validate on hardware at milestones. This story (1.9) performs thorough validation of all Epic 1 features in nt_emu before hardware deployment in Story 1.10. Testing includes functional validation (all features work), performance profiling (CPU usage estimation), stress testing (30 minute continuous operation), memory validation (within budget), and audio quality assessment (authentic Elements sound).
    </standards>

    <locations>
      - Testing environment: VCV Rack with nt_emu module
      - Test patch: VCV Rack patch with MIDI-CV, audio sources, scope, spectrum analyzer
      - Test results: docs/testing/desktop-validation-results.md (to be created)
    </locations>

    <ideas>
      - AC1: Test each story feature systematically - plugin loading (1.3), synthesis sound production (1.4), pitch accuracy across C1-C6 (1.5), MIDI note-on/off and pitch bend (1.6), parameter changes for geometry/brightness/damping (1.7), bus routing and external excitation (1.8)
      - AC2: Profile CPU usage in VCV Rack nt_emu module display during idle, single note sustained, rapid notes, and parameter sweeps. Estimate hardware CPU usage as desktop CPU multiplied by 5-10 factor. Document baseline for Story 1.10 comparison.
      - AC3: Stress test by running synthesis continuously for 30 minutes with MIDI note bursts 100+ notes, rapid parameter changes, bus/mode switching. Monitor for crashes, audio glitches, memory leaks, visual scope anomalies. Pass criteria - no crashes, no audible glitches, stable CPU/memory usage.
      - AC4: Validate memory usage by tracking DTC (approximately 4KB), SRAM (approximately 20KB), DRAM (approximately 32KB) allocations during construct. Verify total under 128KB SRAM and 2MB DRAM limits per architecture.md memory budget.
      - AC5: Document all test results in markdown format including environment details, feature validation pass/fail for each Story 1.1-1.8, performance metrics (idle/synthesis/estimated hardware CPU), memory usage (DTC/SRAM/DRAM), stress test duration/result, audio quality assessment, known issues, and hardware validation baseline for Story 1.10.
      - Audio quality: Listen for modal resonance bell-like/string-like quality, rich harmonic content, smooth parameter transitions, authentic Elements timbre. Check for red flags - clicks/pops/artifacts, harsh aliased sound, parameter zipper noise, inaccurate pitch, crashes/silence.
      - Test patch configuration: MIDI-CV to nt_emu MIDI input, VCO to nt_emu audio input for bus routing test, nt_emu output to VCV Audio output, nt_emu output to Scope for visual monitoring, nt_emu output to Spectrum Analyzer for frequency measurement.
    </ideas>
  </tests>
</story-context>
