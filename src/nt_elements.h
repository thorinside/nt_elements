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
#include "sample_manager.h"

// Elements requires exactly 16 samples per block
static constexpr int kElementsBlockSize = 16;

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

    // Block size adaptation buffers (VCV uses 4-sample blocks, Elements needs 16)
    // We accumulate input until we have 16 samples, then process through Elements.
    // Dual input buffers for blow (processed) and strike (direct) paths
    float blow_input_buffer[kElementsBlockSize];
    float strike_input_buffer[kElementsBlockSize];
    float output_main[kElementsBlockSize];
    float output_aux[kElementsBlockSize];
    int buffer_pos;  // Current position in buffers (0-15)

    // Memory region pointers for cleanup tracking
    uint16_t* reverb_buffer;

    // Sample manager for loading wavetables and noise samples
    SampleManager sample_manager;

    // Track SD card mount state for late loading
    bool sd_card_was_mounted;

    // MIDI state for monophonic voice management
    uint8_t current_note;

    // Pending MIDI updates
    volatile bool pending_update;
    elements::PerformanceState pending_state;

    // Page navigation state
    int current_page;
    int last_displayed_page;      // For detecting page changes
    uint32_t page_change_frame;   // Frame count when page changed
    uint32_t draw_frame_count;    // Incremented each draw() call (~60Hz)

    // Tuning state
    float tuning_offset_semitones;

    // Output scaling
    float output_level_scale;

    // Excitation strength (base value, can be modulated by MIDI velocity)
    float base_strength;

    // CV input state
    bool gate_cv_was_high;  // For gate edge detection
};

#endif // NT_ELEMENTS_H_
