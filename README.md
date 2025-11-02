# nt_elements

Modal synthesis for disting NT, based on Mutable Instruments Elements.

## Description

nt_elements brings the iconic Elements modal synthesis engine to the Expert Sleepers disting NT Eurorack module. Experience authentic physical modeling synthesis with 4 parameter pages covering exciters, resonators, reverb, and performance controls.

## Features

- Complete Elements DSP engine (bow/blow/strike exciters)
- 64-filter modal resonator with geometry, brightness, damping controls
- Stereo reverb with amount, size, damping parameters
- 4 parameter pages organized: Exciter, Resonator, Space, Performance
- MIDI note input with pitch bend support
- 8 factory presets demonstrating diverse timbres
- Preset save/load integration
- 48kHz sample rate operation
- CPU usage < 30% (typical operation)

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

Send MIDI notes to the configured MIDI channel to trigger synthesis. Pitch bend is supported for expressive performance.

### Factory Presets

8 factory presets are included demonstrating various timbres:
- Bell Strike
- Bowed String
- Blown Pipe
- Drum Hit
- Harmonic Pad
- Metallic Pluck
- Resonant Sweep
- Ethereal Voices

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

## Version

v1.0.0 - Initial Release
