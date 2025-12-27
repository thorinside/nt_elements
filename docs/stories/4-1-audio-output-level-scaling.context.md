<story-context id="bmad/bmm/workflows/4-implementation/story-context/template" v="1.0">
  <metadata>
    <epicId>4</epicId>
    <storyId>1</storyId>
    <title>Audio Output Level Scaling</title>
    <status>drafted</status>
    <generatedAt>2025-12-26</generatedAt>
    <generator>BMAD Story Context Workflow</generator>
    <sourceStoryPath>docs/stories/4-1-audio-output-level-scaling.md</sourceStoryPath>
  </metadata>

  <story>
    <asA>user</asA>
    <iWant>audio output at proper Eurorack levels (±5V)</iWant>
    <soThat>nt_elements integrates correctly with other modules without being too quiet</soThat>
    <tasks>
      - Task 1: Locate audio output scaling code (AC: 1, 2)
        - Find `nt_elements.cpp` output section (~lines 625-642)
        - Identify `temp_main_out` and `temp_aux_out` processing
        - Document current scaling behavior
      - Task 2: Apply 5V scaling to main output (AC: 1, 3)
        - Modify output calculation: `outputChunk[i] = algo->temp_main_out[i] * algo->output_level_scale * 5.0f`
        - Ensure output_level_scale still provides 0-100% volume control
      - Task 3: Apply 5V scaling to aux output (AC: 2)
        - Locate aux output processing
        - Apply same 5.0f multiplication
      - Task 4: Test for clipping (AC: 4)
        - Test with default settings - verify no clipping
        - Test with resonator at high brightness/low damping (loud settings)
        - Verify signal stays within ±5V envelope
      - Task 5: Compare with reference (AC: 5)
        - Load Audible Instruments Modal Synthesizer in VCV Rack
        - Compare output levels at identical settings
        - Document perceived loudness comparison
    </tasks>
  </story>

  <acceptanceCriteria>
    1. Audio output multiplied by 5.0f for Eurorack standard levels
    2. Main output and aux output both scaled correctly
    3. Output level parameter (0-100%) still functions as volume control on top of 5V scaling
    4. No clipping or distortion at default settings
    5. Output level comparable to Audible Instruments Modal Synthesizer
  </acceptanceCriteria>

  <artifacts>
    <docs>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Audio Buffer Access Pattern</section>
        <snippet>NT bus indexing is 1-based, array access is 0-based. Respect output mode (add vs replace). Always use temp buffers for Elements processing.</snippet>
      </doc>
      <doc>
        <path>docs/epic-4-audio-quality-and-parameters.md</path>
        <title>Epic 4: Audio Quality and Missing Parameters</title>
        <section>Story 4.1: Audio Output Level Scaling (5V)</section>
        <snippet>Elements outputs normalized audio (-1.0 to +1.0 range). Eurorack standard is ±5V audio signals. Multiply final output by 5.0f constant.</snippet>
      </doc>
    </docs>
    <code>
      <file>
        <path>src/nt_elements.cpp</path>
        <kind>main plugin implementation</kind>
        <symbol>step()</symbol>
        <lines>620-643</lines>
        <reason>Contains output buffer processing where 5V scaling must be applied to temp_main_out and temp_aux_out</reason>
      </file>
      <file>
        <path>src/nt_elements.h</path>
        <kind>header</kind>
        <symbol>nt_elementsAlgorithm</symbol>
        <lines>22-69</lines>
        <reason>Defines algorithm structure including temp_main_out, temp_aux_out, and output_level_scale fields</reason>
      </file>
    </code>
    <dependencies>
      <cpp>
        <package>distingNT_API</package>
        <version>v9</version>
        <purpose>Plugin API framework</purpose>
      </cpp>
      <cpp>
        <package>mutable-instruments/elements</package>
        <version>submodule</version>
        <purpose>Elements DSP source code</purpose>
      </cpp>
    </dependencies>
  </artifacts>

  <constraints>
    - Single file modification: `src/nt_elements.cpp`
    - No new files required
    - No header changes needed
    - Preserve output_level_scale multiplicative behavior (user volume control on top of 5V base)
    - Never modify files in external/mutable-instruments/
    - Elements outputs normalized -1.0 to +1.0 range
    - Eurorack standard is ±5V (some modules ±10V but 5V is common)
  </constraints>

  <interfaces>
    <interface>
      <name>step() callback</name>
      <kind>NT API callback</kind>
      <signature>static void step(_NT_algorithm* self, float* busFrames, int numFramesBy4)</signature>
      <path>src/nt_elements.cpp</path>
    </interface>
    <interface>
      <name>elements::Part::Process()</name>
      <kind>DSP function</kind>
      <signature>void Process(const PerformanceState&amp; performance_state, const float* blow_in, const float* strike_in, float* main, float* aux, size_t size)</signature>
      <path>external/mutable-instruments/elements/dsp/part.h</path>
    </interface>
  </interfaces>

  <tests>
    <standards>Desktop-first testing using VCV Rack with nt_emu module. Build with `make test` to produce .dylib, load in nt_emu. Hardware validation at milestones. A/B comparison with Audible Instruments Modal Synthesizer for output level verification.</standards>
    <locations>
      - VCV Rack with nt_emu module (desktop testing)
      - distingNT hardware (final validation)
    </locations>
    <ideas>
      - AC1: Measure output amplitude with scope module, verify ~±5V peaks
      - AC2: Compare main and aux output levels, both should be scaled identically
      - AC3: Sweep Output Level parameter 0-100%, verify volume control still works
      - AC4: Play sustained notes with high brightness/low damping, verify no clipping
      - AC5: Side-by-side comparison with Audible Instruments Modal Synthesizer at matched settings
    </ideas>
  </tests>
</story-context>
