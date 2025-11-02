<story-context id="bmad/bmm/workflows/4-implementation/story-context/template" v="1.0">
  <metadata>
    <epicId>2</epicId>
    <storyId>6</storyId>
    <title>Sound Quality A/B Validation</title>
    <status>drafted</status>
    <generatedAt>2025-10-26</generatedAt>
    <generator>BMAD Story Context Workflow</generator>
    <sourceStoryPath>docs/stories/2-6-sound-quality-a-b-validation.md</sourceStoryPath>
  </metadata>

  <story>
    <asA>user</asA>
    <iWant>nt_elements to sound identical to hardware Elements</iWant>
    <soThat>I get authentic modal synthesis</soThat>
    <tasks>
- [ ] Prepare test environment (AC: #1)
  - [ ] Obtain hardware Elements module (borrow or access)
  - [ ] Set up recording interface for both hardware and NT
  - [ ] Prepare identical MIDI test sequences
  - [ ] Configure identical parameter settings on both
  - [ ] Document test setup and methodology

- [ ] Create test patches (AC: #1, #5, #6)
  - [ ] Test Patch 1: Bow exciter, string geometry
  - [ ] Test Patch 2: Blow exciter, tube geometry
  - [ ] Test Patch 3: Strike exciter, metallic geometry
  - [ ] Test Patch 4: Mixed exciters, mid-range geometry
  - [ ] Test Patch 5: Extreme settings (edge cases)
  - [ ] Document parameter values for each test patch

- [ ] Record hardware Elements (AC: #1)
  - [ ] Load each test patch on hardware Elements
  - [ ] Record MIDI test sequences (C2, C4, C6 notes)
  - [ ] Record parameter sweeps (geometry, brightness, damping)
  - [ ] Save recordings with clear labels
  - [ ] Document recording settings (sample rate, bit depth)

- [ ] Record nt_elements (AC: #1)
  - [ ] Load identical patches on nt_elements
  - [ ] Record identical MIDI test sequences
  - [ ] Record identical parameter sweeps
  - [ ] Use same recording settings as hardware
  - [ ] Save recordings with matching labels

- [ ] Perform spectral analysis (AC: #2)
  - [ ] Load recordings into spectral analysis software
  - [ ] Generate FFT plots for each recording pair
  - [ ] Overlay hardware and nt_elements spectra
  - [ ] Measure frequency peaks (should match)
  - [ ] Measure harmonic content (should match)
  - [ ] Document spectral comparison results

- [ ] Validate pitch accuracy (AC: #4)
  - [ ] Measure fundamental frequency for C2 (65.41 Hz)
  - [ ] Measure fundamental frequency for C4 (261.63 Hz)
  - [ ] Measure fundamental frequency for C6 (1046.50 Hz)
  - [ ] Compare hardware vs nt_elements frequencies
  - [ ] Verify accuracy within 1 cent deviation
  - [ ] Document pitch accuracy measurements

- [ ] Validate exciter modes (AC: #5)
  - [ ] Compare bow exciter recordings (hardware vs nt_elements)
  - [ ] Compare blow exciter recordings
  - [ ] Compare strike exciter recordings
  - [ ] Listen for timbral differences
  - [ ] Analyze attack/decay characteristics
  - [ ] Document exciter mode comparison results

- [ ] Validate resonator response (AC: #6)
  - [ ] Compare recordings across geometry range (0%, 50%, 100%)
  - [ ] Compare recordings across brightness range
  - [ ] Compare recordings across damping range
  - [ ] Verify material transitions sound identical
  - [ ] Verify decay characteristics match
  - [ ] Document resonator comparison results

- [ ] Conduct blind listening tests (AC: #3)
  - [ ] Recruit 3+ beta testers with Elements experience
  - [ ] Prepare blind test: randomly ordered recording pairs
  - [ ] Ask testers to identify hardware vs nt_elements
  - [ ] Collect feedback on sound authenticity
  - [ ] Analyze results (should be indistinguishable)
  - [ ] Document beta tester feedback

- [ ] Identify and document differences (AC: #7)
  - [ ] List any sonic differences discovered
  - [ ] Explain root cause of each difference
  - [ ] Assess whether differences are acceptable
  - [ ] Note any sample rate or platform-specific effects
  - [ ] Document known limitations

- [ ] Address significant differences (AC: #2, #5, #6)
  - [ ] If pitch inaccurate: adjust frequency calculations
  - [ ] If timbre different: review parameter mappings
  - [ ] If resonator different: check filter coefficients
  - [ ] Re-record and re-test after fixes
  - [ ] Verify fixes resolve differences

- [ ] Create sound quality validation report (AC: #1-7)
  - [ ] Document test methodology
  - [ ] Present spectral analysis results
  - [ ] Present pitch accuracy measurements
  - [ ] Present beta tester feedback
  - [ ] Document any differences and explanations
  - [ ] Conclude validation results (pass/fail)
    </tasks>
  </story>

  <acceptanceCriteria>
1. Side-by-side recordings captured (hardware Elements vs nt_elements)
2. Spectral analysis shows matching frequency response
3. At least 3 beta testers confirm sound authenticity in blind tests
4. Pitch accuracy validated across full MIDI range
5. All exciter modes (bow/blow/strike) behave identically
6. Resonator response matches hardware across geometry settings
7. Any differences documented with explanation
  </acceptanceCriteria>

  <artifacts>
    <docs>
      <doc>
        <path>docs/PRD.md</path>
        <title>nt_elements Product Requirements Document</title>
        <section>Goals</section>
        <snippet>Goal #3: Preserve 100% sound authenticity - Indistinguishable from hardware Elements in A/B testing. This establishes authentic modal synthesis as the project's quality benchmark.</snippet>
      </doc>
      <doc>
        <path>docs/PRD.md</path>
        <title>nt_elements Product Requirements Document</title>
        <section>Non-Functional Requirements</section>
        <snippet>NFR002: Sound Quality - Audio output must be indistinguishable from hardware Elements module in A/B comparison testing. Frequency response, timbral characteristics, and synthesis behavior must match original within measurable tolerances. Verified via spectral analysis and blind listening tests with Elements owners.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Performance Considerations - Audio Quality</section>
        <snippet>Target: 100% sound authenticity vs hardware Elements. Validation via A/B comparison with hardware Elements (Story 2.6), spectral analysis of identical patches, frequency response measurements, and blind listening tests. Elements DSP used unmodified (submodule) to preserve authenticity.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>ADR-001: Use Header-Only Integration for Elements DSP</section>
        <snippet>Decision to compile Elements .cc files directly enables link-time optimization and preserves original source in read-only submodule, ensuring 100% authentic sound transfer from hardware to plugin.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>ADR-002: Runtime Sample Rate Mode Toggle</section>
        <snippet>Users can toggle between 32kHz (Elements native) and 48kHz (NT native) modes, enabling A/B testing of sample rate effects on sound quality without reloading plugin.</snippet>
      </doc>
      <doc>
        <path>docs/epics.md</path>
        <title>Epic Breakdown</title>
        <section>Story 2.6: Sound Quality A/B Validation</section>
        <snippet>Prerequisites: Story 2.4 (factory presets for testing). Validates that nt_elements sounds identical to hardware Elements through side-by-side recordings, spectral analysis, beta tester blind tests, pitch accuracy validation across MIDI range, and exciter/resonator mode verification.</snippet>
      </doc>
      <doc>
        <path>docs/stories/2-6-sound-quality-a-b-validation.md</path>
        <title>Story 2.6: Sound Quality A/B Validation</title>
        <section>Dev Notes - Sound Quality Validation Goals</section>
        <snippet>Primary Goal: nt_elements should sound indistinguishable from hardware Mutable Instruments Elements module in A/B comparison. Acceptable differences include minor noise floor differences and sample rate differences. Unacceptable differences include pitch drift, timbral changes, missing harmonics, or different attack/decay characteristics.</snippet>
      </doc>
      <doc>
        <path>docs/stories/2-6-sound-quality-a-b-validation.md</path>
        <title>Story 2.6: Sound Quality A/B Validation</title>
        <section>Dev Notes - Test Environment Setup</section>
        <snippet>Hardware Requirements: Mutable Instruments Elements module (hardware), disting NT running nt_elements plugin, audio interface for recording both, MIDI controller. Recording Settings: 48kHz or 96kHz, 24-bit, WAV uncompressed, normalize for level matching.</snippet>
      </doc>
      <doc>
        <path>docs/stories/2-6-sound-quality-a-b-validation.md</path>
        <title>Story 2.6: Sound Quality A/B Validation</title>
        <section>Dev Notes - Test Patches Design</section>
        <snippet>Five test patches defined: Test Patch 1 (Bowed String), Test Patch 2 (Blown Pipe), Test Patch 3 (Strike Bell), Test Patch 4 (Mixed Exciters), Test Patch 5 (Extreme Settings). Each patch tests specific exciter modes and resonator geometries with documented parameter values.</snippet>
      </doc>
      <doc>
        <path>docs/stories/2-6-sound-quality-a-b-validation.md</path>
        <title>Story 2.6: Sound Quality A/B Validation</title>
        <section>Dev Notes - Spectral Analysis Methodology</section>
        <snippet>Analysis Tools: Sonic Visualiser, Audacity, MATLAB/Python, or Voxengo SPAN. Procedure: Generate FFT spectra (1024-4096 bins), overlay hardware vs nt_elements spectra, identify frequency peaks, measure peak frequencies and amplitudes. Metrics: fundamental frequency (within 1 cent), harmonic spacing (identical), harmonic amplitude ratios (similar), inharmonic content for metallic timbres.</snippet>
      </doc>
      <doc>
        <path>docs/stories/2-6-sound-quality-a-b-validation.md</path>
        <title>Story 2.6: Sound Quality A/B Validation</title>
        <section>Dev Notes - Blind Listening Test Methodology</section>
        <snippet>Test Design: 10 recording pairs (hardware vs nt_elements), randomized A/B or B/A order. Hypothesis: If nt_elements is authentic, testers should guess randomly (~50% accuracy). Success Criteria: Tester accuracy &lt; 60%, feedback indicates "difficult to tell apart" or "sounds identical". Minimum 3 testers with Elements experience required.</snippet>
      </doc>
      <doc>
        <path>docs/stories/2-6-sound-quality-a-b-validation.md</path>
        <title>Story 2.6: Sound Quality A/B Validation</title>
        <section>Dev Notes - Pitch Accuracy Validation</section>
        <snippet>Measurement: Isolate sustained note portion, use pitch detection or manual FFT, calculate cent deviation = 1200 * log2(measured / expected). Expected frequencies: C2=65.41Hz, C3=130.81Hz, C4=261.63Hz, C5=523.25Hz, C6=1046.50Hz. Tolerance: &lt; 1 cent acceptable, &lt; 0.5 cent excellent.</snippet>
      </doc>
      <doc>
        <path>docs/stories/2-6-sound-quality-a-b-validation.md</path>
        <title>Story 2.6: Sound Quality A/B Validation</title>
        <section>Dev Notes - Sound Quality Validation Report</section>
        <snippet>Report Structure: Executive Summary (pass/fail), Test Methodology, Test Results (pitch accuracy, spectral analysis, exciter mode validation, resonator validation, blind listening tests), Documented Differences, Conclusion. Report confirms if nt_elements achieves authentic Elements sound quality and production readiness.</snippet>
      </doc>
      <doc>
        <path>docs/stories/2-4-factory-preset-library.md</path>
        <title>Story 2.4: Factory Preset Library</title>
        <section>Acceptance Criteria</section>
        <snippet>8 factory presets required covering diverse timbres: Bell Strike, Bowed String, Blown Pipe, Wooden Mallet, Glass Chime, Bass Drum, Ambient Pad, Plucked String. These presets provide validation reference patches for sound quality testing.</snippet>
      </doc>
    </docs>
    <code>
      <artifact>
        <path>No implementation code exists yet</path>
        <kind>N/A</kind>
        <symbol>N/A</symbol>
        <lines>N/A</lines>
        <reason>Story 2.6 is a validation/testing story that primarily involves manual testing, recording, and analysis activities. No code implementation is required - this story validates the audio quality of the already-implemented nt_elements plugin against hardware Elements.</reason>
      </artifact>
    </code>
    <dependencies>
      <note>No code dependencies detected - this is a testing and validation story</note>
      <tooling>
        <tool>
          <name>Sonic Visualiser</name>
          <purpose>Free spectral analysis software for FFT plots and frequency comparison</purpose>
        </tool>
        <tool>
          <name>Audacity</name>
          <purpose>Free audio editor with basic spectrum analysis capabilities</purpose>
        </tool>
        <tool>
          <name>MATLAB/Python</name>
          <purpose>Advanced spectral analysis and frequency measurement (optional)</purpose>
        </tool>
        <tool>
          <name>Voxengo SPAN</name>
          <purpose>Free VST spectrum analyzer plugin</purpose>
        </tool>
        <tool>
          <name>Audio Interface</name>
          <purpose>Record both hardware Elements and disting NT simultaneously or sequentially at 48kHz or 96kHz, 24-bit</purpose>
        </tool>
        <tool>
          <name>MIDI Controller or Sequencer</name>
          <purpose>Send identical MIDI test sequences to both hardware and nt_elements</purpose>
        </tool>
      </tooling>
      <hardware>
        <item>
          <name>Mutable Instruments Elements</name>
          <purpose>Hardware reference module for A/B comparison testing</purpose>
          <requirement>Must obtain or borrow hardware Elements module for validation</requirement>
        </item>
        <item>
          <name>disting NT</name>
          <purpose>Target platform running nt_elements plugin</purpose>
          <requirement>Already available from previous stories</requirement>
        </item>
      </hardware>
    </dependencies>
  </artifacts>

  <constraints>
    <constraint>This is a validation story, not an implementation story - no code development required</constraint>
    <constraint>Requires access to hardware Mutable Instruments Elements module for comparison testing</constraint>
    <constraint>Audio recordings must use identical parameter settings on both hardware and nt_elements</constraint>
    <constraint>Spectral analysis must measure frequency response with sufficient resolution (1024-4096 FFT bins)</constraint>
    <constraint>Pitch accuracy must be validated within 1 cent tolerance (&lt; 0.06% frequency error)</constraint>
    <constraint>Blind listening tests require minimum 3 beta testers with Elements experience</constraint>
    <constraint>Any discovered differences must be documented with root cause analysis</constraint>
    <constraint>Test both 32kHz and 48kHz sample rate modes if discrepancies found</constraint>
    <constraint>Recording settings must be consistent: 48kHz or 96kHz sample rate, 24-bit depth, uncompressed WAV format</constraint>
    <constraint>All acceptance criteria must pass for story completion - if significant differences found, implementation fixes may be required (coordinate with development team)</constraint>
    <constraint>Validation report must conclude with clear pass/fail determination and production readiness assessment</constraint>
  </constraints>

  <interfaces>
    <interface>
      <name>nt_elements Plugin Audio Output</name>
      <kind>Audio Interface</kind>
      <signature>Stereo audio output from disting NT running nt_elements plugin, processed through Elements DSP engine with all exciter modes (bow/blow/strike) and resonator models</signature>
      <path>N/A - Hardware audio interface</path>
    </interface>
    <interface>
      <name>Hardware Elements Audio Output</name>
      <kind>Audio Interface</kind>
      <signature>Stereo audio output from Mutable Instruments Elements Eurorack module, reference implementation for comparison</signature>
      <path>N/A - Hardware audio interface</path>
    </interface>
    <interface>
      <name>MIDI Input</name>
      <kind>Control Interface</kind>
      <signature>MIDI note messages (note-on, note-off, pitch bend) sent to both hardware Elements and nt_elements for synchronized testing</signature>
      <path>N/A - MIDI interface</path>
    </interface>
    <interface>
      <name>Parameter Control</name>
      <kind>Control Interface</kind>
      <signature>Elements synthesis parameters (exciter levels, geometry, brightness, damping, reverb) configured identically on hardware and plugin via respective interfaces</signature>
      <path>N/A - Manual parameter configuration</path>
    </interface>
    <interface>
      <name>Factory Presets</name>
      <kind>Preset System</kind>
      <signature>8 factory presets from Story 2.4 provide reference patches for validation testing: Bell Strike, Bowed String, Blown Pipe, Wooden Mallet, Glass Chime, Bass Drum, Ambient Pad, Plucked String</signature>
      <path>docs/stories/2-4-factory-preset-library.md</path>
    </interface>
  </interfaces>

  <tests>
    <standards>
This story focuses on audio quality validation rather than automated testing. Testing methodology includes: (1) Comparative audio recording - capturing identical test sequences from hardware Elements and nt_elements under identical parameter settings; (2) Spectral analysis - FFT-based frequency response comparison using tools like Sonic Visualiser or MATLAB to verify matching harmonic content and frequency peaks within 1 cent tolerance; (3) Pitch accuracy measurement - validating fundamental frequencies across MIDI range (C2-C6) using pitch detection algorithms; (4) Blind listening tests - recruiting minimum 3 beta testers with Elements experience to perform A/B comparison with randomized sample pairs, targeting &lt;60% identification accuracy (random guess baseline ~50%); (5) Exciter mode validation - comparing bow, blow, and strike exciter recordings for timbral accuracy; (6) Resonator validation - comparing recordings across geometry, brightness, and damping parameter ranges; (7) Documentation - creating detailed validation report with methodology, results, and any identified differences with root cause analysis.
    </standards>
    <locations>
      <location>docs/validation/</location>
      <note>Create new validation directory for storing test results, recordings, spectral analysis plots, and final validation report</note>
    </locations>
    <ideas>
      <idea ac="1">
        <description>Test Environment Preparation</description>
        <approach>Obtain or borrow hardware Mutable Instruments Elements module. Set up audio recording interface capable of capturing both hardware Elements and disting NT (either simultaneously via multi-channel interface or sequentially with consistent settings). Configure MIDI controller or sequencer to send identical test sequences. Document complete test setup including equipment models, signal chain, and configuration.</approach>
      </idea>
      <idea ac="1,5,6">
        <description>Test Patch Creation</description>
        <approach>Create 5 test patches as specified in Dev Notes covering: (1) Bowed String - bow exciter 80%, string geometry 15%, brightness 55%, damping 70%; (2) Blown Pipe - blow exciter 80%, tube geometry 45%, brightness 50%, damping 60%; (3) Strike Bell - strike exciter 85%, metallic geometry 75%, brightness 70%, damping 40%; (4) Mixed Exciters - bow 40%, blow 40%, strike 20%, mid-range geometry 50%; (5) Extreme Settings - strike 100%, geometry 100%, brightness 100%, damping 0%. Document exact parameter values for replication.</approach>
      </idea>
      <idea ac="1,4">
        <description>MIDI Test Sequence Recording</description>
        <approach>Create 4 MIDI test sequences: (1) Pitch Accuracy - C2, C3, C4, C5, C6 notes held 3 seconds each; (2) Chromatic Scale - C4 through C5, 1 second per note; (3) Parameter Sweeps - hold C4 while sweeping geometry 0-100% over 10 seconds, repeat for brightness and damping; (4) Dynamic Response - C4 at velocities 30, 60, 90, 127 held 2 seconds each. Record each sequence on both hardware Elements and nt_elements using all 5 test patches. Save with clear naming convention (e.g., "Hardware_Patch1_PitchAccuracy.wav", "NT_Patch1_PitchAccuracy.wav").</approach>
      </idea>
      <idea ac="2">
        <description>Spectral Analysis Comparison</description>
        <approach>Load recording pairs into spectral analysis software (Sonic Visualiser recommended). Generate FFT spectra with 2048-4096 bins for sufficient frequency resolution. Create overlay plots comparing hardware vs nt_elements for each test. Measure and document: (1) Fundamental frequency (should match within 1 Hz or 1 cent); (2) Harmonic spacing (should be identical); (3) Harmonic amplitude ratios (should be similar within 1-2 dB); (4) Inharmonic content for metallic patches (should match). Export overlay plots as images for validation report. Focus on frequency range 20Hz-20kHz, with particular attention to fundamental and first 10 harmonics.</approach>
      </idea>
      <idea ac="4">
        <description>Pitch Accuracy Validation</description>
        <approach>Use pitch detection algorithm (Python librosa, MATLAB, or Sonic Visualiser pitch track) to measure fundamental frequency of sustained note portions. Calculate cent deviation using formula: cents = 1200 * log2(measured_freq / expected_freq). Expected frequencies: C2=65.41Hz, C3=130.81Hz, C4=261.63Hz, C5=523.25Hz, C6=1046.50Hz. Compare hardware vs nt_elements measurements. Acceptance: &lt;1 cent deviation (excellent: &lt;0.5 cent). If deviation exceeds tolerance, investigate potential causes: (1) MIDI note-to-Hz conversion error, (2) Sample rate handling issue, (3) Oscillator stability problem.</approach>
      </idea>
      <idea ac="5">
        <description>Exciter Mode Validation</description>
        <approach>Focus validation on Test Patches 1-3 which isolate bow, blow, and strike exciters respectively. Critical listening comparison: (1) Bow exciter - smooth sustain characteristics, bowing noise texture, envelope attack/decay; (2) Blow exciter - airy/breathy tone quality, turbulence noise, flutter characteristics; (3) Strike exciter - transient attack sharpness, initial impulse spectrum, decay behavior. Spectral analysis should confirm matching harmonic content and noise characteristics. Load level-matched recording pairs in audio editor, use ABX switching for direct comparison. Document any perceptible timbral differences.</approach>
      </idea>
      <idea ac="6">
        <description>Resonator Validation</description>
        <approach>Analyze parameter sweep recordings (geometry, brightness, damping from 0-100%). Create spectrograms showing parameter evolution over time. Compare hardware vs nt_elements spectrograms for: (1) Geometry sweep - material transitions from string to tube to metallic should occur at same parameter values with matching timbral characteristics; (2) Brightness sweep - filter cutoff modulation should track identically, harmonic content evolution should match; (3) Damping sweep - decay time changes should be proportional, sustain characteristics should match. Measure resonator decay times at key parameter positions (0%, 50%, 100%) using envelope analysis. Verify filter frequency response by identifying formant peaks in sustained notes.</approach>
      </idea>
      <idea ac="3">
        <description>Blind Listening Tests</description>
        <approach>Recruit 3+ beta testers with Mutable Instruments Elements experience (post in disting NT Discord, Mutable Instruments Users group). Prepare blind test: select 10 recording pairs from test patches, randomly order as A/B or B/A. Create anonymous audio files (Test1_A.wav, Test1_B.wav, etc.) with answer key stored separately. Provide testers with instructions: "For each pair, identify which sample is hardware Elements (A or B)". Collect responses and confidence ratings. Calculate accuracy percentage per tester and average. Success criteria: average accuracy &lt;60% (indicates indistinguishable). If accuracy significantly exceeds 60%, conduct follow-up interviews to understand which sonic characteristics enabled identification.</approach>
      </idea>
      <idea ac="7">
        <description>Difference Documentation</description>
        <approach>Systematically document any discovered differences using format: (1) Observation - describe what sounds different; (2) Root Cause - technical explanation (e.g., "48kHz sample rate changes filter response vs 32kHz hardware"); (3) Severity - Minor/Moderate/Major; (4) Acceptable - Yes/No with justification; (5) Action - fix applied or explanation why acceptable. Compare both 32kHz and 48kHz modes if differences detected - some may be sample-rate specific. Example acceptable difference: "Slightly brighter timbre at high brightness settings due to 48kHz sample rate, not perceptible in blind tests." Example unacceptable difference: "Pitch deviation of 5 cents at C2 - requires fix to MIDI note-to-Hz conversion."</approach>
      </idea>
      <idea ac="1-7">
        <description>Validation Report Creation</description>
        <approach>Create detailed markdown report at docs/validation/sound-quality-report.md with structure: (1) Executive Summary - overall pass/fail and key findings; (2) Test Methodology - equipment, setup, test patch specifications, recording settings; (3) Test Results - organized by AC: pitch accuracy table with measurements and deviations, spectral analysis section with overlay plots, exciter mode validation findings, resonator validation findings, blind test results table with tester accuracy percentages and feedback quotes; (4) Documented Differences - list any differences found with full analysis; (5) Conclusion - production readiness determination. Include references to audio recordings and spectral plots. Report should enable independent verification of results and provide confidence in sound quality authenticity.</approach>
      </idea>
    </ideas>
  </tests>
</story-context>
