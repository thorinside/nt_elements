<story-context id="bmad/bmm/workflows/4-implementation/story-context/template" v="1.0">
  <metadata>
    <epicId>4</epicId>
    <storyId>7</storyId>
    <title>Validation and A/B Testing</title>
    <status>drafted</status>
    <generatedAt>2025-12-26</generatedAt>
    <generator>BMAD Story Context Workflow</generator>
    <sourceStoryPath>docs/stories/4-7-validation-and-ab-testing.md</sourceStoryPath>
  </metadata>

  <story>
    <asA>developer</asA>
    <iWant>to validate all parameter and audio changes against the reference implementation</iWant>
    <soThat>nt_elements sounds and behaves correctly</soThat>
    <tasks>
      - Task 1: Set up A/B testing environment
        - Install Audible Instruments in VCV Rack
        - Create comparison patch with Modal Synthesizer and nt_elements
      - Task 2: Validate audio output levels
        - Compare peak levels on oscilloscope/meter
        - Verify similar loudness at unity gain
      - Task 3: Test existing parameters
        - Geometry, Brightness, Damping, Exciter levels
        - Reverb parameters comparison
      - Task 4: Test new parameters
        - Flow, Mallet, Strength verification
      - Task 5: Regression testing
        - MIDI, pitch, preset save/load, navigation, OLED
      - Task 6: CPU profiling
        - Deploy to hardware, measure CPU usage
        - Compare to pre-Epic 4 baseline
      - Task 7: Document findings
        - Create validation report
    </tasks>
  </story>

  <acceptanceCriteria>
    1. A/B comparison with Audible Instruments Modal Synthesizer
    2. Audio output levels match (both at similar perceived loudness)
    3. All parameters function identically to reference
    4. Flow, Mallet, Strength produce expected timbral changes
    5. No regressions in existing functionality
    6. CPU usage remains within acceptable bounds
  </acceptanceCriteria>

  <artifacts>
    <docs>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Performance Considerations</section>
        <snippet>Target: &lt; 30% CPU usage @ 48kHz. Expected breakdown: Elements DSP ~15-20%, Reverb ~5-10%, Adapter overhead &lt; 1%.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Audio Quality</section>
        <snippet>Target: 100% sound authenticity vs hardware Elements. A/B comparison, spectral analysis, blind listening tests.</snippet>
      </doc>
    </docs>
    <code>
      <file>
        <path>src/nt_elements.cpp</path>
        <kind>main plugin implementation</kind>
        <symbol>step(), parameterChanged()</symbol>
        <reason>Main audio processing and parameter handling - verify all Epic 4 changes work correctly</reason>
      </file>
    </code>
    <dependencies>
      <cpp>
        <package>Audible Instruments</package>
        <version>VCV Rack Library</version>
        <purpose>Reference implementation for A/B comparison (Modal Synthesizer)</purpose>
      </cpp>
    </dependencies>
  </artifacts>

  <constraints>
    - Prerequisite: All Epic 4 stories (4.1-4.6, 4.8-4.9) must be complete
    - No code changes in this story - testing and documentation only
    - CPU budget target: &lt; 30% at 32kHz
    - Level matching should be within ±1dB
    - May create validation report document
  </constraints>

  <interfaces>
    <interface>
      <name>VCV Rack comparison setup</name>
      <kind>test environment</kind>
      <signature>Both modules receiving same MIDI, routing to separate audio outputs</signature>
      <path>N/A - test patch configuration</path>
    </interface>
  </interfaces>

  <tests>
    <standards>A/B testing in VCV Rack with Audible Instruments Modal Synthesizer. Hardware CPU profiling on distingNT. Create validation report documenting all test results.</standards>
    <locations>
      - VCV Rack with nt_emu + Audible Instruments (desktop A/B testing)
      - distingNT hardware (CPU profiling)
    </locations>
    <ideas>
      - AC1: Create side-by-side patch, switch between outputs, listen for differences
      - AC2: Use scope module, compare peak levels with matched settings
      - AC3: Parameter sweep tests on Geometry, Brightness, Damping
      - AC4: Sweep Flow/Mallet/Strength, verify expected timbral changes
      - AC5: Full regression test: MIDI notes, presets, navigation, display
      - AC6: Deploy to NT hardware, observe CPU % on display, verify &lt; 30%
    </ideas>
  </tests>
</story-context>
