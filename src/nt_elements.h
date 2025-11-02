/*
 * nt_elements.h - Forward declarations for nt_elements
 *
 * Provides forward declarations to break circular dependencies
 * between nt_elements.cpp and oled_display.cpp
 *
 * Copyright (c) 2025
 * Licensed under MIT License
 */

#ifndef NT_ELEMENTS_H_
#define NT_ELEMENTS_H_

#include "distingnt/api.h"
#include "elements/dsp/part.h"

// Forward declaration of algorithm structure
struct nt_elementsAlgorithm : public _NT_algorithm {
    // Elements DSP engine (in DTC)
    elements::Part* elements_part;

    // Current performance state and patch
    elements::PerformanceState perf_state;

    // Temp buffers for Elements processing (in SRAM)
    float* temp_blow_in;
    float* temp_strike_in;
    float* temp_main_out;
    float* temp_aux_out;

    // Memory region pointers for cleanup tracking
    uint16_t* reverb_buffer;

    // MIDI state for monophonic voice management
    uint8_t current_note;

    // Pending MIDI updates
    volatile bool pending_update;
    elements::PerformanceState pending_state;

    // Button debouncing
    uint16_t last_button_state;

    // Page navigation state
    int current_page;

    // Tuning state
    float tuning_offset_semitones;

    // Output scaling
    float output_level_scale;
};

#endif // NT_ELEMENTS_H_
