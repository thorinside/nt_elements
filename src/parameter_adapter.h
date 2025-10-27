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
    kParamInputBus = 0,
    kParamOutputBus,
    kParamOutputMode,
    kParamGeometry,      // Resonator geometry (0-100% → 0.0-1.0)
    kParamBrightness,    // Resonator brightness (0-100% → 0.0-1.0)
    kParamDamping,       // Resonator damping (0-100% → 0.0-1.0)
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
