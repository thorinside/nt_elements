# Decision Architecture

## Executive Summary

nt_elements is a C++ plugin for the Expert Sleepers disting NT Eurorack module that ports Mutable Instruments Elements modal synthesis engine. The project consists of 2 epics with 14 stories total.

Epic 1 (9 stories) establishes the development environment, integrates Elements DSP source code at native 32kHz, implements core audio/MIDI processing, and validates on both desktop emulator and hardware.

Epic 2 (5 stories) completes the user interface with 4 parameter pages and performs final performance optimization and sound quality validation.

Critical requirements include maintaining < 30% CPU usage on the ARM Cortex-M7 platform, achieving 100% sound authenticity compared to hardware Elements, and providing stable operation. Elements runs at its native 32kHz sample rate with disting NT handling platform resampling as needed, ensuring authentic sound and optimal CPU efficiency.

The hardware interface is constrained to 3 potentiometers, 2 encoders, 4 buttons, and a small OLED display, requiring careful parameter organization across 4 pages: Exciter, Resonator, Space (reverb), and Performance.

## Project Initialization

**Foundation: distingNT API + Standard Build System**

The first implementation story (1.1) should execute the following initialization commands to establish the project structure:

```bash
# Initialize repository
git init

# Add distingNT API
git submodule add https://github.com/expertsleepersltd/distingNT_API.git

# Add Elements source (original MI code)
git submodule add https://github.com/pichenettes/eurorack.git external/mutable-instruments

# Install ARM toolchain (macOS)
brew install arm-none-eabi-gcc

# Install VCV Rack + nt_emu for desktop testing
# (Download from VCV Rack Library)
```

**Architecture Pattern: Clean Adapter Layer**

- **Elements Source**: Git submodule at `external/mutable-instruments/elements/dsp/` - preserved as-is, read-only
- **Adapter Layer**: Custom code that bridges NT API ↔ Elements DSP
  - Converts NT parameter values to Elements Patch structure
  - Routes NT audio buses to Elements audio I/O
  - Handles MIDI → Elements note control
  - Maps NT UI controls (pots/encoders) to parameter pages
  - Implements custom OLED display rendering

**Decisions Provided by This Foundation:**

- Build system: ARM GCC with Cortex-M7 optimization (`-Os`, `-mcpu=cortex-m7`, `-mfpu=fpv5-d16`)
- API structure: distingNT factory/parameter pattern
- Audio routing: 28-bus system with add/replace modes
- Memory regions: SRAM/DRAM/DTC/ITC allocation
- Testing workflow: Desktop (nt_emu) → Hardware validation
- Source management: Submodules for external dependencies
- UI approach: Replace Elements hardware interface with NT parameter pages + custom OLED

## Decision Summary

| Category | Decision | Version | Affects Epics | Rationale |
| -------- | -------- | ------- | ------------- | --------- |
| Build System | ARM GCC Toolchain | arm-none-eabi-gcc (latest) | Epic 1 | Standard embedded ARM compiler for Cortex-M7 |
| Build System | Dual Build Targets | Makefile: hardware + test | Epic 1, Epic 2 | Desktop testing (nt_emu) + hardware deployment |
| DSP Integration | Elements Source Integration | Header-only compilation (Option C) | Epic 1 | Zero-copy optimization, inline across boundaries |
| DSP Integration | Elements Repository | Git submodule @ external/mutable-instruments | Epic 1 | Preserve original source, clean separation |
| DSP Integration | Source Files | 8 .cc files compiled directly | Epic 1 | part.cc, voice.cc, exciter.cc, resonator.cc, etc. |
| Sample Rate | Native 32kHz Only | Elements runs at 32kHz, NT resamples | Epic 1 | Elements has hardcoded 32kHz, NT handles resampling |
| Memory Management | Memory Regions | DTC/SRAM/DRAM split (Option B) | Epic 1 | Optimized: DTC for hot state, SRAM for filters, DRAM for reverb |
| Memory Management | DTC Allocation | Part structure, current parameters | Epic 1 | Fastest access for frequently-used data |
| Memory Management | SRAM Allocation | Resonator state (~16KB) | Epic 1 | Medium-speed, right size for filter states |
| Memory Management | DRAM Allocation | Reverb buffer (~32KB) | Epic 1, Epic 2 | Large buffer, infrequent access |
| Parameters | Parameter Count | 20 user parameters + 3 system | Epic 2 | 4 pages × 5 controls (pots + encoders) |
| Parameters | Mapping Strategy | Direct copy to Patch struct | Epic 1 (Story 1.7) | Simple translation, Elements handles smoothing |
| Parameters | Page Navigation | Encoder press: prev/next page | Epic 2 | Encoder 1 = prev, Encoder 2 = next |
| MIDI | Note Handling | Simple note-to-freq conversion | Epic 1 (Story 1.6) | MIDI note → frequency calculation, gate on/off |
| MIDI | Voice Mode | Monophonic, last-note priority | Epic 1 | Matches hardware Elements behavior |
| MIDI | Future Extensions | Velocity, pitch bend (Epic 2+) | Deferred | Add if time allows, not v1.0 critical |
| Audio Routing | Buffer Strategy | Temp buffers for I/O (Option A) | Epic 1 (Story 1.8) | Clean separation, 4KB overhead (negligible) |
| Audio Routing | Input Buffer | 512 floats in SRAM | Epic 1 | Elements needs contiguous input |
| Audio Routing | Output Buffer | 512 floats in SRAM | Epic 1 | Handle add vs replace mode cleanly |
| Audio Routing | Bus System | NT 28-bus with add/replace | Epic 1 | Standard NT audio routing |
| Testing | Primary Strategy | Desktop-first (nt_emu) | Epic 1, Epic 2 | Fast iteration with debugging |
| Testing | Hardware Validation | Milestones: Story 1.9, 1.10, 2.5-2.7 | Epic 1, Epic 2 | Verify on actual hardware at key points |
| Testing | Performance Testing | NT built-in CPU display | Epic 2 (Story 2.5) | No custom instrumentation needed |
| UI/Display | OLED Strategy | Page name + parameter values | Epic 2 (Story 2.2) | Simple, clear, update on change only |
| UI/Display | Display Updates | Event-driven (parameter change) | Epic 2 | No polling, no animations |
| UI/Display | Sample Rate Indicator | Show 32k/48k mode | Epic 2 | User awareness of current mode |
| Reverb | Inclusion | Optional, bypassable | Epic 1, Epic 2 | Include for completeness, bypass at 0% for CPU savings |
| Reverb | Bypass Logic | Skip processing when Amount = 0 | Epic 2 (Story 2.5) | Save ~5-10% CPU when unused |
| Reverb | User Choice | Use built-in or route to Clouds | Epic 2 | Flexibility: Elements reverb OR external NT effects |
| Code Organization | Project Structure | src/ for adapter, external/ for submodules | Epic 1 (Story 1.1) | Clean separation of concerns |
| Code Organization | Naming Convention | Follow Elements C++ style | All | Consistency with upstream codebase |
| Code Organization | Documentation | Minimal inline, README for build | All | Code quality over heavy comments |
| Error Handling | Strategy | Defensive coding, no exceptions | All | Clamp values, bounds checking, safe fallbacks |
| Error Handling | Memory Failures | Fail gracefully in construct() | Epic 1 | Check allocations, return error if failed |
| Debugging | Desktop Logging | printf with conditional compilation | Epic 1 | #ifdef NT_EMU_DEBUG for development |
| Debugging | Hardware Logging | OLED display only | Epic 2 | No console available on hardware |
| Version Control | Repository Structure | Git with 2 submodules | Epic 1 (Story 1.1) | distingNT_API + mutable-instruments |
| Versioning | Release Version | v1.0.0 | Epic 2 (Story 2.7) | Semantic versioning |

## Project Structure

```
nt_elements/
├── .git/
├── .gitignore
├── .gitmodules                     # Submodule configuration
├── README.md                       # Build instructions, attribution
├── Makefile                        # Dual-target build system
│
├── distingNT_API/                  # Git submodule: Expert Sleepers API
│   ├── include/
│   │   ├── distingNT.h            # Main API header
│   │   └── ...
│   └── ...
│
├── external/                       # External dependencies
│   └── mutable-instruments/        # Git submodule: MI Eurorack firmware
│       └── elements/
│           └── dsp/                # Elements DSP source files
│               ├── part.h/cc       # Main synthesis engine
│               ├── voice.h/cc      # Voice management
│               ├── exciter.h/cc    # Bow/blow/strike exciters
│               ├── resonator.h/cc  # Modal resonator
│               ├── string.h/cc     # String modeling
│               ├── tube.h/cc       # Tube modeling
│               ├── multistage_envelope.h/cc
│               └── ominous_voice.h/cc
│
├── src/                            # Plugin adapter code
│   ├── nt_elements.cpp            # Main plugin (factory, callbacks, adapter)
│   ├── parameter_adapter.h        # NT parameters → Elements Patch mapping
│   ├── parameter_pages.h          # 4-page UI definition
│   ├── oled_display.h             # OLED rendering functions
│   └── stmlib_stubs/              # Stub implementations for stmlib dependencies
│       ├── dsp.h                  # DSP utility stubs
│       └── utils.h                # Utility stubs
│
├── plugins/                        # Build output directory
│   ├── nt_elements.o              # Hardware build (ARM)
│   └── nt_elements.dylib          # Desktop test build (native)
│
└── build/                          # Intermediate build artifacts (hardware only)
    └── *.o                        # Object files
```

## Epic to Architecture Mapping

### Epic 1: Project Foundation & Core DSP Integration

| Story | Architecture Components | Key Files/Modules |
|-------|------------------------|-------------------|
| 1.1: Development Environment | Build system, toolchain, submodules | Makefile, distingNT_API/, external/mutable-instruments/ |
| 1.2: Elements Source Integration | Git submodule, build configuration | external/mutable-instruments/elements/dsp/*.cc |
| 1.3: Minimal Plugin Stub | NT API factory, callbacks, audio passthrough | src/nt_elements.cpp (construct, destruct, step) |
| 1.4: Elements Part Instance | Memory allocation (DTC/SRAM/DRAM), Elements init at 32kHz | DTC: Part state, SRAM: resonator, DRAM: reverb |
| 1.6: MIDI Note Input | MIDI message handler, note→frequency conversion | midiMessage() callback, Patch gate/note fields |
| 1.7: Parameter Adapter Layer | NT params → Elements Patch mapping | src/parameter_adapter.h, parameterChanged() callback |
| 1.8: Bus Routing and Audio I/O | Temp buffers, NT 28-bus system | Input/output buffers in SRAM, bus indexing |
| 1.9: Desktop Testing | nt_emu build target, debugging | make test → plugins/nt_elements.dylib |
| 1.10: Hardware Deployment | ARM build target, SD card deployment | make hardware → plugins/nt_elements.o |

### Epic 2: UI, Presets & Production Polish

| Story | Architecture Components | Key Files/Modules |
|-------|------------------------|-------------------|
| 2.1: Complete Parameter Pages | 4-page structure, 20 parameters, encoder navigation | src/parameter_pages.h, page navigation logic |
| 2.2: OLED Display | Display rendering, parameter value formatting | src/oled_display.h, draw() callback |
| 2.5: Performance Optimization | Memory optimization, reverb bypass | Conditional reverb processing, profile with NT CPU display |
| 2.6: Sound Quality A/B | Hardware testing, frequency response validation | Hardware deployment, recording setup |
| 2.7: Final Testing | Stability testing, release build | Production .o file, README, MIT attribution |

## Technology Stack Details

### Core Technologies

**Language & Compiler:**
- **C++11** - Elements DSP written in modern C++
- **ARM GCC** (arm-none-eabi-gcc) - Cross-compiler for ARM Cortex-M7
- **Compiler Flags**: `-std=c++11 -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard -Os -fno-rtti -fno-exceptions`

**Hardware Platform:**
- **disting NT** - Expert Sleepers Eurorack module
- **CPU**: ARM Cortex-M7 @ 480MHz (STM32H7 series)
- **RAM**: ~128KB SRAM, ~8MB DRAM, ~64KB DTC, ~64KB ITC
- **Audio**: 32kHz internal processing (Elements native), NT platform resampling
- **Interface**: 3 pots, 2 encoders, 4 buttons, OLED display (128x64)

**DSP Source:**
- **Mutable Instruments Elements** - Modal synthesis engine
  - Repository: https://github.com/pichenettes/eurorack
  - License: MIT
  - Version: Git submodule (fixed commit)
  - Native sample rate: 32kHz

**API Framework:**
- **distingNT API v9** - Expert Sleepers plugin API
  - Repository: https://github.com/expertsleepersltd/distingNT_API
  - Features: Factory pattern, parameter system, MIDI, 28-bus audio routing

**Development Tools:**
- **VCV Rack** (v2.x latest) + **nt_emu module** - Desktop testing environment
- **nt_emu**: VCV Rack module that emulates distingNT, loads .dylib/.so plugins
- **Build System**: GNU Make with dual targets (hardware + test)

### Integration Points

**Elements DSP ↔ NT Adapter:**
- **Parameter Translation**: NT float parameters (0-100%) → Elements Patch structure (0.0-1.0)
- **Audio Routing**: NT bus system → Elements input/output buffers (via temp buffers)
- **MIDI Handling**: NT MIDI callbacks → Elements gate/note control
- **Memory Management**: NT memory allocators → Elements buffer allocation (DTC/SRAM/DRAM)

**Key Adapter Responsibilities:**
1. Convert NT parameter changes to Elements Patch updates
2. Handle sample rate mode switching (32kHz vs 48kHz)
3. Route audio between NT's 28-bus system and Elements' I/O
4. Translate MIDI note messages to frequency + gate signals
5. Render OLED display with current page and parameter values
6. Manage 4-page parameter navigation via encoder button presses
7. Serialize/deserialize preset data via NT preset system

## Implementation Patterns

These patterns ensure consistent implementation across all AI agents working on this project.

### Memory Allocation Pattern

**CRITICAL: All memory must be allocated via NT_globals.getMemory():**

```cpp
// DTC allocation (ultra-fast, small)
_nt_elementsAlgorithm_DTC* dtc = (_nt_elementsAlgorithm_DTC*)
    NT_globals.getMemory(sizeof(_nt_elementsAlgorithm_DTC), kNT_memDTC);

// SRAM allocation (fast, medium-sized buffers)
float* input_buffer = (float*)
    NT_globals.getMemory(512 * sizeof(float), kNT_memSRAM);

// DRAM allocation (large buffers, slower)
uint8_t* reverb_buffer = (uint8_t*)
    NT_globals.getMemory(32768, kNT_memDRAM);
```

**Never use malloc/new - all allocations must go through NT memory manager.**

### Audio Buffer Access Pattern

**CRITICAL: NT bus indexing is 1-based, array access is 0-based:**

```cpp
void step(_NT_algorithm* self, float* busFrames, int numFramesBy4) {
    _nt_elementsAlgorithm* pThis = (_nt_elementsAlgorithm*)self;
    int numFrames = numFramesBy4 * 4;  // MUST multiply by 4

    // Bus parameters are 1-indexed, subtract 1 for array access
    const float* input_bus = busFrames + (pThis->v[kParamInputBus] - 1) * numFrames;
    float* output_bus = busFrames + (pThis->v[kParamOutputBus] - 1) * numFrames;

    // Respect output mode (add vs replace)
    bool replace_mode = pThis->v[kParamOutputMode];

    // Always use temp buffers for Elements processing
    memcpy(pThis->input_buffer, input_bus, numFrames * sizeof(float));
    pThis->elements_part->Process(pThis->patch, pThis->input_buffer,
                                   pThis->temp_output, numFrames);

    if (replace_mode) {
        memcpy(output_bus, pThis->temp_output, numFrames * sizeof(float));
    } else {
        for (int i = 0; i < numFrames; ++i) {
            output_bus[i] += pThis->temp_output[i];
        }
    }
}
```

### Parameter Update Pattern

**Parameters update via parameterChanged() callback:**

```cpp
void parameterChanged(_NT_algorithm* self, int p) {
    _nt_elementsAlgorithm* pThis = (_nt_elementsAlgorithm*)self;

    // Convert NT parameter (0-100%) to Elements Patch (0.0-1.0)
    switch(p) {
        case kParamBrightness:
            pThis->patch.resonator_brightness = pThis->v[kParamBrightness] / 100.0f;
            break;

        case kParamGeometry:
            pThis->patch.resonator_geometry = pThis->v[kParamGeometry] / 100.0f;
            break;
    }
}
```

### MIDI Handling Pattern

**MIDI note to frequency conversion:**

```cpp
void midiMessage(_NT_algorithm* self, uint8_t b0, uint8_t b1, uint8_t b2) {
    _nt_elementsAlgorithm* pThis = (_nt_elementsAlgorithm*)self;

    uint8_t status = b0 & 0xf0;
    uint8_t note = b1;
    uint8_t velocity = b2;

    if (status == 0x90 && velocity > 0) {  // Note on
        // MIDI note to Hz: f = 440 * 2^((note - 69)/12)
        float freq = 440.0f * powf(2.0f, (note - 69) / 12.0f);
        pThis->patch.note = freq;
        pThis->patch.gate = true;
        pThis->current_note = note;  // For last-note priority
    }
    else if (status == 0x80 || (status == 0x90 && velocity == 0)) {  // Note off
        if (note == pThis->current_note) {
            pThis->patch.gate = false;
        }
    }
}
```

### Build Pattern

**Makefile source list - MUST be explicit:**

```makefile
PLUGIN_NAME = nt_elements

SOURCES = src/nt_elements.cpp \
          external/mutable-instruments/elements/dsp/part.cc \
          external/mutable-instruments/elements/dsp/voice.cc \
          external/mutable-instruments/elements/dsp/exciter.cc \
          external/mutable-instruments/elements/dsp/resonator.cc \
          external/mutable-instruments/elements/dsp/string.cc \
          external/mutable-instruments/elements/dsp/tube.cc \
          external/mutable-instruments/elements/dsp/multistage_envelope.cc \
          external/mutable-instruments/elements/dsp/ominous_voice.cc

INCLUDES = -I./src \
           -I./distingNT_API/include \
           -I./external/mutable-instruments \
           -I./src/stmlib_stubs
```

### Testing Pattern

**Desktop-first development workflow:**

```bash
# 1. Develop and iterate on desktop
make test
# Load plugins/nt_elements.dylib in VCV Rack nt_emu

# 2. Once feature works, validate on hardware
make hardware
# Copy plugins/nt_elements.o to distingNT SD card

# 3. Profile performance (hardware only)
# Observe CPU % on NT main display
# Adjust reverb/features if needed to meet < 30% target
```

## Consistency Rules

### Naming Conventions

**C++ Style (Follow Elements Convention):**
- **Classes**: PascalCase (`ElementsAdapter`, `ParameterMapper`)
- **Functions**: camelCase (`parameterChanged`, `processAudio`)
- **Variables**: snake_case (`input_buffer`, `sample_rate_mode`)
- **Constants**: k-prefix PascalCase (`kParamBrightness`, `kMaxBufferSize`)
- **Enums**: ALL_CAPS (`MODE_32KHZ`, `MODE_48KHZ`)

**Parameter Naming:**
- Use descriptive names matching PRD: `kParamGeometry`, `kParamBrightness`, `kParamReverbAmount`
- Page indices: `kPageExciter`, `kPageResonator`, `kPageSpace`, `kPagePerformance`

**File Naming:**
- Headers: `.h` extension
- Implementation: `.cpp` (adapter code) or `.cc` (Elements source)
- Snake_case for filenames: `parameter_adapter.h`, `oled_display.h`

### Code Organization

**Header Files:**
- Minimal includes in headers (forward declare when possible)
- Include guards: `#ifndef NT_ELEMENTS_PARAMETER_ADAPTER_H` / `#define` / `#endif`
- Group includes: NT API, Elements, standard library, project

**Source Files:**
- Group related functions together
- Comment sections: `// ============ Memory Management ============`
- Keep adapter code separate from Elements DSP (never modify submodule files)

**Elements DSP:**
- **NEVER modify files in external/mutable-instruments/**
- Read-only submodule
- All customization in adapter layer

### Error Handling

**Defensive Programming:**
```cpp
// Check memory allocation
float* buffer = (float*)NT_globals.getMemory(size, kNT_memSRAM);
if (!buffer) {
    // Fail gracefully - return error or use fallback
    return false;
}

// Clamp parameter values
float clamped = std::max(0.0f, std::min(1.0f, raw_value));

// Bounds checking
if (index >= 0 && index < array_size) {
    array[index] = value;
}
```

**No Exceptions:**
- Compiler flag: `-fno-exceptions`
- Use return codes or sentinel values
- Check pointers before dereferencing

**Audio Fallback:**
- On error in step(): Output silence, don't crash
- Log error state to OLED if possible

### Logging Strategy

**Desktop Development (nt_emu):**
```cpp
#ifdef NT_EMU_DEBUG
    #define DEBUG_LOG(fmt, ...) fprintf(stderr, "[nt_elements] " fmt "\n", ##__VA_ARGS__)
#else
    #define DEBUG_LOG(fmt, ...) ((void)0)
#endif

// Usage:
DEBUG_LOG("MIDI Note On: %d (%.2f Hz)", note, freq);
DEBUG_LOG("Sample rate mode: %s", mode == MODE_48KHZ ? "48kHz" : "32kHz");
```

**Hardware (distingNT):**
- No printf/console output
- Use OLED for debug info when needed
- Minimize debug code in release builds

## Data Architecture

### Core Data Structures

**Elements Patch Structure:**
```cpp
// From Elements DSP (elements/dsp/patch.h)
struct Patch {
    float note;                    // Frequency in Hz
    bool gate;                     // Note on/off

    // Exciter parameters
    float exciter_bow_level;
    float exciter_bow_timbre;
    float exciter_blow_level;
    float exciter_blow_timbre;
    float exciter_strike_level;
    float exciter_strike_timbre;

    // Resonator parameters
    float resonator_geometry;
    float resonator_brightness;
    float resonator_damping;
    float resonator_position;
    float resonator_modulation;

    // Reverb parameters
    float reverb_amount;
    float reverb_time;
    float reverb_diffusion;

    // ... (Elements internal fields)
};
```

**NT Parameter Array:**
```cpp
// Adapter layer maintains parameter state
enum ParameterIndices {
    kParamInputBus,
    kParamOutputBus,
    kParamOutputMode,
    kParamSampleRateMode,

    // Page 1 - Exciter
    kParamBowLevel,
    kParamBlowLevel,
    kParamStrikeLevel,
    kParamBowTimbre,
    kParamBlowTimbre,

    // Page 2 - Resonator
    kParamGeometry,
    kParamBrightness,
    kParamDamping,
    kParamResonatorModel,
    kParamInharmonicity,

    // Page 3 - Space
    kParamReverbAmount,
    kParamReverbSize,
    kParamReverbDamping,
    kParamReverbType,

    // Page 4 - Performance
    kParamCoarseTune,
    kParamFineTune,
    kParamOutputLevel,
    kParamFMAmount,
    kParamExciterContour,

    kNumParameters
};
```

### Data Flow

```
User Input (Pots/Encoders/MIDI)
    ↓
NT Parameter System
    ↓
parameterChanged() callback
    ↓
Adapter: NT params (0-100%) → Elements Patch (0.0-1.0)
    ↓
Elements::Part structure
    ↓
step() callback: Elements::Part::Process()
    ↓
Audio Output (NT bus system)
```

### Memory Layout

**DTC (~4KB):**
- Current Patch structure
- Active parameter values
- Page state, mode flags

**SRAM (~20KB):**
- Input buffer (512 floats = 2KB)
- Output buffer (512 floats = 2KB)
- Elements Part structure (~4KB)
- Resonator filter states (~16KB)

**DRAM (~32KB):**
- Reverb delay buffer

## Performance Considerations

### CPU Budget

**Target: < 30% CPU usage @ 48kHz**

**Expected Breakdown:**
- Elements DSP core: ~15-20% (resonator, exciters)
- Reverb: ~5-10% (bypassable)
- Sample rate conversion: ~2-3% (if 48kHz mode)
- Adapter overhead: < 1% (parameter mapping, buffer copies)
- **Total**: 20-30% depending on mode and reverb

**Optimization Strategies:**
1. **Sample rate mode**: 32kHz mode saves ~15-20% vs 48kHz
2. **Reverb bypass**: When Amount = 0, skip reverb processing entirely
3. **Compiler optimizations**: `-Os` for size, could try `-O3` if needed
4. **Memory access**: DTC for hot paths reduces latency

**Profiling Approach:**
- Develop in nt_emu (fast iteration)
- Deploy to hardware for CPU measurement
- Observe NT's built-in CPU % display on main page
- Adjust reverb size/quality if needed during Story 2.5

### Audio Quality

**Target: 100% sound authenticity vs hardware Elements**

**Validation:**
- A/B comparison with hardware Elements (Story 2.6)
- Spectral analysis of identical patches
- Frequency response measurements
- Blind listening tests

**Quality Assurance:**
- Use Elements DSP unmodified (submodule)
- Verify sample rate conversion maintains frequency response
- Test all exciter modes (bow/blow/strike)
- Validate resonator behavior across geometry range

## Deployment Architecture

### Build Artifacts

**Hardware Deployment:**
```
plugins/nt_elements.o  →  Copy to distingNT SD card root
```

**Testing Deployment:**
```
plugins/nt_elements.dylib  →  Load in VCV Rack nt_emu module
```

### Distribution

**v1.0 Release Package:**
- `nt_elements.o` - Plugin binary
- `README.md` - Installation instructions, attribution
- `LICENSE` - MIT license with Elements attribution
- `CHANGELOG.md` - Version history

**Installation:**
1. Download `nt_elements.o`
2. Copy to distingNT SD card
3. On NT: Navigate to file browser
4. Select `nt_elements.o`
5. Plugin loads and appears in algorithm list

### Version Control

**Repository Structure:**
```bash
git init
git submodule add https://github.com/expertsleepersltd/distingNT_API.git
git submodule add https://github.com/pichenettes/eurorack.git external/mutable-instruments
```

**Branching Strategy:**
- `main` - Stable releases
- `develop` - Active development
- Feature branches for complex stories

**Versioning:**
- v1.0.0 - Initial release (Epic 2 complete)
- v1.1.0+ - Future enhancements

## Development Environment

### Prerequisites

**Required Tools:**
- **macOS** (primary development platform)
- **ARM GCC Toolchain**: `brew install arm-none-eabi-gcc`
- **Git**: For version control and submodules
- **Make**: GNU Make (included with Xcode Command Line Tools)
- **VCV Rack**: Desktop modular synthesizer (for nt_emu testing)
- **nt_emu module**: VCV Rack plugin that emulates distingNT

**Optional Tools:**
- **Spectral analysis software**: For sound quality validation
- **Audio recording software**: For A/B testing vs hardware Elements
- **Text editor/IDE**: VSCode, CLion, or similar with C++ support

### Setup Commands

```bash
# 1. Install ARM toolchain
brew install arm-none-eabi-gcc

# Verify installation
arm-none-eabi-g++ --version
# Should show: arm-none-eabi-g++ (GNU Arm Embedded Toolchain) ...

# 2. Clone project and initialize submodules
git clone <your-repo-url> nt_elements
cd nt_elements

# 3. Add distingNT API submodule
git submodule add https://github.com/expertsleepersltd/distingNT_API.git

# 4. Add Mutable Instruments Elements source submodule
git submodule add https://github.com/pichenettes/eurorack.git external/mutable-instruments

# 5. Initialize all submodules
git submodule update --init --recursive

# 6. Download and install VCV Rack
# Visit: https://vcvrack.com/
# Install VCV Rack and the nt_emu module from Library

# 7. Verify build system
make help
# Should show build targets: hardware, test, both, clean

# 8. Build for desktop testing (first time)
make test
# Output: plugins/nt_elements.dylib

# 9. Load in VCV Rack
# Launch VCV Rack
# Add nt_emu module to patch
# Right-click nt_emu → Load plugin → Select plugins/nt_elements.dylib
```

## Architecture Decision Records (ADRs)

### ADR-001: Use Header-Only Integration for Elements DSP

**Context**: Elements DSP needs to be integrated with distingNT plugin. Options: static library vs header-only compilation.

**Decision**: Header-only integration (compile Elements .cc files directly with plugin).

**Rationale**:
- Enables link-time optimization across adapter/Elements boundary
- Simplifies debugging in nt_emu (single compilation unit)
- Allows flexible sample rate mode implementation
- No build complexity of managing static libraries
- Preserves original Elements source (submodule, read-only)

**Consequences**:
- Slightly longer build times (negligible for project size)
- Must list all Elements .cc files explicitly in Makefile
- Adapter must provide stmlib stub implementations

---

### ADR-002: Dynamic Sample Rate Injection via dsp.h Modification

**Context**: Elements DSP has hardcoded `static const float kSampleRate = 32000.0f` in dsp.h, but disting NT provides user-configurable sample rate via `NT_globals.sampleRate` (32/48/96kHz).

**Decision**: Modify Elements' dsp.h to inject NT's dynamic sample rate using inline function + macro redefinition.

**Implementation**:
```cpp
// external/mutable-instruments/elements/dsp/dsp.h (lines 36-54)
#include "distingnt/api.h"

namespace elements {
    inline float get_sample_rate() {
        return static_cast<float>(NT_globals.sampleRate);
    }
}
#define kSampleRate elements::get_sample_rate()
```

**Rationale**:
- Respects MIT license (modification permitted for nt_elements project)
- Zero runtime overhead (inline function optimized to register read)
- Adapts automatically to NT's user-configured sample rate
- Correct pitch and filter frequencies at all sample rates
- Simpler than sample rate conversion (no CPU overhead, no artifacts)
- Elements submodule is local to this project, not pushed upstream

**Consequences**:
- Elements dsp.h modified (documented in file with nt_elements comment block)
- Makefile updated with `-undefined dynamic_lookup` for macOS test builds
- Story 1.5 completed (not removed)
- Story 1.11 removed (no mode selection UI needed - automatic adaptation)
- Plugin works correctly at any NT sample rate setting

**Date**: 2025-11-02

---

### ADR-003: Optimized Memory Region Split

**Context**: disting NT provides DTC, SRAM, DRAM with different performance characteristics.

**Decision**: Split allocation - DTC for hot state, SRAM for filters, DRAM for reverb.

**Rationale**:
- Maximizes performance for < 30% CPU target
- DTC for frequently accessed data (Patch, parameters)
- SRAM for medium-sized processing buffers
- DRAM for large, infrequently accessed reverb buffer

**Consequences**:
- More complex memory management than SRAM-only
- Must carefully track which data goes where
- Performance benefit justifies complexity

---

### ADR-004: Desktop-First Testing Strategy

**Context**: Need efficient development workflow for embedded target.

**Decision**: Develop on desktop (nt_emu), validate on hardware at milestones.

**Rationale**:
- Fast iteration with printf debugging
- No hardware deploy cycle during development
- Hardware testing reserved for performance and final validation
- Matches industry best practices for embedded development

**Consequences**:
- Must maintain two build targets (hardware + test)
- Potential for desktop/hardware behavioral differences (rare)
- Requires VCV Rack and nt_emu setup

---

### ADR-005: Make Reverb Optional/Bypassable

**Context**: Elements includes reverb, but disting NT already has Clouds reverb available.

**Decision**: Include reverb but make it bypassable (skip processing when Amount = 0).

**Rationale**:
- Preserves Elements character (reverb is part of the sound)
- Users can choose: built-in reverb OR route to external effects
- Bypass saves ~5-10% CPU when unused
- Flexibility without removing functionality

**Consequences**:
- Slightly more complex step() logic (conditional processing)
- Must allocate reverb buffer even if bypassed (or dynamic allocation)
- Page 3 remains useful (reverb controls)

---

### ADR-006: Four-Page Parameter Organization

**Context**: Elements has 20+ parameters, disting NT has 3 pots + 2 encoders per page.

**Decision**: Organize into 4 pages: Exciter, Resonator, Space, Performance.

**Rationale**:
- Matches hardware Elements' three-section layout (familiar mental model)
- Logical grouping by synthesis function
- 5 controls per page fits NT hardware perfectly
- Encoder press navigation (Enc1=prev, Enc2=next) is intuitive

**Consequences**:
- Page navigation required to access all parameters
- Pot 3 press used for sample rate toggle (convenient but non-obvious)
- OLED must clearly show current page context

---

_Generated by BMAD Decision Architecture Workflow v1.3.2_
_Date: 2025-10-26_
_For: Neal_
