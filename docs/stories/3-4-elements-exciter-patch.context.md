<story-context id="bmad/bmm/workflows/4-implementation/story-context/template" v="1.0">
  <metadata>
    <epicId>3</epicId>
    <storyId>4</storyId>
    <title>Elements Exciter Patch for Sample Indirection</title>
    <status>ready-for-dev</status>
    <generatedAt>2025-12-25</generatedAt>
    <generator>BMAD Story Context Workflow</generator>
    <sourceStoryPath>docs/stories/3-4-elements-exciter-patch.md</sourceStoryPath>
  </metadata>

  <story>
    <asA>developer</asA>
    <iWant>the Elements exciter code patched to use indirect sample access</iWant>
    <soThat>samples can be provided by the SampleManager instead of static arrays</soThat>
    <tasks>
      - Create patches/elements-dynamic-samples.patch
      - Add extern pointer declarations to resources.h
      - Add macro redirections for smp_sample_data, smp_noise_sample, smp_boundaries
      - Remove resources_minimal_wavetables.cc from SOURCES
      - Update Makefile to apply new patch
      - Document patch in patches/README.md
    </tasks>
  </story>

  <acceptanceCriteria>
    <criterion id="1">New patch file patches/elements-dynamic-samples.patch created</criterion>
    <criterion id="2">Patch modifies exciter.cc to use function-based sample access</criterion>
    <criterion id="3">Patch modifies resources.h to declare external sample accessors</criterion>
    <criterion id="4">Original smp_sample_data and smp_noise_sample arrays no longer compiled</criterion>
    <criterion id="5">Makefile applies new patch alongside existing elements-dynamic-sample-rate.patch</criterion>
    <criterion id="6">Build succeeds with patched code</criterion>
    <criterion id="7">No changes to exciter algorithm logic (only data access pattern)</criterion>
  </acceptanceCriteria>

  <artifacts>
    <docs>
      <doc>
        <path>docs/epic-3-dynamic-sample-loading.md</path>
        <title>Epic 3 Detail Document</title>
        <section>Story 3.4 - Patch Strategy</section>
        <snippet>Pointer injection approach: extern pointers in resources.h, macro redirection. smp_sample_data becomes smp_sample_data_ptr via #define.</snippet>
      </doc>
    </docs>
    <code>
      <artifact>
        <path>patches/elements-dynamic-sample-rate.patch</path>
        <kind>patch</kind>
        <symbol>kSampleRate, get_sample_rate</symbol>
        <lines>1-32</lines>
        <reason>Existing patch example. Shows patch format, header modification pattern. New patch follows same style.</reason>
      </artifact>
      <artifact>
        <path>external/mutable-instruments/elements/resources/resources.h</path>
        <kind>header</kind>
        <symbol>smp_sample_data, smp_noise_sample, smp_boundaries</symbol>
        <lines>full file</lines>
        <reason>Target for patch. Will add extern pointer declarations and macro redirections.</reason>
      </artifact>
      <artifact>
        <path>external/mutable-instruments/elements/dsp/exciter.cc</path>
        <kind>source</kind>
        <symbol>Exciter::Process</symbol>
        <lines>sample access</lines>
        <reason>Uses smp_sample_data for wavetable access. With macro redirection, no changes needed here.</reason>
      </artifact>
      <artifact>
        <path>Makefile</path>
        <kind>build</kind>
        <symbol>apply-patches, SOURCES</symbol>
        <lines>82-94, 18-31</lines>
        <reason>Patch application target. SOURCES list to remove resources_minimal_wavetables.cc.</reason>
      </artifact>
      <artifact>
        <path>src/resources_minimal_wavetables.cc</path>
        <kind>source</kind>
        <symbol>smp_sample_data</symbol>
        <lines>full file</lines>
        <reason>Static sample data file to be removed from build (replaced by dynamic loading).</reason>
      </artifact>
    </code>
    <dependencies>
      <story>
        <id>3.3</id>
        <note>SampleManager provides the pointer values to set</note>
      </story>
    </dependencies>
  </artifacts>

  <constraints>
    <constraint>Patch must apply cleanly to fresh submodule checkout</constraint>
    <constraint>Patch must be reversible (git checkout . in submodule)</constraint>
    <constraint>Existing elements-dynamic-sample-rate.patch must still apply</constraint>
    <constraint>Patch order: sample-rate patch first, then samples patch</constraint>
    <constraint>No changes to Elements algorithm logic</constraint>
  </constraints>

  <interfaces>
    <interface>
      <name>smp_sample_data_ptr</name>
      <kind>extern pointer</kind>
      <signature>extern const int16_t* smp_sample_data_ptr</signature>
      <path>external/mutable-instruments/elements/resources/resources.h (patched)</path>
    </interface>
    <interface>
      <name>smp_noise_sample_ptr</name>
      <kind>extern pointer</kind>
      <signature>extern const int16_t* smp_noise_sample_ptr</signature>
      <path>external/mutable-instruments/elements/resources/resources.h (patched)</path>
    </interface>
    <interface>
      <name>smp_boundaries_ptr</name>
      <kind>extern pointer</kind>
      <signature>extern const size_t* smp_boundaries_ptr</signature>
      <path>external/mutable-instruments/elements/resources/resources.h (patched)</path>
    </interface>
  </interfaces>

  <tests>
    <standards>Build validation. Patch application/reversal testing. Audio output comparison (should match static version when pointers set correctly).</standards>
    <locations>
      - patches/elements-dynamic-samples.patch (new)
      - patches/README.md (updated)
      - Makefile (modified)
    </locations>
    <ideas>
      <idea acRef="1">Verify patch file created with correct diff format</idea>
      <idea acRef="2,3">Apply patch to fresh submodule, verify resources.h modified correctly</idea>
      <idea acRef="4">Remove file from SOURCES, verify no linker errors (after Story 3.5 provides pointers)</idea>
      <idea acRef="5">Run make clean-all && make, verify both patches apply</idea>
      <idea acRef="6">make test and make hardware succeed</idea>
      <idea acRef="7">Compare audio output before/after patch (should be identical)</idea>
    </ideas>
  </tests>
</story-context>
