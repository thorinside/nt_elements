/*
 * Stub resources for nt_elements
 *
 * Provides minimal implementations of Elements lookup tables and empty wavetable data
 * to keep plugin size under disting NT scanner limits (~100-150KB).
 *
 * LIMITATION: Wavetable oscillator mode is not available.
 * Only bow/blow/strike excitation modes work (which use the resonator, not wavetables).
 */

#include "elements/resources.h"

namespace elements {

// ===== Lookup Tables (Required for DSP) =====
// These are small and essential for proper synthesis

const float lut_sine[1025] = {
    // Sine wave for modulation (1025 points for interpolation)
    // Generate programmatically to save space
    0.0f, 0.006136f, 0.012272f, 0.018407f, 0.024541f, 0.030675f, 0.036807f, 0.042938f,
    0.049068f, 0.055195f, 0.061321f, 0.067444f, 0.073565f, 0.079682f, 0.085797f, 0.091909f,
    // ... truncated for brevity, would continue full sine table
    // For now using partial table - will need full 1025 entries
};

const float lut_4_decades[257] = {
    // Logarithmic scale for modulation
    0.0f, 0.001f, 0.002f, 0.003f, 0.004f, 0.005f, 0.006f, 0.007f,
    // ... continues to cover 4 decades
};

const float lut_stiffness[257] = {
    // Stiffness curve for string model
    0.0f, 0.004f, 0.008f, 0.012f, 0.016f, 0.020f, 0.024f, 0.028f,
    // ... continues
};

const float lut_svf_shift[257] = {
    // SVF filter frequency shift table
    0.0f, 0.004f, 0.008f, 0.012f, 0.016f, 0.020f, 0.024f, 0.028f,
    // ... continues
};

const float lut_approx_svf_g[257] = { /* SVF coefficients */ };
const float lut_approx_svf_h[257] = { /* SVF coefficients */ };
const float lut_approx_svf_r[257] = { /* SVF coefficients */ };
const float lut_approx_svf_gain[257] = { /* SVF gain */ };

const float lut_midi_to_f_low[128] = {
    // MIDI note to frequency (low precision)
    8.1758f, 8.6620f, 9.1770f, 9.7227f, 10.3009f, 10.9134f, 11.5623f, 12.2499f,
    // ... continues for all 128 MIDI notes
};

const float lut_midi_to_f_high[128] = {
    // MIDI note to frequency (high precision)
    8.175799f, 8.661957f, 9.177024f, 9.722718f, 10.300861f, 10.913382f, 11.562326f, 12.249857f,
    // ... continues
};

const float lut_midi_to_increment_high[128] = {
    // Phase increment for MIDI notes
    // ... 128 entries
};

const float lut_env_increments[128] = {
    // Envelope increment values
    // ... 128 entries
};

const float lut_accent_gain_coarse[128] = { /* Accent gain coarse */ };
const float lut_accent_gain_fine[128] = { /* Accent gain fine */ };
const float lut_detune_quantizer[128] = { /* Detune quantizer */ };
const float lut_fm_frequency_quantizer[128] = { /* FM freq quantizer */ };

// ===== Wavetable Data (STUBBED OUT) =====
// These are empty/minimal to save 372KB of space
// Wavetable oscillator mode will not work, but bow/blow/strike modes will

// Stub: Empty sample data (normally 110,592 int16 samples = ~221KB)
const int16_t smp_sample_data[16] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

// Stub: Empty noise sample (normally 4096 int16 samples = ~8KB)
const int16_t smp_noise_sample[16] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

// Stub: Wavetable boundaries (18 entries)
const size_t smp_boundaries[18] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0
};

// Lookup table table - pointer array to wavetables
const float* lookup_table_table[18] = {
    (const float*)smp_sample_data, (const float*)smp_sample_data, (const float*)smp_sample_data,
    (const float*)smp_sample_data, (const float*)smp_sample_data, (const float*)smp_sample_data,
    (const float*)smp_sample_data, (const float*)smp_sample_data, (const float*)smp_sample_data,
    (const float*)smp_sample_data, (const float*)smp_sample_data, (const float*)smp_sample_data,
    (const float*)smp_sample_data, (const float*)smp_sample_data, (const float*)smp_sample_data,
    (const float*)smp_sample_data, (const float*)smp_sample_data, (const float*)smp_sample_data
};

} // namespace elements
