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
- **Stereo Output**: Main Output (bus 13) + Aux Output (bus 14) for reverb spreading
- **MIDI Input**: Note on/off, pitch bend, channel filtering (1-16 or omni)
- **Essential CV Inputs**:
  - V/Oct CV: 1V/octave pitch tracking
  - Gate CV: Trigger/gate input
  - FM CV: Dynamic FM modulation (±5V)
  - Brightness CV: Filter cutoff modulation (±5V)
  - Expression CV: Dynamics/velocity control (0-10V)

### Performance
- Native 32kHz sample rate (optimized for distingNT)
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
- SD card (FAT32 formatted)
- MIDI controller (for note input, optional)

## Usage

### Parameter Pages

nt_elements organizes parameters into 4 pages accessible via the button:

**Page 1: Exciter**
- Controls the excitation method (bow, blow, strike)
- Adjust contour, timbre, and bow/blow balance

**Page 2: Resonator**
- Shape the resonator response
- Control geometry, brightness, damping, position, inharmonicity

**Page 3: Space**
- Stereo reverb parameters
- Amount, size, damping

**Page 4: Performance**
- Global controls
- Tuning, FM amount, output level

### MIDI Control

Send MIDI notes to the configured MIDI channel to trigger synthesis. Configure MIDI channel on the Routing page (0=omni, 1-16=specific channel). Pitch bend is supported for expressive performance.

### CV Modulation

**Dedicated CV Inputs** (for frequently-used modulations):
- **FM CV**: Vibrato, pitch warble effects
- **Brightness CV**: Filter sweeps with envelopes/LFOs
- **Expression CV**: Dynamics from velocity or pressure sources

**Manual Mapping**: Any parameter can be mapped to any CV bus via the distingNT interface for advanced control.

## Credits

nt_elements is a port of Mutable Instruments Elements modal synthesis engine to the Expert Sleepers disting NT platform.

Original Elements DSP code: Copyright (c) 2013-2014 Emilie Gillet, Mutable Instruments
Port to disting NT: Copyright (c) 2025 Neal Sanche

## Building from Source

For developers wanting to build from source, see [DEVELOPMENT.md](DEVELOPMENT.md) for detailed build instructions.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Support

For issues, questions, or feedback:
- Open an issue on GitHub
- Join the discussion on the disting NT Discord

## Version History

### v1.0.0 (2025-11-02)
- Initial release
- Full Elements DSP integration at 32kHz
- 4 parameter pages + routing page
- **Stereo output**: Main + Aux for reverb spreading
- **Essential CV inputs**: FM, Brightness, Expression
- MIDI and CV/Gate support
- Custom OLED display with page navigation
