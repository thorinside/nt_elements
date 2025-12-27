<story-context id="bmad/bmm/workflows/4-implementation/story-context/template" v="1.0">
  <metadata>
    <epicId>3</epicId>
    <storyId>2</storyId>
    <title>Sample Extraction Script</title>
    <status>ready-for-dev</status>
    <generatedAt>2025-12-25</generatedAt>
    <generator>BMAD Story Context Workflow</generator>
    <sourceStoryPath>docs/stories/3-2-sample-extraction-script.md</sourceStoryPath>
  </metadata>

  <story>
    <asA>developer</asA>
    <iWant>a script that extracts the static sample arrays into WAV files</iWant>
    <soThat>samples can be loaded from SD card at runtime</soThat>
    <tasks>
      - Create Python extraction script (scripts/extract_samples.py)
      - Parse smp_boundaries[] and smp_sample_data[] arrays from resources.cc
      - Write 9 wavetable WAV files + 1 noise WAV file
      - Add idempotency check and --force flag
      - Update .gitignore and Makefile with extract-samples target
    </tasks>
  </story>

  <acceptanceCriteria>
    <criterion id="1">Python script reads resources.cc or resources_minimal_wavetables.cc</criterion>
    <criterion id="2">Extracts smp_sample_data into 9 separate WAV files (one per wavetable region)</criterion>
    <criterion id="3">Extracts smp_noise_sample into 1 WAV file</criterion>
    <criterion id="4">Output files: samples/elements/wavetable_00.wav through wavetable_08.wav plus noise.wav</criterion>
    <criterion id="5">WAV format: mono, 16-bit PCM, 48kHz</criterion>
    <criterion id="6">Script is idempotent (skips extraction if files exist)</criterion>
    <criterion id="7">samples/elements/ added to .gitignore</criterion>
    <criterion id="8">Makefile target extract-samples added</criterion>
  </acceptanceCriteria>

  <artifacts>
    <docs>
      <doc>
        <path>docs/epic-3-dynamic-sample-loading.md</path>
        <title>Epic 3 Detail Document</title>
        <section>Story 3.2 - Data Mapping</section>
        <snippet>Documents exact boundary offsets: wavetable_00 (0-17098), wavetable_01 (17099-20851), through wavetable_08 (117606-128012). Noise sample: 0-40962.</snippet>
      </doc>
    </docs>
    <code>
      <artifact>
        <path>src/resources_minimal_wavetables.cc</path>
        <kind>source</kind>
        <symbol>smp_sample_data, smp_noise_sample, smp_boundaries</symbol>
        <lines>1-100</lines>
        <reason>Source file containing static sample arrays to extract. Contains LUTs and sample data.</reason>
      </artifact>
      <artifact>
        <path>external/mutable-instruments/elements/resources/resources.cc</path>
        <kind>source</kind>
        <symbol>smp_sample_data, smp_noise_sample</symbol>
        <lines>full file</lines>
        <reason>Original Elements resources with full wavetable data (372KB). Alternative source for extraction.</reason>
      </artifact>
      <artifact>
        <path>Makefile</path>
        <kind>build</kind>
        <symbol>SOURCES</symbol>
        <lines>18-31</lines>
        <reason>Build configuration. Will add extract-samples target.</reason>
      </artifact>
      <artifact>
        <path>.gitignore</path>
        <kind>config</kind>
        <symbol>n/a</symbol>
        <lines>full file</lines>
        <reason>Will add samples/elements/ to ignore extracted WAV files.</reason>
      </artifact>
    </code>
    <dependencies>
      <python>
        <package>wave</package>
        <version>stdlib</version>
        <note>Python standard library for WAV file writing</note>
      </python>
      <python>
        <package>struct</package>
        <version>stdlib</version>
        <note>Python standard library for binary data packing</note>
      </python>
    </dependencies>
  </artifacts>

  <constraints>
    <constraint>WAV files must be 16-bit PCM, mono, 48kHz (disting NT native rate)</constraint>
    <constraint>Script must parse C++ array syntax correctly</constraint>
    <constraint>Extracted files go to samples/elements/ directory</constraint>
    <constraint>This story can run in parallel with Story 3.1</constraint>
  </constraints>

  <interfaces>
    <interface>
      <name>smp_boundaries</name>
      <kind>array</kind>
      <signature>const size_t smp_boundaries[10]</signature>
      <path>external/mutable-instruments/elements/resources/resources.h</path>
    </interface>
    <interface>
      <name>smp_sample_data</name>
      <kind>array</kind>
      <signature>const int16_t smp_sample_data[128013]</signature>
      <path>external/mutable-instruments/elements/resources/resources.cc</path>
    </interface>
    <interface>
      <name>smp_noise_sample</name>
      <kind>array</kind>
      <signature>const int16_t smp_noise_sample[40963]</signature>
      <path>external/mutable-instruments/elements/resources/resources.cc</path>
    </interface>
  </interfaces>

  <tests>
    <standards>Script testing via command line execution. Verify WAV files are valid and playable. Verify idempotency by running twice.</standards>
    <locations>
      - scripts/extract_samples.py (new file)
      - samples/elements/*.wav (output)
    </locations>
    <ideas>
      <idea acRef="1,2,3">Run script, verify all 10 WAV files created</idea>
      <idea acRef="4">Check filenames match expected pattern</idea>
      <idea acRef="5">Open WAV files in audio editor, verify format (48kHz, 16-bit, mono)</idea>
      <idea acRef="6">Run script twice, verify second run skips extraction</idea>
      <idea acRef="6">Run with --force, verify files are regenerated</idea>
      <idea acRef="7">Verify git status shows samples/elements/ not tracked</idea>
      <idea acRef="8">Run make extract-samples, verify target works</idea>
    </ideas>
  </tests>
</story-context>
