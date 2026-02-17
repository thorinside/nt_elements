# Patches for Elements DSP

This directory contains patches applied to the Elements DSP submodule to adapt it for disting NT.

## elements-dynamic-sample-rate.patch

**Purpose:** Inject NT's user-configurable sample rate into Elements DSP

**File Modified:** `external/mutable-instruments/elements/dsp/dsp.h`

**Changes:**
- Replaces hardcoded `kSampleRate = 32000.0f` with dynamic `NT_globals.sampleRate`
- Uses inline function + macro for zero runtime overhead
- Adapts automatically to NT's user-configured sample rate (32/48/96kHz)

**Application:**
Patches are automatically applied during build by the Makefile. The build system checks if the patch has already been applied to avoid reapplying.

**Manual Application:**
```bash
cd external/mutable-instruments/elements/dsp
patch -p5 < ../../../../patches/elements-dynamic-sample-rate.patch
```

**Manual Reversal:**
```bash
cd external/mutable-instruments/elements/dsp
patch -R -p5 < ../../../../patches/elements-dynamic-sample-rate.patch
```

## elements-dynamic-samples.patch

**Purpose:** Enable dynamic sample loading and runtime LUT generation

**File Modified:** `external/mutable-instruments/elements/resources.h`

**Changes:**
- Replaces `extern const int16_t smp_sample_data[]` with pointer `smp_sample_data_ptr`
- Replaces `extern const int16_t smp_noise_sample[]` with pointer `smp_noise_sample_ptr`
- Replaces `extern const size_t smp_boundaries[]` with pointer `smp_boundaries_ptr`
- Adds macro redirections so existing code (exciter.cc) works unchanged
- Changes all 20 LUT declarations from `const float lut_xxx[]` to `const float* lut_xxx`
- Changes `const int16_t lut_db_led_brightness[]` to `const int16_t* lut_db_led_brightness`
- Eliminates ~33KB of .rodata (LUT data now generated at runtime by `lut_generator.cpp`)

**Pointer Initialization:**
Sample pointers are defined in `nt_elements.cpp` and initialized in `construct()`.
LUT pointers are defined in `lut_generator.cpp` and initialized in `initialise()` using API-allocated static DRAM.

**Application:**
Patches are automatically applied during build by the Makefile. This patch is applied after `elements-dynamic-sample-rate.patch`.

## stmlib-runtime-luts.patch

**Purpose:** Change stmlib pitch ratio LUTs from arrays to extern pointers

**Files Modified:** `external/mutable-instruments/stmlib/dsp/units.h`, `external/mutable-instruments/stmlib/dsp/units.cc`

**Changes:**
- Changes `const float lut_pitch_ratio_high[257]` and `lut_pitch_ratio_low[257]` to `const float*`
- Removes array data definitions from `units.cc` (generated at runtime by `lut_generator.cpp`)

**Application:**
Applied from the stmlib subdirectory after the Elements patches.

---

## Patch Application

**Automatic (via Makefile):**
```bash
make apply-patches  # or just 'make' - applied automatically
```

**Manual Application:**
```bash
cd external/mutable-instruments
patch -p1 < ../../patches/elements-dynamic-sample-rate.patch
patch -p1 < ../../patches/elements-dynamic-samples.patch
cd stmlib
patch -p1 < ../../../patches/stmlib-runtime-luts.patch
```

**Manual Reversal:**
```bash
cd external/mutable-instruments
git checkout .
cd stmlib
git checkout .
```

**License:**
Elements is MIT licensed, which permits modification. These patches are specific to the nt_elements project and are not pushed upstream to the Elements repository.

**See Also:**
- docs/architecture.md ADR-002 - Architectural decision record
- docs/stories/1-5-sample-rate-conversion-implementation.md - Sample rate patch story
- docs/stories/3-4-elements-exciter-patch.md - Dynamic samples patch story
