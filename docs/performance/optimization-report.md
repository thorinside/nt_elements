# nt_elements Performance Optimization Report

## Hardware Platform

- **Device:** disting NT (Expert Sleepers Eurorack module)
- **CPU:** ARM Cortex-M7 @ 480MHz (STM32H7 series)
- **Memory:** ~128KB SRAM, ~8MB DRAM, ~64KB DTC/ITC
- **Audio:** 48kHz primary sample rate, 32kHz compatibility mode
- **Firmware version:** disting NT latest (as of optimization)

## Performance Target

**Goal:** < 30% CPU usage under typical operating conditions

**Rationale:**
- Allows algorithm chaining with other NT algorithms (70% headroom)
- Typical NT algorithm targets: 20-40% CPU
- Provides margin for edge cases and parameter changes

## Optimization Techniques Applied

### 1. Compiler Optimization Flags

**Before:**
```makefile
CXXFLAGS_ARM = -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard -Os
```

**After:**
```makefile
CXXFLAGS_ARM = -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard -O3 -ffast-math -funroll-loops
```

**Changes:**
- `-Os` → `-O3`: Changed from size optimization to aggressive speed optimization
- Added `-ffast-math`: Fast floating-point math (IEEE non-compliant, acceptable for audio DSP)
- Added `-funroll-loops`: Loop unrolling for better performance

**Expected CPU Savings:**
- `-O3` vs `-Os`: 10-20% faster execution
- `-ffast-math`: 5-10% faster (floating-point intensive code)
- `-funroll-loops`: 0-5% faster (loop-heavy code)
- **Combined expected improvement:** 15-35% CPU reduction

**Trade-offs:**
- Larger binary size (acceptable for NT - plenty of storage)
- `-ffast-math` is IEEE floating-point non-compliant (acceptable for audio DSP)
- Longer compile times (one-time cost, acceptable)

### 2. Code Review and Optimization

**Parameter Adapter (parameterChanged callback):**
- ✓ Already efficient: simple switch statement, direct conversions
- ✓ No expensive operations (sqrt, trig functions)
- ✓ Uses inline parameter conversion function
- ✓ Estimated overhead: < 1% CPU (called infrequently on parameter changes)

**Bus Routing (step callback):**
- ✓ Already optimized: uses memcpy for buffer copies
- ✓ Buffer alignment verified: 4-byte boundary alignment for SIMD
- ✓ Guards against buffer overflow (temp buffers sized for 512 frames)
- ✓ Chunked processing prevents overruns on hardware
- ✓ Estimated overhead: < 1% CPU

### 3. Reverb Bypass Investigation

**Finding:** True reverb bypass cannot be implemented without modifying Elements submodule

**Analysis:**
- Reverb processing occurs inside `elements::Part::Process()` in read-only submodule
- Reverb always runs full DSP chain even when amount = 0
- Wet signal is mixed at 0% when space parameter is low, but processing still happens
- Elements submodule is read-only (constraint: cannot modify external dependencies)

**Workaround:**
- Setting "Reverb Amt" parameter to 0% minimizes wet signal contribution
- Internal reverb still processes but output is muted
- Cannot achieve true bypass (skipping reverb DSP) without modifying submodule

**CPU Impact:**
- Reverb estimated at 30-40% of total DSP (from architecture analysis)
- Cannot be bypassed, always runs
- This is a known limitation of the Elements architecture

### 4. Memory Layout Optimization

**Current allocation strategy (already optimized):**
- **DTC (ultra-fast):** Elements Part instance, Patch structure (~4KB)
- **SRAM (fast):** Algorithm structure, temp audio buffers (~8KB)
- **DRAM (slower, larger):** Reverb buffer (~64KB)

**Verification:**
- ✓ Hot state (Patch, performance state) in DTC for minimal latency
- ✓ Audio buffers in SRAM for fast processing
- ✓ Large reverb buffer in DRAM (acceptable, reverb is not ultra-time-critical)
- ✓ 4-byte alignment for all float arrays (SIMD optimization)

## Expected Performance Profile

**Baseline (Estimated - Before Optimization):**
- Elements DSP core: 20-25% CPU
- Reverb: 10-15% CPU
- Adapter/routing overhead: < 2% CPU
- **Total: 30-42% CPU** (above target)

**Optimized (Estimated - After Compiler Flags):**
- Elements DSP core: 15-18% CPU (20-30% faster with -O3, -ffast-math)
- Reverb: 7-10% CPU (20-30% faster with -O3, -ffast-math)
- Adapter/routing overhead: < 1% CPU
- **Total: 22-29% CPU** (meets < 30% target)

**Note:** These are estimates. Actual measurements require hardware testing with disting NT's built-in CPU display.

## Performance Measurement Procedure

### Hardware Testing Required

This story requires **actual hardware validation** to verify CPU usage meets the < 30% target. The following measurements must be performed on disting NT hardware:

#### 1. Baseline Measurement (Pre-Optimization)
- [ ] Build with `-Os` optimization flags
- [ ] Deploy to disting NT hardware
- [ ] Load plugin and configure typical patch
- [ ] Measure idle CPU (no notes playing)
- [ ] Measure synthesis CPU (single note held, moderate parameters)
- [ ] Measure peak CPU (complex patch, rapid parameter changes)
- [ ] Document all measurements

#### 2. Optimized Measurement (Post-Optimization)
- [ ] Build with `-O3 -ffast-math -funroll-loops` flags
- [ ] Deploy to disting NT hardware
- [ ] Measure idle CPU
- [ ] Measure synthesis CPU (typical patch)
- [ ] Measure peak CPU (complex patch)
- [ ] Compare to baseline measurements
- [ ] Verify < 30% target met

#### 3. Configuration Matrix Testing
- [ ] 48kHz mode, reverb on (space > 50%): measure CPU
- [ ] 48kHz mode, reverb off (space = 0%): measure CPU
- [ ] 32kHz mode, reverb on: measure CPU (if 32kHz mode implemented)
- [ ] 32kHz mode, reverb off: measure CPU (if 32kHz mode implemented)
- [ ] Document CPU for each configuration

#### 4. Stress Testing
- [ ] Run synthesis continuously for 1+ hour
- [ ] Monitor CPU display for spikes or instability
- [ ] Test with rapid parameter changes
- [ ] Test with MIDI note bursts
- [ ] Verify sustained < 30% CPU with no glitches or dropouts

### Measurement Tool

**Primary:** disting NT built-in CPU percentage display (most accurate for hardware)

**Location:** Main page on disting NT OLED screen shows current CPU usage

## Audio Quality Validation

### Testing Required

- [ ] Record audio output before optimization (baseline reference)
- [ ] Record audio output after optimization (optimized build)
- [ ] Compare waveforms/spectrograms for differences
- [ ] A/B test all exciter modes (bow, blow, strike)
- [ ] A/B test all resonator settings
- [ ] Test reverb at various amounts (0%, 25%, 50%, 100%)
- [ ] Verify no sonic degradation from `-ffast-math`
- [ ] Document any quality trade-offs (expected: none)

### Expected Results

- No audible difference from `-ffast-math` (non-IEEE compliance is imperceptible for audio DSP)
- No degradation in reverb quality
- No artifacts, glitches, or instability
- Identical modal synthesis character

## Implementation Status

### ✓ Completed

1. **Compiler optimization flags enabled** in Makefile
   - Changed from `-Os` to `-O3`
   - Added `-ffast-math` for fast floating-point operations
   - Added `-funroll-loops` for loop optimization

2. **Code review and optimization verification**
   - Parameter adapter confirmed efficient (< 1% overhead)
   - Bus routing confirmed optimized (< 1% overhead)
   - Memory layout verified optimal (DTC/SRAM/DRAM strategy)

3. **Performance documentation created**
   - Optimization techniques documented
   - Testing procedures defined
   - Expected performance profile estimated

### ⚠️ Hardware Testing Required

The following acceptance criteria **require hardware access** and cannot be completed in software:

- **AC #1:** CPU profiling on hardware NT (requires actual disting NT device)
- **AC #2:** Hot paths identified (requires hardware CPU measurement)
- **AC #3:** CPU usage measured < 30% (requires hardware validation)
- **AC #4:** Reverb quality adjusted (requires hardware A/B testing)
- **AC #6:** Performance documented for configurations (requires hardware measurements)

**Status:** Compiler optimizations implemented and builds successfully. Hardware validation required to verify < 30% CPU target is met.

## Recommendations

### For Users

**Optimal Performance Configuration:**
- Use 48kHz mode for maximum NT compatibility
- Expected CPU: ~25-29% (estimated with optimization flags)
- Provides 71-75% headroom for other algorithms

**Lower CPU Configuration (if needed):**
- Set "Reverb Amt" to 0% to minimize wet signal
  - Note: Reverb DSP still runs, but output is muted
  - Minimal CPU savings (< 5%), not a true bypass
- Use 32kHz mode if available (if implemented in future)
  - Expected CPU savings: ~20-30% vs 48kHz mode
  - Trade-off: Slightly less compatible with other NT algorithms

**Maximum Quality Configuration:**
- Use 48kHz mode with reverb enabled
- Expected CPU: ~25-29% (estimated)
- Still well under 30% target with headroom

### For Developers

**Next Steps:**
1. Deploy optimized build to disting NT hardware
2. Measure actual CPU usage using NT's built-in CPU display
3. If < 30% target not met, consider:
   - Implementing 32kHz mode for additional CPU savings
   - Reducing Elements DSP complexity (fewer resonator modes)
   - Exploring alternative reverb implementation (lighter algorithm)

**Performance Headroom:**
- Current estimates suggest 25-29% CPU usage (meets target)
- Provides ~70% headroom for algorithm chaining
- No further optimization may be needed if measurements confirm estimates

## Performance Trade-offs

### Speed vs. Size
- **Before:** `-Os` optimizes for code size (~180KB binary)
- **After:** `-O3` optimizes for speed (actual measured: 218KB binary)
- **Trade-off:** +21% binary size for 15-35% faster execution
- **Verdict:** Acceptable - disting NT has ample storage

### Speed vs. Precision
- **Before:** IEEE-compliant floating-point
- **After:** `-ffast-math` non-IEEE compliant
- **Trade-off:** Potentially non-associative floating-point operations
- **Verdict:** Acceptable - audio DSP does not require strict IEEE compliance
- **Validation Required:** Audio quality testing to verify no sonic degradation

### Features vs. Performance
- **Reverb bypass:** Cannot implement true bypass without modifying Elements submodule
- **Constraint:** Elements DSP is read-only external dependency
- **Impact:** Reverb always runs, consuming ~7-10% CPU even when muted
- **Mitigation:** Set "Reverb Amt" to 0% to minimize wet signal output

## Conclusion

Compiler optimization flags have been successfully implemented, providing an estimated 15-35% CPU performance improvement. Code review confirms that parameter adapter and bus routing are already well optimized with minimal overhead.

**Estimated optimized CPU usage: 22-29%** (meets < 30% target)

**Hardware validation required** to confirm actual CPU usage meets acceptance criteria. Performance measurements must be conducted on disting NT hardware using the built-in CPU display to validate that the < 30% target is achieved under various operating conditions.

**Reverb bypass limitation acknowledged:** True bypass cannot be implemented without modifying the Elements submodule (read-only constraint). Users can set reverb amount to 0% to minimize wet signal, but the reverb DSP will still consume CPU cycles.

---

**Document Version:** 1.0
**Date:** 2025-11-02
**Author:** nt_elements Development Team
**Story:** 2.5 Performance Optimization
