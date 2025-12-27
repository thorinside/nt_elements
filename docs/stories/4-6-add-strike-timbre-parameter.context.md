<story-context id="bmad/bmm/workflows/4-implementation/story-context/template" v="1.0">
  <metadata>
    <epicId>4</epicId>
    <storyId>6</storyId>
    <title>Add Strike Timbre Parameter</title>
    <status>drafted</status>
    <generatedAt>2025-12-26</generatedAt>
    <generator>BMAD Story Context Workflow</generator>
    <sourceStoryPath>docs/stories/4-6-add-strike-timbre-parameter.md</sourceStoryPath>
  </metadata>

  <story>
    <asA>user</asA>
    <iWant>access to the Strike Timbre parameter</iWant>
    <soThat>I can control the brightness/speed of the percussive excitation</soThat>
    <tasks>
      - Task 1: Audit current Strike Timbre exposure
        - Search for existing kParamStrikeTimbre
        - If not exposed, document the gap
      - Task 2: Add parameter enum if missing
        - Add kParamStrikeTimbre to ParameterIndices enum
      - Task 3: Define parameter in factory
        - Set range 0-100, default 50
        - Set display name "Strike Timbre"
      - Task 4: Map parameter to patch structure
        - Map to patch.exciter_strike_timbre = ntToElements(value)
      - Task 5: Add to Timbre page (Page 2)
        - Add Strike Timbre as Pot 3 on Timbre page
      - Task 6: Update OLED display
        - Add label "StrTim" or "StrikeTim"
      - Task 7: Verify default initialization
        - Confirm exciter_strike_timbre is set to 0.5f in defaults
    </tasks>
  </story>

  <acceptanceCriteria>
    1. Verify exciter_strike_timbre is exposed as parameter
    2. Parameter added to Timbre page (Page 2)
    3. OLED displays "StrTim" or similar abbreviation
    4. Range: 0-100% mapped to 0.0-1.0
    5. Default value: 50%
  </acceptanceCriteria>

  <artifacts>
    <docs>
      <doc>
        <path>docs/epic-4-audio-quality-and-parameters.md</path>
        <title>Epic 4: Audio Quality and Missing Parameters</title>
        <section>Story 4.6: Add Strike Timbre Parameter</section>
        <snippet>exciter_strike_timbre may not be exposed. Completes the timbre trio. STRIKE timbre controls brightness/speed of percussive excitation.</snippet>
      </doc>
    </docs>
    <code>
      <file>
        <path>src/nt_elements.cpp</path>
        <kind>main plugin implementation</kind>
        <symbol>parameters[]</symbol>
        <lines>62-101</lines>
        <reason>Parameter definitions - check if Strike Timbre exists, add if missing</reason>
      </file>
      <file>
        <path>src/nt_elements.cpp</path>
        <kind>main plugin implementation</kind>
        <symbol>parameterChanged()</symbol>
        <lines>323-402</lines>
        <reason>Add case for kParamStrikeTimbre mapping to patch->exciter_strike_timbre</reason>
      </file>
      <file>
        <path>src/oled_display.cpp</path>
        <kind>display implementation</kind>
        <symbol>PARAM_NAMES[]</symbol>
        <lines>16-57</lines>
        <reason>Add "StrTim" entry for Strike Timbre display</reason>
      </file>
      <file>
        <path>external/mutable-instruments/elements/dsp/patch.h</path>
        <kind>header (read-only)</kind>
        <symbol>Patch struct</symbol>
        <lines>34-56</lines>
        <reason>Defines exciter_strike_timbre field in Patch structure</reason>
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
        <purpose>Elements DSP source code including Patch struct</purpose>
      </cpp>
    </dependencies>
  </artifacts>

  <constraints>
    - Prerequisite: Story 4.5 (page reorganization must be done first)
    - Completes the timbre parameter trio: Bow Timbre, Blow Timbre, Strike Timbre
    - Follow existing parameter definition pattern
    - Never modify files in external/mutable-instruments/
    - Strike Timbre from manual: "brightness/speed of percussive excitation"
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
    <standards>Desktop-first testing using VCV Rack with nt_emu module. Verify parameter affects strike exciter brightness/speed.</standards>
    <locations>
      - VCV Rack with nt_emu module (desktop testing)
      - distingNT hardware (final validation)
    </locations>
    <ideas>
      - AC1: Verify exciter_strike_timbre is mapped and responds to parameter changes
      - AC2: Navigate to Timbre page, confirm Strike Timbre on Pot 3
      - AC3: Verify "StrTim" label displays on OLED
      - AC4: Sweep 0-100%, verify brightness/speed changes in strike character
      - AC5: Reset to defaults, verify shows 50%
    </ideas>
  </tests>
</story-context>
