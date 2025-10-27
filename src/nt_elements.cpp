/*
 * nt_elements - Mutable Instruments Elements port for disting NT
 *
 * Story 1.4: Elements Part Instance Integration
 * Instantiates Elements::Part and routes audio through modal synthesis DSP.
 *
 * Copyright (c) 2025
 * Licensed under MIT License
 */

#include <cstddef>
#include <cstdint>
#include <cmath>
#include <cstring>
#ifdef NT_EMU_DEBUG
#include <cstdio>
#endif
#include "distingnt/api.h"
#include "elements/dsp/part.h"
#include "elements/dsp/dsp.h"

// Diagnostic version - increment this with each build to verify correct version loaded
#define DIAG_VERSION 0x01

// Diagnostic markers (must be 7-bit: 0x00-0x7F for valid SysEx data)
#define DIAG_MIDI_ENTRY_MARKER  0x10
#define DIAG_MIDI_CAST_MARKER   0x11
#define DIAG_MIDI_PARSE_MARKER  0x12
#define DIAG_MIDI_EXIT_MARKER   0x1F
#define DIAG_STEP_MARKER        0x20
#define DIAG_BUTTON4_MARKER     0x44  // Changed from 0xB4 (Button 4 -> 0x44)

#include "parameter_adapter.h"

// Algorithm structure with Elements integration
struct nt_elementsAlgorithm : public _NT_algorithm {
    // Elements DSP engine (in DTC)
    elements::Part* elements_part;

    // Current performance state and patch
    elements::PerformanceState perf_state;

    // Temp buffers for Elements processing (in SRAM)
    float* temp_blow_in;    // 512 floats
    float* temp_strike_in;  // 512 floats
    float* temp_main_out;   // 512 floats
    float* temp_aux_out;    // 512 floats

    // Memory region pointers for cleanup tracking
    uint16_t* reverb_buffer;  // DRAM reverb buffer

    // MIDI state for monophonic voice management
    uint8_t current_note;     // Currently playing note (for last-note priority)

    // Pending MIDI updates (written by midiMessage, read by step for thread safety)
    volatile bool pending_update;
    elements::PerformanceState pending_state;

    // Button debouncing (track last button state to avoid bouncing)
    uint16_t last_button_state;
};

// Factory functions forward declarations
static void calculateStaticRequirements(_NT_staticRequirements& req);
static void initialise(_NT_staticMemoryPtrs& ptrs, const _NT_staticRequirements& req);
static void calculateRequirements(_NT_algorithmRequirements& req, const int32_t* specifications);
static _NT_algorithm* construct(const _NT_algorithmMemoryPtrs& ptrs, const _NT_algorithmRequirements& req, const int32_t* specifications);
static void parameterChanged(_NT_algorithm* self, int p);
static void step(_NT_algorithm* self, float* busFrames, int numFramesBy4);
static void midiMessage(_NT_algorithm* self, uint8_t b0, uint8_t b1, uint8_t b2);
static uint32_t hasCustomUi(_NT_algorithm* self);
static void customUi(_NT_algorithm* self, const _NT_uiData& data);

// Parameter definitions
static const _NT_parameter parameters[kNumParams] = {
    NT_PARAMETER_AUDIO_INPUT("Input", 1, 1)
    NT_PARAMETER_AUDIO_OUTPUT_WITH_MODE("Output", 1, 13)
    { .name = "Geometry", .min = 0, .max = 100, .def = 50, .unit = kNT_unitPercent, .scaling = 0, .enumStrings = NULL },
    { .name = "Brightness", .min = 0, .max = 100, .def = 70, .unit = kNT_unitPercent, .scaling = 0, .enumStrings = NULL },
    { .name = "Damping", .min = 0, .max = 100, .def = 60, .unit = kNT_unitPercent, .scaling = 0, .enumStrings = NULL },
};

// Parameter pages for menu organization
static const uint8_t pageResonator[] = {
    kParamGeometry,
    kParamBrightness,
    kParamDamping
};

static const uint8_t pageRouting[] = {
    kParamInputBus,
    kParamOutputBus,
    kParamOutputMode
};

static const _NT_parameterPage pages[] = {
    { .name = "Resonator", .numParams = sizeof(pageResonator), .params = pageResonator },
    { .name = "Routing", .numParams = sizeof(pageRouting), .params = pageRouting },
};

static const _NT_parameterPages parameterPages = {
    .numPages = sizeof(pages) / sizeof(pages[0]),
    .pages = pages,
};

// Factory definition
static const _NT_factory factory = {
    .guid = NT_MULTICHAR('T','h','E','l'),  // 'ThEl' - nt_elements
    .name = "nt_elements",
    .description = "Elements modal synthesis",
    .numSpecifications = 0,
    .specifications = NULL,
    .calculateStaticRequirements = calculateStaticRequirements,
    .initialise = initialise,
    .calculateRequirements = calculateRequirements,
    .construct = construct,
    .parameterChanged = parameterChanged,
    .step = step,
    .draw = NULL,
    .midiRealtime = NULL,
    .midiMessage = midiMessage,
    .tags = kNT_tagInstrument,
    .hasCustomUi = hasCustomUi,
    .customUi = customUi,
    .setupUi = NULL
};

// Factory implementations

static void calculateStaticRequirements(_NT_staticRequirements& req) {
    req.dram = 0;  // No shared static memory needed
}

static void initialise(_NT_staticMemoryPtrs& /*ptrs*/, const _NT_staticRequirements& /*req*/) {
    // No static initialization needed
}

static void calculateRequirements(_NT_algorithmRequirements& req, const int32_t* /*specifications*/) {
    req.numParameters = kNumParams;

    // DTC: Elements Part instance (~4KB for hot state)
    req.dtc = sizeof(elements::Part) + sizeof(elements::Patch);

    // SRAM: Temp buffers for audio processing (4 * 512 floats = 8KB)
    req.sram = sizeof(nt_elementsAlgorithm) + (4 * 512 * sizeof(float));

    // DRAM: Reverb buffer (32768 samples = 64KB for uint16_t)
    req.dram = 32768 * sizeof(uint16_t);

    req.itc = 0;
}

static _NT_algorithm* construct(const _NT_algorithmMemoryPtrs& ptrs, const _NT_algorithmRequirements& /*req*/, const int32_t* /*specifications*/) {
    // Use placement new to properly construct algorithm instance in SRAM
    nt_elementsAlgorithm* self = new (ptrs.sram) nt_elementsAlgorithm();

    // Initialize base _NT_algorithm fields
    self->parameters = parameters;
    self->parameterPages = &parameterPages;

    // Allocate temp buffers from SRAM (after algorithm structure)
    // Ensure proper 4-byte alignment for float arrays on ARM
    uintptr_t sram_addr = reinterpret_cast<uintptr_t>(ptrs.sram) + sizeof(nt_elementsAlgorithm);
    sram_addr = (sram_addr + 3) & ~3;  // Align to 4-byte boundary
    uint8_t* sram_ptr = reinterpret_cast<uint8_t*>(sram_addr);

    self->temp_blow_in = reinterpret_cast<float*>(sram_ptr);
    sram_ptr += 512 * sizeof(float);
    self->temp_strike_in = reinterpret_cast<float*>(sram_ptr);
    sram_ptr += 512 * sizeof(float);
    self->temp_main_out = reinterpret_cast<float*>(sram_ptr);
    sram_ptr += 512 * sizeof(float);
    self->temp_aux_out = reinterpret_cast<float*>(sram_ptr);

    // Zero temp buffers
    memset(self->temp_blow_in, 0, 512 * sizeof(float));
    memset(self->temp_strike_in, 0, 512 * sizeof(float));
    memset(self->temp_main_out, 0, 512 * sizeof(float));
    memset(self->temp_aux_out, 0, 512 * sizeof(float));

    // Allocate reverb buffer from DRAM
    self->reverb_buffer = reinterpret_cast<uint16_t*>(ptrs.dram);

    // Use placement new to construct Elements Part in DTC
    self->elements_part = new (ptrs.dtc) elements::Part();

    // Initialize Elements Part with reverb buffer
    self->elements_part->Init(self->reverb_buffer);

    // Initialize default performance state
    self->perf_state.gate = false;
    self->perf_state.note = 69.0f;  // MIDI note number A4 (69)
    self->perf_state.modulation = 0.0f;  // Pitch bend offset in semitones
    self->perf_state.strength = 0.8f;

    // Initialize MIDI state
    self->current_note = 0;
    self->pending_update = false;

    // Initialize pending state to match current state
    self->pending_state.gate = false;
    self->pending_state.note = 69.0f;
    self->pending_state.modulation = 0.0f;
    self->pending_state.strength = 0.8f;

    // Initialize button state
    self->last_button_state = 0;

    // Initialize default patch parameters (balanced modal sound)
    elements::Patch* patch = self->elements_part->mutable_patch();

    // Exciter defaults - disable strike/blow to avoid wavetable issues
    patch->exciter_envelope_shape = 0.5f;
    patch->exciter_bow_level = 0.8f;  // Only use bow exciter (no wavetables)
    patch->exciter_bow_timbre = 0.5f;
    patch->exciter_blow_level = 0.0f;  // Disabled
    patch->exciter_blow_meta = 0.5f;
    patch->exciter_blow_timbre = 0.5f;
    patch->exciter_strike_level = 0.0f;  // Disabled - uses wavetables
    patch->exciter_strike_meta = 0.5f;
    patch->exciter_strike_timbre = 0.5f;
    patch->exciter_signature = 1.0f;  // Force to NOISE model (avoid sample players)

    // Resonator defaults (matching NT parameter defaults: 50%, 70%, 60%)
    // parameterChanged() will be called after construct() to sync with actual parameter values
    patch->resonator_geometry = 0.5f;    // 50% default
    patch->resonator_brightness = 0.7f;  // 70% default
    patch->resonator_damping = 0.6f;     // 60% default
    patch->resonator_position = 0.5f;
    patch->resonator_modulation_frequency = 0.5f;
    patch->resonator_modulation_offset = 0.0f;

    // Reverb defaults
    patch->reverb_diffusion = 0.7f;
    patch->reverb_lp = 0.7f;
    patch->space = 0.2f;  // Subtle reverb

    patch->modulation_frequency = 0.5f;

    return self;
}

static void parameterChanged(_NT_algorithm* self, int p) {
    nt_elementsAlgorithm* algo = static_cast<nt_elementsAlgorithm*>(self);
    elements::Patch* patch = algo->elements_part->mutable_patch();

    // Convert NT parameters (0-100%) to Elements Patch fields (0.0-1.0)
    switch (p) {
        case kParamGeometry:
            patch->resonator_geometry = parameter_adapter::ntToElements(self->v[kParamGeometry]);
            break;

        case kParamBrightness:
            patch->resonator_brightness = parameter_adapter::ntToElements(self->v[kParamBrightness]);
            break;

        case kParamDamping:
            patch->resonator_damping = parameter_adapter::ntToElements(self->v[kParamDamping]);
            break;

        // Bus routing parameters don't need handling (used directly in step())
        case kParamInputBus:
        case kParamOutputBus:
        case kParamOutputMode:
        default:
            break;
    }
}

static void step(_NT_algorithm* self, float* busFrames, int numFramesBy4) {
    nt_elementsAlgorithm* algo = static_cast<nt_elementsAlgorithm*>(self);

    // Apply pending MIDI updates atomically (thread-safe)
    if (algo->pending_update) {
        // Copy fields individually to ensure proper memory ordering on ARM
        algo->perf_state.gate = algo->pending_state.gate;
        algo->perf_state.note = algo->pending_state.note;
        algo->perf_state.modulation = algo->pending_state.modulation;
        algo->perf_state.strength = algo->pending_state.strength;

        // Memory barrier for ARM - ensure all writes complete before clearing flag
        __asm__ volatile("" ::: "memory");
        algo->pending_update = false;
    }

    // busFrames layout: [bus0_frames][bus1_frames]...[bus27_frames]
    const int numFrames = numFramesBy4 * 4;

    // Guard against buffer overflow (temp buffers sized for 512 frames)
    if (numFrames > 512 || numFrames <= 0) {
        return;  // Protect SRAM; invalid buffer sizes
    }

#ifdef NT_EMU_DEBUG
    static int debug_counter = 0;
    if (++debug_counter % 1000 == 0) {
        printf("step() called: numFrames=%d, gate=%d, note=%.1f\n",
               numFrames, algo->perf_state.gate, algo->perf_state.note);
    }
#endif

    // Get bus assignments (1-based, convert to 0-based for array access)
    const int inputBus = static_cast<int>(self->v[kParamInputBus]) - 1;
    const int outputBus = static_cast<int>(self->v[kParamOutputBus]) - 1;
    const int outputMode = static_cast<int>(self->v[kParamOutputMode]);

    // Validate bus indices
    if (inputBus < 0 || inputBus >= 28 || outputBus < 0 || outputBus >= 28) {
        return;
    }

    // Get bus pointers
    const float* input = busFrames + (inputBus * numFrames);
    float* output = busFrames + (outputBus * numFrames);

    // Elements' internal buffers operate on kMaxBlockSize (16) samples.
    // Slice disting NT blocks into safe chunks to avoid overruns on hardware.
    const int kElementsBlockSize = 16;
    for (int offset = 0; offset < numFrames; offset += kElementsBlockSize) {
        const int remaining = numFrames - offset;
        const int chunk = remaining >= kElementsBlockSize ? kElementsBlockSize : remaining;

        if (algo->perf_state.gate) {
            // Copy current sub-block. For partial chunks, extend with the last sample
            // to avoid hard transitions that would create clicks.
            const float* inputChunk = input + offset;
            memcpy(algo->temp_blow_in, inputChunk, chunk * sizeof(float));
            if (chunk < kElementsBlockSize) {
                const float padValue = chunk > 0 ? inputChunk[chunk - 1] : 0.0f;
                for (int i = chunk; i < kElementsBlockSize; ++i) {
                    algo->temp_blow_in[i] = padValue;
                }
            }
            memset(algo->temp_strike_in, 0, kElementsBlockSize * sizeof(float));

            algo->elements_part->Process(
                algo->perf_state,
                algo->temp_blow_in,
                algo->temp_strike_in,
                algo->temp_main_out,
                algo->temp_aux_out,
                static_cast<size_t>(kElementsBlockSize)
            );
        } else {
            memset(algo->temp_main_out, 0, kElementsBlockSize * sizeof(float));
        }

        float* outputChunk = output + offset;
        if (outputMode == 1) {
            memcpy(outputChunk, algo->temp_main_out, chunk * sizeof(float));
        } else {
            for (int i = 0; i < chunk; ++i) {
                outputChunk[i] += algo->temp_main_out[i];
            }
        }
    }
}

// Custom UI handler - used for button 4 diagnostic trigger
static uint32_t hasCustomUi(_NT_algorithm* /*self*/) {
    return kNT_button4;  // We handle button 4
}

static void customUi(_NT_algorithm* self, const _NT_uiData& data) {
    nt_elementsAlgorithm* algo = static_cast<nt_elementsAlgorithm*>(self);

    // Debounce button 4: only trigger on rising edge if state actually changed
    uint16_t current_buttons = data.controls & 0xFFFF;
    bool button4_now = current_buttons & kNT_button4;
    bool button4_last = algo->last_button_state & kNT_button4;

    // Save current state for next call
    algo->last_button_state = current_buttons;

    // Button 4 functionality can be added here if needed
    (void)button4_now;  // Suppress unused warning for now
    (void)button4_last;
}

static void midiMessage(_NT_algorithm* self, uint8_t b0, uint8_t b1, uint8_t b2) {
    nt_elementsAlgorithm* algo = static_cast<nt_elementsAlgorithm*>(self);

    // Parse MIDI message
    const uint8_t status = b0 & 0xF0;
    const uint8_t channel = b0 & 0x0F;
    const uint8_t data1 = b1 & 0x7F;
    const uint8_t data2 = b2 & 0x7F;

    (void)channel;  // Suppress unused warning

    // Copy current state to pending (preserves fields we don't modify)
    if (!algo->pending_update) {
        algo->pending_state = algo->perf_state;
    }

    if (status == 0x90) {  // Note on
        if (data2 > 0) {  // Velocity > 0 = true note on
            // Update current note (last-note priority)
            algo->current_note = data1;

            // Update pending state with MIDI note number (not Hz!)
            // Elements::Part::Process will convert to Hz internally using lookup tables
            algo->pending_state.note = static_cast<float>(data1);
            algo->pending_state.gate = true;

            // Map velocity to strength
            algo->pending_state.strength = static_cast<float>(data2) / 127.0f;

            // Mark update ready
            algo->pending_update = true;

#ifdef NT_EMU_DEBUG
            printf("MIDI Note On: %d velocity=%d\n", data1, data2);
#endif
        } else {  // Velocity 0 = note off (running status)
            // Only turn off gate if this note is currently playing
            if (data1 == algo->current_note) {
                algo->pending_state.gate = false;
                algo->pending_update = true;
#ifdef NT_EMU_DEBUG
                printf("MIDI Note Off (running status): %d\n", data1);
#endif
            }
        }
    } else if (status == 0x80) {  // Note off
        // Only turn off gate if this note is currently playing (monophonic last-note priority)
        if (data1 == algo->current_note) {
            algo->pending_state.gate = false;
            algo->pending_update = true;
#ifdef NT_EMU_DEBUG
            printf("MIDI Note Off: %d\n", data1);
#endif
        }
    } else if (status == 0xE0) {  // Pitch bend
        // Parse 14-bit pitch bend value (LSB in data1, MSB in data2)
        const int bend_value = (data2 << 7) | data1;  // 0-16383

        // Convert to semitones: center (8192) = 0, range ±2 semitones
        // Store in pending_state.modulation - Elements will add this to the MIDI note
        algo->pending_state.modulation = ((static_cast<float>(bend_value) - 8192.0f) / 8192.0f) * 2.0f;
        algo->pending_update = true;

#ifdef NT_EMU_DEBUG
        printf("MIDI Pitch Bend: %d (%.2f semitones)\n", bend_value, algo->perf_state.modulation);
#endif
    }
}

// Plugin entry point
extern "C" {
    uintptr_t pluginEntry(_NT_selector selector, uint32_t data) {
        switch (selector) {
            case kNT_selector_version:
                return kNT_apiVersionCurrent;

            case kNT_selector_numFactories:
                return 1;

            case kNT_selector_factoryInfo:
                // data is the factory index (0-based)
                return (data == 0) ? reinterpret_cast<uintptr_t>(&factory) : 0;

            default:
                return 0;
        }
    }
}
