/*
 * lut_generator.cpp - Runtime LUT generation for nt_elements
 *
 * Generates all Elements lookup tables at runtime, matching the formulas
 * from external/mutable-instruments/elements/resources/lookup_tables.py.
 * Tables are stored in API-allocated static DRAM to eliminate ~33KB of .rodata.
 *
 * Copyright (c) 2025 Neal Sanche
 * Licensed under MIT License
 */

#include "lut_generator.h"
#include <cmath>
#include <cstring>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Original Elements sample rate used for LUT generation
static const float SAMPLE_RATE = 32000.0f;

// ------------------------------------------------------------------
// Pointer definitions — these are the symbols referenced by Elements DSP code.
// They start as nullptr and are set by lutGeneratorInit().
// ------------------------------------------------------------------

namespace elements {

const int16_t* lut_db_led_brightness = nullptr;
const float* lut_sine = nullptr;
const float* lut_approx_svf_gain = nullptr;
const float* lut_approx_svf_g = nullptr;
const float* lut_approx_svf_r = nullptr;
const float* lut_approx_svf_h = nullptr;
const float* lut_4_decades = nullptr;
const float* lut_accent_gain_coarse = nullptr;
const float* lut_accent_gain_fine = nullptr;
const float* lut_stiffness = nullptr;
const float* lut_env_increments = nullptr;
const float* lut_env_linear = nullptr;
const float* lut_env_expo = nullptr;
const float* lut_env_quartic = nullptr;
const float* lut_midi_to_f_high = nullptr;
const float* lut_midi_to_increment_high = nullptr;
const float* lut_midi_to_f_low = nullptr;
const float* lut_fm_frequency_quantizer = nullptr;
const float* lut_detune_quantizer = nullptr;
const float* lut_svf_shift = nullptr;

} // namespace elements

namespace stmlib {

const float* lut_pitch_ratio_high = nullptr;
const float* lut_pitch_ratio_low = nullptr;

} // namespace stmlib

// ------------------------------------------------------------------
// Table sizes (matching resources.h #defines)
// ------------------------------------------------------------------

static const int kSinSize             = 4097;
static const int kSvfGainSize         = 257;
static const int kSvfGSize            = 257;
static const int kSvfRSize            = 257;
static const int kSvfHSize            = 257;
static const int k4DecadesSize        = 257;
static const int kAccentCoarseSize    = 257;
static const int kAccentFineSize      = 257;
static const int kStiffnessSize       = 257;
static const int kEnvIncrementsSize   = 258;
static const int kEnvLinearSize       = 258;
static const int kEnvExpoSize         = 258;
static const int kEnvQuarticSize      = 258;
static const int kMidiToFHighSize     = 256;
static const int kMidiToIncHighSize   = 256;
static const int kMidiToFLowSize      = 256;
static const int kFmFreqQuantSize     = 129;
static const int kDetuneQuantSize     = 65;
static const int kSvfShiftSize        = 257;
static const int kDbLedSize           = 513;
static const int kPitchRatioHighSize  = 257;
static const int kPitchRatioLowSize   = 257;

// Total float entries
static const size_t kTotalFloats =
    kSinSize + kSvfGainSize + kSvfGSize + kSvfRSize + kSvfHSize +
    k4DecadesSize + kAccentCoarseSize + kAccentFineSize + kStiffnessSize +
    kEnvIncrementsSize + kEnvLinearSize + kEnvExpoSize + kEnvQuarticSize +
    kMidiToFHighSize + kMidiToIncHighSize + kMidiToFLowSize +
    kFmFreqQuantSize + kDetuneQuantSize + kSvfShiftSize +
    kPitchRatioHighSize + kPitchRatioLowSize;

// Total int16 entries
static const size_t kTotalInt16s = kDbLedSize;

size_t lutGeneratorTotalBytes() {
    // float tables + int16 table, with 4-byte alignment for safety
    size_t bytes = kTotalFloats * sizeof(float) + kTotalInt16s * sizeof(int16_t);
    // Round up to 4-byte alignment
    bytes = (bytes + 3) & ~3;
    return bytes;
}

// ------------------------------------------------------------------
// Helper: Allocate a float array from the DRAM cursor
// ------------------------------------------------------------------
static float* allocFloats(uint8_t*& cursor, int count) {
    float* ptr = reinterpret_cast<float*>(cursor);
    cursor += count * sizeof(float);
    return ptr;
}

static int16_t* allocInt16s(uint8_t*& cursor, int count) {
    int16_t* ptr = reinterpret_cast<int16_t*>(cursor);
    cursor += count * sizeof(int16_t);
    return ptr;
}

// ------------------------------------------------------------------
// Generation functions (one per table, matching lookup_tables.py)
// ------------------------------------------------------------------

static void generateSine(float* out) {
    // sin(2*pi*i/4096), with out[4096] = out[0]
    for (int i = 0; i < 4096; ++i) {
        out[i] = sinf(2.0f * (float)M_PI * (float)i / 4096.0f);
    }
    out[4096] = out[0];
}

static void generateApproxSvf(float* gain, float* g, float* r, float* h) {
    // frequency = 32 * 10^(2.7 * i/256), normalized to sample rate
    for (int i = 0; i < 257; ++i) {
        float freq = 32.0f * powf(10.0f, 2.7f * (float)i / 256.0f);
        freq /= SAMPLE_RATE;
        if (freq >= 0.499f) freq = 0.499f;

        float g_val = tanf((float)M_PI * freq);
        float r_default = 2.0f;
        float h_val = 1.0f / (1.0f + r_default * g_val + g_val * g_val);
        float gain_val = (0.42f / freq) * powf(4.0f, freq * freq);

        float r_val = 1.0f / (0.5f * powf(10.0f, 3.0f * (float)i / 256.0f));

        gain[i] = gain_val;
        g[i] = g_val;
        r[i] = r_val;
        h[i] = h_val;
    }
}

static void generate4Decades(float* out) {
    for (int i = 0; i < 257; ++i) {
        float x = (float)i / 256.0f;
        out[i] = powf(10.0f, 4.0f * x);
    }
}

static void generateAccentGain(float* coarse, float* fine) {
    // coarse = 10^(1.5*(x-0.5))
    for (int i = 0; i < 257; ++i) {
        float x = (float)i / 256.0f;
        coarse[i] = powf(10.0f, 1.5f * (x - 0.5f));
    }
    // fine = 10^(x * log10(coarse[1]/coarse[0]))
    float ratio = log10f(coarse[1] / coarse[0]);
    for (int i = 0; i < 257; ++i) {
        float x = (float)i / 256.0f;
        fine[i] = powf(10.0f, x * ratio);
    }
}

static void generateDbLedBrightness(int16_t* out) {
    // brightness = (9 + log2(x)) / 9 * 256
    // x = i/512, with x[0]=x[1], x[512]=x[511]
    for (int i = 0; i < 513; ++i) {
        float x = (float)i / 512.0f;
        if (i == 0) x = 1.0f / 512.0f;         // x[0] = x[1]
        if (i == 512) x = 511.0f / 512.0f;      // x[-1] = x[-2]

        float brightness = (9.0f + log2f(x)) / 9.0f;
        float val = brightness * 256.0f;

        // Clamp to int16 range
        if (val < -32768.0f) val = -32768.0f;
        if (val > 32767.0f) val = 32767.0f;
        out[i] = (int16_t)(val + 0.5f);
    }
}

static void generateStiffness(float* out) {
    for (int i = 0; i < 257; ++i) {
        float g = (float)i / 256.0f;
        float s;
        if (g < 0.25f) {
            float d = 0.25f - g;
            s = -d * 0.25f;
        } else if (g < 0.3f) {
            s = 0.0f;
        } else if (g < 0.9f) {
            float d = g - 0.3f;
            d /= 0.6f;
            s = 0.01f * powf(10.0f, d * 2.005f) - 0.01f;
        } else {
            float d = g - 0.9f;
            d /= 0.1f;
            d *= d;
            s = 1.5f - cosf(d * (float)M_PI) / 2.0f;
        }
        out[i] = s;
    }
    out[256] = 2.0f;
    out[255] = 2.0f;
}

static void generateEnvIncrements(float* out) {
    // control_rate = SAMPLE_RATE / 16
    float control_rate = SAMPLE_RATE / 16.0f;
    float max_time = 8.0f;
    float min_time = 0.0005f;
    float gamma = 0.175f;
    float min_increment = 1.0f / (max_time * control_rate);
    float max_increment = 1.0f / (min_time * control_rate);
    float a = powf(max_increment, -gamma);
    float b = powf(min_increment, -gamma);

    for (int i = 0; i < 258; ++i) {
        float t = (float)i / 256.0f;
        if (i >= 256) t = 1.0f;
        out[i] = powf(a + (b - a) * t, -1.0f / gamma);
    }
}

static void generateEnvLinear(float* out) {
    for (int i = 0; i < 258; ++i) {
        float t = (float)i / 256.0f;
        if (i >= 256) t = 1.0f;
        // Normalized: t / t_max, but t_max = 1.0
        out[i] = t;
    }
}

static void generateEnvExpo(float* out) {
    // env_expo = 1 - exp(-4*t), normalized
    float max_val = 1.0f - expf(-4.0f);
    for (int i = 0; i < 258; ++i) {
        float t = (float)i / 256.0f;
        if (i >= 256) t = 1.0f;
        out[i] = (1.0f - expf(-4.0f * t)) / max_val;
    }
}

static void generateEnvQuartic(float* out) {
    // env_quartic = t^3.32, normalized
    for (int i = 0; i < 258; ++i) {
        float t = (float)i / 256.0f;
        if (i >= 256) t = 1.0f;
        out[i] = powf(t, 3.32f);
        // max is t=1.0 -> 1.0, so already normalized
    }
}

static void generateMidiToFHigh(float* out) {
    for (int i = 0; i < 256; ++i) {
        float midi_note = (float)(i - 48);
        float frequency = 440.0f * powf(2.0f, (midi_note - 69.0f) / 12.0f);
        float max_frequency = 12000.0f;
        if (max_frequency > SAMPLE_RATE / 2.0f) max_frequency = SAMPLE_RATE / 2.0f;
        if (frequency >= max_frequency) frequency = max_frequency;
        frequency /= SAMPLE_RATE;
        out[i] = frequency;
    }
}

static void generateMidiToIncrementHigh(float* out) {
    for (int i = 0; i < 256; ++i) {
        float midi_note = (float)(i - 48);
        float frequency = 440.0f * powf(2.0f, (midi_note - 69.0f) / 12.0f);
        float max_frequency = 12000.0f;
        if (max_frequency > SAMPLE_RATE / 2.0f) max_frequency = SAMPLE_RATE / 2.0f;
        if (frequency >= max_frequency) frequency = max_frequency;
        frequency /= SAMPLE_RATE;
        // frequency * 2^32 — stored as float
        out[i] = frequency * 4294967296.0f;
    }
}

static void generateMidiToFLow(float* out) {
    for (int i = 0; i < 256; ++i) {
        out[i] = powf(2.0f, (float)i / 256.0f / 12.0f);
    }
}

static void generateFmFrequencyQuantizer(float* out) {
    // FM frequency ratios from lookup_tables.py
    static const float fm_ratios[] = {
        0.5f,
        0.5f * 1.01331484531f,  // 0.5 * 2^(16/1200)
        0.70710678118f,          // sqrt(2)/2
        0.78539816340f,          // pi/4
        1.0f,
        1.0f * 1.01331484531f,  // 1.0 * 2^(16/1200)
        1.41421356237f,          // sqrt(2)
        1.57079632679f,          // pi/2
        1.75f,                   // 7/4
        2.0f,
        2.0f * 1.01331484531f,  // 2.0 * 2^(16/1200)
        2.25f,                   // 9/4
        2.75f,                   // 11/4
        2.82842712475f,          // 2*sqrt(2)
        3.0f,
        3.14159265359f,          // pi
        3.46410161514f,          // sqrt(3)*2
        4.0f,
        4.24264068712f,          // sqrt(2)*3
        4.71238898038f,          // pi*3/2
        5.0f,
        5.65685424949f,          // sqrt(2)*4
        8.0f
    };

    // Convert to semitones: 12 * log2(ratio)
    // Then replicate each value 3 times
    float scale[128];
    int scale_len = 0;
    for (int r = 0; r < 23; ++r) {
        float semitones = 12.0f * log2f(fm_ratios[r]);
        scale[scale_len++] = semitones;
        scale[scale_len++] = semitones;
        scale[scale_len++] = semitones;
    }
    // scale_len = 69

    // Pad to next power of 2 (128) by inserting midpoints at largest gaps
    while (scale_len < 128) {
        // Find largest gap
        int gap = 0;
        float max_diff = 0;
        for (int i = 0; i < scale_len - 1; ++i) {
            float d = scale[i + 1] - scale[i];
            if (d > max_diff) {
                max_diff = d;
                gap = i;
            }
        }
        // Insert midpoint after gap
        float mid = (scale[gap] + scale[gap + 1]) / 2.0f;
        // Shift right
        for (int j = scale_len; j > gap + 1; --j) {
            scale[j] = scale[j - 1];
        }
        scale[gap + 1] = mid;
        scale_len++;
    }

    // Copy to output (128 + 1 sentinel)
    for (int i = 0; i < 128; ++i) {
        out[i] = scale[i];
    }
    out[128] = scale[127];  // sentinel
}

static void generateDetuneQuantizer(float* out) {
    static const float detune_ratios[] = {
        -24.0f, -12.0f, -11.95f, -5.0f, -0.05f,
        0.0f, 0.05f, 7.0f, 12.0f, 19.0f, 24.0f
    };

    // Replicate each value 3 times
    float scale[128];
    int scale_len = 0;
    for (int r = 0; r < 11; ++r) {
        scale[scale_len++] = detune_ratios[r];
        scale[scale_len++] = detune_ratios[r];
        scale[scale_len++] = detune_ratios[r];
    }
    // scale_len = 33

    // Pad to next power of 2 (64) by inserting midpoints at largest gaps
    while (scale_len < 64) {
        int gap = 0;
        float max_diff = 0;
        for (int i = 0; i < scale_len - 1; ++i) {
            float d = scale[i + 1] - scale[i];
            if (d > max_diff) {
                max_diff = d;
                gap = i;
            }
        }
        float mid = (scale[gap] + scale[gap + 1]) / 2.0f;
        for (int j = scale_len; j > gap + 1; --j) {
            scale[j] = scale[j - 1];
        }
        scale[gap + 1] = mid;
        scale_len++;
    }

    for (int i = 0; i < 64; ++i) {
        out[i] = scale[i];
    }
    out[64] = scale[63];  // sentinel
}

static void generateSvfShift(float* out) {
    for (int i = 0; i < 257; ++i) {
        float ratio = powf(2.0f, (float)i / 12.0f);
        out[i] = 2.0f * atanf(1.0f / ratio) / (2.0f * (float)M_PI);
    }
}

static void generatePitchRatioHigh(float* out) {
    // ratio = 2^((i-128)/12)
    for (int i = 0; i < 257; ++i) {
        out[i] = powf(2.0f, ((float)i - 128.0f) / 12.0f);
    }
}

static void generatePitchRatioLow(float* out) {
    // semitone = 2^(i/256/12)
    for (int i = 0; i < 257; ++i) {
        out[i] = powf(2.0f, (float)i / 256.0f / 12.0f);
    }
}

// ------------------------------------------------------------------
// Main init function
// ------------------------------------------------------------------

void lutGeneratorInit(uint8_t* dram) {
    uint8_t* cursor = dram;

    // Allocate and generate all float tables (elements namespace)
    float* sine = allocFloats(cursor, kSinSize);
    generateSine(sine);
    elements::lut_sine = sine;

    float* svf_gain = allocFloats(cursor, kSvfGainSize);
    float* svf_g = allocFloats(cursor, kSvfGSize);
    float* svf_r = allocFloats(cursor, kSvfRSize);
    float* svf_h = allocFloats(cursor, kSvfHSize);
    generateApproxSvf(svf_gain, svf_g, svf_r, svf_h);
    elements::lut_approx_svf_gain = svf_gain;
    elements::lut_approx_svf_g = svf_g;
    elements::lut_approx_svf_r = svf_r;
    elements::lut_approx_svf_h = svf_h;

    float* four_decades = allocFloats(cursor, k4DecadesSize);
    generate4Decades(four_decades);
    elements::lut_4_decades = four_decades;

    float* accent_coarse = allocFloats(cursor, kAccentCoarseSize);
    float* accent_fine = allocFloats(cursor, kAccentFineSize);
    generateAccentGain(accent_coarse, accent_fine);
    elements::lut_accent_gain_coarse = accent_coarse;
    elements::lut_accent_gain_fine = accent_fine;

    float* stiffness = allocFloats(cursor, kStiffnessSize);
    generateStiffness(stiffness);
    elements::lut_stiffness = stiffness;

    float* env_increments = allocFloats(cursor, kEnvIncrementsSize);
    generateEnvIncrements(env_increments);
    elements::lut_env_increments = env_increments;

    float* env_linear = allocFloats(cursor, kEnvLinearSize);
    generateEnvLinear(env_linear);
    elements::lut_env_linear = env_linear;

    float* env_expo = allocFloats(cursor, kEnvExpoSize);
    generateEnvExpo(env_expo);
    elements::lut_env_expo = env_expo;

    float* env_quartic = allocFloats(cursor, kEnvQuarticSize);
    generateEnvQuartic(env_quartic);
    elements::lut_env_quartic = env_quartic;

    float* midi_to_f_high = allocFloats(cursor, kMidiToFHighSize);
    generateMidiToFHigh(midi_to_f_high);
    elements::lut_midi_to_f_high = midi_to_f_high;

    float* midi_to_inc_high = allocFloats(cursor, kMidiToIncHighSize);
    generateMidiToIncrementHigh(midi_to_inc_high);
    elements::lut_midi_to_increment_high = midi_to_inc_high;

    float* midi_to_f_low = allocFloats(cursor, kMidiToFLowSize);
    generateMidiToFLow(midi_to_f_low);
    elements::lut_midi_to_f_low = midi_to_f_low;

    float* fm_freq_quant = allocFloats(cursor, kFmFreqQuantSize);
    generateFmFrequencyQuantizer(fm_freq_quant);
    elements::lut_fm_frequency_quantizer = fm_freq_quant;

    float* detune_quant = allocFloats(cursor, kDetuneQuantSize);
    generateDetuneQuantizer(detune_quant);
    elements::lut_detune_quantizer = detune_quant;

    float* svf_shift = allocFloats(cursor, kSvfShiftSize);
    generateSvfShift(svf_shift);
    elements::lut_svf_shift = svf_shift;

    // int16 table
    int16_t* db_led = allocInt16s(cursor, kDbLedSize);
    generateDbLedBrightness(db_led);
    elements::lut_db_led_brightness = db_led;

    // stmlib namespace tables
    float* pitch_ratio_high = allocFloats(cursor, kPitchRatioHighSize);
    generatePitchRatioHigh(pitch_ratio_high);
    stmlib::lut_pitch_ratio_high = pitch_ratio_high;

    float* pitch_ratio_low = allocFloats(cursor, kPitchRatioLowSize);
    generatePitchRatioLow(pitch_ratio_low);
    stmlib::lut_pitch_ratio_low = pitch_ratio_low;
}
