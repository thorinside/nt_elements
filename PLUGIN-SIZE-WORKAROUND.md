# Plugin Size Workaround - Story 1.10

## Problem

The full Elements plugin with complete lookup tables and wavetable data is **407KB**, which exceeds the disting NT plugin scanner limit (est. ~100-150KB based on working plugins).

## Root Cause

`external/mutable-instruments/elements/resources.cc` contains:
- **372KB** of const data (wavetables and lookup tables)
- `smp_sample_data`: 110,592 int16 samples (~221KB) - wavetable data
- `smp_noise_sample`: 4,096 int16 samples (~8KB) - noise samples
- Various LUTs: ~2-4KB each (sine, MIDI-to-freq, envelope, filter coefficients)

## Current Workaround (Partial)

**File:** `src/resources_stub.cc`
**Size:** 56KB plugin (vs 407KB full version)
**Status:** Plugin appears in algorithm list but **WILL CRASH** when used

The stub file provides:
- ✅ Correct symbol exports (no linker errors)
- ❌ Incomplete/wrong-sized LUT arrays (will cause crashes)
- ❌ Empty wavetable data (no wavetable oscillator mode)

## What Works

- Plugin loads successfully on disting NT
- Appears in algorithm list
- Factory struct recognized by scanner

## What Doesn't Work

- **Will crash or sound incorrect** due to incomplete LUTs
- Wavetable oscillator mode unavailable (only resonator modes)

## Solution Needed (Future Story)

To make a functional plugin under size limit, need to:

1. **Generate LUTs at runtime** instead of const data
   - Sine table: `sin(2*pi*i/1024)` - trivial to compute
   - MIDI-to-frequency: `440 * pow(2, (note-69)/12)` - simple formula
   - Filter coefficients: Can be approximated or computed

2. **Remove wavetable oscillator**
   - Disable ominous_voice mode in Elements
   - Only support bow/blow/strike excitation modes (which use resonator, not wavetables)
   - Document limitation in user manual

3. **Optimize remaining LUTs**
   - Keep only critical tables that can't be computed efficiently
   - Use lower precision where acceptable (int16 vs float)

## Estimated Sizes

- Current full: 407KB ❌
- With runtime LUT generation: ~80-100KB ✓ (estimated)
- Without wavetable mode: ~50-70KB ✓ (estimated)

## References

- Working plugins: nt_grids (28KB), nt_enosc (104KB)
- Elements resources analysis: `build/resources.o` = 372KB
- Story: docs/stories/1-10-hardware-deployment-and-validation.md
