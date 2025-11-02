<story-context id="bmad/bmm/workflows/4-implementation/story-context/template" v="1.0">
  <metadata>
    <epicId>1</epicId>
    <storyId>4</storyId>
    <title>Elements Part Instance Integration</title>
    <status>drafted</status>
    <generatedAt>2025-10-26</generatedAt>
    <generator>BMAD Story Context Workflow</generator>
    <sourceStoryPath>docs/stories/1-4-elements-part-instance-integration.md</sourceStoryPath>
  </metadata>

  <story>
    <asA>developer</asA>
    <iWant>to instantiate Elements' Part class and route audio through it</iWant>
    <soThat>Elements DSP processes audio in the plugin</soThat>
    <tasks>
      - [ ] Allocate memory regions for Elements (AC: #2)
        - [ ] Allocate DTC for Part structure and current Patch (~4KB)
        - [ ] Allocate SRAM for resonator state and audio buffers (~20KB)
        - [ ] Allocate DRAM for reverb buffer (~32KB)
        - [ ] Check all allocations succeed, fail gracefully if not

      - [ ] Create and initialize Elements Part instance (AC: #1)
        - [ ] Instantiate elements::Part in DTC memory
        - [ ] Call Part::Init() with 48kHz sample rate (Story 1.5 will add mode toggle)
        - [ ] Verify initialization succeeds

      - [ ] Initialize default patch parameters (AC: #4)
        - [ ] Set gate = false, note = 440Hz (A4)
        - [ ] Set exciter defaults (bow: 0.5, blow: 0.0, strike: 0.5)
        - [ ] Set resonator defaults (geometry: 0.5, brightness: 0.7, damping: 0.7)
        - [ ] Set reverb defaults (amount: 0.2, size: 0.5)
        - [ ] Document default patch rationale (balanced starting sound)

      - [ ] Integrate Part::Process() into step() callback (AC: #3)
        - [ ] Remove passthrough code from Story 1.3
        - [ ] Allocate temp input/output buffers (512 floats each in SRAM)
        - [ ] Copy NT bus input to temp buffer
        - [ ] Call elements_part->Process(patch, input, output, numFrames)
        - [ ] Copy temp output to NT bus (respecting output mode)

      - [ ] Add basic MIDI note handling (AC: #5)
        - [ ] Implement midiMessage() callback
        - [ ] Parse MIDI note-on: set patch.gate = true, patch.note = freq
        - [ ] Parse MIDI note-off: set patch.gate = false
        - [ ] Use MIDI note 60 (C4 = 261.6Hz) for initial testing
        - [ ] Basic conversion: freq = 440 * pow(2, (note - 69) / 12)

      - [ ] Test modal synthesis sound in nt_emu (AC: #5, #6)
        - [ ] Build with `make test`
        - [ ] Load in VCV Rack nt_emu
        - [ ] Send MIDI note from VCV MIDI-CV module
        - [ ] Verify modal synthesis sound (resonant, bell-like)
        - [ ] Test multiple notes across range (C2-C6)
        - [ ] Verify no crashes or audio artifacts

      - [ ] Stress test for stability (AC: #6)
        - [ ] Test rapid MIDI note-ons (10+ notes/second)
        - [ ] Test held notes for 30+ seconds
        - [ ] Monitor VCV Rack CPU usage
        - [ ] Check VCV log for errors or warnings
        - [ ] Verify no memory corruption (valgrind if available)

      - [ ] Build hardware version (AC: #1-6)
        - [ ] Run `make hardware`
        - [ ] Verify ARM build succeeds with Elements integration
        - [ ] Check .o file size (~300-400KB expected)
    </tasks>
  </story>

  <acceptanceCriteria>
    1. Elements::Part instantiated in plugin construct()
    2. Audio buffers allocated (SRAM for state, DRAM for reverb)
    3. Elements Part::Process() called in step() callback
    4. Basic patch parameters initialized to default values
    5. Plugin produces modal synthesis sound when MIDI note received
    6. No crashes or audio glitches during operation
  </acceptanceCriteria>

  <artifacts>
    <docs>
      - path: docs/PRD.md
        title: Product Requirements Document
        section: Functional Requirements
        snippet: "FR001: Port complete Elements DSP engine including all three exciter modes. FR002: Implement full 64-filter modal resonator. FR003: Include string and tube modeling algorithms from Elements codebase."

      - path: docs/PRD.md
        title: Product Requirements Document
        section: Non-Functional Requirements
        snippet: "NFR001: CPU usage must remain below 30% under typical operating conditions. NFR003: Plugin must load and operate stably on disting NT hardware without crashes, undefined behavior, or memory leaks."

      - path: docs/architecture.md
        title: Decision Architecture
        section: Memory Allocation Pattern
        snippet: "All memory must be allocated via NT_globals.getMemory(). DTC allocation (ultra-fast, small) for Part state, SRAM allocation (fast, medium-sized buffers) for processing, DRAM allocation (large buffers, slower) for reverb."

      - path: docs/architecture.md
        title: Decision Architecture
        section: Audio Buffer Access Pattern
        snippet: "NT bus indexing is 1-based, array access is 0-based. numFramesBy4 MUST be multiplied by 4. Always use temp buffers for Elements processing."

      - path: docs/architecture.md
        title: Decision Architecture
        section: MIDI Handling Pattern
        snippet: "MIDI note to frequency conversion: f = 440 * 2^((note - 69)/12). Handle note-on (status 0x90, velocity > 0) and note-off (status 0x80 or 0x90 with velocity 0)."

      - path: docs/architecture.md
        title: Decision Architecture
        section: ADR-003 Optimized Memory Region Split
        snippet: "DTC (~4KB) for hot state (Patch, parameters), SRAM (~20KB) for filters and buffers, DRAM (~32KB) for reverb buffer. Maximizes performance for < 30% CPU target."

      - path: docs/architecture.md
        title: Decision Architecture
        section: Testing Pattern
        snippet: "Desktop-first development workflow: develop and iterate on desktop with nt_emu, once feature works validate on hardware, profile performance on hardware only."

      - path: docs/architecture.md
        title: Decision Architecture
        section: Data Architecture
        snippet: "Elements Patch structure contains note (Hz), gate (bool), exciter parameters (bow/blow/strike level and timbre), resonator parameters (geometry, brightness, damping, position, modulation), and reverb parameters (amount, time, diffusion)."

      - path: docs/epics.md
        title: Epic Breakdown
        section: Story 1.4
        snippet: "Elements::Part instantiated in plugin construct(), audio buffers allocated, Part::Process() called in step() callback, basic patch parameters initialized, plugin produces modal synthesis sound when MIDI note received."

      - path: docs/stories/1-3-minimal-plugin-stub-with-audio-passthrough.md
        title: Previous Story - Plugin Stub
        section: Completion
        snippet: "Story 1.3 established the basic plugin structure with construct(), destruct(), and step() callbacks implementing audio passthrough. This story builds on that foundation to integrate Elements DSP."

      - path: docs/stories/1-1-development-environment-setup.md
        title: Development Environment Setup
        section: Dev Notes
        snippet: "Build system uses GNU Make with dual targets. All memory allocation MUST use NT_globals.getMemory(). Memory regions: DTC (ultra-fast, ~64KB), SRAM (fast, ~128KB), DRAM (large, ~8MB)."

      - path: docs/stories/1-2-elements-source-integration.md
        title: Elements Source Integration
        section: Dev Notes
        snippet: "Elements consists of 8 primary DSP source files including part.cc/h (main synthesis engine), voice.cc/h, exciter.cc/h, resonator.cc/h, string.cc/h, tube.cc/h, multistage_envelope.cc/h. All files compiled directly with adapter."
    </docs>

    <code>
      - path: external/mutable-instruments/elements/dsp/part.h
        kind: header
        symbol: elements::Part
        lines:
        reason: Main Elements synthesis engine class that needs to be instantiated and initialized. Contains Init() and Process() methods.

      - path: external/mutable-instruments/elements/dsp/part.cc
        kind: implementation
        symbol: elements::Part::Init, elements::Part::Process
        lines:
        reason: Core DSP processing methods. Init() must be called with sample rate (48000), Process() called each audio callback with Patch, input/output buffers.

      - path: external/mutable-instruments/elements/dsp/patch.h
        kind: header
        symbol: elements::Patch
        lines:
        reason: Data structure containing all synthesis parameters (note, gate, exciter levels, resonator settings, reverb). Must be initialized with default values.

      - path: distingNT_API/include/distingNT.h
        kind: header
        symbol: NT_globals, _NT_algorithm
        lines:
        reason: NT API providing getMemory() for allocation and base algorithm structure. Defines memory region constants (kNT_memDTC, kNT_memSRAM, kNT_memDRAM).
    </code>

    <dependencies>
      cpp:
        - arm-none-eabi-gcc: ARM cross-compiler toolchain for Cortex-M7
        - distingNT_API: Expert Sleepers disting NT plugin API (git submodule)
        - mutable-instruments: Elements DSP source code (git submodule at external/mutable-instruments)
      tools:
        - make: GNU Make build system
        - VCV Rack: Desktop modular synthesizer for testing
        - nt_emu: VCV Rack module that emulates distingNT
    </dependencies>
  </artifacts>

  <constraints>
    - All memory allocation MUST use NT_globals.getMemory() - never malloc/new
    - Memory must be allocated from appropriate regions: DTC for hot state, SRAM for processing buffers, DRAM for large buffers
    - NT bus indexing is 1-based but array access is 0-based (subtract 1)
    - numFramesBy4 parameter MUST be multiplied by 4 to get actual frame count
    - Output mode must be respected (add vs replace) when writing to output bus
    - Elements source files in external/mutable-instruments/ are read-only (git submodule) - never modify
    - All adapter code goes in src/ directory, separate from Elements DSP
    - Desktop-first development: test in nt_emu before hardware deployment
    - No exceptions allowed (compiler flag -fno-exceptions)
    - Defensive programming: check allocation success, clamp values, bounds checking
    - Elements Part::Init() must be called with sample rate before Process() can be used
    - Audio buffers for Elements must be contiguous arrays (use temp buffers for NT bus system)
  </constraints>

  <interfaces>
    - name: elements::Part::Init
      kind: function
      signature: void Init(uint32_t sample_rate)
      path: external/mutable-instruments/elements/dsp/part.h

    - name: elements::Part::Process
      kind: function
      signature: void Process(const Patch& patch, const float* input, float* output, size_t size)
      path: external/mutable-instruments/elements/dsp/part.h

    - name: NT_globals.getMemory
      kind: function
      signature: void* getMemory(size_t size, NT_memoryRegion region)
      path: distingNT_API/include/distingNT.h

    - name: _NT_algorithm::step
      kind: callback
      signature: void step(_NT_algorithm* self, float* busFrames, int numFramesBy4)
      path: distingNT_API/include/distingNT.h

    - name: _NT_algorithm::midiMessage
      kind: callback
      signature: void midiMessage(_NT_algorithm* self, uint8_t b0, uint8_t b1, uint8_t b2)
      path: distingNT_API/include/distingNT.h

    - name: elements::Patch
      kind: structure
      signature: struct with fields: note (float Hz), gate (bool), exciter params, resonator params, reverb params
      path: external/mutable-instruments/elements/dsp/patch.h
  </interfaces>

  <tests>
    <standards>
      Testing follows desktop-first development workflow using VCV Rack with nt_emu module. Build with 'make test' to create .dylib for desktop testing, then 'make hardware' to create .o for hardware deployment. Test functionality in nt_emu first for fast iteration with debugging, then validate on actual hardware at key milestones. Performance testing happens on hardware only using NT's built-in CPU display. No unit test framework - validation is integration testing via audio output and MIDI input. Stress testing includes rapid parameter changes, sustained operation, and MIDI input bursts.
    </standards>

    <locations>
      - plugins/nt_elements.dylib: Desktop test build output
      - plugins/nt_elements.o: Hardware build output
      - VCV Rack patches for testing with nt_emu module
    </locations>

    <ideas>
      - AC #1: Test Elements::Part instantiation by verifying construct() succeeds and returns non-null algorithm pointer
      - AC #2: Verify memory allocations by checking return values from NT_globals.getMemory() for DTC, SRAM, DRAM regions
      - AC #3: Confirm Part::Process() integration by sending audio through and verifying synthesized output instead of passthrough
      - AC #4: Validate default patch by inspecting initial synthesis sound characteristics (should be balanced, pleasant modal sound)
      - AC #5: Test MIDI note input by sending note-on/note-off messages and verifying pitch-accurate synthesis across octave range (C2-C6)
      - AC #6: Stability testing with 30+ second sustained notes, rapid note-ons (10+ per second), and monitoring for crashes or audio glitches
      - Integration test: Load plugin in nt_emu, send MIDI note 60 (C4), verify modal synthesis sound output
      - Stress test: Send rapid MIDI note bursts, verify no buffer overruns or crashes
      - Hardware build verification: Ensure ARM compilation succeeds and .o file size is reasonable (~300-400KB)
    </ideas>
  </tests>
</story-context>
