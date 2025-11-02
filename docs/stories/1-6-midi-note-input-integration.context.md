<story-context id="1-6-midi-note-input-integration" v="1.0">
  <metadata>
    <epicId>1</epicId>
    <storyId>6</storyId>
    <title>MIDI Note Input Integration</title>
    <status>drafted</status>
    <generatedAt>2025-10-26</generatedAt>
    <generator>BMAD Story Context Workflow</generator>
    <sourceStoryPath>docs/stories/1-6-midi-note-input-integration.md</sourceStoryPath>
  </metadata>

  <story>
    <asA>user</asA>
    <iWant>to play notes via MIDI input</iWant>
    <soThat>I can control the pitch of the synthesis</soThat>
    <tasks>
      - [ ] Implement midiMessage() callback (AC: #1, #2)
        - [ ] Add midiMessage() function to NT_algorithm structure
        - [ ] Parse MIDI status byte to identify message type
        - [ ] Extract note number and velocity from data bytes
        - [ ] Handle note-on (0x90) and note-off (0x80) messages
        - [ ] Support running status (note-on with velocity 0 = note-off)

      - [ ] Implement MIDI note to frequency conversion (AC: #3)
        - [ ] Create note-to-Hz conversion function: `f = 440 * 2^((note - 69)/12)`
        - [ ] Update Elements Patch.note field with calculated frequency
        - [ ] Set Elements Patch.gate to true on note-on
        - [ ] Set Elements Patch.gate to false on note-off
        - [ ] Verify tuning against standard MIDI frequency table

      - [ ] Implement monophonic voice management (AC: #5)
        - [ ] Track current_note variable in algorithm state
        - [ ] On note-on: update current_note, set gate, update frequency
        - [ ] On note-off: only turn off gate if note matches current_note
        - [ ] Implement last-note priority (most recent note-on takes precedence)
        - [ ] Handle overlapping note-ons gracefully

      - [ ] Add pitch bend support (AC: #4)
        - [ ] Implement pitch bend message handler (0xE0)
        - [ ] Parse 14-bit pitch bend value from LSB/MSB bytes
        - [ ] Calculate pitch bend amount in semitones (default ±2 semitones)
        - [ ] Apply pitch bend multiplier to base frequency
        - [ ] Smooth pitch bend changes to avoid zipper noise

      - [ ] Test MIDI input in nt_emu (AC: #1-5)
        - [ ] Connect MIDI keyboard to nt_emu
        - [ ] Verify note-on triggers synthesis (hear sound)
        - [ ] Verify note-off stops synthesis (sound ends)
        - [ ] Test pitch accuracy: play C4, measure 261.63 Hz
        - [ ] Test chromatic scale: verify semitone spacing
        - [ ] Test pitch bend: verify smooth frequency modulation

      - [ ] Test rapid note input handling (AC: #5)
        - [ ] Send rapid note-on bursts (10+ notes/second)
        - [ ] Verify no crashes or audio glitches
        - [ ] Verify last-note priority works correctly
        - [ ] Test legato playing (overlapping notes)
        - [ ] Document any latency or responsiveness issues

      - [ ] Validate frequency accuracy (AC: #3)
        - [ ] Play MIDI notes C1 through C6
        - [ ] Measure output frequency with spectral analyzer
        - [ ] Compare measured vs expected frequencies
        - [ ] Verify accuracy within 1 cent (&lt; 0.06% error)
        - [ ] Document any detuning issues

      - [ ] Add debug logging for MIDI events (AC: #1-5)
        - [ ] Log MIDI note-on events (note number, velocity)
        - [ ] Log MIDI note-off events
        - [ ] Log pitch bend changes
        - [ ] Use conditional compilation for desktop-only logging
        - [ ] Verify logging doesn't impact performance
    </tasks>
  </story>

  <acceptanceCriteria>
    1. MIDI note-on messages trigger synthesis
    2. MIDI note-off messages stop synthesis
    3. MIDI pitch maps correctly to synthesized frequency
    4. Pitch bend messages modulate frequency smoothly
    5. Multiple rapid note-ons handled gracefully (monophonic, last-note priority)
  </acceptanceCriteria>

  <artifacts>
    <docs>
      <doc>
        <path>docs/PRD.md</path>
        <title>Product Requirements Document</title>
        <section>FR011: MIDI Note Input</section>
        <snippet>Implement MIDI note input for pitch control with note-on/note-off and pitch bend. Target platform: disting NT Eurorack module with ARM Cortex-M7 @ 480MHz.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>MIDI Handling Pattern</section>
        <snippet>MIDI messages are 3-byte sequences. Note-on: 0x90+channel, note number, velocity. Note-off: 0x80+channel or 0x90 with velocity 0. Conversion: f = 440 * 2^((note - 69)/12). Elements is monophonic with last-note priority.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Data Flow</section>
        <snippet>User input (MIDI/Pots/Encoders) → NT Parameter System → parameterChanged() callback → Adapter: NT params → Elements Patch → step(): Elements::Part::Process() → Audio Output (NT bus system)</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Decision Summary - MIDI</section>
        <snippet>Note Handling: Simple note-to-freq conversion (Epic 1, Story 1.6). Voice Mode: Monophonic, last-note priority (matches hardware Elements). Future Extensions: Velocity, pitch bend deferred to Epic 2+ if time allows.</snippet>
      </doc>
      <doc>
        <path>docs/epics.md</path>
        <title>Epic Breakdown</title>
        <section>Story 1.6: MIDI Note Input Integration</section>
        <snippet>As a user, I want to play notes via MIDI input, so that I can control the pitch of the synthesis. Five acceptance criteria covering note-on/off, pitch mapping, pitch bend, and rapid note handling.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Implementation Patterns - MIDI Handling Pattern</section>
        <snippet>MIDI note to frequency conversion using powf(): freq = 440.0f * powf(2.0f, (note - 69) / 12.0f). Handle status byte 0x90/0x80, track current_note for last-note priority, support running status (velocity 0 = note-off).</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Technology Stack Details</section>
        <snippet>C++11 with ARM GCC, ARM Cortex-M7 @ 480MHz, 48kHz sample rate. Compiler flags: -std=c++11 -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard -Os -fno-rtti -fno-exceptions.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Error Handling - Defensive Programming</section>
        <snippet>No exceptions (compiler flag: -fno-exceptions). Use return codes or sentinel values. Check pointers before dereferencing. Clamp parameter values. On error in step(): Output silence, don't crash.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Logging Strategy</section>
        <snippet>Desktop: #ifdef NT_EMU_DEBUG for printf debugging. Hardware: No printf/console, use OLED for debug info. Minimize debug code in release builds.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Data Architecture - Elements Patch Structure</section>
        <snippet>Elements Patch structure (elements/dsp/patch.h): float note (frequency in Hz), bool gate (note on/off), plus exciter/resonator/reverb parameters.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Performance Considerations</section>
        <snippet>MIDI Processing Cost: Extremely lightweight (&lt; 0.1% CPU). Processing happens outside audio callback. Frequency calculation (powf) only on note changes, not per sample. Target &lt; 30% CPU total.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>ADR-004: Desktop-First Testing Strategy</section>
        <snippet>Develop on desktop (nt_emu), validate on hardware at milestones. Fast iteration with printf debugging. No hardware deploy cycle during development. Hardware testing for performance and final validation.</snippet>
      </doc>
    </docs>

    <code>
      <artifact>
        <path>No existing code - greenfield story</path>
        <kind>none</kind>
        <symbol>N/A</symbol>
        <lines>N/A</lines>
        <reason>This is Story 1.6 in Epic 1. Stories 1.1-1.5 establish foundation (build system, Elements integration, plugin stub, Part instance, sample rate conversion). Story 1.6 adds MIDI handling for the first time. No existing MIDI code to reference - will create midiMessage() callback from scratch.</reason>
      </artifact>
    </code>

    <dependencies>
      <framework>
        <name>distingNT API v9</name>
        <source>Git submodule: https://github.com/expertsleepersltd/distingNT_API</source>
        <version>Latest (submodule pinned)</version>
        <purpose>Plugin API framework providing MIDI callback interface (_NT_algorithm::midiMessage)</purpose>
      </framework>
      <framework>
        <name>Mutable Instruments Elements DSP</name>
        <source>Git submodule: https://github.com/pichenettes/eurorack</source>
        <version>Latest (submodule pinned)</version>
        <license>MIT</license>
        <purpose>Modal synthesis engine. Patch structure with note/gate fields for MIDI control.</purpose>
      </framework>
      <toolchain>
        <name>ARM GCC</name>
        <package>arm-none-eabi-gcc</package>
        <install>brew install arm-none-eabi-gcc</install>
        <purpose>Cross-compiler for ARM Cortex-M7 hardware target</purpose>
      </toolchain>
      <toolchain>
        <name>VCV Rack + nt_emu</name>
        <purpose>Desktop testing environment for rapid iteration before hardware deployment</purpose>
      </toolchain>
      <library>
        <name>C++ Standard Library</name>
        <version>C++11</version>
        <functions>powf() for note-to-frequency conversion</functions>
      </library>
    </dependencies>
  </artifacts>

  <constraints>
    - **No Exceptions**: Compiler flag -fno-exceptions. Use return codes, defensive checks, clamp values.
    - **No RTTI**: Compiler flag -fno-rtti. No dynamic_cast or typeid.
    - **Memory Management**: All allocations via NT_globals.getMemory(). Never use malloc/new.
    - **Memory Regions**: Use appropriate region (DTC for hot state, SRAM for buffers).
    - **Audio Safety**: On error in midiMessage(), fail gracefully. Don't crash. Invalid MIDI = ignore.
    - **Monophonic Only**: Elements is single-voice. Last-note priority for overlapping notes.
    - **Performance**: MIDI processing is lightweight, outside audio callback. Target &lt; 30% total CPU.
    - **Elements DSP Read-Only**: Never modify files in external/mutable-instruments/. All customization in adapter.
    - **Code Organization**: Follow Elements C++ style. PascalCase classes, camelCase functions, snake_case variables.
    - **Testing Strategy**: Desktop-first (nt_emu). Validate on hardware at milestones only.
    - **Logging**: #ifdef NT_EMU_DEBUG for desktop printf. No console on hardware.
    - **MIDI Standard**: Support running status (0x90 velocity 0 = note-off). Handle 0x80 and 0x90 note-offs.
    - **Pitch Bend**: 14-bit value (LSB/MSB), center 8192, range 0-16383. Default ±2 semitones.
    - **Frequency Accuracy**: MIDI note to Hz conversion must be accurate. Verify against standard table.
  </constraints>

  <interfaces>
    <interface>
      <name>_NT_algorithm::midiMessage</name>
      <kind>C++ callback function</kind>
      <signature>void midiMessage(_NT_algorithm* self, uint8_t b0, uint8_t b1, uint8_t b2)</signature>
      <path>distingNT_API/include/distingNT.h</path>
      <description>MIDI message handler callback. b0=status byte, b1=data1, b2=data2. Parse status (0x90/0x80/0xE0), extract note/velocity/pitch bend, update Elements Patch note/gate fields.</description>
    </interface>
    <interface>
      <name>Elements Patch structure</name>
      <kind>C++ struct</kind>
      <signature>struct Patch { float note; bool gate; /* ... other fields */ }</signature>
      <path>external/mutable-instruments/elements/dsp/patch.h</path>
      <description>Elements DSP control structure. note field = frequency in Hz (NOT MIDI note number). gate field = true for note-on, false for note-off.</description>
    </interface>
    <interface>
      <name>MIDI Note to Frequency Formula</name>
      <kind>Math function</kind>
      <signature>float note_to_frequency(uint8_t midi_note) { return 440.0f * powf(2.0f, (midi_note - 69) / 12.0f); }</signature>
      <path>To be implemented in story</path>
      <description>Standard MIDI-to-Hz conversion. A4 (note 69) = 440 Hz. Each semitone = 2^(1/12) ratio.</description>
    </interface>
    <interface>
      <name>MIDI Message Format</name>
      <kind>Protocol specification</kind>
      <signature>Note-On: 0x90+channel, note(0-127), velocity(1-127). Note-Off: 0x80+channel, note, velocity. Pitch Bend: 0xE0+channel, LSB(0-127), MSB(0-127)</signature>
      <path>MIDI 1.0 Specification</path>
      <description>Standard MIDI protocol. Status byte high nibble = message type. Low nibble = channel (0-15). Running status: 0x90 with velocity 0 = note-off.</description>
    </interface>
  </interfaces>

  <tests>
    <standards>
      Desktop-first testing strategy using VCV Rack nt_emu module. Build test target (make test → plugins/nt_elements.dylib), load in nt_emu, test MIDI functionality with virtual MIDI keyboard or hardware controller. Validate note-on/off, frequency accuracy, pitch bend, rapid note handling. Use spectral analysis to verify frequency accuracy. Debug with printf (NT_EMU_DEBUG). Hardware validation at milestone (Story 1.9/1.10) only. No formal unit test framework - manual testing and validation against acceptance criteria.
    </standards>

    <locations>
      Desktop testing: VCV Rack with nt_emu module loading plugins/nt_elements.dylib
      Hardware validation: disting NT loading plugins/nt_elements.o (milestones only)
      Build targets: make test (desktop), make hardware (ARM)
    </locations>

    <ideas>
      <test ac="1">
        <description>Test MIDI Note-On Triggers Synthesis</description>
        <approach>Load plugin in nt_emu. Send MIDI note-on (e.g., C4 = note 60) from virtual keyboard. Verify audio output appears (waveform visible). Listen for modal synthesis sound. Check Patch.gate = true and Patch.note = 261.63 Hz via debug logging.</approach>
      </test>
      <test ac="2">
        <description>Test MIDI Note-Off Stops Synthesis</description>
        <approach>After note-on, send note-off for same note. Verify audio output decays/stops (Elements has natural decay). Check Patch.gate = false via debug logging. Verify gate only turns off if note matches current_note (monophonic behavior).</approach>
      </test>
      <test ac="3">
        <description>Test MIDI Pitch Maps Correctly</description>
        <approach>Send MIDI notes C1(36), C4(60), A4(69), C6(84). Measure output frequency with spectral analyzer. Compare to expected: 32.70Hz, 261.63Hz, 440.00Hz, 1046.50Hz. Verify accuracy within 1 cent (0.06% error). Test chromatic scale (12 semitones), verify 2:1 octave ratio.</approach>
      </test>
      <test ac="4">
        <description>Test Pitch Bend Modulates Frequency</description>
        <approach>Play sustained note. Send pitch bend messages: center (8192), full up (16383), full down (0). Verify frequency modulation: center = no change, up = +2 semitones, down = -2 semitones. Check smoothness (no zipper noise, no clicks). Verify Elements handles smoothing internally.</approach>
      </test>
      <test ac="5">
        <description>Test Rapid Note-Ons and Last-Note Priority</description>
        <approach>Send rapid note-on burst (10+ notes/sec). Verify no crashes, no audio glitches. Test overlapping notes: play C4, before releasing play E4 (legato). Verify E4 becomes current (last-note priority). Release C4, verify E4 still plays. Release E4, verify gate off. Test running status: 0x90 velocity 0 = note-off.</approach>
      </test>
      <test ac="all">
        <description>Debug Logging Verification</description>
        <approach>Enable NT_EMU_DEBUG. Send various MIDI messages. Verify debug output: "MIDI Note On: 60 (261.63 Hz)", "MIDI Note Off: 60", "Pitch Bend: 8192". Confirm logging doesn't impact performance (measure CPU, should be negligible). Verify logging compiled out in release build (hardware).</approach>
      </test>
    </ideas>
  </tests>
</story-context>
