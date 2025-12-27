<story-context id="bmad/bmm/workflows/4-implementation/story-context/template" v="1.0">
  <metadata>
    <epicId>3</epicId>
    <storyId>1</storyId>
    <title>Update distingNT_API Submodule for WAV Support</title>
    <status>ready-for-dev</status>
    <generatedAt>2025-12-25</generatedAt>
    <generator>BMAD Story Context Workflow</generator>
    <sourceStoryPath>docs/stories/3-1-update-distingnt-api-submodule.md</sourceStoryPath>
  </metadata>

  <story>
    <asA>developer</asA>
    <iWant>the distingNT_API submodule updated to include the new WAV file API</iWant>
    <soThat>I can use NT_readSampleFrames() and related functions</soThat>
    <tasks>
      - Update submodule to WAV-enabled commit (fb30ae4 or later)
      - Verify wav.h header availability in include path
      - Update build configuration for API version 10+
      - Regression test existing functionality
    </tasks>
  </story>

  <acceptanceCriteria>
    <criterion id="1">distingNT_API submodule pinned to commit fb30ae4 or later</criterion>
    <criterion id="2">New header distingnt/wav.h available in include path</criterion>
    <criterion id="3">Project compiles with new API version (kNT_apiVersion10+)</criterion>
    <criterion id="4">Existing functionality unaffected by API update</criterion>
  </acceptanceCriteria>

  <artifacts>
    <docs>
      <doc>
        <path>docs/epic-3-dynamic-sample-loading.md</path>
        <title>Epic 3 Detail Document</title>
        <section>Story 3.1</section>
        <snippet>Covers WAV API update requirements. Target commit fb30ae4 adds wav.h header with NT_readSampleFrames and related declarations.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Technology Stack Details</section>
        <snippet>distingNT API v9 is current. Project uses git submodule at distingNT_API/. API provides factory pattern, parameter system, MIDI, 28-bus audio routing.</snippet>
      </doc>
    </docs>
    <code>
      <artifact>
        <path>distingNT_API/include/distingnt/api.h</path>
        <kind>header</kind>
        <symbol>kNT_apiVersionCurrent</symbol>
        <lines>1-50</lines>
        <reason>Main API header - version constant needs verification after update</reason>
      </artifact>
      <artifact>
        <path>Makefile</path>
        <kind>build</kind>
        <symbol>DISTINGNT_API</symbol>
        <lines>10-40</lines>
        <reason>Build configuration references distingNT_API submodule. Includes path already configured.</reason>
      </artifact>
      <artifact>
        <path>src/nt_elements.cpp</path>
        <kind>source</kind>
        <symbol>pluginEntry</symbol>
        <lines>864-880</lines>
        <reason>Plugin entry point returns kNT_apiVersionCurrent - verify compatibility after update</reason>
      </artifact>
    </code>
    <dependencies>
      <git-submodule>
        <name>distingNT_API</name>
        <current>api v9</current>
        <target>api v10+ (commit fb30ae4)</target>
        <url>https://github.com/expertsleepersltd/distingNT_API</url>
      </git-submodule>
    </dependencies>
  </artifacts>

  <constraints>
    <constraint>Submodule update must not break existing plugin functionality</constraint>
    <constraint>API version returned by pluginEntry must remain compatible</constraint>
    <constraint>Build must succeed for both hardware (ARM) and test (desktop) targets</constraint>
  </constraints>

  <interfaces>
    <interface>
      <name>NT_isSdCardMounted</name>
      <kind>function</kind>
      <signature>bool NT_isSdCardMounted(void)</signature>
      <path>distingNT_API/include/distingnt/wav.h (after update)</path>
    </interface>
    <interface>
      <name>NT_readSampleFrames</name>
      <kind>function</kind>
      <signature>bool NT_readSampleFrames(const _NT_wavRequest* request)</signature>
      <path>distingNT_API/include/distingnt/wav.h (after update)</path>
    </interface>
    <interface>
      <name>NT_getNumSampleFolders</name>
      <kind>function</kind>
      <signature>uint32_t NT_getNumSampleFolders(void)</signature>
      <path>distingNT_API/include/distingnt/wav.h (after update)</path>
    </interface>
  </interfaces>

  <tests>
    <standards>Desktop-first testing using nt_emu in VCV Rack. Verify plugin loads, audio processes, MIDI works, parameters respond correctly after API update.</standards>
    <locations>
      - VCV Rack with nt_emu module (manual testing)
      - make test (desktop build validation)
      - make hardware (ARM build validation)
    </locations>
    <ideas>
      <idea acRef="1">Verify git submodule shows correct commit hash after update</idea>
      <idea acRef="2">Check wav.h exists and contains expected declarations</idea>
      <idea acRef="3">Build succeeds without errors for both targets</idea>
      <idea acRef="4">Load plugin in nt_emu, play notes, verify audio output unchanged</idea>
    </ideas>
  </tests>
</story-context>
