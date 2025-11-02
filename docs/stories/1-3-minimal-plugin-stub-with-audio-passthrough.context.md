<story-context id="1-3-minimal-plugin-stub-with-audio-passthrough" v="1.0">
  <metadata>
    <epicId>1</epicId>
    <storyId>3</storyId>
    <title>Minimal Plugin Stub with Audio Passthrough</title>
    <status>drafted</status>
    <generatedAt>2025-10-26</generatedAt>
    <generator>BMAD Story Context Workflow</generator>
    <sourceStoryPath>docs/stories/1-3-minimal-plugin-stub-with-audio-passthrough.md</sourceStoryPath>
  </metadata>

  <story>
    <asA>developer</asA>
    <iWant>a minimal nt_elements plugin that loads and passes audio through</iWant>
    <soThat>I can verify the plugin interface works before adding DSP</soThat>
    <tasks>
- [ ] Implement NT algorithm factory (AC: #1)
  - [ ] Define _nt_elementsAlgorithm structure extending _NT_algorithm
  - [ ] Implement constructAlgorithm() factory function
  - [ ] Return properly initialized algorithm instance

- [ ] Implement construct() callback (AC: #1, #5)
  - [ ] Allocate algorithm structure using NT_globals.getMemory(kNT_memSRAM)
  - [ ] Check allocation success, return nullptr on failure
  - [ ] Initialize NT_algorithm base fields
  - [ ] Set algorithm name to "nt_elements"
  - [ ] Return algorithm pointer

- [ ] Implement destruct() callback (AC: #1, #5)
  - [ ] Verify algorithm pointer is valid
  - [ ] Free any allocated memory (currently just algorithm struct)
  - [ ] No explicit free needed (NT manages memory regions)

- [ ] Define initial parameters (AC: #1, #4)
  - [ ] Add kParamInputBus (default: 1, range: 1-28)
  - [ ] Add kParamOutputBus (default: 1, range: 1-28)
  - [ ] Add kParamOutputMode (default: 0=add, range: 0-1)
  - [ ] Create parameter definition array with names and ranges

- [ ] Implement step() audio callback with passthrough (AC: #3)
  - [ ] Calculate numFrames = numFramesBy4 * 4 (CRITICAL)
  - [ ] Get input bus pointer: busFrames + (inputBus - 1) * numFrames
  - [ ] Get output bus pointer: busFrames + (outputBus - 1) * numFrames
  - [ ] Copy input to output respecting output mode (add vs replace)
  - [ ] Handle add mode: output[i] += input[i]
  - [ ] Handle replace mode: output[i] = input[i]

- [ ] Implement parameterChanged() callback (AC: #1)
  - [ ] Store parameter updates in algorithm structure
  - [ ] No processing needed yet (just storage)

- [ ] Test in nt_emu desktop environment (AC: #2, #3)
  - [ ] Build with `make test`
  - [ ] Load plugins/nt_elements.dylib in VCV Rack nt_emu
  - [ ] Verify plugin appears in module list as "nt_elements"
  - [ ] Connect audio source to nt_emu input
  - [ ] Connect nt_emu output to audio analyzer/scope
  - [ ] Verify audio passes through unmodified

- [ ] Test load/unload cycles (AC: #5)
  - [ ] Load plugin in nt_emu
  - [ ] Remove nt_emu module from patch
  - [ ] Re-add nt_emu and reload plugin
  - [ ] Repeat 10+ times
  - [ ] Check VCV Rack memory usage (should not grow)
  - [ ] Check VCV Rack log for errors

- [ ] Build hardware version (AC: #1)
  - [ ] Build with `make hardware`
  - [ ] Verify plugins/nt_elements.o builds successfully
  - [ ] Check file size is reasonable (~100-200KB for stub)
    </tasks>
  </story>

  <acceptanceCriteria>
1. Plugin implements required disting NT callbacks (construct, destruct, step)
2. Plugin loads in nt_emu without errors
3. Audio input passes through to output unmodified
4. Plugin displays "nt_elements" in NT interface
5. No memory leaks detected during load/unload cycles
  </acceptanceCriteria>

  <artifacts>
    <docs>
      <doc>
        <path>docs/PRD.md</path>
        <title>Product Requirements Document</title>
        <section>Functional Requirements</section>
        <snippet>FR009: Support mono/stereo audio input with external audio routing to resonator. FR010: Output stereo audio integrated with NT's 28-bus routing system. FR014: Process audio at 48kHz sample rate.</snippet>
      </doc>
      <doc>
        <path>docs/PRD.md</path>
        <title>Product Requirements Document</title>
        <section>Non-Functional Requirements</section>
        <snippet>NFR003: Plugin must load and operate stably on disting NT hardware without crashes, undefined behavior, or memory leaks. All parameters must respond correctly and predictably.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Memory Allocation Pattern</section>
        <snippet>All memory MUST use NT_globals.getMemory(). Never use malloc, calloc, or new - embedded platform with custom memory regions. Example: algo = (_nt_elementsAlgorithm*)NT_globals.getMemory(sizeof(_nt_elementsAlgorithm), kNT_memSRAM);</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Audio Buffer Access Pattern</section>
        <snippet>NT bus indexing is 1-based, array access is 0-based. numFramesBy4 is NOT the actual frame count. Multiply by 4: int numFrames = numFramesBy4 * 4. Bus access: busFrames + (busNumber - 1) * numFrames.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Algorithm Structure</section>
        <snippet>struct _nt_elementsAlgorithm : public _NT_algorithm { float v[kNumParameters]; }. Extends NT's base _NT_algorithm which provides callback function pointers.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Testing Pattern</section>
        <snippet>Desktop-first workflow: 1. Develop in nt_emu (fast iteration, printf debugging) 2. Test audio passthrough with scope/analyzer 3. Build hardware target to verify ARM compilation 4. Deploy to hardware only after desktop validation.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Build Pattern</section>
        <snippet>Makefile source list must be explicit. Build targets: make test (desktop .dylib) and make hardware (ARM .o). Includes: -I./src -I./distingNT_API/include -I./external/mutable-instruments -I./src/stmlib_stubs</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Project Structure</section>
        <snippet>src/ for adapter code, distingNT_API/ submodule, external/mutable-instruments/ submodule, plugins/ for build output (.o and .dylib), build/ for intermediate artifacts.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Output Modes</section>
        <snippet>NT supports two output modes: Replace (0): output[i] = input[i] - overwrites bus content. Add (1): output[i] += input[i] - mixes with existing bus content. Add mode enables chaining multiple algorithms to the same bus.</snippet>
      </doc>
      <doc>
        <path>docs/epics.md</path>
        <title>Epic Breakdown</title>
        <section>Story 1.3</section>
        <snippet>Minimal Plugin Stub with Audio Passthrough - verify the plugin interface works before adding DSP. Prerequisites: Story 1.1, 1.2. Acceptance criteria: callbacks implemented, loads without errors, audio passthrough, displays name, no memory leaks.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Development Environment</section>
        <snippet>VCV Rack + nt_emu module for desktop testing. nt_emu: VCV Rack module that emulates distingNT, loads .dylib/.so plugins. Build System: GNU Make with dual targets (hardware + test).</snippet>
      </doc>
    </docs>
    <code>
      <artifact>
        <path>distingNT_API/include/distingNT.h</path>
        <kind>header</kind>
        <symbol>_NT_algorithm</symbol>
        <lines>N/A</lines>
        <reason>Base structure that defines callback function pointers (construct, destruct, step, parameterChanged, midiMessage) for NT plugins</reason>
      </artifact>
      <artifact>
        <path>distingNT_API/include/distingNT.h</path>
        <kind>header</kind>
        <symbol>NT_globals</symbol>
        <lines>N/A</lines>
        <reason>Global object providing getMemory() for allocating plugin memory in DTC/SRAM/DRAM regions</reason>
      </artifact>
      <artifact>
        <path>src/nt_elements.cpp</path>
        <kind>source</kind>
        <symbol>constructAlgorithm</symbol>
        <lines>N/A</lines>
        <reason>Factory function to create - needs to be implemented in this story as main entry point</reason>
      </artifact>
    </code>
    <dependencies>
      <cpp>
        <package>distingNT_API</package>
        <version>git submodule (latest)</version>
        <note>Expert Sleepers API for disting NT plugins</note>
      </cpp>
      <toolchain>
        <package>arm-none-eabi-gcc</package>
        <version>latest</version>
        <note>ARM cross-compiler for hardware builds</note>
      </toolchain>
      <testing>
        <package>VCV Rack</package>
        <version>v2.x</version>
        <note>Desktop modular synth environment for nt_emu testing</note>
      </testing>
      <testing>
        <package>nt_emu</package>
        <version>latest</version>
        <note>VCV Rack module that emulates distingNT hardware</note>
      </testing>
    </dependencies>
  </artifacts>

  <constraints>
- CRITICAL: All memory allocation must use NT_globals.getMemory() - never malloc/new/calloc
- CRITICAL: numFramesBy4 MUST be multiplied by 4 to get actual frame count
- CRITICAL: NT bus numbering is 1-28 (1-based), but array indexing is 0-based (subtract 1)
- Output mode handling: Replace mode overwrites bus, Add mode mixes with existing content
- Algorithm structure must extend _NT_algorithm base class
- No exceptions allowed (embedded platform) - use -fno-exceptions compiler flag
- No RTTI allowed - use -fno-rtti compiler flag
- Desktop testing (nt_emu) must come before hardware deployment
- Plugin must display correct name "nt_elements" in NT interface
- Memory leaks are unacceptable - validate with load/unload cycling
- Build must produce both .dylib (desktop) and .o (hardware) targets
- Header-only integration approach - compile all sources together
  </constraints>

  <interfaces>
    <interface>
      <name>_NT_algorithm</name>
      <kind>C++ base class</kind>
      <signature>struct _NT_algorithm with virtual callbacks: construct(), destruct(), step(), parameterChanged(), midiMessage(), draw()</signature>
      <path>distingNT_API/include/distingNT.h</path>
    </interface>
    <interface>
      <name>constructAlgorithm</name>
      <kind>Factory function</kind>
      <signature>extern "C" _NT_algorithm* constructAlgorithm(void)</signature>
      <path>src/nt_elements.cpp</path>
      <note>Entry point called by NT to instantiate plugin</note>
    </interface>
    <interface>
      <name>NT_globals.getMemory</name>
      <kind>Memory allocator</kind>
      <signature>void* getMemory(size_t size, NT_memoryType type)</signature>
      <path>distingNT_API/include/distingNT.h</path>
      <note>Types: kNT_memDTC, kNT_memSRAM, kNT_memDRAM</note>
    </interface>
    <interface>
      <name>step</name>
      <kind>Audio callback</kind>
      <signature>void step(_NT_algorithm* self, float* busFrames, int numFramesBy4)</signature>
      <path>N/A</path>
      <note>Called per audio block. busFrames contains 28 buses interleaved. numFramesBy4 * 4 = actual frames</note>
    </interface>
    <interface>
      <name>parameterChanged</name>
      <kind>Parameter callback</kind>
      <signature>void parameterChanged(_NT_algorithm* self, int paramIndex)</signature>
      <path>N/A</path>
      <note>Called when parameter value changes via NT interface</note>
    </interface>
  </interfaces>

  <tests>
    <standards>
This project follows desktop-first testing: develop and validate in nt_emu (VCV Rack) before deploying to hardware. Testing emphasizes load/unload cycling to detect memory leaks, audio passthrough verification with scope/analyzer, and visual confirmation in NT interface. Hardware builds are validation-only after desktop testing succeeds. No formal unit test framework - acceptance criteria validation through manual testing in nt_emu and hardware deployment.</standards>
    <locations>
      <location>plugins/nt_elements.dylib</location>
      <location>plugins/nt_elements.o</location>
      <note>Build artifacts tested in VCV Rack (desktop) and distingNT (hardware)</note>
    </locations>
    <ideas>
      <idea ac="1">Test that plugin implements construct(), destruct(), step() callbacks and loads without crashing</idea>
      <idea ac="2">Verify plugin loads in nt_emu, appears in module list, no VCV Rack console errors</idea>
      <idea ac="3">Connect audio source (VCV Scope, oscillator) to nt_emu input bus, verify output matches input bit-for-bit using scope/analyzer</idea>
      <idea ac="4">Check that OLED/interface displays "nt_elements" as algorithm name</idea>
      <idea ac="5">Load/unload plugin 10+ times in nt_emu, monitor VCV Rack memory usage (Activity Monitor on macOS), check for leaks</idea>
      <idea>Test both output modes: Replace (should overwrite bus) and Add (should mix with existing content)</idea>
      <idea>Verify correct bus indexing: parameter set to bus 1 should access busFrames[0..numFrames-1], bus 2 should access busFrames[numFrames..2*numFrames-1]</idea>
      <idea>Test with different buffer sizes by adjusting VCV Rack sample rate/buffer settings</idea>
      <idea>Build hardware target and verify .o file size is reasonable (100-200KB for minimal stub)</idea>
    </ideas>
  </tests>
</story-context>
