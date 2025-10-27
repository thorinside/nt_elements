# nt_elements

Mutable Instruments Elements modal synthesis port for the Expert Sleepers disting NT Eurorack module.

## Overview

This project ports the Mutable Instruments Elements physical modeling synthesizer to the disting NT platform, providing modal synthesis capabilities in a hardware Eurorack module.

## Prerequisites

### Required Software

- **macOS** (tested on macOS 14.6+)
- **ARM GCC Toolchain**: `brew install arm-none-eabi-gcc`
- **Git**: For cloning and managing submodules
- **Make**: Build system (included with Xcode Command Line Tools)
- **VCV Rack v2.x**: Desktop testing environment
  - Download from https://vcvrack.com/
  - Install the **nt_emu** module from the VCV Rack Library

### ARM Toolchain Installation

```bash
brew install arm-none-eabi-gcc
arm-none-eabi-g++ --version  # Verify installation
```

Current version used: ARM Embedded Toolchain 14.3.Rel1

## Project Setup

### Clone and Initialize

```bash
# Clone repository (if not already done)
git clone <repository-url>
cd nt_elements

# Initialize git submodules
git submodule update --init --recursive
```

### Submodules

This project uses two git submodules:

1. **distingNT_API** - Expert Sleepers disting NT plugin API (v9)
2. **external/mutable-instruments** - Mutable Instruments Elements source code (MIT License)

**Important**: Never modify files in git submodules. All customization happens in the `src/` adapter layer.

## Building

The project uses a dual-target build system:

### Desktop Test Build

Builds a native `.dylib` (macOS) or `.so` (Linux) for testing in VCV Rack:

```bash
make test
```

Output: `plugins/nt_elements.dylib`

### Hardware Build

Builds ARM Cortex-M7 `.o` file for disting NT hardware:

```bash
make hardware
```

Output: `plugins/nt_elements.o` (< 2KB for minimal stub)

### Clean Build

Remove all build artifacts:

```bash
make clean
```

## Testing

### Desktop Testing with VCV Rack

1. Build the test plugin: `make test`
2. Launch VCV Rack
3. Add the **nt_emu** module to your patch
4. Right-click nt_emu → **Load plugin**
5. Navigate to `plugins/nt_elements.dylib`
6. Select the plugin

The plugin should load without errors. Check VCV Rack's log (Help → Show Log) for any warnings.

### Hardware Deployment

Hardware deployment will be covered in Story 1.10. The `.o` file produced by `make hardware` is ready for deployment to disting NT.

## Project Structure

```
nt_elements/
├── distingNT_API/          # Git submodule - Expert Sleepers API
├── external/
│   └── mutable-instruments/ # Git submodule - Elements source
├── src/                    # Adapter code (our code)
│   └── nt_elements.cpp     # Main plugin implementation
├── plugins/                # Build output
│   ├── nt_elements.dylib   # Desktop test build
│   └── nt_elements.o       # Hardware build
├── build/                  # Intermediate build artifacts
├── Makefile               # Build system
└── README.md              # This file
```

## Development Workflow

This project follows a **desktop-first development workflow**:

1. Develop and iterate using nt_emu (fast, printf debugging available)
2. Test thoroughly on desktop
3. Build hardware version and validate on disting NT at milestones

## Memory Management

All memory allocation **must** use `NT_globals.getMemory()` - never use `malloc`/`new`.

Memory regions:
- **DTC**: Ultra-fast, small (~64KB) - for hot state and parameters
- **SRAM**: Fast, medium (~128KB) - for audio buffers and processing state
- **DRAM**: Large, slower (~8MB) - for reverb buffers

## License

This project is licensed under the MIT License.

### Attribution

Based on **Mutable Instruments Elements** by Emilie Gillet (MIT License)
- Copyright 2014 Emilie Gillet
- Source: https://github.com/pichenettes/eurorack

Uses **Expert Sleepers distingNT API** (MIT License)
- Copyright 2025 Expert Sleepers Ltd

## Current Status

- ✅ Development environment setup complete
- ✅ ARM toolchain installed and verified (v14.3.1)
- ✅ Git submodules initialized (distingNT_API, Mutable Instruments Elements)
- ✅ VCV Rack with nt_emu ready for desktop testing
- ✅ Minimal "hello world" plugin compiles for both desktop and hardware
- ⏳ Elements DSP integration - next step

## Build System Details

### Compiler Flags

**Hardware (ARM Cortex-M7)**:
```
-std=c++11 -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard
-Os -fno-rtti -fno-exceptions
```

**Desktop Test**:
```
-std=c++11 -O2 -fPIC -fno-rtti -fno-exceptions
```

### Include Paths

- `distingNT_API/include` - distingNT plugin API headers

Future stories will add Elements DSP and stmlib include paths.

## Resources

- [disting NT Manual](https://www.expert-sleepers.co.uk/distingNT.html)
- [VCV Rack Documentation](https://vcvrack.com/manual/)
- [Mutable Instruments Elements](https://mutable-instruments.net/modules/elements/)
- [Elements Source Code](https://github.com/pichenettes/eurorack)

## Versions

- **ARM GCC Toolchain**: 14.3.1 (arm-14.174)
- **distingNT API**: v9
- **VCV Rack**: v2.x
- **nt_emu**: Latest from VCV Library
