<story-context id="bmad/bmm/workflows/4-implementation/story-context/template" v="1.0">
  <metadata>
    <epicId>1</epicId>
    <storyId>1</storyId>
    <title>Development Environment Setup</title>
    <status>drafted</status>
    <generatedAt>2025-10-26</generatedAt>
    <generator>BMAD Story Context Workflow</generator>
    <sourceStoryPath>docs/stories/1-1-development-environment-setup.md</sourceStoryPath>
  </metadata>

  <story>
    <asA>a developer</asA>
    <iWant>a working ARM cross-compilation environment with disting NT tooling</iWant>
    <soThat>I can build and test plugins for the disting NT platform</soThat>
    <tasks>
- [ ] Install ARM toolchain (AC: #1)
  - [ ] Run `brew install arm-none-eabi-gcc` on macOS
  - [ ] Verify installation with `arm-none-eabi-g++ --version`
  - [ ] Document installed version
- [ ] Initialize Git repository and add distingNT API submodule (AC: #2)
  - [ ] Run `git init` if not already initialized
  - [ ] Add submodule: `git submodule add https://github.com/expertsleepersltd/distingNT_API.git`
  - [ ] Run `git submodule update --init --recursive`
  - [ ] Verify distingNT_API/include/distingNT.h exists
- [ ] Add Mutable Instruments Elements source submodule (AC: #2)
  - [ ] Add submodule: `git submodule add https://github.com/pichenettes/eurorack.git external/mutable-instruments`
  - [ ] Initialize submodule recursively
  - [ ] Verify external/mutable-instruments/elements/dsp/ directory structure exists
- [ ] Setup VCV Rack and nt_emu for desktop testing (AC: #3)
  - [ ] Download and install VCV Rack v2.x from https://vcvrack.com/
  - [ ] Launch VCV Rack and install nt_emu module from Library
  - [ ] Verify nt_emu module loads in VCV Rack patch
  - [ ] Document nt_emu version
- [ ] Create project structure and basic Makefile (AC: #4)
  - [ ] Create src/ directory for adapter code
  - [ ] Create plugins/ directory for build output
  - [ ] Create build/ directory for intermediate artifacts
  - [ ] Create Makefile with dual targets (hardware + test)
  - [ ] Add compiler flags: `-std=c++11 -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard -Os -fno-rtti -fno-exceptions`
- [ ] Implement minimal "hello world" plugin (AC: #4)
  - [ ] Create src/nt_elements.cpp with basic factory pattern
  - [ ] Implement construct() callback (returns algorithm instance)
  - [ ] Implement destruct() callback (cleanup)
  - [ ] Implement step() callback (silence output for now)
  - [ ] Add required NT_algorithm structure
- [ ] Build and test desktop version (AC: #4, #5)
  - [ ] Run `make test` to build .dylib/.so
  - [ ] Launch VCV Rack and add nt_emu module
  - [ ] Right-click nt_emu → Load plugin → Select plugins/nt_elements.dylib
  - [ ] Verify plugin loads without errors
  - [ ] Check VCV Rack log for any warnings
- [ ] Build hardware version (AC: #4)
  - [ ] Run `make hardware` to build .o file
  - [ ] Verify plugins/nt_elements.o exists
  - [ ] Check file size is reasonable (< 500KB initially)
  - [ ] Document build success
- [ ] Create README with build instructions (AC: #1-5)
  - [ ] Document prerequisites (ARM toolchain, VCV Rack)
  - [ ] Document build commands (make test, make hardware)
  - [ ] Document testing procedure (loading in nt_emu)
  - [ ] Add MIT license notice for Elements attribution
    </tasks>
  </story>

  <acceptanceCriteria>
1. ARM toolchain (arm-none-eabi-gcc) installed and verified
2. disting NT API repository cloned as git submodule
3. nt_emu VCV Rack module installed for desktop testing
4. Basic "hello world" plugin compiles for both desktop (.dylib/.so) and hardware (.o)
5. Test plugin loads successfully in nt_emu
  </acceptanceCriteria>

  <artifacts>
    <docs>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Project Initialization</section>
        <snippet>Foundation commands for initializing git repository, adding distingNT_API and mutable-instruments submodules, installing ARM toolchain via brew. Defines build system with dual targets (hardware/test) and establishes clean adapter layer pattern separating Elements source from NT adapter code.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Project Structure</section>
        <snippet>Directory layout showing distingNT_API/ and external/mutable-instruments/ as git submodules, src/ for adapter code, plugins/ for build output (.o/.dylib), and build/ for intermediate artifacts. Critical rule: never modify files in git submodules.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Build Pattern</section>
        <snippet>Makefile structure with explicit source lists, dual targets (hardware with arm-none-eabi-gcc, test with native compiler), compiler flags including -std=c++11 -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard -Os -fno-rtti -fno-exceptions. Includes paths for distingNT_API, Elements source, and stmlib stubs.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Testing Pattern</section>
        <snippet>Desktop-first development workflow: develop and iterate using nt_emu (make test → plugins/nt_elements.dylib), then validate on hardware (make hardware → plugins/nt_elements.o). Fast iteration with debugging on desktop, hardware deployment for performance validation.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Development Environment</section>
        <snippet>Prerequisites include macOS, ARM GCC toolchain (brew install arm-none-eabi-gcc), Git, Make, VCV Rack with nt_emu module. Setup commands cover toolchain installation, submodule initialization, and build verification steps.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Memory Allocation Pattern</section>
        <snippet>All memory must be allocated via NT_globals.getMemory() - never use malloc/new. Memory regions: DTC (ultra-fast, small ~64KB) for hot state, SRAM (fast, ~128KB) for audio buffers, DRAM (large, ~8MB) for reverb buffers.</snippet>
      </doc>
      <doc>
        <path>docs/PRD.md</path>
        <title>nt_elements Product Requirements Document</title>
        <section>Epic 1: Project Foundation & Core DSP Integration</section>
        <snippet>Timeline weeks 1-4, 8-10 stories. Establish development environment, build system, and integrate Elements DSP core with basic audio I/O. Key deliverables include working build pipeline with ARM toolchain and nt_emu testing.</snippet>
      </doc>
      <doc>
        <path>docs/PRD.md</path>
        <title>nt_elements Product Requirements Document</title>
        <section>Goals and Background Context</section>
        <snippet>Deliver production-ready nt_elements v1.0 within 6 weeks, achieve < 30% CPU usage, preserve 100% sound authenticity. Port Mutable Instruments Elements modal synthesis to disting NT platform, establishing repeatable MI porting methodology.</snippet>
      </doc>
      <doc>
        <path>docs/epics.md</path>
        <title>nt_elements Epic Breakdown</title>
        <section>Story 1.1: Development Environment Setup</section>
        <snippet>Acceptance criteria include ARM toolchain installation/verification, distingNT API as git submodule, nt_emu VCV Rack module installed, basic hello world plugin compiling for desktop and hardware, test plugin loads in nt_emu. No prerequisites - first story in epic.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Technology Stack Details</section>
        <snippet>C++11 with ARM GCC (arm-none-eabi-gcc), targeting ARM Cortex-M7 @ 480MHz (STM32H7). Hardware: disting NT with ~128KB SRAM, ~8MB DRAM, 48kHz sample rate. Development tools: VCV Rack v2.x with nt_emu module, GNU Make dual targets.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Implementation Patterns - Build Pattern</section>
        <snippet>Makefile must explicitly list all source files: src/nt_elements.cpp plus Elements DSP sources (part.cc, voice.cc, exciter.cc, resonator.cc, string.cc, tube.cc, multistage_envelope.cc, ominous_voice.cc). Include paths for distingNT_API, mutable-instruments, and stmlib_stubs.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Consistency Rules - Naming Conventions</section>
        <snippet>Follow Elements C++ style: PascalCase for classes, camelCase for functions, snake_case for variables, k-prefix PascalCase for constants. File naming uses snake_case with .h/.cpp extensions (adapter) or .cc (Elements source).</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>ADR-004: Desktop-First Testing Strategy</section>
        <snippet>Decision to develop on desktop using nt_emu, validate on hardware at milestones. Enables fast iteration with printf debugging, no hardware deploy cycle during development. Requires maintaining two build targets but matches embedded development best practices.</snippet>
      </doc>
    </docs>
    <code>
      <!-- No existing code - this is the first implementation story establishing the project structure -->
    </code>
    <dependencies>
      <system>
        <package name="arm-none-eabi-gcc" version="latest via brew">ARM GCC cross-compiler toolchain for Cortex-M7</package>
        <package name="git" version="system">Version control and submodule management</package>
        <package name="make" version="system">GNU Make build system (Xcode Command Line Tools)</package>
        <package name="VCV Rack" version="v2.x latest">Desktop modular synthesizer for nt_emu testing</package>
        <package name="nt_emu" version="latest from VCV Library">VCV Rack module emulating distingNT</package>
      </system>
      <external>
        <repository url="https://github.com/expertsleepersltd/distingNT_API.git">distingNT plugin API v9 - factory pattern, parameters, MIDI, 28-bus audio routing</repository>
        <repository url="https://github.com/pichenettes/eurorack.git">Mutable Instruments Elements source - MIT licensed modal synthesis DSP at external/mutable-instruments/elements/dsp/</repository>
      </external>
    </dependencies>
  </artifacts>

  <constraints>
- Build system must support dual targets: hardware (ARM .o) and test (native .dylib/.so)
- All external dependencies must be managed via git submodules (never modify submodule contents)
- Project structure must follow architecture.md: distingNT_API/, external/mutable-instruments/, src/, plugins/, build/
- Compiler flags must include: -std=c++11 -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard -Os -fno-rtti -fno-exceptions
- Memory allocation must use NT_globals.getMemory() exclusively - never malloc/new
- Desktop-first development workflow: iterate on nt_emu, validate on hardware at milestones
- MIT license compliance required with proper Elements attribution
- Minimal hello world plugin must implement NT_algorithm factory pattern (construct, destruct, step callbacks)
- Build artifacts: plugins/nt_elements.dylib (desktop) and plugins/nt_elements.o (hardware)
- Never modify files in git submodules - all customization in src/ adapter layer
  </constraints>

  <interfaces>
    <api>
      <name>distingNT Plugin Factory Pattern</name>
      <kind>C plugin API</kind>
      <signature>
extern "C" {
  _NT_algorithm* construct(float sampleRate, int numFrames);
  void destruct(_NT_algorithm* self);
  void step(_NT_algorithm* self, float* busFrames, int numFramesBy4);
}
      </signature>
      <path>distingNT_API/include/distingNT.h</path>
    </api>
    <api>
      <name>NT Memory Allocation</name>
      <kind>Memory management API</kind>
      <signature>
void* NT_globals.getMemory(size_t size, NT_memoryType type);
// Types: kNT_memDTC, kNT_memSRAM, kNT_memDRAM
      </signature>
      <path>distingNT_API/include/distingNT.h</path>
    </api>
    <api>
      <name>NT_algorithm Structure</name>
      <kind>Base plugin structure</kind>
      <signature>
struct _NT_algorithm {
  float v[kNumParameters];  // Parameter array
  // Plugin must extend this structure
};
      </signature>
      <path>distingNT_API/include/distingNT.h</path>
    </api>
  </interfaces>

  <tests>
    <standards>
This is the first story establishing the development environment. Testing standards for this story focus on build system verification and toolchain validation rather than code testing. Verify ARM toolchain installation (arm-none-eabi-g++ --version), successful compilation of minimal hello world plugin for both targets (make test, make hardware), plugin loading in nt_emu without errors, and git submodule integrity (distingNT_API/include/distingNT.h and external/mutable-instruments/elements/dsp/ structure exists). Desktop testing workflow uses VCV Rack with nt_emu module as the primary validation environment. Future stories will implement actual DSP functionality and require audio processing tests.
    </standards>
    <locations>
- VCV Rack application for loading and testing .dylib plugins via nt_emu module
- Terminal for build verification commands: make test, make hardware
- File system verification for directory structure: src/, plugins/, build/, distingNT_API/, external/mutable-instruments/
    </locations>
    <ideas>
      <idea ac="1">Verify ARM toolchain installation by running arm-none-eabi-g++ --version and confirming output shows ARM Embedded Toolchain version info</idea>
      <idea ac="2">Verify distingNT API submodule by checking distingNT_API/include/distingNT.h exists and contains factory pattern function declarations</idea>
      <idea ac="2">Verify Elements submodule by checking external/mutable-instruments/elements/dsp/part.h exists and contains Elements::Part class definition</idea>
      <idea ac="3">Verify nt_emu installation by launching VCV Rack, adding nt_emu module to patch, and confirming it loads without errors</idea>
      <idea ac="4">Test hardware build by running make hardware and verifying plugins/nt_elements.o exists with reasonable file size (< 500KB for minimal plugin)</idea>
      <idea ac="4">Test desktop build by running make test and verifying plugins/nt_elements.dylib (macOS) or .so (Linux) exists</idea>
      <idea ac="5">Test plugin loading by right-clicking nt_emu in VCV Rack, selecting Load plugin, choosing plugins/nt_elements.dylib, and verifying no error messages appear</idea>
      <idea ac="5">Verify VCV Rack log output (Help → Show Log) shows plugin loaded successfully without warnings or errors</idea>
      <idea ac="all">Create README documenting all setup steps and build commands as final validation that environment is properly configured</idea>
    </ideas>
  </tests>
</story-context>
