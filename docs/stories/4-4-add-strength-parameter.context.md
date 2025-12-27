<story-context id="bmad/bmm/workflows/4-implementation/story-context/template" v="1.0">
  <metadata>
    <epicId>4</epicId>
    <storyId>4</storyId>
    <title>Add Strength Parameter (Gate Strength)</title>
    <status>drafted</status>
    <generatedAt>2025-12-26</generatedAt>
    <generator>BMAD Story Context Workflow</generator>
    <sourceStoryPath>docs/stories/4-4-add-strength-parameter.md</sourceStoryPath>
  </metadata>

  <story>
    <asA>user</asA>
    <iWant>access to the Strength parameter</iWant>
    <soThat>I can control the excitation intensity/velocity</soThat>
    <tasks>
      - Task 1: Add parameter enum (AC: 1)
        - Add `kParamStrength` to parameter enum with performance parameters
      - Task 2: Define parameter in factory (AC: 1, 3, 6)
        - Add parameter definition in parameters[] array
        - Set range 0-100, default 50
        - Set display name "Strength"
      - Task 3: Map parameter to performance state (AC: 2)
        - Locate where PerformanceState is populated before Part::Process()
        - In parameterChanged() callback, store strength value
        - Map to perf_state.strength = ntToElements(value) in step()
      - Task 4: Add to parameter page (AC: 4)
        - Add Strength to pagePerformance[] array
      - Task 5: Update OLED display (AC: 5)
        - Ensure "Strength" or "Str" label displays correctly
      - Task 6: Implement MIDI velocity modulation (AC: 7)
        - In midiMessage() callback, capture velocity (0-127)
        - Optionally blend velocity with parameter value
    </tasks>
  </story>

  <acceptanceCriteria>
    1. New parameter `kParamStrength` added to parameter adapter
    2. Parameter maps to `perf_state.strength` in PerformanceState
    3. Range: 0-100% mapped to 0.0-1.0
    4. Parameter added to Performance page (Page 4)
    5. OLED displays "Strength" with percentage value
    6. Default value: 50%
    7. MIDI velocity can optionally modulate this parameter
  </acceptanceCriteria>

  <artifacts>
    <docs>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>MIDI Handling Pattern</section>
        <snippet>MIDI note to frequency conversion. Capture velocity for strength modulation. Gate on/off control.</snippet>
      </doc>
      <doc>
        <path>docs/epic-4-audio-quality-and-parameters.md</path>
        <title>Epic 4: Audio Quality and Missing Parameters</title>
        <section>Story 4.4: Add Strength Parameter (Gate Strength)</section>
        <snippet>PerformanceState.strength controls gate/excitation strength. In original Elements, this comes from STRENGTH CV input. VCA for excitation signal with +/- 15dB range.</snippet>
      </doc>
    </docs>
    <code>
      <file>
        <path>src/nt_elements.cpp</path>
        <kind>main plugin implementation</kind>
        <symbol>parameters[]</symbol>
        <lines>62-101</lines>
        <reason>Parameter definitions array - add new Strength parameter definition here</reason>
      </file>
      <file>
        <path>src/nt_elements.cpp</path>
        <kind>main plugin implementation</kind>
        <symbol>pagePerformance[]</symbol>
        <lines>116-118</lines>
        <reason>Performance page parameter list - add kParamStrength here</reason>
      </file>
      <file>
        <path>src/nt_elements.cpp</path>
        <kind>main plugin implementation</kind>
        <symbol>perf_state.strength</symbol>
        <lines>241, 465, 513</lines>
        <reason>Currently hardcoded to 0.8f or 0.5f - needs to read from parameter value</reason>
      </file>
      <file>
        <path>src/nt_elements.cpp</path>
        <kind>main plugin implementation</kind>
        <symbol>midiMessage()</symbol>
        <lines>varies</lines>
        <reason>MIDI callback - can optionally modulate strength with velocity</reason>
      </file>
      <file>
        <path>external/mutable-instruments/elements/dsp/part.h</path>
        <kind>header (read-only)</kind>
        <symbol>PerformanceState struct</symbol>
        <lines>41-46</lines>
        <reason>Defines PerformanceState with gate, note, modulation, and strength fields</reason>
      </file>
    </code>
    <dependencies>
      <cpp>
        <package>distingNT_API</package>
        <version>v9</version>
        <purpose>Plugin API framework, _NT_parameter struct</purpose>
      </cpp>
      <cpp>
        <package>mutable-instruments/elements</package>
        <version>submodule</version>
        <purpose>Elements DSP source code including PerformanceState struct</purpose>
      </cpp>
    </dependencies>
  </artifacts>

  <constraints>
    - Follow existing parameter definition pattern
    - Strength affects PerformanceState (not Patch) - different from other parameters
    - perf_state.strength is used per-sample in step(), not just on parameter change
    - Never modify files in external/mutable-instruments/
    - MIDI velocity modulation is optional enhancement (AC7)
    - Strength acts as VCA for excitation signal (+/- 15dB range at 3dB/V)
  </constraints>

  <interfaces>
    <interface>
      <name>PerformanceState struct</name>
      <kind>Elements data structure</kind>
      <signature>struct PerformanceState { bool gate; float note; float modulation; float strength; }</signature>
      <path>external/mutable-instruments/elements/dsp/part.h</path>
    </interface>
    <interface>
      <name>elements::Part::Process()</name>
      <kind>DSP function</kind>
      <signature>void Process(const PerformanceState&amp; performance_state, const float* blow_in, const float* strike_in, float* main, float* aux, size_t size)</signature>
      <path>external/mutable-instruments/elements/dsp/part.h</path>
    </interface>
    <interface>
      <name>midiMessage() callback</name>
      <kind>NT API callback</kind>
      <signature>static void midiMessage(_NT_algorithm* self, uint8_t b0, uint8_t b1, uint8_t b2)</signature>
      <path>src/nt_elements.cpp</path>
    </interface>
  </interfaces>

  <tests>
    <standards>Desktop-first testing using VCV Rack with nt_emu module. Build with `make test`. Verify parameter affects excitation intensity.</standards>
    <locations>
      - VCV Rack with nt_emu module (desktop testing)
      - distingNT hardware (final validation)
    </locations>
    <ideas>
      - AC1: Verify new parameter appears in parameter list
      - AC2: Sweep Strength 0-100%, listen for excitation intensity changes
      - AC3: Verify range clamps to 0-100 in UI
      - AC4: Confirm Strength appears on Performance page
      - AC5: Verify "Strength" label and percentage value display correctly
      - AC6: Reset to defaults, verify Strength shows 50%
      - AC7: Test MIDI with different velocities, verify modulation if implemented
    </ideas>
  </tests>
</story-context>
