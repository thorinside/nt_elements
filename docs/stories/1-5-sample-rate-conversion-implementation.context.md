<story-context id="1-5-sample-rate-conversion-implementation" v="1.0">
  <metadata>
    <epicId>1</epicId>
    <storyId>5</storyId>
    <title>Sample Rate Conversion Implementation</title>
    <status>drafted</status>
    <generatedAt>2025-10-26</generatedAt>
    <generator>BMAD Story Context Workflow</generator>
    <sourceStoryPath>docs/stories/1-5-sample-rate-conversion-implementation.md</sourceStoryPath>
  </metadata>

  <story>
    <asA>developer</asA>
    <iWant>all filter coefficients recalculated for 48kHz operation</iWant>
    <soThat>synthesis sounds correct at disting NT's sample rate</soThat>
    <tasks>
- [ ] Analyze Elements sample rate dependencies (AC: #1)
  - [ ] Review Elements DSP code to identify filter coefficient calculations
  - [ ] Document which components depend on sample rate (resonator, filters, envelopes)
  - [ ] Identify Init() functions that accept sample rate parameters
  - [ ] Note any hardcoded 32kHz assumptions
- [ ] Implement 48kHz initialization path (AC: #1)
  - [ ] Modify Elements Part::Init() call to use 48000 instead of 32000
  - [ ] Verify all filter coefficients are recalculated correctly
  - [ ] Check resonator frequency calculations scale properly
  - [ ] Ensure envelope timings remain consistent
- [ ] Create test MIDI note sweep (AC: #3, #4)
  - [ ] Generate MIDI test sequence: C1 (36) through C6 (84) in semitone steps
  - [ ] Implement test harness to send notes and capture output
  - [ ] Record frequency measurements for each note
  - [ ] Document expected vs actual frequencies
- [ ] Measure frequency response at 48kHz (AC: #2)
  - [ ] Set up spectral analysis tools in nt_emu
  - [ ] Excite resonator at known frequencies
  - [ ] Capture FFT of output for each test frequency
  - [ ] Compare measured peaks to expected frequencies
  - [ ] Verify resonator Q-factors remain consistent
- [ ] Validate pitch accuracy (AC: #3, #4)
  - [ ] Send MIDI note C4 (60), measure fundamental frequency (should be 261.63 Hz)
  - [ ] Test octaves: C1, C2, C3, C4, C5, C6
  - [ ] Verify each octave is 2x previous frequency
  - [ ] Check chromatic scale notes for accurate semitone spacing
  - [ ] Document any detuning > 1 cent
- [ ] A/B comparison with 32kHz mode (AC: #5)
  - [ ] Record identical patch at 32kHz (native Elements rate)
  - [ ] Record same patch at 48kHz (converted)
  - [ ] Perform spectral comparison of recordings
  - [ ] Listen for timbral differences (brightness, resonance character)
  - [ ] Document expected differences vs unexpected artifacts
- [ ] Desktop validation in nt_emu (AC: #2-5)
  - [ ] Run pitch accuracy tests in nt_emu
  - [ ] Verify no audio glitches or instability
  - [ ] Test rapid note changes for stability
  - [ ] Check CPU usage remains reasonable
  - [ ] Document any issues discovered
- [ ] Document sample rate conversion approach (AC: #1-5)
  - [ ] Explain coefficient scaling methodology
  - [ ] Document measured frequency accuracy
  - [ ] Note any compromises or limitations
  - [ ] Add comments to code explaining 48kHz initialization
    </tasks>
  </story>

  <acceptanceCriteria>
1. All Elements filter coefficients scaled from 32kHz to 48kHz
2. Frequency response measurements match expected values
3. Pitch tracking accurate across MIDI note range (tested C1-C6)
4. Resonator frequencies align with MIDI notes (no detuning)
5. A/B comparison shows no unintended timbral changes
  </acceptanceCriteria>

  <artifacts>
    <docs>
      <doc>
        <path>docs/PRD.md</path>
        <title>nt_elements Product Requirements Document</title>
        <section>FR014: Process audio at 48kHz sample rate</section>
        <snippet>Process audio at 48kHz sample rate with proper filter coefficient conversion from Elements' 32kHz</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>ADR-002: Runtime Sample Rate Mode Toggle</section>
        <snippet>Decision: Runtime parameter toggle between 32kHz and 48kHz modes. Elements runs at 32kHz, disting NT at 48kHz. User can A/B test modes without reloading plugin. Mode switch happens at Init() (not per-sample overhead).</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Sample Rate - Dual Mode Support</section>
        <snippet>Elements DSP was designed for 32kHz operation. disting NT runs at 48kHz. Runtime parameter with Init() implementation allows user choice: 32kHz (lower CPU) or 48kHz (better NT integration). Elements Part::Init(sample_rate) accepts sample rate parameter and recalculates all filter coefficients.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Implementation Patterns - Parameter Update Pattern</section>
        <snippet>Sample rate mode switch example: if (pThis->v[kParamSampleRateMode] == MODE_48KHZ) { pThis->elements_part->Init(48000); } else { pThis->elements_part->Init(32000); }</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Testing Pattern - Desktop-first workflow</section>
        <snippet>Develop and iterate on desktop using nt_emu (make test → plugins/nt_elements.dylib). Once feature works, validate on hardware (make hardware → plugins/nt_elements.o). Profile performance on hardware only.</snippet>
      </doc>
      <doc>
        <path>docs/epics.md</path>
        <title>nt_elements Epic Breakdown</title>
        <section>Story 1.5: Sample Rate Conversion Implementation</section>
        <snippet>As a developer, I want all filter coefficients recalculated for 48kHz operation, so that synthesis sounds correct at disting NT's sample rate. AC: All Elements filter coefficients scaled from 32kHz to 48kHz, frequency response measurements match expected values, pitch tracking accurate across MIDI note range (C1-C6), resonator frequencies align with MIDI notes (no detuning), A/B comparison shows no unintended timbral changes.</snippet>
      </doc>
      <doc>
        <path>docs/stories/1-5-sample-rate-conversion-implementation.md</path>
        <title>Story 1.5: Sample Rate Conversion Implementation</title>
        <section>Dev Notes - Sample Rate Conversion Strategy</section>
        <snippet>Elements provides Part::Init(sample_rate) that accepts sample rate parameter. Calling Init(48000) instead of Init(32000) recalculates all filter coefficients. This is runtime initialization, not compile-time preprocessing. No manual coefficient adjustment needed - Elements handles it internally. Critical components affected: resonator filters, exciter filters, envelopes, reverb.</snippet>
      </doc>
      <doc>
        <path>docs/stories/1-5-sample-rate-conversion-implementation.md</path>
        <title>Story 1.5: Sample Rate Conversion Implementation</title>
        <section>Dev Notes - Frequency Response Validation</section>
        <snippet>Use spectral analysis (FFT) to measure actual output frequencies. Compare to expected MIDI note frequencies using formula: f = 440 * 2^((note - 69)/12). Acceptable tolerance: &lt; 1 cent deviation (&lt; 0.06% frequency error). Test coverage: Low range C1 (32.70 Hz), mid range C4 (261.63 Hz), high range C6 (1046.50 Hz), chromatic scale for semitone spacing.</snippet>
      </doc>
      <doc>
        <path>docs/stories/1-5-sample-rate-conversion-implementation.md</path>
        <title>Story 1.5: Sample Rate Conversion Implementation</title>
        <section>Dev Notes - A/B Comparison Considerations</section>
        <snippet>Expected differences: None - if implementation is correct, 48kHz should sound identical. Slightly higher frequency headroom (Nyquist: 24kHz vs 16kHz). Unexpected differences would indicate bugs: pitch drift, loss of brightness, resonance character changes, instability.</snippet>
      </doc>
      <doc>
        <path>docs/stories/1-5-sample-rate-conversion-implementation.md</path>
        <title>Story 1.5: Sample Rate Conversion Implementation</title>
        <section>Dev Notes - Technical Implementation Details</section>
        <snippet>Elements Init() call: elements_part->Init(48000); // 48kHz mode. Elements handles coefficient scaling internally. Filter cutoff frequencies, resonator modes, and envelope timings are all recalculated based on sample rate parameter. No manual scaling required.</snippet>
      </doc>
    </docs>
    <code>
      <artifact>
        <path>external/mutable-instruments/elements/dsp/part.h</path>
        <kind>header</kind>
        <symbol>Part::Init</symbol>
        <lines>unknown</lines>
        <reason>Elements Part class Init() method accepts sample_rate parameter and recalculates all filter coefficients. This is the primary interface for implementing 48kHz support.</reason>
      </artifact>
      <artifact>
        <path>external/mutable-instruments/elements/dsp/part.cc</path>
        <kind>implementation</kind>
        <symbol>Part::Init</symbol>
        <lines>unknown</lines>
        <reason>Implementation of sample rate initialization logic. Need to verify this correctly scales all filter coefficients, resonator frequencies, envelope timings, and reverb parameters.</reason>
      </artifact>
      <artifact>
        <path>external/mutable-instruments/elements/dsp/resonator.h</path>
        <kind>header</kind>
        <symbol>Resonator class</symbol>
        <lines>unknown</lines>
        <reason>Modal resonator filter bank - critical component affected by sample rate. Frequencies and Q-factors must scale correctly to maintain pitch accuracy.</reason>
      </artifact>
      <artifact>
        <path>external/mutable-instruments/elements/dsp/exciter.h</path>
        <kind>header</kind>
        <symbol>Exciter class</symbol>
        <lines>unknown</lines>
        <reason>Bow/blow/strike shaping filters - sample rate dependent. Need to verify exciter filter coefficients scale correctly.</reason>
      </artifact>
      <artifact>
        <path>src/nt_elements.cpp</path>
        <kind>implementation</kind>
        <symbol>construct callback</symbol>
        <lines>unknown</lines>
        <reason>Plugin construct() callback where Elements Part::Init() will be called. This is where sample rate (48000 vs 32000) will be specified.</reason>
      </artifact>
    </code>
    <dependencies>
      <cpp>
        <package>arm-none-eabi-gcc</package>
        <version>latest</version>
        <purpose>ARM cross-compiler for Cortex-M7 target</purpose>
      </cpp>
      <submodules>
        <package>distingNT_API</package>
        <repo>https://github.com/expertsleepersltd/distingNT_API.git</repo>
        <purpose>Expert Sleepers disting NT plugin API</purpose>
      </submodules>
      <submodules>
        <package>mutable-instruments/elements</package>
        <repo>https://github.com/pichenettes/eurorack.git</repo>
        <purpose>Mutable Instruments Elements DSP source (32kHz native)</purpose>
      </submodules>
      <tools>
        <package>VCV Rack + nt_emu</package>
        <purpose>Desktop testing environment for plugin development</purpose>
      </tools>
      <tools>
        <package>FFT/spectral analysis tools</package>
        <purpose>Frequency response measurement and validation</purpose>
      </tools>
    </dependencies>
  </artifacts>

  <constraints>
    <constraint>Elements DSP source code is read-only (git submodule at external/mutable-instruments) - never modify Elements source files directly</constraint>
    <constraint>All customization must be in adapter layer (src/nt_elements.cpp and related files)</constraint>
    <constraint>Sample rate conversion must be runtime initialization via Elements Part::Init(sample_rate), not compile-time preprocessing</constraint>
    <constraint>No manual coefficient adjustment allowed - Elements handles all scaling internally through Init()</constraint>
    <constraint>Desktop-first testing workflow: develop in nt_emu, validate on hardware at milestones</constraint>
    <constraint>Target &lt; 30% CPU usage on disting NT hardware (ARM Cortex-M7 @ 480MHz)</constraint>
    <constraint>Frequency accuracy requirement: &lt; 1 cent deviation (&lt; 0.06% frequency error)</constraint>
    <constraint>A/B comparison must show no unintended timbral changes between 32kHz and 48kHz modes</constraint>
    <constraint>Build system: ARM GCC with Cortex-M7 optimization (-Os, -mcpu=cortex-m7, -mfpu=fpv5-d16)</constraint>
    <constraint>Memory allocation must use NT_globals.getMemory() with appropriate region (kNT_memDTC, kNT_memSRAM, kNT_memDRAM)</constraint>
  </constraints>

  <interfaces>
    <interface>
      <name>Elements Part::Init</name>
      <kind>C++ method signature</kind>
      <signature>void Part::Init(uint32_t sample_rate)</signature>
      <path>external/mutable-instruments/elements/dsp/part.h</path>
      <description>Primary interface for initializing Elements DSP at specified sample rate. Accepts sample rate parameter (32000 or 48000) and recalculates all filter coefficients, resonator frequencies, envelope timings, and reverb parameters accordingly.</description>
    </interface>
    <interface>
      <name>Elements Part::Process</name>
      <kind>C++ method signature</kind>
      <signature>void Part::Process(const Patch&amp; patch, const float* in, float* out, size_t size)</signature>
      <path>external/mutable-instruments/elements/dsp/part.h</path>
      <description>Main audio processing method. Processes size samples from in buffer to out buffer using current Patch parameters. Must be called after Init() with correct sample rate.</description>
    </interface>
    <interface>
      <name>distingNT construct callback</name>
      <kind>C callback function</kind>
      <signature>_NT_algorithm* construct(void)</signature>
      <path>distingNT_API/include/distingNT.h</path>
      <description>Plugin initialization callback where memory allocation and Elements Part::Init() will occur. This is where sample rate (48000) will be specified for this story.</description>
    </interface>
    <interface>
      <name>distingNT step callback</name>
      <kind>C callback function</kind>
      <signature>void step(_NT_algorithm* self, float* busFrames, int numFramesBy4)</signature>
      <path>distingNT_API/include/distingNT.h</path>
      <description>Audio processing callback called every buffer. Elements Part::Process() will be invoked here. Note: numFrames = numFramesBy4 * 4 (must multiply by 4).</description>
    </interface>
    <interface>
      <name>MIDI note to frequency conversion</name>
      <kind>Algorithm</kind>
      <signature>frequency_hz = 440.0f * powf(2.0f, (midi_note - 69) / 12.0f)</signature>
      <path>N/A - mathematical formula</path>
      <description>Standard MIDI note to frequency conversion. Note 69 = A4 = 440 Hz. Used for validation: given MIDI note, calculate expected frequency and compare to measured FFT output.</description>
    </interface>
  </interfaces>

  <tests>
    <standards>
This project follows a desktop-first testing pattern using VCV Rack + nt_emu for rapid iteration, with hardware validation at key milestones. For this story, testing focuses on frequency response validation, pitch accuracy measurement, and A/B comparison between 32kHz and 48kHz modes. Testing approach combines automated frequency measurement (FFT/spectral analysis) with subjective listening tests. Acceptable tolerance is &lt; 1 cent pitch deviation (&lt; 0.06% frequency error). No formal unit test framework is used; validation is primarily integration testing in nt_emu and hardware testing on actual disting NT.
    </standards>
    <locations>
      <location>plugins/nt_elements.dylib - Desktop test build loaded in VCV Rack nt_emu module</location>
      <location>plugins/nt_elements.o - Hardware build deployed to disting NT SD card</location>
    </locations>
    <ideas>
      <test ac="1">
        <description>Verify Elements Part::Init(48000) successfully recalculates filter coefficients</description>
        <approach>Code inspection of Elements source to confirm Init() handles sample rate parameter correctly. Run plugin in nt_emu with 48kHz mode and verify no crashes or audio artifacts.</approach>
      </test>
      <test ac="2">
        <description>Measure frequency response at known test frequencies using FFT</description>
        <approach>Generate MIDI notes at octave intervals (C1, C2, C3, C4, C5, C6). Record output in nt_emu. Perform FFT analysis to measure fundamental frequency. Compare measured vs expected using MIDI note to frequency formula. Document results in table.</approach>
      </test>
      <test ac="3">
        <description>Validate pitch tracking accuracy across MIDI note range C1-C6</description>
        <approach>Send MIDI notes from C1 (36) through C6 (84) in semitone steps. For each note, measure fundamental frequency via FFT. Calculate cent deviation: cents = 1200 * log2(measured_freq / expected_freq). Verify all notes &lt; 1 cent deviation.</approach>
      </test>
      <test ac="4">
        <description>Verify resonator frequencies align with MIDI notes (no detuning)</description>
        <approach>Test chromatic scale to verify semitone spacing follows 2^(1/12) ratio (1.05946). Test octaves to verify 2x frequency doubling. Send rapid note sequences to test stability. Listen for beating or detuning artifacts.</approach>
      </test>
      <test ac="5">
        <description>A/B comparison between 32kHz and 48kHz modes for timbral changes</description>
        <approach>Configure identical patch parameters. Record same MIDI sequence in both 32kHz mode (native Elements) and 48kHz mode (converted). Perform spectral analysis overlay to compare frequency content. Blind listening test to detect timbral differences. Document expected vs unexpected differences.</approach>
      </test>
    </ideas>
  </tests>
</story-context>
