# nt_elements

Modal synthesis for disting NT, based on Mutable Instruments Elements.

## Description

nt_elements brings the iconic Elements modal synthesis engine to the Expert Sleepers disting NT Eurorack module. Experience authentic physical modeling synthesis with 4 parameter pages covering exciters, resonators, reverb, and performance controls.

## Features

### Core Synthesis
- Complete Elements DSP engine (bow/blow/strike exciters)
- 64-filter modal resonator with geometry, brightness, damping controls
- Stereo reverb with separate Main and Aux outputs for flexible routing
- 4 parameter pages: Exciter, Resonator, Space, Performance
- 5th Routing page for I/O and CV configuration

### Connectivity
- Stereo Output: Main Output (bus 13) + Aux Output (bus 14) for reverb spreading
- MIDI Input: Note on/off, pitch bend, channel filtering (1-16 or omni)
- Essential CV Inputs:
  - V/Oct CV: 1V/octave pitch tracking
  - Gate CV: Trigger/gate input
  - FM CV: Dynamic FM modulation (±5V)
  - Brightness CV: Filter cutoff modulation (±5V)
  - Expression CV: Dynamics/velocity control (0-10V)

### Performance
- Adaptive sample rate (32/48/96kHz - matches NT system settings)
- CPU usage < 30% (allows chaining with other algorithms)
- ~207KB hardware build size

## Installation

1. Download `nt_elements.o` from the [releases page](https://github.com/yourusername/nt_elements/releases)
2. Copy the file to your disting NT SD card (root directory or subdirectory)
3. Insert SD card into disting NT
4. Power on disting NT
5. Navigate to file browser
6. Select `nt_elements.o` to load plugin
7. Plugin appears in algorithm list as "nt_elements"

## Requirements

- disting NT Eurorack module
- disting NT firmware version 1.0 or later
- SD card (any file system supported by disting NT)
- MIDI controller (for note input, optional)

## Usage

### Quick Start

1. **Load the plugin** - Select nt_elements from the disting NT file browser
2. **Connect MIDI** - Send MIDI to your configured channel (default: channel 1)
3. **Play notes** - The default patch produces a bowed string sound
4. **Navigate pages** - Press the button to cycle through parameter pages
5. **Adjust parameters** - Turn pots and encoders to shape your sound

### Parameter Pages

nt_elements organizes 23 parameters across 5 pages:

```
┌─────────────────────────────────────┐
│  Page 1: EXCITER                    │
├─────────────────────────────────────┤
│  Bow Level      80%                 │
│  Blow Level      0%                 │
│  Strike Level    0%                 │
│  Bow Timbre     50%                 │
│  Blow Timbre    50%                 │
└─────────────────────────────────────┘
```

**Page 1: Exciter** - Choose your excitation method
- Bow Level: Continuous friction exciter (strings, sustained tones)
- Blow Level: Air pressure exciter (brass, woodwinds)
- Strike Level: Percussive exciter (bells, mallets)
- Bow/Blow Timbre: Shape the character of each exciter

```
┌─────────────────────────────────────┐
│  Page 2: RESONATOR                  │
├─────────────────────────────────────┤
│  Geometry       50% (String)        │
│  Brightness     70%                 │
│  Damping        60%                 │
│  Position       50%                 │
│  Inharmonic      0%                 │
└─────────────────────────────────────┘
```

**Page 2: Resonator** - Shape the physical model
- Geometry: String (0%) ↔ Tube (100%)
- Brightness: Filter cutoff / harmonic content
- Damping: Decay time (0% = infinite sustain, 100% = quick decay)
- Position: Strike/bow position along the resonator
- Inharmonic: Add metallic, bell-like inharmonicity

```
┌─────────────────────────────────────┐
│  Page 3: SPACE                      │
├─────────────────────────────────────┤
│  Reverb Amt     20%                 │
│  Reverb Size    50%                 │
│  Reverb Damp    70%                 │
└─────────────────────────────────────┘
```

**Page 3: Space** - Stereo reverb
- Reverb Amt: Dry/wet mix (sent to Aux Output)
- Reverb Size: Room size (small chamber ↔ large hall)
- Reverb Damp: High frequency absorption

```
┌─────────────────────────────────────┐
│  Page 4: PERFORMANCE                │
├─────────────────────────────────────┤
│  Coarse Tune     0 semitones        │
│  Fine Tune       0 cents            │
│  Output Lvl    100%                 │
│  FM Amount       0%                 │
│  Exciter Cnt    50%                 │
└─────────────────────────────────────┘
```

**Page 4: Performance** - Global controls
- Coarse Tune: ±12 semitones
- Fine Tune: ±50 cents
- Output Lvl: Master volume
- FM Amount: Pitch modulation depth
- Exciter Cnt: Envelope contour (0% = percussive, 100% = sustained)

```
┌─────────────────────────────────────┐
│  Page 5: ROUTING                    │
├─────────────────────────────────────┤
│  MIDI Chan       1                  │
│  V/Oct CV       Bus 0               │
│  Gate CV        Bus 0               │
│  FM CV          Bus 0               │
│  Bright CV      Bus 0               │
│  Expr CV        Bus 0               │
└─────────────────────────────────────┘
```

**Page 5: Routing** - I/O and MIDI configuration
- MIDI Chan: 0 = omni, 1-16 = specific channel
- CV Inputs: Assign buses for pitch, gate, modulation sources

### MIDI Control

Send MIDI notes to trigger synthesis. The plugin responds to:
- Note On/Off: Triggers gate and sets pitch
- Pitch Bend: Continuous pitch modulation
- Channel: Configurable on Page 5 (0=omni, 1-16=specific)

### CV Modulation

Essential CV Inputs (Page 5: Routing):
- V/Oct CV: 1V/octave pitch control
- Gate CV: Trigger/gate input
- FM CV: Frequency modulation (±5V)
- Brightness CV: Filter cutoff modulation (±5V)
- Expression CV: Dynamics/velocity (0-10V)

Advanced Mapping: Any parameter can be CV-mapped via disting NT's parameter CV system.

## Credits

nt_elements is a port of Mutable Instruments Elements modal synthesis engine to the Expert Sleepers disting NT platform.

Original Elements DSP code: Copyright (c) 2014 Emilie Gillet, Mutable Instruments
disting NT adapter code: Copyright (c) 2025 Neal Sanche

## Building from Source

For developers wanting to build from source, see [DEVELOPMENT.md](DEVELOPMENT.md) for detailed build instructions.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

The project combines:
- Adapter code for disting NT (src/) - Copyright (c) 2025 Neal Sanche
- Original Elements DSP code (external/mutable-instruments/) - Copyright (c) 2014 Emilie Gillet

Both components are licensed under the MIT License.

## Support

For issues, questions, or feedback:
- Open an issue on GitHub
- Join the discussion on the disting NT Discord

## Version History

### v1.0.0 (2025-11-02)
- Initial release
- Full Elements DSP integration with adaptive sample rate (32/48/96kHz)
- 4 parameter pages + routing page (23 parameters total)
- Stereo output: Main + Aux for reverb spreading
- Essential CV inputs: V/Oct, Gate, FM, Brightness, Expression
- MIDI note input with pitch bend support
- Custom OLED display with page navigation
- CPU usage < 30% at all supported sample rates
