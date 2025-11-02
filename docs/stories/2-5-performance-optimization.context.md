<story-context id="bmad/bmm/workflows/4-implementation/story-context/template" v="1.0">
  <metadata>
    <epicId>2</epicId>
    <storyId>5</storyId>
    <title>Performance Optimization</title>
    <status>drafted</status>
    <generatedAt>2025-10-26</generatedAt>
    <generator>BMAD Story Context Workflow</generator>
    <sourceStoryPath>docs/stories/2-5-performance-optimization.md</sourceStoryPath>
  </metadata>

  <story>
    <asA>developer</asA>
    <iWant>CPU usage optimized to meet < 30% target</iWant>
    <soThat>users can chain nt_elements with other NT algorithms</soThat>
    <tasks>
- [ ] Measure baseline CPU usage (AC: #1)
  - [ ] Deploy current build to disting NT hardware
  - [ ] Measure idle CPU (no synthesis)
  - [ ] Measure synthesis CPU (typical patch, note held)
  - [ ] Measure peak CPU (complex patch, multiple parameters)
  - [ ] Document baseline measurements

- [ ] Profile CPU usage by component (AC: #1, #2)
  - [ ] Estimate Elements Part processing cost
  - [ ] Estimate reverb processing cost
  - [ ] Estimate parameter adapter overhead
  - [ ] Estimate bus routing overhead
  - [ ] Identify hot paths (highest CPU components)

- [ ] Enable compiler optimizations (AC: #5)
  - [ ] Change Makefile optimization flag from -Os to -O3
  - [ ] Add -ffast-math flag (fast floating-point)
  - [ ] Add -funroll-loops flag (loop unrolling)
  - [ ] Rebuild and measure CPU improvement
  - [ ] Document optimization flags used

- [ ] Optimize reverb processing (AC: #4)
  - [ ] Measure reverb-only CPU cost
  - [ ] Implement reverb bypass when Amount = 0%
  - [ ] Test reduced reverb quality settings
  - [ ] Adjust reverb buffer size if needed
  - [ ] Measure CPU savings from reverb optimization

- [ ] Optimize parameter adapter (AC: #2)
  - [ ] Review parameterChanged() efficiency
  - [ ] Minimize parameter conversion overhead
  - [ ] Cache computed values if beneficial
  - [ ] Verify no unnecessary processing in hot paths
  - [ ] Measure adapter overhead (should be < 1%)

- [ ] Optimize bus routing (AC: #2)
  - [ ] Review step() callback efficiency
  - [ ] Minimize memcpy operations
  - [ ] Consider DMA or optimized copy routines
  - [ ] Verify buffer alignment for SIMD
  - [ ] Measure routing overhead

- [ ] Test 32kHz mode (if implemented) (AC: #3)
  - [ ] Switch to 32kHz mode
  - [ ] Measure CPU usage in 32kHz mode
  - [ ] Compare to 48kHz mode
  - [ ] Verify ~15-20% CPU savings
  - [ ] Document 32kHz mode performance

- [ ] Measure optimized CPU usage (AC: #3, #6)
  - [ ] Deploy optimized build to hardware
  - [ ] Measure idle CPU
  - [ ] Measure synthesis CPU (typical patch)
  - [ ] Measure peak CPU (complex patch)
  - [ ] Verify < 30% target met
  - [ ] Compare to baseline measurements

- [ ] Test different configurations (AC: #6)
  - [ ] 48kHz mode, reverb on: measure CPU
  - [ ] 48kHz mode, reverb off: measure CPU
  - [ ] 32kHz mode, reverb on: measure CPU (if applicable)
  - [ ] 32kHz mode, reverb off: measure CPU (if applicable)
  - [ ] Document CPU for each configuration

- [ ] Validate audio quality after optimization (AC: #4)
  - [ ] Compare audio output before/after optimization
  - [ ] Verify no sonic degradation
  - [ ] Test reverb bypass (should be silent when bypassed)
  - [ ] Test -ffast-math impact (should be negligible)
  - [ ] Document any quality trade-offs

- [ ] Create performance documentation (AC: #6)
  - [ ] Document baseline vs optimized CPU usage
  - [ ] Document optimization techniques applied
  - [ ] Document configuration recommendations
  - [ ] Note performance headroom remaining
  - [ ] Provide user guidance on CPU management

- [ ] Stress test optimized build (AC: #3)
  - [ ] Run synthesis for 1+ hour
  - [ ] Monitor for CPU spikes or instability
  - [ ] Test with rapid parameter changes
  - [ ] Test with MIDI note bursts
  - [ ] Verify sustained < 30% CPU usage
    </tasks>
  </story>

  <acceptanceCriteria>
1. CPU profiling completed on hardware NT
2. Hot paths identified and optimized
3. CPU usage measured < 30% in both 32kHz and 48kHz modes
4. Reverb quality adjusted if needed to meet target
5. Compiler optimizations enabled (-O3, -ffast-math)
6. Performance documented for different configurations
  </acceptanceCriteria>

  <artifacts>
    <docs>
      <doc>
        <path>docs/PRD.md</path>
        <title>Product Requirements Document</title>
        <section>NFR001: Performance</section>
        <snippet>CPU usage must remain below 30% under typical operating conditions to allow algorithm chaining. Target 20-30% with all features active. No audio dropouts or glitches during sustained operation (validated via 1-hour stress test).</snippet>
      </doc>

      <doc>
        <path>docs/PRD.md</path>
        <title>Product Requirements Document</title>
        <section>Goals and Background Context</section>
        <snippet>Achieve < 30% CPU usage - Maintain performance headroom for algorithm chaining on disting NT. disting NT's 3-5x performance advantage (480MHz ARM Cortex-M7 vs Elements' 168MHz M4) provides margin for optimization.</snippet>
      </doc>

      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Performance Considerations</section>
        <snippet>Target: < 30% CPU usage @ 48kHz. Expected Breakdown: Elements DSP core ~15-20% (resonator, exciters), Reverb ~5-10% (bypassable), Sample rate conversion ~2-3% (if 48kHz mode), Adapter overhead < 1%. Optimization Strategies: 32kHz mode saves ~15-20% vs 48kHz, Reverb bypass when Amount = 0, Compiler optimizations -Os for size (could try -O3), Memory access in DTC for hot paths reduces latency.</snippet>
      </doc>

      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>ADR-005: Make Reverb Optional/Bypassable</section>
        <snippet>Include reverb but make it bypassable (skip processing when Amount = 0). Preserves Elements character while allowing users to choose built-in reverb OR route to external effects. Bypass saves ~5-10% CPU when unused.</snippet>
      </doc>

      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>ADR-002: Runtime Sample Rate Mode Toggle</section>
        <snippet>Runtime parameter toggle between 32kHz and 48kHz modes. User can A/B test modes without reloading plugin. Mode switch happens at Init() (not per-sample overhead). 32kHz mode provides ~20-30% CPU savings vs 48kHz.</snippet>
      </doc>

      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Memory Allocation Pattern</section>
        <snippet>All memory must be allocated via NT_globals.getMemory(). DTC allocation (ultra-fast, small) for hot state. SRAM allocation (fast, medium-sized buffers) for processing. DRAM allocation (large buffers, slower) for reverb. Never use malloc/new - all allocations must go through NT memory manager.</snippet>
      </doc>

      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Technology Stack Details - Hardware Platform</section>
        <snippet>disting NT - Expert Sleepers Eurorack module. CPU: ARM Cortex-M7 @ 480MHz (STM32H7 series). RAM: ~128KB SRAM, ~8MB DRAM, ~64KB DTC, ~64KB ITC. Audio: 48kHz sample rate (primary), 32kHz (compatibility mode).</snippet>
      </doc>

      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Compiler Flags</section>
        <snippet>C++11 with ARM GCC (arm-none-eabi-gcc). Compiler Flags: -std=c++11 -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard -Os -fno-rtti -fno-exceptions. Current optimization -Os (size), could enable -O3 (speed), -ffast-math (fast floating-point), -funroll-loops for performance.</snippet>
      </doc>

      <doc>
        <path>docs/epics.md</path>
        <title>Epic Breakdown</title>
        <section>Story 2.5: Performance Optimization</section>
        <snippet>As a developer, I want CPU usage optimized to meet < 30% target, So that users can chain nt_elements with other NT algorithms. Prerequisites: Story 1.10 (hardware validation). Includes CPU profiling, hot path optimization, compiler optimization flags, reverb bypass implementation, and performance documentation.</snippet>
      </doc>

      <doc>
        <path>docs/stories/2-5-performance-optimization.md</path>
        <title>Story 2.5: Performance Optimization</title>
        <section>Dev Notes - Performance Target</section>
        <snippet>Goal: < 30% CPU Usage. Why 30%: Allows chaining with other NT algorithms (70% headroom), Typical NT algorithm targets 20-40% CPU. Measurement: Use disting NT built-in CPU display. Expected Breakdown (Optimized): Elements DSP 15-20%, Reverb 5-8% (or 0% if bypassed), Adapter/routing < 2%, Total: 20-30%.</snippet>
      </doc>

      <doc>
        <path>docs/stories/2-5-performance-optimization.md</path>
        <title>Story 2.5: Performance Optimization</title>
        <section>Dev Notes - Compiler Optimizations</section>
        <snippet>Current: -Os (optimize for size). Optimized: -O3 (aggressive speed), -ffast-math (fast floating-point, IEEE non-compliant), -funroll-loops (unroll loops). Expected CPU Savings: -O3 vs -Os 10-20% faster, -ffast-math 5-10% faster, -funroll-loops 0-5% faster. Trade-offs: Larger binary size (acceptable), -ffast-math non-IEEE compliant (acceptable for audio DSP).</snippet>
      </doc>

      <doc>
        <path>docs/stories/2-5-performance-optimization.md</path>
        <title>Story 2.5: Performance Optimization</title>
        <section>Dev Notes - Reverb Optimization</section>
        <snippet>Reverb is typically the most expensive component (~30-40% of total DSP). Optimization: Bypass when not used - skip processing when reverb_amount > 0.01f. Expected Savings: 5-10% CPU when reverb bypassed. Fallback: Reduce reverb quality if target not met (shorten buffer, reduce diffusion stages, lower sample rate).</snippet>
      </doc>

      <doc>
        <path>docs/stories/2-5-performance-optimization.md</path>
        <title>Story 2.5: Performance Optimization</title>
        <section>Dev Notes - Profiling Hot Paths</section>
        <snippet>Hot Path Identification: 1) Elements Part::Process() likely 80-90% of CPU time (internal to Elements DSP, can't optimize without modifying submodule), 2) Audio Buffer Copies (memcpy for bus routing, should be < 1% CPU), 3) Parameter Conversion (parameterChanged() processing, should be negligible), 4) Display Updates (OLED rendering, should be < 2% CPU). Profiling Tools: CPU percentage display, timing code sections with cycle counters, binary search (disable components and measure CPU impact).</snippet>
      </doc>

      <doc>
        <path>docs/stories/2-5-performance-optimization.md</path>
        <title>Story 2.5: Performance Optimization</title>
        <section>Dev Notes - 32kHz Mode Performance</section>
        <snippet>If 32kHz Mode Implemented: ~33% fewer samples to process (32k vs 48k), Proportional CPU reduction ~20-30% savings, Trade-off: Sample rate conversion overhead. User Guidance: "Use 32kHz mode for lower CPU usage when maximum NT compatibility is not required."</snippet>
      </doc>

      <doc>
        <path>docs/stories/2-5-performance-optimization.md</path>
        <title>Story 2.5: Performance Optimization</title>
        <section>Dev Notes - Performance Documentation</section>
        <snippet>Documentation Format: Hardware Platform, Baseline Performance (Before Optimization), Optimization Techniques Applied, Optimized Performance (with checkmarks for < 30% target met), CPU Savings breakdown, Recommendations for users, Performance Headroom (typical usage shows CPU % with headroom for other algorithms).</snippet>
      </doc>
    </docs>

    <code>
      <file>
        <path>N/A - No source code exists yet</path>
        <kind>N/A</kind>
        <symbol>N/A</symbol>
        <lines>N/A</lines>
        <reason>Project is in planning phase. Story 1.1 (Development Environment Setup) has not been completed yet. The src/ directory does not exist. This performance optimization story depends on previous stories 1.1-1.10 being completed first.</reason>
      </file>
    </code>

    <dependencies>
      <embedded>
        <item>ARM GCC Toolchain (arm-none-eabi-gcc) - Cross-compiler for ARM Cortex-M7</item>
        <item>distingNT API - Expert Sleepers plugin API framework (Git submodule: https://github.com/expertsleepersltd/distingNT_API)</item>
        <item>Mutable Instruments Elements DSP - Modal synthesis engine source code (Git submodule: https://github.com/pichenettes/eurorack)</item>
        <item>VCV Rack + nt_emu module - Desktop testing environment for plugin development</item>
      </embedded>

      <build>
        <item>GNU Make - Build system</item>
        <item>Compiler flags (current): -std=c++11 -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard -Os -fno-rtti -fno-exceptions</item>
        <item>Compiler flags (optimized target): -O3, -ffast-math, -funroll-loops</item>
      </build>

      <platform>
        <item>disting NT Hardware - ARM Cortex-M7 @ 480MHz (STM32H7 series)</item>
        <item>Memory Regions - DTC (~64KB ultra-fast), SRAM (~128KB fast), DRAM (~8MB slower)</item>
      </platform>
    </dependencies>
  </artifacts>

  <constraints>
    <constraint>Must not modify Elements DSP source code in external/mutable-instruments/ submodule - all optimization must be in adapter layer or build configuration</constraint>
    <constraint>All memory allocation must use NT_globals.getMemory() - never use malloc/new</constraint>
    <constraint>Memory layout must follow architecture: DTC for hot state (Patch, parameters), SRAM for processing buffers, DRAM for reverb buffer</constraint>
    <constraint>CPU target is < 30% to allow algorithm chaining - this is a hard requirement per NFR001</constraint>
    <constraint>Audio quality must not degrade - any optimization that affects sound must be validated (AC #4)</constraint>
    <constraint>Reverb bypass must skip processing entirely when Amount = 0%, not just mute output</constraint>
    <constraint>Performance must be measured on actual disting NT hardware using built-in CPU display - desktop nt_emu estimates are insufficient for validation</constraint>
    <constraint>Compiler optimization flags must be tested for audio quality impact, particularly -ffast-math which is IEEE non-compliant</constraint>
    <constraint>Both 32kHz and 48kHz modes must meet the < 30% CPU target</constraint>
    <constraint>Performance documentation must include specific measurements for different configurations (sample rate modes, reverb on/off)</constraint>
    <constraint>Stress testing must verify sustained < 30% CPU over 1+ hour operation with no spikes or instability</constraint>
    <constraint>No exceptions or RTTI allowed (compiler flags: -fno-exceptions -fno-rtti)</constraint>
  </constraints>

  <interfaces>
    <interface>
      <name>NT_globals.getMemory()</name>
      <kind>Memory allocation API</kind>
      <signature>void* NT_globals.getMemory(size_t size, kNT_memType type)</signature>
      <path>distingNT_API/include/distingNT.h</path>
      <notes>Types: kNT_memDTC (ultra-fast), kNT_memSRAM (fast), kNT_memDRAM (large/slower). Must be used for all allocations.</notes>
    </interface>

    <interface>
      <name>step() callback</name>
      <kind>Audio processing callback</kind>
      <signature>void step(_NT_algorithm* self, float* busFrames, int numFramesBy4)</signature>
      <path>distingNT_API/include/distingNT.h</path>
      <notes>Called for each audio block. numFramesBy4 must be multiplied by 4 to get actual frame count. Bus indexing is 1-based. This is the primary hot path for CPU optimization.</notes>
    </interface>

    <interface>
      <name>parameterChanged() callback</name>
      <kind>Parameter update callback</kind>
      <signature>void parameterChanged(_NT_algorithm* self, int p)</signature>
      <path>distingNT_API/include/distingNT.h</path>
      <notes>Called when parameter p changes. Must be efficient - no expensive operations. Convert NT parameters (0-100%) to Elements Patch structure (0.0-1.0).</notes>
    </interface>

    <interface>
      <name>Elements::Part::Process()</name>
      <kind>DSP processing function</kind>
      <signature>void Part::Process(const Patch& patch, const float* in, float* out, int numFrames)</signature>
      <path>external/mutable-instruments/elements/dsp/part.h</path>
      <notes>Main Elements synthesis processing. Likely 80-90% of CPU time. Cannot be modified (read-only submodule). Optimization must focus on surrounding code and build flags.</notes>
    </interface>

    <interface>
      <name>Elements::Part::Init()</name>
      <kind>DSP initialization function</kind>
      <signature>void Part::Init(float sample_rate)</signature>
      <path>external/mutable-instruments/elements/dsp/part.h</path>
      <notes>Initializes Elements DSP at specified sample rate (32000 or 48000). Recalculates filter coefficients. Called during plugin construction and mode switching.</notes>
    </interface>

    <interface>
      <name>Reverb Bypass Logic</name>
      <kind>Conditional processing pattern</kind>
      <signature>if (reverb_amount > 0.01f) { ProcessReverb(...); }</signature>
      <path>To be implemented in src/nt_elements.cpp step() callback</path>
      <notes>Skip reverb processing entirely when Amount = 0%. Expected CPU savings: 5-10%. Must verify reverb buffer is still allocated but processing is skipped.</notes>
    </interface>

    <interface>
      <name>disting NT CPU Display</name>
      <kind>Hardware performance measurement</kind>
      <signature>N/A - Visual display on hardware</signature>
      <path>disting NT hardware main page</path>
      <notes>Built-in CPU percentage display on NT main screen. Primary method for measuring CPU usage. Most accurate measurement tool available for validation.</notes>
    </interface>
  </interfaces>

  <tests>
    <standards>
This is an embedded C++ audio DSP project without a traditional testing framework. Testing follows a hardware-centric approach:

**Desktop Testing (Development):**
- Build for nt_emu (VCV Rack module) using native compiler (.dylib/.so)
- Load plugin in VCV Rack for functional validation
- Iterate quickly with printf debugging
- Verify audio output, parameter changes, MIDI handling
- Performance estimates only (not accurate for CPU measurement)

**Hardware Testing (Validation):**
- Build for ARM hardware using arm-none-eabi-gcc (.o file)
- Deploy to disting NT SD card
- Test on actual Eurorack module
- Measure CPU usage using NT's built-in CPU display
- Validate audio quality, stability, performance targets
- Perform stress testing (1+ hour operation, rapid parameter changes, MIDI bursts)

**Performance Testing (This Story's Focus):**
- Measure baseline CPU before optimization
- Profile by component (Elements, reverb, adapter, routing)
- Apply optimizations incrementally
- Measure CPU after each optimization
- Validate audio quality hasn't degraded
- Test different configurations (32kHz/48kHz, reverb on/off)
- Document all measurements
- Verify < 30% CPU target met

**Quality Assurance:**
- No exceptions or RTTI (embedded constraints)
- Defensive coding: bounds checking, clamping, null checks
- No malloc/new - only NT_globals.getMemory()
- Audio fallback: output silence on error, don't crash
- Memory leak detection: load/unload cycles
    </standards>

    <locations>
      <location>Hardware: disting NT Eurorack module (primary validation environment)</location>
      <location>Desktop: VCV Rack + nt_emu module (development iteration)</location>
      <location>Build outputs: plugins/nt_elements.o (hardware), plugins/nt_elements.dylib (desktop)</location>
      <location>No automated test suite - manual validation approach for embedded audio DSP</location>
    </locations>

    <ideas>
      <idea ac="1">
        <test>Baseline CPU Measurement Suite</test>
        <description>Deploy current build to hardware. Measure and document: idle CPU (no synthesis), typical patch CPU (single note held), peak CPU (complex patch with rapid parameter changes). Create baseline performance profile for comparison after optimization.</description>
      </idea>

      <idea ac="1,2">
        <test>Component CPU Profiling</test>
        <description>Use binary search approach to identify hot paths. Measure: Elements Part::Process() alone, reverb processing alone, parameter adapter overhead, bus routing overhead. Estimate CPU percentage for each component. Identify optimization targets (likely reverb as highest cost).</description>
      </idea>

      <idea ac="5">
        <test>Compiler Optimization Validation</test>
        <description>Change Makefile flags from -Os to -O3, add -ffast-math and -funroll-loops. Rebuild and deploy. Measure CPU improvement. Test audio quality - verify -ffast-math hasn't introduced sonic artifacts. Document optimization flags used and CPU reduction achieved.</description>
      </idea>

      <idea ac="4">
        <test>Reverb Bypass Implementation Test</test>
        <description>Implement conditional reverb processing: if (reverb_amount > 0.01f) { ProcessReverb(); }. Measure CPU with reverb bypassed vs active. Verify expected 5-10% CPU savings. Test audio: reverb should be silent when bypassed, no glitches when toggling.</description>
      </idea>

      <idea ac="2">
        <test>Adapter Layer Efficiency Audit</test>
        <description>Review parameterChanged() implementation for expensive operations. Profile parameter conversion overhead (should be < 1% CPU). Look for unnecessary processing, caching opportunities, hot paths. Verify adapter is not causing performance bottlenecks.</description>
      </idea>

      <idea ac="2">
        <test>Bus Routing Optimization Test</test>
        <description>Review step() callback for memcpy efficiency. Test buffer alignment for SIMD. Consider DMA or optimized copy routines. Measure routing overhead (should be < 1% CPU). Verify no unnecessary buffer copies.</description>
      </idea>

      <idea ac="3">
        <test>Sample Rate Mode Performance Comparison</test>
        <description>Test both 32kHz and 48kHz modes on hardware. Measure CPU in each mode. Verify 32kHz mode saves ~15-20% CPU (fewer samples to process). Document trade-offs: 32kHz = lower CPU / authentic, 48kHz = better NT integration. Both modes must meet < 30% target.</description>
      </idea>

      <idea ac="3,6">
        <test>Configuration Matrix Testing</test>
        <description>Measure CPU for all combinations: 48kHz/reverb on, 48kHz/reverb off, 32kHz/reverb on (if applicable), 32kHz/reverb off (if applicable). Document CPU percentage for each configuration. Identify recommended configuration for users. Verify all configurations meet < 30% target.</description>
      </idea>

      <idea ac="4">
        <test>Audio Quality Regression Test</test>
        <description>Record audio before and after optimization. Compare waveforms/spectrograms. Verify no sonic degradation from -ffast-math or other optimizations. Test reverb bypass is truly silent. A/B test all exciter modes and resonator settings. Document any quality trade-offs (should be none).</description>
      </idea>

      <idea ac="3">
        <test>Sustained Load Stress Test</test>
        <description>Run synthesis continuously for 1+ hours on hardware. Monitor CPU display for spikes or instability. Test with: continuous note, rapid parameter changes, MIDI note bursts. Verify CPU stays < 30% sustained. Check for memory leaks or thermal throttling. Document any edge cases.</description>
      </idea>

      <idea ac="6">
        <test>Performance Documentation Validation</test>
        <description>Create performance report with: baseline vs optimized CPU measurements, optimization techniques applied (compiler flags, reverb bypass, etc.), configuration recommendations, performance headroom remaining. Provide user guidance on CPU management. Review documentation completeness against AC #6.</description>
      </idea>

      <idea ac="3">
        <test>Target Validation Test</test>
        <description>Final validation: Deploy optimized build to hardware. Test typical usage scenario (single note, moderate parameter changes). Measure CPU on NT display. Verify < 30% CPU met. If not met, identify remaining bottlenecks and apply additional optimizations (reduce reverb quality, force 32kHz mode, etc.). Document actual achieved CPU percentage.</description>
      </idea>
    </ideas>
  </tests>
</story-context>
