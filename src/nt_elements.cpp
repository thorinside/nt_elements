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
#include "distingnt/wav.h"
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
#include "sample_manager.h"

// Global pointers for Elements sample data (declared in elements/resources.h via patch)
// These are set after SampleManager loads samples from SD card
namespace elements {
const int16_t* smp_sample_data_ptr = nullptr;
const int16_t* smp_noise_sample_ptr = nullptr;
const size_t* smp_boundaries_ptr = nullptr;
}

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
    // System parameters - Dual external inputs for Elements
    // Blow Input: Audio goes through diffusion → envelope → STRENGTH VCA → resonator
    // Strike Input: Audio goes directly to resonator (unprocessed)
    NT_PARAMETER_AUDIO_INPUT("Blow Input", 1, 1)
    NT_PARAMETER_AUDIO_INPUT("Strike Input", 0, 0)
    NT_PARAMETER_AUDIO_OUTPUT_WITH_MODE("Main Output", 1, 13)
    NT_PARAMETER_AUDIO_OUTPUT_WITH_MODE("Aux Output", 1, 14)

    // Page 1 - Exciter (8 parameters)
    { .name = "Bow Level", .min = 0, .max = 100, .def = 80, .unit = kNT_unitPercent, .scaling = 0, .enumStrings = NULL },
    { .name = "Blow Level", .min = 0, .max = 100, .def = 0, .unit = kNT_unitPercent, .scaling = 0, .enumStrings = NULL },
    { .name = "Strike Level", .min = 0, .max = 100, .def = 0, .unit = kNT_unitPercent, .scaling = 0, .enumStrings = NULL },
    { .name = "Bow Timbre", .min = 0, .max = 100, .def = 50, .unit = kNT_unitPercent, .scaling = 0, .enumStrings = NULL },
    { .name = "Blow Timbre", .min = 0, .max = 100, .def = 50, .unit = kNT_unitPercent, .scaling = 0, .enumStrings = NULL },
    { .name = "Str Timbre", .min = 0, .max = 100, .def = 50, .unit = kNT_unitPercent, .scaling = 0, .enumStrings = NULL },
    { .name = "Flow", .min = 0, .max = 100, .def = 50, .unit = kNT_unitPercent, .scaling = 0, .enumStrings = NULL },
    { .name = "Mallet", .min = 0, .max = 100, .def = 50, .unit = kNT_unitPercent, .scaling = 0, .enumStrings = NULL },

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

    // Page 4 - Performance (6 parameters)
    { .name = "Coarse Tune", .min = 0, .max = 100, .def = 50, .unit = kNT_unitPercent, .scaling = 0, .enumStrings = NULL },
    { .name = "Fine Tune", .min = 0, .max = 100, .def = 50, .unit = kNT_unitPercent, .scaling = 0, .enumStrings = NULL },
    { .name = "Output Lvl", .min = 0, .max = 100, .def = 100, .unit = kNT_unitPercent, .scaling = 0, .enumStrings = NULL },
    { .name = "FM Amount", .min = 0, .max = 100, .def = 0, .unit = kNT_unitPercent, .scaling = 0, .enumStrings = NULL },
    { .name = "Exciter Cnt", .min = 0, .max = 100, .def = 50, .unit = kNT_unitPercent, .scaling = 0, .enumStrings = NULL },
    { .name = "Strength", .min = 0, .max = 100, .def = 80, .unit = kNT_unitPercent, .scaling = 0, .enumStrings = NULL },

    // Page 5 - Routing (MIDI Chan 0=Off, 1-16=specific channel + CV inputs)
    { .name = "MIDI Chan", .min = 0, .max = 16, .def = 1, .unit = kNT_unitNone, .scaling = 0, .enumStrings = NULL },
    NT_PARAMETER_CV_INPUT("V/Oct CV", 0, 0)
    NT_PARAMETER_CV_INPUT("Gate CV", 0, 0)
    NT_PARAMETER_CV_INPUT("FM CV", 0, 0)
    NT_PARAMETER_CV_INPUT("Bright CV", 0, 0)
    NT_PARAMETER_CV_INPUT("Expr CV", 0, 0)
};

// Parameter pages for menu organization
static const uint8_t pageExciter[] = {
    kParamBowLevel, kParamBlowLevel, kParamStrikeLevel, kParamBowTimbre, kParamBlowTimbre, kParamStrikeTimbre, kParamBlowFlow, kParamStrikeMallet
};

static const uint8_t pageResonator[] = {
    kParamGeometry, kParamBrightness, kParamDamping, kParamResonatorPosition, kParamInharmonicity
};

static const uint8_t pageSpace[] = {
    kParamReverbAmount, kParamReverbSize, kParamReverbDamping
};

static const uint8_t pagePerformance[] = {
    kParamCoarseTune, kParamFineTune, kParamOutputLevel, kParamFMAmount, kParamExciterContour, kParamStrength
};

static const uint8_t pageRouting[] = {
    kParamBlowInputBus, kParamStrikeInputBus, kParamOutputBus, kParamOutputMode,
    kParamAuxOutputBus, kParamAuxOutputMode,
    kParamMidiChannel, kParamVOctCV, kParamGateCV,
    kParamFMCV, kParamBrightnessCV, kParamExpressionCV
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

    // DRAM: Reverb buffer (32768 samples = 64KB for uint16_t) + Sample data (~338KB)
    // Layout: [reverb_buffer (64KB)] [sample_data (~338KB)]
    req.dram = (32768 * sizeof(uint16_t)) + SampleManager::kTotalDramBytes;

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

    // Allocate reverb buffer from DRAM (first 64KB)
    self->reverb_buffer = reinterpret_cast<uint16_t*>(ptrs.dram);

    // Initialize sample manager with remaining DRAM (after reverb buffer)
    // DRAM layout: [reverb_buffer (64KB)] [sample_data (~338KB)]
    uint8_t* sample_dram = reinterpret_cast<uint8_t*>(ptrs.dram) + (32768 * sizeof(uint16_t));
    self->sample_manager.init(reinterpret_cast<int16_t*>(sample_dram));
    self->sd_card_was_mounted = false;

    // Initialize global sample pointers to point to zero-filled buffers
    // These will be updated to real samples when loading completes in step()
    elements::smp_sample_data_ptr = self->sample_manager.getSampleData();
    elements::smp_noise_sample_ptr = self->sample_manager.getNoiseSample();
    elements::smp_boundaries_ptr = self->sample_manager.getBoundaries();

    // Use placement new to construct Elements Part in DTC
    self->elements_part = new (ptrs.dtc) elements::Part();

    // Initialize Elements Part with reverb buffer
    self->elements_part->Init(self->reverb_buffer);

    // Initialize base strength (default = 80%) - must be before perf_state init
    self->base_strength = 0.8f;

    // Initialize default performance state (gate ON by default to trigger bow exciter)
    self->perf_state.gate = true;   // Enable gate so bow exciter produces sound on startup
    self->perf_state.note = 69.0f;  // MIDI note number A4 (69)
    self->perf_state.modulation = 0.0f;  // Pitch bend offset in semitones
    self->perf_state.strength = self->base_strength;

    // Initialize MIDI state
    self->current_note = 0;
    self->pending_update = false;

    // Initialize pending state to match current state
    self->pending_state.gate = true;   // Match perf_state
    self->pending_state.note = 69.0f;
    self->pending_state.modulation = 0.0f;
    self->pending_state.strength = self->base_strength;

    // Initialize page navigation (start on Page 1 - Exciter)
    self->current_page = kPageExciter;
    self->last_displayed_page = -1;  // Force page name display on first draw
    self->page_change_frame = 0;
    self->draw_frame_count = 0;

    // Initialize tuning offset (centered = no offset)
    self->tuning_offset_semitones = 0.0f;

    // Initialize output level (default = 100% = full volume)
    self->output_level_scale = 1.0f;

    // Initialize CV input state
    self->gate_cv_was_high = false;

    // Initialize block size adaptation buffers
    memset(self->blow_input_buffer, 0, sizeof(self->blow_input_buffer));
    memset(self->strike_input_buffer, 0, sizeof(self->strike_input_buffer));
    memset(self->output_main, 0, sizeof(self->output_main));
    memset(self->output_aux, 0, sizeof(self->output_aux));
    self->buffer_pos = 0;

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
    // Defensive validation: Check for null pointer (protection against emulator reload race conditions)
    if (!self) {
        return;
    }

    nt_elementsAlgorithm* algo = static_cast<nt_elementsAlgorithm*>(self);

    // Validate algorithm structure integrity
    if (!algo->elements_part) {
        return;  // Plugin being destroyed during reload
    }

    elements::Patch* patch = algo->elements_part->mutable_patch();

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

        case kParamStrikeTimbre:
            patch->exciter_strike_timbre = parameter_adapter::ntToElements(self->v[kParamStrikeTimbre]);
            break;

        case kParamBlowFlow:
            patch->exciter_blow_meta = parameter_adapter::ntToElements(self->v[kParamBlowFlow]);
            break;

        case kParamStrikeMallet:
            patch->exciter_strike_meta = parameter_adapter::ntToElements(self->v[kParamStrikeMallet]);
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

        case kParamStrength:
            algo->base_strength = parameter_adapter::ntToElements(self->v[kParamStrength]);
            break;

        // Bus routing and CV input parameters don't need handling (used directly in step())
        case kParamBlowInputBus:
        case kParamStrikeInputBus:
        case kParamOutputBus:
        case kParamOutputMode:
        case kParamAuxOutputBus:
        case kParamAuxOutputMode:
        case kParamMidiChannel:
        case kParamVOctCV:
        case kParamGateCV:
        case kParamFMCV:
        case kParamBrightnessCV:
        case kParamExpressionCV:
        default:
            break;
    }
}

static void step(_NT_algorithm* self, float* busFrames, int numFramesBy4) {
    // Defensive validation: Check for null pointers (protection against emulator reload race conditions)
    if (!self || !busFrames) {
        return;
    }

    nt_elementsAlgorithm* algo = static_cast<nt_elementsAlgorithm*>(self);

    // Validate algorithm structure integrity
    if (!algo->elements_part || !algo->temp_blow_in || !algo->temp_strike_in ||
        !algo->temp_main_out || !algo->temp_aux_out) {
        return;  // Plugin being destroyed during reload
    }

    // Non-blocking sample loading via state machine
    // Per distingNT API: "All built-in algorithms watch for card (un)mount in step()"
    bool sd_mounted = NT_isSdCardMounted();
    if (!sd_mounted && algo->sd_card_was_mounted) {
        // SD card just unmounted - reset sample manager for potential reload
        algo->sample_manager.reset();
        // Reset global pointers to point to zero-filled buffers
        elements::smp_sample_data_ptr = algo->sample_manager.getSampleData();
        elements::smp_noise_sample_ptr = algo->sample_manager.getNoiseSample();
        elements::smp_boundaries_ptr = algo->sample_manager.getBoundaries();
    }
    algo->sd_card_was_mounted = sd_mounted;

    // Call loadStep() each frame - it's non-blocking and manages its own state machine
    // Samples will load progressively over multiple step() calls
    if (algo->sample_manager.loadStep()) {
        // Samples just loaded (or already loaded) - update global pointers for Elements DSP
        // These pointers are used by exciter.cc via macros in resources.h
        elements::smp_sample_data_ptr = algo->sample_manager.getSampleData();
        elements::smp_noise_sample_ptr = algo->sample_manager.getNoiseSample();
        elements::smp_boundaries_ptr = algo->sample_manager.getBoundaries();
    }

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

    // Process CV inputs (CV takes priority over MIDI when Gate CV is high)
    const int voct_bus = static_cast<int>(self->v[kParamVOctCV]) - 1;
    const int gate_bus = static_cast<int>(self->v[kParamGateCV]) - 1;

    // busFrames layout: [bus0_frames][bus1_frames]...[bus27_frames]
    const int numFrames = numFramesBy4 * 4;

    // Guard against buffer overflow (temp buffers sized for 512 frames)
    if (numFrames > 512 || numFrames <= 0) {
        return;  // Protect SRAM; invalid buffer sizes
    }

    const float* voct_cv = nullptr;
    const float* gate_cv = nullptr;

    if (voct_bus >= 0 && voct_bus < 28) {
        voct_cv = busFrames + (voct_bus * numFrames);
    }
    if (gate_bus >= 0 && gate_bus < 28) {
        gate_cv = busFrames + (gate_bus * numFrames);
    }

    // Process CV inputs (use first sample of block for control-rate processing)
    if (gate_cv != nullptr) {
        float gate_voltage = gate_cv[0];
        bool gate_high = gate_voltage > 1.0f;  // Eurorack gate threshold

        if (gate_high) {
            // Gate is HIGH - CV takes priority over MIDI
            // Read V/OCT CV (default to 0V = C4 if not connected)
            float voct_voltage = voct_cv ? voct_cv[0] : 0.0f;

            // Convert V/OCT to MIDI note: 1V/octave, 0V = C4 (MIDI 60)
            float cv_note = (voct_voltage * 12.0f) + 60.0f;

            // Clamp to valid MIDI range (0-127)
            cv_note = fmaxf(0.0f, fminf(127.0f, cv_note));

            // Override performance state with CV values
            algo->perf_state.note = cv_note;
            algo->perf_state.gate = true;
            algo->perf_state.strength = algo->base_strength;  // Use parameter-defined strength for CV input
        }
        // If gate is LOW, fall through to use MIDI state (already applied above)

        algo->gate_cv_was_high = gate_high;
    }

    // Apply tuning offset to MIDI note
    // Elements::Part stores note as MIDI note number, modulation is added during processing
    // We add our tuning offset to the modulation field
    float original_modulation = algo->perf_state.modulation;
    algo->perf_state.modulation = original_modulation + algo->tuning_offset_semitones;

    // Apply CV modulation to patch parameters (control-rate using first sample)
    elements::Patch* patch = algo->elements_part->mutable_patch();

    // FM Amount CV modulation
    const int fm_cv_bus = static_cast<int>(self->v[kParamFMCV]) - 1;
    if (fm_cv_bus >= 0 && fm_cv_bus < 28 && busFrames) {
        const float* fm_cv = busFrames + (fm_cv_bus * numFrames);
        // CV range: -5V to +5V modulates FM amount (bipolar)
        // Clamp to prevent excessive modulation
        float fm_mod = fmaxf(-1.0f, fminf(1.0f, fm_cv[0] * 0.2f));
        patch->modulation_frequency = fmaxf(0.0f, fminf(1.0f,
            parameter_adapter::ntToElements(self->v[kParamFMAmount]) + fm_mod));
    }

    // Brightness CV modulation
    const int brightness_cv_bus = static_cast<int>(self->v[kParamBrightnessCV]) - 1;
    if (brightness_cv_bus >= 0 && brightness_cv_bus < 28 && busFrames) {
        const float* brightness_cv = busFrames + (brightness_cv_bus * numFrames);
        // CV range: -5V to +5V modulates brightness (bipolar)
        float bright_mod = fmaxf(-1.0f, fminf(1.0f, brightness_cv[0] * 0.2f));
        patch->resonator_brightness = fmaxf(0.0f, fminf(1.0f,
            parameter_adapter::ntToElements(self->v[kParamBrightness]) + bright_mod));
    }

    // Expression CV modulation (affects exciter levels)
    const int expression_cv_bus = static_cast<int>(self->v[kParamExpressionCV]) - 1;
    if (expression_cv_bus >= 0 && expression_cv_bus < 28 && busFrames) {
        const float* expression_cv = busFrames + (expression_cv_bus * numFrames);
        // CV range: 0-10V modulates exciter strength (unipolar)
        // Scale 0-10V to 0.0-1.0 multiplier
        float expr_mod = fmaxf(0.0f, fminf(1.0f, expression_cv[0] * 0.1f));

        // Apply expression to active exciter levels
        patch->exciter_bow_level = parameter_adapter::ntToElements(self->v[kParamBowLevel]) * expr_mod;
        patch->exciter_blow_level = parameter_adapter::ntToElements(self->v[kParamBlowLevel]) * expr_mod;
        patch->exciter_strike_level = parameter_adapter::ntToElements(self->v[kParamStrikeLevel]) * expr_mod;
    }

#ifdef NT_EMU_DEBUG
    static int debug_counter = 0;
    if (++debug_counter % 1000 == 0) {
        printf("step() called: numFrames=%d, gate=%d, note=%.1f\n",
               numFrames, algo->perf_state.gate, algo->perf_state.note);
    }
#endif

    // Get bus assignments (1-based, convert to 0-based for array access)
    // Dual external inputs: Blow input (processed) and Strike input (direct)
    const int blowInputBus = static_cast<int>(self->v[kParamBlowInputBus]) - 1;
    const int strikeInputBus = static_cast<int>(self->v[kParamStrikeInputBus]) - 1;
    const int outputBus = static_cast<int>(self->v[kParamOutputBus]) - 1;
    const int outputMode = static_cast<int>(self->v[kParamOutputMode]);
    const int auxOutputBus = static_cast<int>(self->v[kParamAuxOutputBus]) - 1;
    const int auxOutputMode = static_cast<int>(self->v[kParamAuxOutputMode]);

    // Validate output bus indices (input buses can be -1 meaning "not connected")
    if (outputBus < 0 || outputBus >= 28 || auxOutputBus < 0 || auxOutputBus >= 28) {
        return;
    }

    // Get bus pointers (with null safety for reload scenarios)
    // Input buses: -1 means not connected (bus 0 in UI), returns nullptr
    const float* blowInput = (busFrames && blowInputBus >= 0 && blowInputBus < 28)
        ? busFrames + (blowInputBus * numFrames) : nullptr;
    const float* strikeInput = (busFrames && strikeInputBus >= 0 && strikeInputBus < 28)
        ? busFrames + (strikeInputBus * numFrames) : nullptr;
    float* output = (busFrames && outputBus >= 0) ? busFrames + (outputBus * numFrames) : nullptr;
    float* auxOutput = (busFrames && auxOutputBus >= 0) ? busFrames + (auxOutputBus * numFrames) : nullptr;

    // Safety check: ensure output pointers are valid before processing
    if (!output || !auxOutput) {
        return;
    }

    // Elements DSP requires exactly 16 samples per block.
    // We accumulate input until we have 16 samples, then process through Elements.
    // The emulator guarantees outputs are read before processing, so single buffering works.

    for (int i = 0; i < numFrames; ++i) {
        // Store input samples in accumulation buffers
        // Blow input: Goes through diffusion → envelope → STRENGTH VCA → resonator
        // Strike input: Goes directly to resonator (unprocessed)
        algo->blow_input_buffer[algo->buffer_pos] = blowInput ? blowInput[i] : 0.0f;
        algo->strike_input_buffer[algo->buffer_pos] = strikeInput ? strikeInput[i] : 0.0f;

        // Output from buffer (filled in previous processing cycle)
        // Scale by 5.0f for Eurorack standard ±5V levels (Elements outputs normalized -1.0 to +1.0)
        float main_out = algo->output_main[algo->buffer_pos] * algo->output_level_scale * 5.0f;
        float aux_out = algo->output_aux[algo->buffer_pos] * algo->output_level_scale * 5.0f;

        if (outputMode == 1) {
            output[i] = main_out;  // Replace mode
        } else {
            output[i] += main_out;  // Mix mode
        }

        if (auxOutputMode == 1) {
            auxOutput[i] = aux_out;  // Replace mode
        } else {
            auxOutput[i] += aux_out;  // Mix mode
        }

        // Advance buffer position
        algo->buffer_pos++;

        // When we've accumulated 16 samples, process through Elements
        if (algo->buffer_pos >= kElementsBlockSize) {
            algo->buffer_pos = 0;

            // Copy accumulated inputs to Elements temp buffers
            memcpy(algo->temp_blow_in, algo->blow_input_buffer, kElementsBlockSize * sizeof(float));
            memcpy(algo->temp_strike_in, algo->strike_input_buffer, kElementsBlockSize * sizeof(float));

            // Process full block through Elements DSP
            algo->elements_part->Process(
                algo->perf_state,
                algo->temp_blow_in,
                algo->temp_strike_in,
                algo->output_main,
                algo->output_aux,
                static_cast<size_t>(kElementsBlockSize)
            );
        }
    }

    // Restore original modulation value (don't let tuning offset accumulate)
    algo->perf_state.modulation = original_modulation;
}

// Draw callback - render OLED display
static bool draw(_NT_algorithm* self) {
    // Defensive validation: Check for null pointer (protection against emulator reload race conditions)
    if (!self) {
        return false;
    }

    nt_elementsAlgorithm* algo = static_cast<nt_elementsAlgorithm*>(self);

    // Validate algorithm structure integrity
    if (!algo->elements_part) {
        return false;  // Plugin being destroyed during reload
    }

    // Always render the display (distingNT calls draw() continuously)
    oled_display::renderDisplay(algo);

    // Return true to indicate we've handled the display (suppress default parameter display)
    return true;
}

// Custom UI handler - used for page navigation and per-page parameter routing
static uint32_t hasCustomUi(_NT_algorithm* /*self*/) {
    // Handle encoder buttons for page navigation and pots/encoders for per-page parameter control
    return kNT_encoderButtonL | kNT_encoderButtonR | kNT_potL | kNT_potC | kNT_potR | kNT_encoderL | kNT_encoderR;
}

static void customUi(_NT_algorithm* self, const _NT_uiData& data) {
    // Defensive validation: Check for null pointer (protection against emulator reload race conditions)
    if (!self) {
        return;
    }

    nt_elementsAlgorithm* algo = static_cast<nt_elementsAlgorithm*>(self);

    // Validate algorithm structure integrity
    if (!algo->elements_part) {
        return;  // Plugin being destroyed during reload
    }

    // Validate current page index (protect against corruption during reload)
    if (algo->current_page < 0 || algo->current_page >= kNumPages) {
        return;  // Invalid page state
    }

    // Get current and last button states
    uint16_t current_buttons = data.controls & 0xFFFF;

    // Detect encoder button press events using lastButtons from uiData (which tracks previous state)
    bool encoderL_pressed = (current_buttons & kNT_encoderButtonL) && !(data.lastButtons & kNT_encoderButtonL);
    bool encoderR_pressed = (current_buttons & kNT_encoderButtonR) && !(data.lastButtons & kNT_encoderButtonR);

    // Encoder 1 button: Previous page (wraps around)
    if (encoderL_pressed) {
        algo->current_page = (algo->current_page - 1 + kNumPages) % kNumPages;
#ifdef NT_EMU_DEBUG
        printf("Page navigation: Encoder L button -> Page %d (%s)\n",
               algo->current_page, PAGE_MAPPINGS[algo->current_page].name);
#endif
    }

    // Encoder 2 button: Next page (wraps around)
    if (encoderR_pressed) {
        algo->current_page = (algo->current_page + 1) % kNumPages;
#ifdef NT_EMU_DEBUG
        printf("Page navigation: Encoder R button -> Page %d (%s)\n",
               algo->current_page, PAGE_MAPPINGS[algo->current_page].name);
#endif
    }

    // Get current page mapping
    const PageMapping& page = PAGE_MAPPINGS[algo->current_page];

    // Get algorithm index and parameter offset for NT_setParameterFromUi calls
    uint32_t algIdx = NT_algorithmIndex(self);
    uint32_t paramOffset = NT_parameterOffset();

    // Handle pot changes - pots provide absolute values (0.0-1.0)
    // Convert to parameter range and call NT_setParameterFromUi to update

    // Check which pots changed using the controls bitmask
    for (int i = 0; i < 3; ++i) {
        uint16_t pot_mask = (i == 0) ? kNT_potL : (i == 1) ? kNT_potC : kNT_potR;
        if (data.controls & pot_mask) {
            // Pot changed - route to parameter based on current page
            int param_index = page.pot_mapping[i];
            const _NT_parameter& param_spec = parameters[param_index];

            // Convert pot value (0.0-1.0) to parameter range (min-max)
            float pot_value = data.pots[i];
            int param_value = roundf(param_spec.min + pot_value * (param_spec.max - param_spec.min));
            param_value = fmaxf(param_spec.min, fminf(param_value, param_spec.max));

            // Update parameter via framework (triggers parameterChanged callback)
            NT_setParameterFromUi(algIdx, param_index + paramOffset, param_value);

#ifdef NT_EMU_DEBUG
            printf("Pot %d changed on page %s: param=%d value=%d\n",
                   i, page.name, param_index, param_value);
#endif
        }
    }

    // Handle encoder changes - encoders provide delta values (±1 or 0)
    for (int i = 0; i < 2; ++i) {
        int8_t delta = data.encoders[i];
        if (delta != 0) {
            // Encoder changed - route to parameter based on current page
            int param_index = page.encoder_mapping[i];

            // Check if encoder is mapped to a parameter (not reserved)
            if (param_index < 0) {
                continue;  // Encoder not assigned on this page
            }

            const _NT_parameter& param_spec = parameters[param_index];

            // Get current value and apply delta
            int current_value = self->v[param_index];
            int new_value = current_value + delta;

            // Clamp to parameter range
            new_value = fmaxf(param_spec.min, fminf(new_value, param_spec.max));

            // Update parameter via framework (triggers parameterChanged callback)
            NT_setParameterFromUi(algIdx, param_index + paramOffset, new_value);

#ifdef NT_EMU_DEBUG
            printf("Encoder %d changed on page %s: param=%d delta=%d new_value=%d\n",
                   i, page.name, param_index, delta, new_value);
#endif
        }
    }
}

static void setupUi(_NT_algorithm* self, _NT_float3& pots) {
    // Defensive validation: Check for null pointer (protection against emulator reload race conditions)
    if (!self) {
        return;
    }

    nt_elementsAlgorithm* algo = static_cast<nt_elementsAlgorithm*>(self);

    // Validate algorithm structure integrity
    if (!algo->elements_part) {
        return;  // Plugin being destroyed during reload
    }

    // Validate current page index (protect against corruption during reload)
    if (algo->current_page < 0 || algo->current_page >= kNumPages) {
        return;  // Invalid page state
    }

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
    // Defensive validation: Check for null pointer (protection against emulator reload race conditions)
    if (!self) {
        return;
    }

    nt_elementsAlgorithm* algo = static_cast<nt_elementsAlgorithm*>(self);

    // Validate algorithm structure integrity
    if (!algo->elements_part) {
        return;  // Plugin being destroyed during reload
    }

    // Parse MIDI message
    const uint8_t status = b0 & 0xF0;
    const uint8_t channel = b0 & 0x0F;  // 0-15
    const uint8_t data1 = b1 & 0x7F;
    const uint8_t data2 = b2 & 0x7F;

    // MIDI channel filtering (0=Off/disabled, 1-16=specific channel)
    int16_t midi_channel_param = self->v[kParamMidiChannel];
    if (midi_channel_param == 0) {
        return;  // MIDI disabled
    }
    if (midi_channel_param >= 1 && midi_channel_param <= 16) {
        // Check if message is on the correct channel (param is 1-based, channel is 0-based)
        if (channel != (midi_channel_param - 1)) {
            return;  // Wrong channel, ignore message
        }
    }
    // If param > 16, treat as "all channels" (omni mode)

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
