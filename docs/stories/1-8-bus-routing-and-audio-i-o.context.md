<story-context id="1-8-bus-routing-and-audio-i-o" v="1.0">
  <metadata>
    <epicId>1</epicId>
    <storyId>8</storyId>
    <title>Bus Routing and Audio I/O</title>
    <status>drafted</status>
    <generatedAt>2025-10-26</generatedAt>
    <generator>BMAD Story Context Workflow</generator>
    <sourceStoryPath>docs/stories/1-8-bus-routing-and-audio-i-o.md</sourceStoryPath>
  </metadata>

  <story>
    <asA>user</asA>
    <iWant>proper audio input/output through disting NT's bus system</iWant>
    <soThat>I can integrate nt_elements with other modules</soThat>
    <tasks>
      - Implement bus parameter definitions (AC: #1, #2)
        - Add kParamInputBus parameter (1-28)
        - Add kParamOutputBus parameter (1-28)
        - Add kParamOutputMode parameter (0=add, 1=replace)
        - Set default input bus (e.g., bus 1)
        - Set default output bus (e.g., bus 1)

      - Allocate audio buffer memory (AC: #1, #2, #5)
        - Allocate input_buffer in SRAM (512 floats = 2KB)
        - Allocate temp_output buffer in SRAM (512 floats = 2KB)
        - Use NT_globals.getMemory() for allocation
        - Check allocation success in construct()
        - Document buffer sizes and memory regions

      - Implement input bus routing (AC: #1, #3)
        - Calculate input bus offset: (input_bus - 1) * numFrames
        - Copy input from bus to input_buffer (handle 1-based indexing)
        - Support mono input (single bus)
        - Support stereo input (future: two buses, sum to mono)
        - Handle edge case: input bus out of range (clamp/default)

      - Implement Elements DSP processing call (AC: #3)
        - Call elements_part->Process(patch, input_buffer, temp_output, numFrames)
        - Pass correct buffer pointers (input/output)
        - Pass correct frame count (numFramesBy4 * 4)
        - Verify Elements processes mono input correctly
        - Handle Elements stereo output (left/right channels)

      - Implement output bus routing with add/replace modes (AC: #2, #4)
        - Calculate output bus offset: (output_bus - 1) * numFrames
        - Implement replace mode: memcpy(output_bus, temp_output, size)
        - Implement add mode: loop and add temp_output[i] to output_bus[i]
        - Handle stereo output (two buses for left/right)
        - Verify output mode parameter switches correctly

      - Test mono input excitation (AC: #3)
        - Route external audio (oscillator) to input bus in nt_emu
        - Trigger note-on to enable resonator
        - Verify external audio excites resonator (hear input-driven sound)
        - Test with different input signals (sine, noise, pulse)
        - Verify clean excitation without artifacts

      - Test output routing and modes (AC: #2, #4, #5)
        - Test replace mode: output should overwrite bus contents
        - Test add mode: output should sum with existing bus contents
        - Verify no clicks or pops at buffer boundaries
        - Test with different output buses (1, 5, 28)
        - Verify stereo output balance (left/right channels)

      - Validate bus boundary handling (AC: #5)
        - Test edge case: input bus = 28 (last bus)
        - Test edge case: output bus = 28
        - Verify no buffer overruns or memory corruption
        - Test rapid bus parameter changes (no glitches)
        - Listen for audio artifacts during bus switching

      - Desktop testing in nt_emu (AC: #1-5)
        - Load plugin in nt_emu
        - Patch audio source to input bus
        - Patch output bus to VCV Rack audio output
        - Test all acceptance criteria in desktop environment
        - Document any issues or limitations
    </tasks>
  </story>

  <acceptanceCriteria>
    1. Mono/stereo audio input supported
    2. Stereo output routes to NT buses correctly
    3. External audio can excite the resonator
    4. Output mode (add/replace) works correctly
    5. No audio artifacts at bus boundaries
  </acceptanceCriteria>

  <artifacts>
    <docs>
      <doc>
        <path>docs/PRD.md</path>
        <title>Product Requirements Document</title>
        <section>FR009: Mono/stereo audio input</section>
        <snippet>Support mono/stereo audio input with external audio routing to resonator. External audio can excite the Elements resonator for unique timbres.</snippet>
      </doc>
      <doc>
        <path>docs/PRD.md</path>
        <title>Product Requirements Document</title>
        <section>FR010: Stereo output and bus routing</section>
        <snippet>Output stereo audio integrated with NT's 28-bus routing system. Supports add/replace modes for flexible signal mixing.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Architecture Decision Document</title>
        <section>Audio Buffer Access Pattern</section>
        <snippet>NT bus indexing is 1-based (user-facing: 1-28), array access is 0-based (subtract 1 for pointer arithmetic). busFrames contains all 28 buses interleaved, calculate offset as (bus_number - 1) * numFrames.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Architecture Decision Document</title>
        <section>Audio Routing - Buffer Strategy</section>
        <snippet>Use temporary buffers for I/O (Option A). Allocate input_buffer and temp_output in SRAM (512 floats each = 4KB total overhead). Elements DSP expects contiguous buffers, NT uses interleaved bus system.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Architecture Decision Document</title>
        <section>Memory Allocation Pattern</section>
        <snippet>All memory must be allocated via NT_globals.getMemory(). Never use malloc/new. Use kNT_memSRAM for audio buffers, kNT_memDTC for hot state, kNT_memDRAM for large buffers like reverb.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Architecture Decision Document</title>
        <section>Output Modes: Add vs Replace</section>
        <snippet>Replace mode (mode=1) overwrites bus (single memcpy), use when sole source. Add mode (mode=0) sums with existing contents (sample loop), use for mixing. User selects via parameter.</snippet>
      </doc>
      <doc>
        <path>docs/epics.md</path>
        <title>Epic Breakdown</title>
        <section>Story 1.8: Bus Routing and Audio I/O</section>
        <snippet>Implement proper audio input/output through disting NT's bus system. Supports external audio excitation of resonator, add/replace modes, and stereo routing to NT buses.</snippet>
      </doc>
      <doc>
        <path>docs/stories/1-8-bus-routing-and-audio-i-o.md</path>
        <title>Story 1.8</title>
        <section>Dev Notes - disting NT Bus System Architecture</section>
        <snippet>28-bus system with user-facing 1-28, array indexing 0-27. Buffer layout is interleaved: all frames for bus0, then bus1, etc. Must multiply numFramesBy4 by 4 to get actual frame count.</snippet>
      </doc>
      <doc>
        <path>docs/stories/1-8-bus-routing-and-audio-i-o.md</path>
        <title>Story 1.8</title>
        <section>Dev Notes - Temporary Buffer Strategy</section>
        <snippet>Elements expects contiguous buffers, NT uses interleaved buses. Allocate input_buffer and temp_output (512 floats each) in SRAM. Processing flow: copy from NT bus → Elements Process() → copy to NT bus with mode.</snippet>
      </doc>
      <doc>
        <path>docs/stories/1-8-bus-routing-and-audio-i-o.md</path>
        <title>Story 1.8</title>
        <section>Dev Notes - External Audio Excitation</section>
        <snippet>Elements has internal exciters (bow/blow/strike) plus external audio input. External audio excites the resonator creating unique timbres. Use cases: noise for wind-like textures, oscillator for harmonic excitation.</snippet>
      </doc>
      <doc>
        <path>docs/stories/1-8-bus-routing-and-audio-i-o.md</path>
        <title>Story 1.8</title>
        <section>Dev Notes - Stereo Handling</section>
        <snippet>Elements produces stereo output (left/right). For Story 1.8 simplicity, use mono output (single channel or sum to mono). True stereo routing to two buses deferred to Epic 2.</snippet>
      </doc>
      <doc>
        <path>docs/stories/1-8-bus-routing-and-audio-i-o.md</path>
        <title>Story 1.8</title>
        <section>Dev Notes - Buffer Size and Frame Count</section>
        <snippet>NT passes numFramesBy4 (frames/4), must multiply by 4 for actual count. Typical: numFramesBy4=32 → 128 frames, numFramesBy4=64 → 256 frames. Allocate 512 samples as safe upper bound.</snippet>
      </doc>
      <doc>
        <path>docs/stories/1-8-bus-routing-and-audio-i-o.md</path>
        <title>Story 1.8</title>
        <section>Dev Notes - Error Handling</section>
        <snippet>Clamp bus indices to valid range [0, 27]. Check memory allocation success in construct(). If allocation fails, output silence and return gracefully.</snippet>
      </doc>
    </docs>

    <code>
      <artifact>
        <path>src/nt_elements.cpp</path>
        <kind>main plugin implementation</kind>
        <symbol>step() callback</symbol>
        <lines>N/A - file not yet created</lines>
        <reason>Main implementation file where bus routing logic will be added. The step() callback is where audio processing happens, including bus input/output routing.</reason>
      </artifact>
      <artifact>
        <path>src/parameter_adapter.h</path>
        <kind>parameter definitions</kind>
        <symbol>kParamInputBus, kParamOutputBus, kParamOutputMode</symbol>
        <lines>N/A - file not yet created</lines>
        <reason>Parameter definitions file where bus parameters (input bus, output bus, output mode) need to be added per this story's requirements.</reason>
      </artifact>
    </code>

    <dependencies>
      <framework name="disting NT API" version="v9">
        NT_globals.getMemory() for memory allocation
        step() callback signature with busFrames parameter
        28-bus audio routing system
      </framework>
      <framework name="Elements DSP" version="submodule">
        Part::Process() method for audio processing
        Patch structure for parameter control
        Stereo output handling (left/right channels)
      </framework>
      <toolchain name="ARM GCC" version="arm-none-eabi-gcc">
        Cross-compiler for ARM Cortex-M7 target
        Optimization flags: -Os -mcpu=cortex-m7 -mfpu=fpv5-d16
      </toolchain>
      <tool name="VCV Rack + nt_emu" version="v2.x">
        Desktop testing environment
        Loads .dylib plugins for rapid iteration
      </tool>
    </dependencies>
  </artifacts>

  <constraints>
    - All memory allocation must use NT_globals.getMemory(), never malloc/new
    - Bus parameters are 1-based (1-28) for user display, 0-based (0-27) for array indexing
    - Must multiply numFramesBy4 by 4 to get actual frame count
    - Temporary buffers required: Elements expects contiguous buffers, NT uses interleaved bus system
    - Allocate audio buffers in SRAM (kNT_memSRAM) for fast access
    - Maximum buffer size: 512 floats (2KB each) to handle all expected frame counts
    - Output mode: replace (1) uses memcpy, add (0) requires sample-by-sample loop
    - Clamp bus indices to valid range [0, 27] to prevent buffer overruns
    - No audio artifacts at buffer boundaries (clicks, pops, glitches)
    - Defensive programming: check allocation success, handle edge cases gracefully
    - Desktop-first testing: validate in nt_emu before hardware deployment
    - Code organization: adapter logic in src/, never modify Elements submodule
    - Prerequisites: Stories 1.1-1.7 must be complete (build system, Elements integration, parameter adapter)
  </constraints>

  <interfaces>
    <interface>
      <name>NT step() callback</name>
      <kind>C function signature</kind>
      <signature>void step(_NT_algorithm* self, float* busFrames, int numFramesBy4)</signature>
      <path>distingNT_API/include/distingNT.h</path>
      <description>Main audio processing callback. busFrames is interleaved array of all 28 buses. Must multiply numFramesBy4 by 4 for actual frame count.</description>
    </interface>
    <interface>
      <name>NT_globals.getMemory()</name>
      <kind>C function</kind>
      <signature>void* getMemory(size_t bytes, NT_memoryType type)</signature>
      <path>distingNT_API/include/distingNT.h</path>
      <description>Allocates memory from specified region (kNT_memSRAM, kNT_memDTC, kNT_memDRAM). Returns pointer or NULL on failure. Must be called during construct().</description>
    </interface>
    <interface>
      <name>Elements Part::Process()</name>
      <kind>C++ method</kind>
      <signature>void Process(const Patch&amp; patch, const float* in, float* out, size_t size)</signature>
      <path>external/mutable-instruments/elements/dsp/part.h</path>
      <description>Main Elements synthesis processing. Takes mono input, produces stereo output. Input and output must be contiguous buffers. Size is actual frame count.</description>
    </interface>
    <interface>
      <name>Bus offset calculation</name>
      <kind>calculation pattern</kind>
      <signature>const float* input_bus_ptr = busFrames + (input_bus - 1) * numFrames</signature>
      <path>docs/architecture.md - Audio Buffer Access Pattern</path>
      <description>Calculate pointer to specific bus in interleaved busFrames array. Subtract 1 from 1-based bus number, multiply by frame count.</description>
    </interface>
  </interfaces>

  <tests>
    <standards>
      This is an embedded C++ project targeting disting NT hardware. Testing follows desktop-first strategy using VCV Rack's nt_emu module. Manual testing validates audio routing, bus selection, output modes, and external excitation. Performance testing on hardware verifies CPU usage and stability. No automated unit tests in Epic 1; focus on functional validation via desktop emulator and hardware deployment.
    </standards>

    <locations>
      - Desktop testing: VCV Rack + nt_emu module (loads .dylib plugins)
      - Hardware testing: disting NT Eurorack module (loads .o plugins from SD card)
      - Build targets: make test (desktop), make hardware (ARM)
      - Test patches: VCV Rack patches routing audio through nt_elements
    </locations>

    <ideas>
      <test criterion="AC1" description="Mono/stereo audio input supported">
        - Load nt_elements in nt_emu, connect mono oscillator to input bus
        - Verify audio passes through to Elements DSP (monitor output)
        - Test multiple input buses (1, 14, 28) to verify routing
        - Verify input_buffer allocation successful (check construct() return)
      </test>

      <test criterion="AC2" description="Stereo output routes to NT buses correctly">
        - Connect nt_elements output bus to VCV Rack audio output/scope
        - Generate test tone, verify output appears on correct bus
        - Test multiple output buses (1, 5, 28) to verify routing
        - For mono simplification: verify single channel output (left or summed)
      </test>

      <test criterion="AC3" description="External audio can excite the resonator">
        - Route external oscillator to input bus
        - Send MIDI note-on to enable resonator
        - Listen for input signal modulating resonator (should hear oscillator timbre in resonance)
        - Test with different input signals: sine wave (pure tone), noise (wind texture), pulse (percussive)
        - Verify clean excitation without distortion or artifacts
      </test>

      <test criterion="AC4" description="Output mode (add/replace) works correctly">
        - Set output mode to replace (1), verify output overwrites bus contents (exclusive)
        - Set output mode to add (0), mix with another source on same bus, verify summing (additive)
        - Toggle mode parameter during playback, verify smooth transition (no glitches)
        - Verify replace mode uses efficient memcpy, add mode uses sample loop
      </test>

      <test criterion="AC5" description="No audio artifacts at bus boundaries">
        - Listen for clicks, pops, or glitches during normal operation
        - Rapidly change input/output bus parameters (stress test)
        - Test edge cases: bus 1 (first), bus 28 (last), bus out of range (should clamp)
        - Monitor for buffer overruns or memory corruption (check allocation, no crashes)
        - Test buffer boundary transitions with different numFramesBy4 values (32, 64, 128)
      </test>

      <test criterion="Integration" description="Desktop testing in nt_emu">
        - Build with make test, load plugins/nt_elements.dylib in nt_emu
        - Create VCV Rack patch: oscillator → nt_elements input bus → output bus → audio out
        - Verify all acceptance criteria in desktop environment
        - Document observed CPU usage (estimate), audio quality, and any issues
        - Prepare for hardware validation in Story 1.9/1.10
      </test>
    </ideas>
  </tests>
</story-context>
