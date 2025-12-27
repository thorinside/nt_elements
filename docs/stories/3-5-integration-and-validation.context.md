<story-context id="bmad/bmm/workflows/4-implementation/story-context/template" v="1.0">
  <metadata>
    <epicId>3</epicId>
    <storyId>5</storyId>
    <title>Integration and Validation</title>
    <status>ready-for-dev</status>
    <generatedAt>2025-12-25</generatedAt>
    <generator>BMAD Story Context Workflow</generator>
    <sourceStoryPath>docs/stories/3-5-integration-and-validation.md</sourceStoryPath>
  </metadata>

  <story>
    <asA>developer</asA>
    <iWant>the complete dynamic sample loading system integrated and tested</iWant>
    <soThat>nt_elements works correctly with samples loaded from SD card</soThat>
    <tasks>
      - Integrate SampleManager into nt_elements.cpp
      - Update DRAM allocation to include sample buffer (~338KB additional)
      - Set global sample pointers before first audio processing
      - Remove resources_minimal_wavetables.cc from build
      - Test audio output quality (A/B comparison)
      - Implement graceful fallback for missing samples
      - Document performance and memory impact
      - Hardware validation on disting NT
    </tasks>
  </story>

  <acceptanceCriteria>
    <criterion id="1">nt_elements.cpp integrates SampleManager</criterion>
    <criterion id="2">DRAM allocation increased to include sample buffer (~338KB additional)</criterion>
    <criterion id="3">Sample pointers set before first audio processing</criterion>
    <criterion id="4">Plugin produces correct audio output (matches static version)</criterion>
    <criterion id="5">Graceful behavior when SD card not mounted (silence, no crash)</criterion>
    <criterion id="6">CPU usage impact documented (expected: minimal)</criterion>
    <criterion id="7">Memory usage documented</criterion>
    <criterion id="8">Hardware validation on actual disting NT</criterion>
  </acceptanceCriteria>

  <artifacts>
    <docs>
      <doc>
        <path>docs/epic-3-dynamic-sample-loading.md</path>
        <title>Epic 3 Detail Document</title>
        <section>Story 3.5 - Integration Points</section>
        <snippet>DRAM calculation: old 64KB + new 338KB = 402KB. construct() partitions DRAM, inits SampleManager, sets global pointers.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Memory Layout</section>
        <snippet>DRAM (~32KB for reverb). Pattern: partition in construct(), set pointers before use.</snippet>
      </doc>
    </docs>
    <code>
      <artifact>
        <path>src/nt_elements.cpp</path>
        <kind>source</kind>
        <symbol>calculateRequirements, construct, step</symbol>
        <lines>162-274, 393-603</lines>
        <reason>Main integration points. calculateRequirements for DRAM, construct for init, step for audio processing.</reason>
      </artifact>
      <artifact>
        <path>src/sample_manager.h</path>
        <kind>header</kind>
        <symbol>SampleManager</symbol>
        <lines>full file</lines>
        <reason>SampleManager class to integrate (from Story 3.3).</reason>
      </artifact>
      <artifact>
        <path>src/sample_manager.cpp</path>
        <kind>source</kind>
        <symbol>SampleManager</symbol>
        <lines>full file</lines>
        <reason>SampleManager implementation (from Story 3.3).</reason>
      </artifact>
      <artifact>
        <path>Makefile</path>
        <kind>build</kind>
        <symbol>SOURCES</symbol>
        <lines>18-31</lines>
        <reason>Remove resources_minimal_wavetables.cc, add sample_manager.cpp.</reason>
      </artifact>
    </code>
    <dependencies>
      <story>
        <id>3.1</id>
        <note>WAV API available</note>
      </story>
      <story>
        <id>3.2</id>
        <note>WAV files on SD card</note>
      </story>
      <story>
        <id>3.3</id>
        <note>SampleManager implemented</note>
      </story>
      <story>
        <id>3.4</id>
        <note>Patch applied for pointer indirection</note>
      </story>
      <story>
        <id>3.0</id>
        <note>nt_emu virtual SD card support (for desktop testing)</note>
      </story>
    </dependencies>
  </artifacts>

  <constraints>
    <constraint>Binary size should decrease by ~1.5MB (removing static samples)</constraint>
    <constraint>DRAM usage increases by ~338KB</constraint>
    <constraint>CPU impact should be negligible (one-time load, pointer access same speed)</constraint>
    <constraint>Must handle samples-not-loaded gracefully (silence, not crash)</constraint>
    <constraint>Audio output must match static version exactly for same parameters</constraint>
  </constraints>

  <interfaces>
    <interface>
      <name>smp_sample_data_ptr</name>
      <kind>global pointer</kind>
      <signature>const int16_t* smp_sample_data_ptr = nullptr</signature>
      <path>src/nt_elements.cpp (definition)</path>
    </interface>
    <interface>
      <name>smp_noise_sample_ptr</name>
      <kind>global pointer</kind>
      <signature>const int16_t* smp_noise_sample_ptr = nullptr</signature>
      <path>src/nt_elements.cpp (definition)</path>
    </interface>
    <interface>
      <name>smp_boundaries_ptr</name>
      <kind>global pointer</kind>
      <signature>const size_t* smp_boundaries_ptr = nullptr</signature>
      <path>src/nt_elements.cpp (definition)</path>
    </interface>
  </interfaces>

  <tests>
    <standards>A/B audio comparison with static version. Extended stability testing (30+ minutes). Hardware validation on disting NT. CPU and memory profiling.</standards>
    <locations>
      - VCV Rack nt_emu (desktop testing with virtual SD card)
      - disting NT hardware (production validation)
    </locations>
    <ideas>
      <idea acRef="1,2,3">Build succeeds, plugin loads in nt_emu</idea>
      <idea acRef="4">Record audio, compare spectral content to static version</idea>
      <idea acRef="5">Test without samples on SD card, verify silence (no crash)</idea>
      <idea acRef="5">Display OLED warning when samples missing</idea>
      <idea acRef="6">Compare CPU % display on NT: before vs after Epic 3</idea>
      <idea acRef="7">Measure binary size reduction, DRAM increase</idea>
      <idea acRef="8">Deploy to hardware, play notes, verify correct audio</idea>
      <idea acRef="8">Extended playback test (30+ minutes) on hardware</idea>
    </ideas>
  </tests>
</story-context>
