<story-context id="bmad/bmm/workflows/4-implementation/story-context/template" v="1.0">
  <metadata>
    <epicId>1</epicId>
    <storyId>10</storyId>
    <title>Hardware Deployment and Validation</title>
    <status>drafted</status>
    <generatedAt>2025-10-26</generatedAt>
    <generator>BMAD Story Context Workflow</generator>
    <sourceStoryPath>docs/stories/1-10-hardware-deployment-and-validation.md</sourceStoryPath>
  </metadata>

  <story>
    <asA>a user</asA>
    <iWant>to load nt_elements on actual disting NT hardware</iWant>
    <soThat>I can verify it works in real Eurorack environment</soThat>
    <tasks>
- Build hardware plugin binary (AC: #1)
  - Run `make hardware` to build ARM .o file
  - Verify plugins/nt_elements.o exists
  - Check file size is reasonable (< 1MB)
  - Document build output and any warnings
  - Verify no build errors

- Prepare disting NT for plugin loading (AC: #2)
  - Format or prepare disting NT SD card
  - Copy nt_elements.o to SD card root directory
  - Safely eject SD card from computer
  - Insert SD card into disting NT
  - Power on disting NT module

- Load plugin on hardware (AC: #2)
  - Navigate to disting NT file browser
  - Select nt_elements.o from file list
  - Load plugin (verify it loads without errors)
  - Check OLED display shows "nt_elements"
  - Verify plugin appears in algorithm list

- Test basic audio output (AC: #3)
  - Connect MIDI keyboard/sequencer to disting NT
  - Send MIDI note-on to trigger synthesis
  - Listen to audio output (verify sound is produced)
  - Compare to desktop version (should sound identical)
  - Record output for comparison with nt_emu recordings

- Test parameter control (AC: #3)
  - Adjust geometry parameter via NT pot/encoder
  - Adjust brightness parameter
  - Adjust damping parameter
  - Verify parameters respond correctly
  - Listen for smooth parameter changes (no artifacts)

- Test bus routing and audio I/O (AC: #6)
  - Patch external audio to disting NT input
  - Configure input bus routing
  - Test external audio excitation of resonator
  - Test output bus routing to other Eurorack modules
  - Test output mode (add vs replace)

- Measure CPU usage (AC: #4)
  - Observe disting NT main display CPU percentage
  - Measure idle CPU (no notes playing)
  - Measure synthesis CPU (note held, typical patch)
  - Measure maximum CPU (complex patch, multiple parameters)
  - Document CPU usage for different scenarios

- Perform 1-hour stability test (AC: #5)
  - Configure MIDI sequencer with continuous note pattern
  - Run synthesis for 1 hour
  - Monitor for crashes, freezes, or glitches
  - Check for audio dropouts or artifacts
  - Verify plugin remains responsive after test

- Compare desktop vs hardware results (AC: #3)
  - Compare recordings from nt_emu vs hardware
  - Perform spectral analysis of both recordings
  - Listen for sonic differences (should be identical)
  - Document any differences or issues
  - Verify pitch accuracy on hardware

- Test MIDI functionality on hardware (AC: #6)
  - Test MIDI note-on/off
  - Test pitch accuracy across note range (C1-C6)
  - Test pitch bend
  - Test rapid note changes
  - Test legato playing

- Create hardware validation report (AC: #1-6)
  - Document hardware test results (pass/fail for each AC)
  - Record CPU usage measurements
  - Document stability test results
  - Compare to desktop baseline (Story 1.9)
  - Note any hardware-specific issues

- Address hardware-specific issues (AC: #2-6)
  - Fix any crashes or instability
  - Resolve audio artifacts if present
  - Optimize performance if CPU usage too high
  - Re-test after fixes
  - Document final hardware validation status
    </tasks>
  </story>

  <acceptanceCriteria>
1. Plugin (.o file) compiles successfully for ARM hardware
2. Loads on disting NT without errors
3. Audio output sounds identical to nt_emu version
4. CPU usage measured and documented (target < 50% at this stage)
5. Operates stably for 1-hour test
6. MIDI and audio I/O work correctly in hardware
  </acceptanceCriteria>

  <artifacts>
    <docs>
- path: docs/PRD.md
  title: Product Requirements Document
  section: Non-Functional Requirements
  snippet: NFR001 requires < 30% CPU usage (final target), with Story 1.10 targeting < 50% as interim milestone. NFR002 requires 100% sound authenticity vs hardware Elements. NFR003 requires stability with no crashes during sustained operation.

- path: docs/PRD.md
  title: Product Requirements Document
  section: User Journeys
  snippet: Sarah's journey demonstrates loading plugin from SD card, selecting from NT file browser, and immediate audio output when MIDI notes are sent. Hardware interface uses 3 pots, 2 encoders, 4 buttons, and OLED display.

- path: docs/architecture.md
  title: Decision Architecture
  section: Technology Stack Details
  snippet: Hardware platform is disting NT Eurorack module with ARM Cortex-M7 @ 480MHz, ~128KB SRAM, ~8MB DRAM. Audio runs at 48kHz (primary) or 32kHz (compatibility mode). Build uses ARM GCC with flags -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard -Os.

- path: docs/architecture.md
  title: Decision Architecture
  section: Build Pattern
  snippet: Hardware build command is 'make hardware' which produces plugins/nt_elements.o ARM object file. Desktop build 'make test' produces .dylib for nt_emu testing. Makefile includes explicit source list with Elements DSP .cc files.

- path: docs/architecture.md
  title: Decision Architecture
  section: Testing Pattern
  snippet: Desktop-first development workflow uses nt_emu for iteration, with hardware validation at milestones (Stories 1.9, 1.10, 2.5-2.7). Performance testing uses NT built-in CPU display, no custom instrumentation needed.

- path: docs/architecture.md
  title: Decision Architecture
  section: Performance Considerations
  snippet: Target < 30% CPU at 48kHz final. Expected breakdown: Elements DSP 15-20%, Reverb 5-10%, sample rate conversion 2-3%. Hardware profiling uses NT's built-in CPU % display. 32kHz mode saves 15-20% vs 48kHz.

- path: docs/architecture.md
  title: Decision Architecture
  section: Deployment Architecture
  snippet: Hardware deployment copies plugins/nt_elements.o to disting NT SD card root. Installation: download .o file, copy to SD card, navigate to NT file browser, select .o file, plugin loads into RAM and appears in algorithm list.

- path: docs/architecture.md
  title: Decision Architecture
  section: Memory Layout
  snippet: Memory allocation uses DTC (~4KB) for Patch structure and parameters, SRAM (~20KB) for input/output buffers and Part structure, DRAM (~32KB) for reverb buffer. Total ~56KB well within hardware limits.

- path: docs/architecture.md
  title: Decision Architecture
  section: Audio Quality
  snippet: Target 100% sound authenticity vs hardware Elements. Validation includes A/B comparison, spectral analysis, frequency response measurements, and blind listening tests. Elements DSP used unmodified to preserve authenticity.

- path: docs/epics.md
  title: Epic Breakdown
  section: Story 1.10
  snippet: Story follows 1.9 desktop validation. Compiles ARM .o file, deploys to SD card, loads on hardware NT, validates audio output matches desktop, measures CPU < 50% interim target, runs 1-hour stability test, confirms MIDI/audio I/O work correctly.

- path: docs/stories/1-9-desktop-testing-validation.md
  title: Story 1.9 Desktop Testing Validation
  section: Dev Notes
  snippet: Story 1.9 establishes desktop baseline with performance metrics, memory usage measurements, stress testing results, and audio quality assessment. Story 1.10 compares hardware results against this baseline.

- path: docs/stories/1-9-desktop-testing-validation.md
  title: Story 1.9 Desktop Testing Validation
  section: Test Results Documentation Format
  snippet: Desktop validation produces test results document with environment details, feature validation checklist, performance metrics, memory usage, stress test results, audio quality assessment, and known issues list.
    </docs>
    <code>
- path: (No existing code files - Story 1.10 validates hardware deployment of code from Stories 1.1-1.9)
  kind: build-artifact
  symbol: plugins/nt_elements.o
  lines: N/A
  reason: ARM object file created by 'make hardware', deployed to disting NT SD card for loading

- path: (Makefile to be created in Story 1.1)
  kind: build-system
  symbol: hardware target
  lines: N/A
  reason: Makefile defines 'hardware' target that compiles ARM .o file with cross-compilation flags

- path: (src/nt_elements.cpp to be created in Stories 1.1-1.8)
  kind: plugin-implementation
  symbol: construct, destruct, step, midiMessage, parameterChanged
  lines: N/A
  reason: Main plugin implementing NT callbacks, Elements Part integration, parameter adapter, bus routing - validated on hardware in this story
    </code>
    <dependencies>
- build-toolchain: arm-none-eabi-gcc (ARM GCC cross-compiler for Cortex-M7)
- api-framework: distingNT API v9 (git submodule from Expert Sleepers)
- dsp-source: Mutable Instruments Elements (git submodule from eurorack repo)
- hardware-platform: disting NT Eurorack module (ARM Cortex-M7 @ 480MHz)
- testing-environment: nt_emu VCV Rack module (desktop validation in Story 1.9 provides baseline)
- deployment-media: SD card formatted FAT32 for disting NT plugin loading
    </dependencies>
  </artifacts>

  <constraints>
- Hardware build must use ARM GCC with flags: -std=c++11 -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard -Os -fno-rtti -fno-exceptions
- Plugin must allocate all memory via NT_globals.getMemory() - never use malloc/new
- Memory must fit within: DTC ~64KB, SRAM ~128KB, DRAM ~8MB
- CPU usage target for Story 1.10 is < 50% (interim), final target < 30% optimized in Story 2.5
- Audio must match desktop version (Story 1.9 baseline) - no hardware-specific sonic differences
- Plugin must load from SD card without installation - runs from RAM after loading
- SD card must be FAT32 filesystem with .o file in root or subdirectory
- No printf/console debugging on hardware - OLED display only debug output
- Stability requirement: 1+ hour continuous operation without crashes or audio glitches
- MIDI and audio I/O must work correctly: note-on/off, pitch bend, external audio routing, bus modes
  </constraints>

  <interfaces>
- name: disting NT Plugin API
  kind: C++ plugin interface
  signature: construct(), destruct(), step(), midiMessage(), parameterChanged(), draw()
  path: distingNT_API/include/distingNT.h

- name: NT Memory Allocator
  kind: Memory management API
  signature: void* NT_globals.getMemory(size_t size, NT_memType type)
  path: distingNT_API/include/distingNT.h

- name: NT Bus System
  kind: Audio routing interface
  signature: float* busFrames array (1-indexed bus parameters, 0-indexed array access)
  path: distingNT_API/include/distingNT.h

- name: MIDI Message Handler
  kind: MIDI callback
  signature: void midiMessage(_NT_algorithm* self, uint8_t b0, uint8_t b1, uint8_t b2)
  path: distingNT_API/include/distingNT.h

- name: Elements Part::Process
  kind: DSP processing function
  signature: void Process(const Patch& patch, const float* in, float* out, size_t frames)
  path: external/mutable-instruments/elements/dsp/part.h

- name: Hardware Build Target
  kind: Makefile target
  signature: make hardware (produces plugins/nt_elements.o)
  path: Makefile (created in Story 1.1)
  </interfaces>

  <tests>
    <standards>
Hardware validation follows desktop testing pattern established in Story 1.9. Testing is manual and observational using disting NT hardware interface and audio monitoring. Performance measured via NT's built-in CPU display. Audio quality validated by A/B comparison with desktop recordings. Stability verified via extended runtime testing (1 hour minimum). MIDI and parameter functionality tested interactively. No automated test framework - validation is exploratory testing with documented results.
    </standards>
    <locations>
- Test results documented in: docs/testing/hardware-validation-results.md (new file created in this story)
- Desktop baseline from: docs/testing/desktop-validation-results.md (created in Story 1.9)
- Build artifacts: plugins/nt_elements.o (ARM object file for hardware deployment)
    </locations>
    <ideas>
- AC#1 Build Test: Run 'make hardware', verify plugins/nt_elements.o exists and is reasonable size (200-800KB), document any build warnings or errors, confirm ARM object file format

- AC#2 Load Test: Copy .o to SD card, insert in disting NT, navigate file browser, select plugin, verify loads without errors, check OLED shows "nt_elements", confirm appears in algorithm list

- AC#3 Audio Identity Test: Record identical MIDI sequence on desktop (nt_emu) and hardware, perform spectral analysis comparison, blind listening test for sonic differences, verify pitch accuracy across range

- AC#4 CPU Performance Test: Measure idle CPU (no notes), synthesis CPU (sustained note), maximum CPU (complex patch + parameter changes), document all measurements, verify < 50% target met

- AC#5 Stability Test: Configure MIDI sequencer with continuous varied note pattern, run for 1+ hour, monitor for crashes/freezes/glitches, check audio dropouts, verify responsiveness after test

- AC#6 I/O Functionality Test: Test MIDI note-on/off triggers synthesis, verify pitch bend modulation, test rapid note changes, patch external audio to input, test bus routing, verify output modes (add/replace)

- Integration Test: Use plugin in full Eurorack patch with other modules, route to external effects, verify compatibility with NT bus system, test with various MIDI sources

- Comparison Test: Side-by-side comparison of desktop (Story 1.9) vs hardware results for all features, identify any hardware-specific issues, document behavioral differences if any

- Regression Test: Validate all features from Stories 1.1-1.8 work correctly on hardware (plugin loads, Elements processes audio, MIDI input, parameters respond, bus routing, sample rate conversion)

- Documentation Test: Create hardware validation report documenting all test results, CPU measurements, stability outcomes, comparison to desktop baseline, list any issues discovered
    </ideas>
  </tests>
</story-context>
