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

**License:**
Elements is MIT licensed, which permits modification. These patches are specific to the nt_elements project and are not pushed upstream to the Elements repository.

**See Also:**
- docs/architecture.md ADR-002 - Architectural decision record
- docs/stories/1-5-sample-rate-conversion-implementation.md - Implementation story
