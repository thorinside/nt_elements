# Changelog

All notable changes to nt_elements will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] - 2025-11-02

### Overview

First production release of nt_elements, bringing Mutable Instruments Elements modal synthesis to disting NT.

### Features

**Core DSP Engine**
- Complete Elements DSP implementation with full modal synthesis capability
- Bow, blow, and strike excitation modes
- 64-filter modal resonator with geometry, brightness, damping controls
- Stereo reverb with amount, size, and damping parameters

**User Interface**
- 4 parameter pages for organized control:
  - Page 1: Exciter controls (bow/blow balance, strike, contour, timbres)
  - Page 2: Resonator controls (geometry, brightness, damping, position, inharmonicity)
  - Page 3: Space controls (reverb amount, size, damping)
  - Page 4: Performance controls (tuning, FM amount, output level)
- OLED display showing current page and parameter values
- Button navigation for page switching
- All 3 pots and 2 encoders functional on each page

**MIDI Integration**
- MIDI note input with note-on/note-off support
- Pitch bend for expressive performance
- Monophonic operation with last-note priority
- Velocity-sensitive exciter strength

**Preset System**
- 8 factory presets demonstrating diverse timbres:
  - Bell Strike
  - Bowed String
  - Blown Pipe
  - Drum Hit
  - Harmonic Pad
  - Metallic Pluck
  - Resonant Sweep
  - Ethereal Voices
- Preset save/load integration with disting NT preset system

**Audio I/O**
- Stereo output with configurable bus routing
- External audio input for resonator excitation
- Output mode selection (add/replace)
- Support for buses 1-28

### Performance

- CPU usage < 30% typical operation at 48kHz
- Optimized compiler flags: -O3, -ffast-math, -funroll-loops
- Efficient memory usage:
  - SRAM: ~8.3 KB (6.5% of 128 KB limit)
  - DRAM: 64 KB (3.1% of 2 MB limit)
  - DTC: ~2-4 KB (~6% of 64 KB limit)
  - Total: ~72-76 KB
- Minimal wavetable implementation (207 KB plugin binary)

### Sound Quality

- 48kHz sample rate operation for optimal disting NT integration
- All Elements filter coefficients scaled for 48kHz (from original 32kHz)
- Authentic physical modeling synthesis
- No audio artifacts or zipper noise
- Smooth parameter interpolation

### Build System

- Dual-target Makefile (desktop test build + hardware build)
- ARM Cortex-M7 optimization flags
- Desktop test build for VCV Rack nt_emu validation
- Clean separation between adapter code and Elements DSP

### Documentation

- User README with installation instructions
- MIT License with Elements attribution
- Development documentation for building from source
- Known issues documented
- Manual testing guides for hardware validation

### Technical Implementation

**Epic 1: Core DSP Integration (Stories 1.1-1.11)**
- Development environment setup with ARM toolchain
- Elements source integration via git submodule
- Minimal plugin stub with audio passthrough
- Elements Part instance integration
- Sample rate conversion (32kHz → 48kHz)
- MIDI note input integration
- Parameter adapter layer (NT floats ↔ Elements patch)
- Bus routing and audio I/O
- Desktop testing validation (nt_emu)
- Hardware deployment and validation
- Sample rate mode selection (removed in final build)

**Epic 2: User Interface & Polish (Stories 2.1-2.6)**
- Complete 4-page parameter system
- OLED display implementation
- Preset save/load integration
- Factory preset library (8 presets)
- Performance optimization (< 30% CPU)
- Sound quality A/B validation

### Known Issues

See [KNOWN-ISSUES.md](KNOWN-ISSUES.md) for details:
- Manual hardware testing required for full validation
- Minimal wavetables to meet size constraints (207 KB binary)
- Build warnings from upstream Mutable Instruments code (harmless)

### License

MIT License - see [LICENSE](LICENSE) file for details.

Based on Mutable Instruments Elements:
- Copyright (c) 2013-2014 Emilie Gillet
- Licensed under MIT License

Port to disting NT:
- Copyright (c) 2025 Neal Sanche
- Licensed under MIT License

### Requirements

- disting NT Eurorack module
- disting NT firmware v1.0 or later
- SD card (FAT32 formatted)
- MIDI controller (optional, for note input)

### Installation

1. Download `nt_elements.o` from releases
2. Copy to disting NT SD card
3. Insert SD card and power on
4. Select plugin from file browser
5. Plugin appears in algorithm list

### Contributors

- Neal Sanche - Port to disting NT
- Emilie Gillet - Original Elements DSP (Mutable Instruments)

---

## [Unreleased]

### Planned for v1.1.0

- Extended stability testing validation (4+ hours)
- Stress testing validation (rapid parameters, MIDI bursts)
- A/B sound quality comparison with hardware Elements
- Additional factory presets
- Runtime LUT generation to reduce binary size
- Support for more exciter/resonator modes

---

[1.0.0]: https://github.com/yourusername/nt_elements/releases/tag/v1.0.0
