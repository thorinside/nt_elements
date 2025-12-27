<story-context id="bmad/bmm/workflows/4-implementation/story-context/template" v="1.0">
  <metadata>
    <epicId>4</epicId>
    <storyId>8</storyId>
    <title>Dual External Input Routing</title>
    <status>drafted</status>
    <generatedAt>2025-12-26</generatedAt>
    <generator>BMAD Story Context Workflow</generator>
    <sourceStoryPath>docs/stories/4-8-dual-external-input-routing.md</sourceStoryPath>
  </metadata>

  <story>
    <asA>user</asA>
    <iWant>two separate external audio inputs (Blow path and Strike path)</iWant>
    <soThat>I can excite the resonator with external audio through either signal path</soThat>
    <tasks>
      - Task 1: Understand current input routing
        - Find existing kParamInputBus
        - Document how external input enters Part::Process()
        - Understand blow_in and strike_in buffers
      - Task 2: Add dual input bus parameters
        - Add kParamBlowInputBus and kParamStrikeInputBus
        - Define with bus selection range (0-28)
      - Task 3: Implement Blow input path
        - Read audio from blow input bus
        - Copy to blow_in buffer (goes through diffusion → envelope → STRENGTH VCA)
      - Task 4: Implement Strike input path
        - Read audio from strike input bus
        - Copy to strike_in buffer (unprocessed, direct to resonator)
      - Task 5: Update step() function
        - Modify audio buffer preparation
        - Handle bus 0 = no external input
      - Task 6: Remove/repurpose old input bus
      - Task 7: Verify routing UI
      - Task 8: Test external audio excitation
    </tasks>
  </story>

  <acceptanceCriteria>
    1. Two input bus parameters: kParamBlowInputBus and kParamStrikeInputBus
    2. Blow Input: Audio routed through diffusion → envelope → STRENGTH VCA → resonator
    3. Strike Input: Audio routed directly to resonator (unprocessed)
    4. Both inputs can be set independently to any NT bus (0=none, 1-28=bus)
    5. Existing single input bus parameter removed or repurposed
    6. OLED shows input bus assignments on Routing page (NT firmware UI)
    7. External audio correctly excites resonator through appropriate path
  </acceptanceCriteria>

  <artifacts>
    <docs>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Audio Buffer Access Pattern</section>
        <snippet>NT bus indexing is 1-based, array access is 0-based. Always use temp buffers for Elements processing. Bus parameters subtract 1 for array access.</snippet>
      </doc>
      <doc>
        <path>docs/epic-4-audio-quality-and-parameters.md</path>
        <title>Epic 4: Audio Quality and Missing Parameters</title>
        <section>External Input Routing</section>
        <snippet>External IN 1 (Blow path): goes through diffusion → envelope → STRENGTH VCA. External IN 2 (Strike path): unprocessed, directly to resonator.</snippet>
      </doc>
    </docs>
    <code>
      <file>
        <path>src/nt_elements.cpp</path>
        <kind>main plugin implementation</kind>
        <symbol>step()</symbol>
        <lines>620-643</lines>
        <reason>Audio processing where blow_in and strike_in buffers are prepared before Part::Process()</reason>
      </file>
      <file>
        <path>src/nt_elements.cpp</path>
        <kind>main plugin implementation</kind>
        <symbol>kParamInputBus</symbol>
        <lines>121</lines>
        <reason>Current single input bus parameter - needs to become two parameters</reason>
      </file>
      <file>
        <path>src/nt_elements.h</path>
        <kind>header</kind>
        <symbol>temp_blow_in, temp_strike_in</symbol>
        <lines>29-33</lines>
        <reason>Temp buffers that receive external audio input for Part::Process()</reason>
      </file>
      <file>
        <path>external/mutable-instruments/elements/dsp/part.h</path>
        <kind>header (read-only)</kind>
        <symbol>Part::Process()</symbol>
        <lines>59-60</lines>
        <reason>Takes blow_in and strike_in buffers as separate inputs to resonator</reason>
      </file>
    </code>
    <dependencies>
      <cpp>
        <package>distingNT_API</package>
        <version>v9</version>
        <purpose>NT 28-bus audio routing system</purpose>
      </cpp>
    </dependencies>
  </artifacts>

  <constraints>
    - Prerequisite: Story 4.4 (Strength parameter needed for blow path VCA)
    - Part::Process() signature: (PerformanceState, blow_in, strike_in, main_out, aux_out, size)
    - Blow path: audio → diffusion → envelope → STRENGTH VCA → resonator
    - Strike path: audio → resonator (unprocessed, direct)
    - Bus index 0 = no external input (use internal exciter only)
    - Bus indices 1-28 = external audio source from NT bus system
    - Never modify files in external/mutable-instruments/
  </constraints>

  <interfaces>
    <interface>
      <name>elements::Part::Process()</name>
      <kind>DSP function</kind>
      <signature>void Process(const PerformanceState&amp; performance_state, const float* blow_in, const float* strike_in, float* main, float* aux, size_t size)</signature>
      <path>external/mutable-instruments/elements/dsp/part.h</path>
    </interface>
    <interface>
      <name>NT bus system</name>
      <kind>audio routing</kind>
      <signature>busFrames + (bus_index - 1) * numFrames</signature>
      <path>distingNT_API - step() callback</path>
    </interface>
  </interfaces>

  <tests>
    <standards>Desktop-first testing using VCV Rack with nt_emu module. Test with external audio sources routed to each input path.</standards>
    <locations>
      - VCV Rack with nt_emu module (desktop testing)
      - distingNT hardware (final validation)
    </locations>
    <ideas>
      - AC1: Verify two separate input bus parameters appear in UI
      - AC2: Route sustained audio (drone) to blow input, verify diffusion/envelope processing
      - AC3: Route percussive audio (drums) to strike input, verify direct passthrough
      - AC4: Test different bus assignments for each input independently
      - AC5: Verify old single input parameter is removed/repurposed
      - AC6: Check routing parameters accessible via NT firmware UI
      - AC7: Compare external excitation results - blow path should be filtered, strike path raw
    </ideas>
  </tests>
</story-context>
