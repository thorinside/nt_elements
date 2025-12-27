/*
 * parameter_adapter.h - Parameter conversion utilities for nt_elements
 *
 * Adapter layer between disting NT parameter system and Elements DSP Patch structure.
 * Provides shared parameter indices and conversion helpers for consistent mapping.
 *
 * Copyright (c) 2025
 * Licensed under MIT License
 */

#ifndef NT_ELEMENTS_PARAMETER_ADAPTER_H_
#define NT_ELEMENTS_PARAMETER_ADAPTER_H_

#include <cstdint>

// Parameter indices for nt_elements
// These map to the parameters array in the NT_algorithm structure
enum ParameterIndices {
    // System parameters (routing)
    kParamBlowInputBus = 0,   // External audio input for blow path (diffused → VCA)
    kParamStrikeInputBus,     // External audio input for strike path (direct to resonator)
    kParamOutputBus,
    kParamOutputMode,
    kParamAuxOutputBus,
    kParamAuxOutputMode,

    // Exciter parameters (used on multiple pages)
    kParamBowLevel,          // Bow exciter level (0-100% → 0.0-1.0)
    kParamBlowLevel,         // Blow exciter level (0-100% → 0.0-1.0)
    kParamStrikeLevel,       // Strike exciter level (0-100% → 0.0-1.0)
    kParamBowTimbre,         // Bow timbre color (0-100% → 0.0-1.0)
    kParamBlowTimbre,        // Blow timbre color (0-100% → 0.0-1.0)
    kParamStrikeTimbre,      // Strike timbre brightness (0-100% → 0.0-1.0)
    kParamBlowFlow,          // Blow meta/flow (0-100% → 0.0-1.0)
    kParamStrikeMallet,      // Strike meta/mallet (0-100% → 0.0-1.0)

    // Resonator parameters
    kParamGeometry,          // Resonator geometry/material (0-100% → 0.0-1.0)
    kParamBrightness,        // Resonator brightness/cutoff (0-100% → 0.0-1.0)
    kParamDamping,           // Resonator damping/decay (0-100% → 0.0-1.0)
    kParamResonatorPosition, // Strike position (0-100% → 0.0-1.0)
    kParamInharmonicity,     // Inharmonicity/modulation (0-100% → 0.0-1.0)

    // Reverb/Space parameters
    kParamReverbAmount,      // Reverb wet/dry mix (0-100% → 0.0-1.0)
    kParamReverbSize,        // Reverb room size/time (0-100% → 0.0-1.0)
    kParamReverbDamping,     // Reverb high-freq damping (0-100% → 0.0-1.0)

    // Additional synthesis parameters
    kParamSignature,         // Exciter signature/character (0-100% → 0.0-1.0)
    kParamStereoMod,         // Stereo modulation offset (0-100% → 0.0-1.0)

    // Performance/Tuning parameters
    kParamCoarseTune,        // Coarse tuning (-12 to +12 semitones, 0-100% scale)
    kParamFineTune,          // Fine tuning (-50 to +50 cents, 0-100% scale)
    kParamOutputLevel,       // Output volume (0-100% → 0.0-1.0)
    kParamFMAmount,          // FM modulation amount (0-100% → 0.0-1.0)
    kParamExciterContour,    // Exciter envelope shape (0-100% → 0.0-1.0)
    kParamStrength,          // Excitation strength (0-100% → 0.0-1.0)

    // Routing parameters (MIDI + CV inputs)
    kParamMidiChannel,       // MIDI channel filter (0=Off/disabled, 1-16=specific channel)
    kParamVOctCV,            // V/OCT CV input bus (0=none, 1-28=bus number)
    kParamGateCV,            // Gate CV input bus (0=none, 1-28=bus number)
    kParamFMCV,              // FM Amount CV input bus (0=none, 1-28=bus number)
    kParamBrightnessCV,      // Resonator Brightness CV input bus (0=none, 1-28=bus number)
    kParamExpressionCV,      // Exciter Expression/Dynamics CV input bus (0=none, 1-28=bus number)

    kNumParams
};

// Parameter conversion helpers
namespace parameter_adapter {

// Convert NT parameter (0-100%) to Elements Patch value (0.0-1.0)
inline float ntToElements(float nt_value) {
    return nt_value / 100.0f;
}

// Convert Elements Patch value (0.0-1.0) to NT parameter (0-100%)
inline float elementsToNt(float elements_value) {
    return elements_value * 100.0f;
}

// Clamp NT parameter to valid range
inline float clampNtParameter(float value, float min = 0.0f, float max = 100.0f) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

// Clamp Elements value to valid range
inline float clampElementsValue(float value, float min = 0.0f, float max = 1.0f) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

} // namespace parameter_adapter

#endif // NT_ELEMENTS_PARAMETER_ADAPTER_H_
