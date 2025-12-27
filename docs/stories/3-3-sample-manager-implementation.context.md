<story-context id="bmad/bmm/workflows/4-implementation/story-context/template" v="1.0">
  <metadata>
    <epicId>3</epicId>
    <storyId>3</storyId>
    <title>Sample Manager Implementation</title>
    <status>ready-for-dev</status>
    <generatedAt>2025-12-25</generatedAt>
    <generator>BMAD Story Context Workflow</generator>
    <sourceStoryPath>docs/stories/3-3-sample-manager-implementation.md</sourceStoryPath>
  </metadata>

  <story>
    <asA>developer</asA>
    <iWant>a sample manager class that loads WAV files from SD card into DRAM</iWant>
    <soThat>sample data is available for the Elements DSP at runtime</soThat>
    <tasks>
      - Create SampleManager class (src/sample_manager.h, src/sample_manager.cpp)
      - Implement DRAM buffer allocation (~338KB)
      - Implement init() and loadSamples() methods
      - Provide getSampleData(), getNoiseSample(), getBoundaries() accessors
      - Handle missing samples gracefully (return zeros)
      - Add to Makefile SOURCES
    </tasks>
  </story>

  <acceptanceCriteria>
    <criterion id="1">SampleManager class implemented in src/sample_manager.h and src/sample_manager.cpp</criterion>
    <criterion id="2">Allocates DRAM buffer in calculateRequirements() for all samples (~338KB)</criterion>
    <criterion id="3">Loads all 10 WAV files synchronously during construct()</criterion>
    <criterion id="4">Provides getSampleData() and getNoiseSample() accessors</criterion>
    <criterion id="5">Provides getBoundaries() accessor returning precomputed boundary array</criterion>
    <criterion id="6">Returns zeros if samples not yet loaded (SD card not mounted)</criterion>
    <criterion id="7">Handles SD card mount/unmount gracefully</criterion>
  </acceptanceCriteria>

  <artifacts>
    <docs>
      <doc>
        <path>docs/epic-3-dynamic-sample-loading.md</path>
        <title>Epic 3 Detail Document</title>
        <section>Story 3.3 - API Design</section>
        <snippet>SampleManager API design with kSampleDataSize=128013, kNoiseSampleSize=40963, kTotalDramBytes=338952. Memory layout: sampleData_ followed by noiseSample_ in DRAM.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Memory Allocation Pattern</section>
        <snippet>All memory via NT_globals.getMemory(). DRAM for large buffers. Current DRAM usage: 32KB for reverb. Pattern: req.dram = size, ptrs.dram in construct().</snippet>
      </doc>
    </docs>
    <code>
      <artifact>
        <path>src/nt_elements.cpp</path>
        <kind>source</kind>
        <symbol>calculateRequirements, construct</symbol>
        <lines>162-274</lines>
        <reason>Shows current memory allocation pattern. DRAM allocated for reverb_buffer. SampleManager will follow same pattern.</reason>
      </artifact>
      <artifact>
        <path>distingNT_API/include/distingnt/api.h</path>
        <kind>header</kind>
        <symbol>_NT_algorithmRequirements, _NT_algorithmMemoryPtrs</symbol>
        <lines>memory section</lines>
        <reason>Defines memory requirement and pointer structures for plugin allocation.</reason>
      </artifact>
      <artifact>
        <path>distingNT_API/include/distingnt/wav.h</path>
        <kind>header</kind>
        <symbol>NT_readSampleFrames, _NT_wavRequest</symbol>
        <lines>full file</lines>
        <reason>WAV API for loading samples from SD card. Required for loadSamples() implementation.</reason>
      </artifact>
    </code>
    <dependencies>
      <distingNT_API>
        <version>v10+ (after Story 3.1)</version>
        <note>Requires wav.h header for NT_readSampleFrames API</note>
      </distingNT_API>
      <story>
        <id>3.1</id>
        <note>API submodule must be updated first</note>
      </story>
      <story>
        <id>3.2</id>
        <note>WAV files must exist on SD card</note>
      </story>
    </dependencies>
  </artifacts>

  <constraints>
    <constraint>Memory allocation via NT_globals.getMemory() only - no malloc/new</constraint>
    <constraint>DRAM for sample storage (large buffer, slower access acceptable)</constraint>
    <constraint>Must handle async callback pattern from NT_readSampleFrames</constraint>
    <constraint>Zero-initialization if samples not available</constraint>
    <constraint>Desktop builds may need conditional filesystem loading for nt_emu testing</constraint>
  </constraints>

  <interfaces>
    <interface>
      <name>SampleManager::init</name>
      <kind>method</kind>
      <signature>void init(int16_t* dramBuffer)</signature>
      <path>src/sample_manager.h (new)</path>
    </interface>
    <interface>
      <name>SampleManager::loadSamples</name>
      <kind>method</kind>
      <signature>bool loadSamples()</signature>
      <path>src/sample_manager.h (new)</path>
    </interface>
    <interface>
      <name>SampleManager::getSampleData</name>
      <kind>method</kind>
      <signature>const int16_t* getSampleData() const</signature>
      <path>src/sample_manager.h (new)</path>
    </interface>
    <interface>
      <name>SampleManager::getNoiseSample</name>
      <kind>method</kind>
      <signature>const int16_t* getNoiseSample() const</signature>
      <path>src/sample_manager.h (new)</path>
    </interface>
    <interface>
      <name>SampleManager::getBoundaries</name>
      <kind>method</kind>
      <signature>const size_t* getBoundaries() const</signature>
      <path>src/sample_manager.h (new)</path>
    </interface>
  </interfaces>

  <tests>
    <standards>Desktop testing in nt_emu. Unit testing via mock data or conditional filesystem loading. Hardware testing on actual disting NT with SD card.</standards>
    <locations>
      - src/sample_manager.h (new)
      - src/sample_manager.cpp (new)
      - VCV Rack nt_emu (integration testing)
    </locations>
    <ideas>
      <idea acRef="1">Verify files compile and link without errors</idea>
      <idea acRef="2">Check DRAM allocation size matches kTotalDramBytes</idea>
      <idea acRef="3">Load plugin, verify samples load during construct (may need debug logging)</idea>
      <idea acRef="4,5">Verify accessors return valid pointers after successful load</idea>
      <idea acRef="6">Test with SD card unmounted, verify zeroed buffers returned</idea>
      <idea acRef="7">Test SD card removal during operation, verify graceful handling</idea>
    </ideas>
  </tests>
</story-context>
