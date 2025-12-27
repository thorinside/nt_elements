<story-context id="bmad/bmm/workflows/4-implementation/story-context/template" v="1.0">
  <metadata>
    <epicId>4</epicId>
    <storyId>3</storyId>
    <title>Add Mallet Parameter (Strike Meta)</title>
    <status>drafted</status>
    <generatedAt>2025-12-26</generatedAt>
    <generator>BMAD Story Context Workflow</generator>
    <sourceStoryPath>docs/stories/4-3-add-mallet-parameter.md</sourceStoryPath>
  </metadata>

  <story>
    <asA>user</asA>
    <iWant>access to the Mallet parameter</iWant>
    <soThat>I can control the mallet/strike character of the strike exciter</soThat>
    <tasks>
      - Task 1: Add parameter enum (AC: 1)
        - Add `kParamStrikeMallet` to parameter enum after existing exciter parameters
      - Task 2: Define parameter in factory (AC: 1, 3, 6)
        - Add parameter definition in parameters[] array
        - Set range 0-100, default 50
        - Set display name "Mallet"
      - Task 3: Map parameter to patch structure (AC: 2)
        - In `parameterChanged()` callback, add case for `kParamStrikeMallet`
        - Map to `patch.exciter_strike_meta = ntToElements(value)`
      - Task 4: Add to parameter page (AC: 4)
        - Add Mallet to pageExciter[] array
      - Task 5: Update OLED display (AC: 5)
        - Ensure "Mallet" label displays correctly
      - Task 6: Update default patch initialization (AC: 6)
        - Verify exciter_strike_meta is set to 0.5f in setDefaultPatch()
    </tasks>
  </story>

  <acceptanceCriteria>
    1. New parameter `kParamStrikeMallet` added to parameter adapter
    2. Parameter maps to `patch->exciter_strike_meta`
    3. Range: 0-100% mapped to 0.0-1.0
    4. Parameter added to Exciter page (Page 1)
    5. OLED displays "Mallet" with percentage value
    6. Default value: 50%
  </acceptanceCriteria>

  <artifacts>
    <docs>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Parameter Update Pattern</section>
        <snippet>Parameters update via parameterChanged() callback. Convert NT parameter (0-100%) to Elements Patch (0.0-1.0).</snippet>
      </doc>
      <doc>
        <path>docs/epic-4-audio-quality-and-parameters.md</path>
        <title>Epic 4: Audio Quality and Missing Parameters</title>
        <section>Story 4.3: Add Mallet Parameter (Strike Meta)</section>
        <snippet>exciter_strike_meta controls strike exciter's "meta" parameter - the large MALLET knob. Morphs through samples of hammers/mallets/sticks, physical models of mallets and plectrums.</snippet>
      </doc>
    </docs>
    <code>
      <file>
        <path>src/nt_elements.cpp</path>
        <kind>main plugin implementation</kind>
        <symbol>parameters[]</symbol>
        <lines>62-101</lines>
        <reason>Parameter definitions array - add new Mallet parameter definition here</reason>
      </file>
      <file>
        <path>src/nt_elements.cpp</path>
        <kind>main plugin implementation</kind>
        <symbol>pageExciter[]</symbol>
        <lines>104-106</lines>
        <reason>Exciter page parameter list - add kParamStrikeMallet here</reason>
      </file>
      <file>
        <path>src/nt_elements.cpp</path>
        <kind>main plugin implementation</kind>
        <symbol>parameterChanged()</symbol>
        <lines>323-402</lines>
        <reason>Parameter change callback - add case for kParamStrikeMallet mapping to patch->exciter_strike_meta</reason>
      </file>
      <file>
        <path>src/nt_elements.cpp</path>
        <kind>main plugin implementation</kind>
        <symbol>setDefaultPatch()</symbol>
        <lines>282</lines>
        <reason>Currently sets patch->exciter_strike_meta = 0.5f - verify default matches parameter default</reason>
      </file>
      <file>
        <path>external/mutable-instruments/elements/dsp/patch.h</path>
        <kind>header (read-only)</kind>
        <symbol>Patch struct</symbol>
        <lines>34-56</lines>
        <reason>Defines Elements Patch structure including exciter_strike_meta field</reason>
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
        <purpose>Elements DSP source code including Patch struct</purpose>
      </cpp>
    </dependencies>
  </artifacts>

  <constraints>
    - Follow existing parameter definition pattern (see Bow Level, Blow Level, etc.)
    - Use parameter_adapter::ntToElements() for 0-100 to 0.0-1.0 conversion
    - Never modify files in external/mutable-instruments/
    - Parameter enum value must be added in correct position to maintain array alignment
    - kNumParams must be updated to reflect new parameter count
    - Mallet morphs through: hammers/mallets/sticks samples, physical models of mallets, plectrums, particles
  </constraints>

  <interfaces>
    <interface>
      <name>parameterChanged() callback</name>
      <kind>NT API callback</kind>
      <signature>static void parameterChanged(_NT_algorithm* self, int p)</signature>
      <path>src/nt_elements.cpp</path>
    </interface>
    <interface>
      <name>parameter_adapter::ntToElements()</name>
      <kind>helper function</kind>
      <signature>inline float ntToElements(float nt_value) { return nt_value / 100.0f; }</signature>
      <path>src/nt_elements.cpp</path>
    </interface>
  </interfaces>

  <tests>
    <standards>Desktop-first testing using VCV Rack with nt_emu module. Build with `make test`. Verify parameter appears in UI and adjusts strike exciter character when changed.</standards>
    <locations>
      - VCV Rack with nt_emu module (desktop testing)
      - distingNT hardware (final validation)
    </locations>
    <ideas>
      - AC1: Verify new parameter appears in parameter list
      - AC2: Sweep Mallet 0-100%, listen for strike character changes (hammers to plectrums to particles)
      - AC3: Verify range clamps to 0-100 in UI
      - AC4: Confirm Mallet appears on Exciter page
      - AC5: Verify "Mallet" label and percentage value display correctly
      - AC6: Reset to defaults, verify Mallet shows 50%
    </ideas>
  </tests>
</story-context>
