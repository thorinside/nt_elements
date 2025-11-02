/*
 * nt_elements - Mutable Instruments Elements port for disting NT
 *
 * Copyright (c) 2025 Neal Sanche
 * Based on Mutable Instruments Elements
 * Copyright (c) 2013-2014 Emilie Gillet
 *
 * Licensed under MIT License - see LICENSE file for details
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
#include "parameter_pages.h"
#include "nt_elements.h"
#include "oled_display.h"

// Factory functions forward declarations
static void calculateStaticRequirements(_NT_staticRequirements& req);
static void initialise(_NT_staticMemoryPtrs& ptrs, const _NT_staticRequirements& req);
static void calculateRequirements(_NT_algorithmRequirements& req, const int32_t* specifications);
static _NT_algorithm* construct(const _NT_algorithmMemoryPtrs& ptrs, const _NT_algorithmRequirements& req, const int32_t* specifications);
static void parameterChanged(_NT_algorithm* self, int p);
static void step(_NT_algorithm* self, float* busFrames, int numFramesBy4);
static bool draw(_NT_algorithm* self);
static void midiMessage(_NT_algorithm* self, uint8_t b0, uint8_t b1, uint8_t b2);
static uint32_t hasCustomUi(_NT_algorithm* self);
static void customUi(_NT_algorithm* self, const _NT_uiData& data);
static void setupUi(_NT_algorithm* self, _NT_float3& pots);

// Parameter definitions
static const _NT_parameter parameters[kNumParams] = {
    // System parameters
    NT_PARAMETER_AUDIO_INPUT("Input", 1, 1)
    NT_PARAMETER_AUDIO_OUTPUT_WITH_MODE("Output", 1, 13)

    // Page 1 - Exciter (5 parameters)
    { .name = "Bow Level", .min = 0, .max = 100, .def = 80, .unit = kNT_unitPercent, .scaling = 0, .enumStrings = NULL },
    { .name = "Blow Level", .min = 0, .max = 100, .def = 0, .unit = kNT_unitPercent, .scaling = 0, .enumStrings = NULL },
    { .name = "Strike Level", .min = 0, .max = 100, .def = 0, .unit = kNT_unitPercent, .scaling = 0, .enumStrings = NULL },
    { .name = "Bow Timbre", .min = 0, .max = 100, .def = 50, .unit = kNT_unitPercent, .scaling = 0, .enumStrings = NULL },
    { .name = "Blow Timbre", .min = 0, .max = 100, .def = 50, .unit = kNT_unitPercent, .scaling = 0, .enumStrings = NULL },

    // Page 2 - Resonator (5 parameters)
    { .name = "Geometry", .min = 0, .max = 100, .def = 50, .unit = kNT_unitPercent, .scaling = 0, .enumStrings = NULL },
    { .name = "Brightness", .min = 0, .max = 100, .def = 70, .unit = kNT_unitPercent, .scaling = 0, .enumStrings = NULL },
    { .name = "Damping", .min = 0, .max = 100, .def = 60, .unit = kNT_unitPercent, .scaling = 0, .enumStrings = NULL },
    { .name = "Position", .min = 0, .max = 100, .def = 50, .unit = kNT_unitPercent, .scaling = 0, .enumStrings = NULL },
    { .name = "Inharmonic", .min = 0, .max = 100, .def = 0, .unit = kNT_unitPercent, .scaling = 0, .enumStrings = NULL },

    // Page 3 - Space (3 parameters)
    { .name = "Reverb Amt", .min = 0, .max = 100, .def = 20, .unit = kNT_unitPercent, .scaling = 0, .enumStrings = NULL },
    { .name = "Reverb Size", .min = 0, .max = 100, .def = 50, .unit = kNT_unitPercent, .scaling = 0, .enumStrings = NULL },
    { .name = "Reverb Damp", .min = 0, .max = 100, .def = 70, .unit = kNT_unitPercent, .scaling = 0, .enumStrings = NULL },

    // Page 4 - Performance (5 parameters)
    { .name = "Coarse Tune", .min = 0, .max = 100, .def = 50, .unit = kNT_unitPercent, .scaling = 0, .enumStrings = NULL },
    { .name = "Fine Tune", .min = 0, .max = 100, .def = 50, .unit = kNT_unitPercent, .scaling = 0, .enumStrings = NULL },
    { .name = "Output Lvl", .min = 0, .max = 100, .def = 100, .unit = kNT_unitPercent, .scaling = 0, .enumStrings = NULL },
    { .name = "FM Amount", .min = 0, .max = 100, .def = 0, .unit = kNT_unitPercent, .scaling = 0, .enumStrings = NULL },
    { .name = "Exciter Cnt", .min = 0, .max = 100, .def = 50, .unit = kNT_unitPercent, .scaling = 0, .enumStrings = NULL },
};

// Parameter pages for menu organization
static const uint8_t pageExciter[] = {
    kParamBowLevel, kParamBlowLevel, kParamStrikeLevel, kParamBowTimbre, kParamBlowTimbre
};

static const uint8_t pageResonator[] = {
    kParamGeometry, kParamBrightness, kParamDamping, kParamResonatorPosition, kParamInharmonicity
};

static const uint8_t pageSpace[] = {
    kParamReverbAmount, kParamReverbSize, kParamReverbDamping
};

static const uint8_t pagePerformance[] = {
    kParamCoarseTune, kParamFineTune, kParamOutputLevel, kParamFMAmount, kParamExciterContour
};

static const uint8_t pageRouting[] = {
    kParamInputBus, kParamOutputBus, kParamOutputMode
};

static const _NT_parameterPage pages[] = {
    { .name = "Exciter", .numParams = sizeof(pageExciter), .params = pageExciter },
    { .name = "Resonator", .numParams = sizeof(pageResonator), .params = pageResonator },
    { .name = "Space", .numParams = sizeof(pageSpace), .params = pageSpace },
    { .name = "Performance", .numParams = sizeof(pagePerformance), .params = pagePerformance },
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
    .draw = draw,
    .midiRealtime = NULL,
    .midiMessage = midiMessage,
    .tags = kNT_tagInstrument,
    .hasCustomUi = hasCustomUi,
    .customUi = customUi,
    .setupUi = setupUi
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

    // Initialize page navigation (start on Page 1 - Exciter)
    self->current_page = kPageExciter;

    // Initialize tuning offset (centered = no offset)
    self->tuning_offset_semitones = 0.0f;

    // Initialize output level (default = 100% = full volume)
    self->output_level_scale = 1.0f;

    // Initialize display state (start with dirty flag set to render initial display)
    self->display_dirty = true;

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

    // Mark display dirty for parameter changes
    algo->display_dirty = true;

    // Convert NT parameters (0-100%) to Elements Patch fields (0.0-1.0)
    switch (p) {
        // Page 1 - Exciter parameters
        case kParamBowLevel:
            patch->exciter_bow_level = parameter_adapter::ntToElements(self->v[kParamBowLevel]);
            break;

        case kParamBlowLevel:
            patch->exciter_blow_level = parameter_adapter::ntToElements(self->v[kParamBlowLevel]);
            break;

        case kParamStrikeLevel:
            patch->exciter_strike_level = parameter_adapter::ntToElements(self->v[kParamStrikeLevel]);
            break;

        case kParamBowTimbre:
            patch->exciter_bow_timbre = parameter_adapter::ntToElements(self->v[kParamBowTimbre]);
            break;

        case kParamBlowTimbre:
            patch->exciter_blow_timbre = parameter_adapter::ntToElements(self->v[kParamBlowTimbre]);
            break;

        // Page 2 - Resonator parameters
        case kParamGeometry:
            patch->resonator_geometry = parameter_adapter::ntToElements(self->v[kParamGeometry]);
            break;

        case kParamBrightness:
            patch->resonator_brightness = parameter_adapter::ntToElements(self->v[kParamBrightness]);
            break;

        case kParamDamping:
            patch->resonator_damping = parameter_adapter::ntToElements(self->v[kParamDamping]);
            break;

        case kParamResonatorPosition:
            patch->resonator_position = parameter_adapter::ntToElements(self->v[kParamResonatorPosition]);
            break;

        case kParamInharmonicity:
            patch->resonator_modulation_frequency = parameter_adapter::ntToElements(self->v[kParamInharmonicity]);
            break;

        // Page 3 - Space (Reverb) parameters
        case kParamReverbAmount:
            patch->space = parameter_adapter::ntToElements(self->v[kParamReverbAmount]);
            break;

        case kParamReverbSize:
            patch->reverb_lp = parameter_adapter::ntToElements(self->v[kParamReverbSize]);
            break;

        case kParamReverbDamping:
            patch->reverb_diffusion = parameter_adapter::ntToElements(self->v[kParamReverbDamping]);
            break;

        // Page 4 - Performance parameters
        case kParamCoarseTune:
        case kParamFineTune:
            {
                // Coarse tune: 0-100% maps to -12 to +12 semitones
                float coarse_semitones = (self->v[kParamCoarseTune] - 50.0f) * 0.24f;  // (x - 50) * 24/100

                // Fine tune: 0-100% maps to -50 to +50 cents
                float fine_cents = (self->v[kParamFineTune] - 50.0f) * 1.0f;  // (x - 50) * 100/100

                // Combine: total offset in semitones
                algo->tuning_offset_semitones = coarse_semitones + (fine_cents / 100.0f);
            }
            break;

        case kParamOutputLevel:
            // Output level: 0-100% maps to 0.0-1.0 scaling
            algo->output_level_scale = parameter_adapter::ntToElements(self->v[kParamOutputLevel]);
            break;

        case kParamFMAmount:
            patch->modulation_frequency = parameter_adapter::ntToElements(self->v[kParamFMAmount]);
            break;

        case kParamExciterContour:
            patch->exciter_envelope_shape = parameter_adapter::ntToElements(self->v[kParamExciterContour]);
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

    // Apply tuning offset to MIDI note
    // Elements::Part stores note as MIDI note number, modulation is added during processing
    // We add our tuning offset to the modulation field
    float original_modulation = algo->perf_state.modulation;
    algo->perf_state.modulation = original_modulation + algo->tuning_offset_semitones;

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
            // Replace mode: copy with output level scaling
            for (int i = 0; i < chunk; ++i) {
                outputChunk[i] = algo->temp_main_out[i] * algo->output_level_scale;
            }
        } else {
            // Mix mode: add with output level scaling
            for (int i = 0; i < chunk; ++i) {
                outputChunk[i] += algo->temp_main_out[i] * algo->output_level_scale;
            }
        }
    }

    // Restore original modulation value (don't let tuning offset accumulate)
    algo->perf_state.modulation = original_modulation;
}

// Draw callback - render OLED display
static bool draw(_NT_algorithm* self) {
    nt_elementsAlgorithm* algo = static_cast<nt_elementsAlgorithm*>(self);

    // Only redraw if display is dirty (parameter or page changed)
    if (!algo->display_dirty) {
        return false;  // Don't suppress standard parameter line
    }

    // Render the display
    oled_display::renderDisplay(algo);

    // Clear dirty flag
    algo->display_dirty = false;

    // Return false to not suppress the standard parameter line at top
    return false;
}

// Custom UI handler - used for page navigation via button presses and pot/encoder routing
static uint32_t hasCustomUi(_NT_algorithm* /*self*/) {
    // We handle buttons 1 and 2 for page navigation (prev/next)
    // We also handle all pots and encoders for per-page parameter routing
    return kNT_button1 | kNT_button2 | kNT_potL | kNT_potC | kNT_potR | kNT_encoderL | kNT_encoderR;
}

static void customUi(_NT_algorithm* self, const _NT_uiData& data) {
    nt_elementsAlgorithm* algo = static_cast<nt_elementsAlgorithm*>(self);

    // Get current and last button states
    uint16_t current_buttons = data.controls & 0xFFFF;

    // Detect button press events (rising edge detection)
    bool button1_pressed = (current_buttons & kNT_button1) && !(algo->last_button_state & kNT_button1);
    bool button2_pressed = (current_buttons & kNT_button2) && !(algo->last_button_state & kNT_button2);
    bool button3_pressed = (current_buttons & kNT_button3) && !(algo->last_button_state & kNT_button3);
    bool button4_pressed = (current_buttons & kNT_button4) && !(algo->last_button_state & kNT_button4);

    // Save current state for next call
    algo->last_button_state = current_buttons;

    // Button 1: Previous page (encoder 1 button)
    if (button1_pressed) {
        algo->current_page = (algo->current_page - 1 + kNumPages) % kNumPages;
        algo->display_dirty = true;  // Mark display dirty for page change
#ifdef NT_EMU_DEBUG
        printf("Page navigation: Button 1 -> Page %d (%s)\n",
               algo->current_page, PAGE_MAPPINGS[algo->current_page].name);
#endif
    }

    // Button 2: Next page (encoder 2 button)
    if (button2_pressed) {
        algo->current_page = (algo->current_page + 1) % kNumPages;
        algo->display_dirty = true;  // Mark display dirty for page change
#ifdef NT_EMU_DEBUG
        printf("Page navigation: Button 2 -> Page %d (%s)\n",
               algo->current_page, PAGE_MAPPINGS[algo->current_page].name);
#endif
    }

    // Buttons 3 and 4: Reserved for future use
    (void)button3_pressed;
    (void)button4_pressed;

    // Get current page mapping
    const PageMapping& page = PAGE_MAPPINGS[algo->current_page];

    // Handle pot changes - pots provide absolute values (0.0-1.0)
    // When we intercept pots/encoders via customUi, we bypass the NT parameter system
    // and directly update the Elements Patch. This allows performance-oriented control
    // where pots change function based on the current page.

    elements::Patch* patch = algo->elements_part->mutable_patch();

    // Check which pots changed using the controls bitmask
    for (int i = 0; i < 3; ++i) {
        uint16_t pot_mask = (i == 0) ? kNT_potL : (i == 1) ? kNT_potC : kNT_potR;
        if (data.controls & pot_mask) {
            // Pot changed - route to Elements Patch field based on current page
            int param_index = page.pot_mapping[i];
            float pot_value = data.pots[i];  // 0.0-1.0 from hardware

            // Mark display dirty for pot change
            algo->display_dirty = true;

            // Update Elements Patch directly based on parameter mapping
            switch (param_index) {
                // Page 1 - Exciter
                case kParamBowLevel:
                    patch->exciter_bow_level = pot_value;
                    break;
                case kParamBlowLevel:
                    patch->exciter_blow_level = pot_value;
                    break;
                case kParamStrikeLevel:
                    patch->exciter_strike_level = pot_value;
                    break;

                // Page 2 - Resonator
                case kParamGeometry:
                    patch->resonator_geometry = pot_value;
                    break;
                case kParamBrightness:
                    patch->resonator_brightness = pot_value;
                    break;
                case kParamDamping:
                    patch->resonator_damping = pot_value;
                    break;

                // Page 3 - Space
                case kParamReverbAmount:
                    patch->space = pot_value;
                    break;
                case kParamReverbSize:
                    patch->reverb_lp = pot_value;
                    break;
                case kParamReverbDamping:
                    patch->reverb_diffusion = pot_value;
                    break;

                // Page 4 - Performance
                case kParamCoarseTune:
                    // Convert 0.0-1.0 to -12 to +12 semitones
                    algo->tuning_offset_semitones = (pot_value - 0.5f) * 24.0f;
                    break;
                case kParamFineTune:
                    // Convert 0.0-1.0 to -50 to +50 cents
                    algo->tuning_offset_semitones = (pot_value - 0.5f) * 1.0f;
                    break;
                case kParamOutputLevel:
                    algo->output_level_scale = pot_value;
                    break;

                default:
                    break;
            }

#ifdef NT_EMU_DEBUG
            printf("Pot %d changed on page %s: param=%d value=%.2f\n",
                   i, page.name, param_index, pot_value);
#endif
        }
    }

    // Handle encoder changes - encoders provide delta values (±1 or 0)
    // For encoders, we need to track the current value and apply deltas
    // We'll store encoder values in the algorithm structure
    for (int i = 0; i < 2; ++i) {
        int8_t delta = data.encoders[i];
        if (delta != 0) {
            // Encoder changed - route to Elements Patch field based on current page
            int param_index = page.encoder_mapping[i];

            // Check if encoder is mapped to a parameter (not reserved)
            if (param_index < 0) {
                continue;  // Encoder not assigned on this page
            }

            // Mark display dirty for encoder change
            algo->display_dirty = true;

            // Get current value from v[] (which is managed by framework for menu parameters)
            // For performance controls, we need to maintain state
            float step_size = 0.01f;  // 1% per detent
            float current_value = 0.5f;  // Default center

            // Try to get current value from v[] if available
            const _NT_parameter& param = parameters[param_index];
            current_value = (self->v[param_index] - param.min) / (param.max - param.min);

            // Apply delta
            float new_value = current_value + (delta * step_size);

            // Clamp to 0.0-1.0 range
            if (new_value < 0.0f) new_value = 0.0f;
            if (new_value > 1.0f) new_value = 1.0f;

            // Update Elements Patch directly based on parameter mapping
            switch (param_index) {
                // Page 1 - Exciter
                case kParamBowTimbre:
                    patch->exciter_bow_timbre = new_value;
                    break;
                case kParamBlowTimbre:
                    patch->exciter_blow_timbre = new_value;
                    break;

                // Page 2 - Resonator
                case kParamResonatorPosition:
                    patch->resonator_position = new_value;
                    break;
                case kParamInharmonicity:
                    patch->resonator_modulation_frequency = new_value;
                    break;

                // Page 4 - Performance
                case kParamFMAmount:
                    patch->modulation_frequency = new_value;
                    break;
                case kParamExciterContour:
                    patch->exciter_envelope_shape = new_value;
                    break;

                default:
                    break;
            }

#ifdef NT_EMU_DEBUG
            printf("Encoder %d changed on page %s: param=%d delta=%d new_value=%.2f\n",
                   i, page.name, param_index, delta, new_value);
#endif
        }
    }
}

static void setupUi(_NT_algorithm* self, _NT_float3& pots) {
    nt_elementsAlgorithm* algo = static_cast<nt_elementsAlgorithm*>(self);

    // Get current page mapping
    const PageMapping& page = PAGE_MAPPINGS[algo->current_page];

    // Initialize pot positions from current parameter values
    for (int i = 0; i < 3; ++i) {
        int param_index = page.pot_mapping[i];
        const _NT_parameter& param = parameters[param_index];

        // Convert parameter value (0-100%) to pot position (0.0-1.0)
        float param_value = self->v[param_index];
        pots[i] = (param_value - param.min) / (param.max - param.min);
    }
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
