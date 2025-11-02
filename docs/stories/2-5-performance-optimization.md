# Story 2.5: Performance Optimization

Status: ready-for-dev

## Story

As a developer,
I want CPU usage optimized to meet < 30% target,
So that users can chain nt_elements with other NT algorithms.

## Acceptance Criteria

1. CPU profiling completed on hardware NT
2. Hot paths identified and optimized
3. CPU usage measured < 30% in both 32kHz and 48kHz modes
4. Reverb quality adjusted if needed to meet target
5. Compiler optimizations enabled (-O3, -ffast-math)
6. Performance documented for different configurations

## Tasks / Subtasks

- [ ] Measure baseline CPU usage (AC: #1)
  - [ ] Deploy current build to disting NT hardware
  - [ ] Measure idle CPU (no synthesis)
  - [ ] Measure synthesis CPU (typical patch, note held)
  - [ ] Measure peak CPU (complex patch, multiple parameters)
  - [ ] Document baseline measurements

- [ ] Profile CPU usage by component (AC: #1, #2)
  - [ ] Estimate Elements Part processing cost
  - [ ] Estimate reverb processing cost
  - [ ] Estimate parameter adapter overhead
  - [ ] Estimate bus routing overhead
  - [ ] Identify hot paths (highest CPU components)

- [ ] Enable compiler optimizations (AC: #5)
  - [ ] Change Makefile optimization flag from -Os to -O3
  - [ ] Add -ffast-math flag (fast floating-point)
  - [ ] Add -funroll-loops flag (loop unrolling)
  - [ ] Rebuild and measure CPU improvement
  - [ ] Document optimization flags used

- [ ] Optimize reverb processing (AC: #4)
  - [ ] Measure reverb-only CPU cost
  - [ ] Implement reverb bypass when Amount = 0%
  - [ ] Test reduced reverb quality settings
  - [ ] Adjust reverb buffer size if needed
  - [ ] Measure CPU savings from reverb optimization

- [ ] Optimize parameter adapter (AC: #2)
  - [ ] Review parameterChanged() efficiency
  - [ ] Minimize parameter conversion overhead
  - [ ] Cache computed values if beneficial
  - [ ] Verify no unnecessary processing in hot paths
  - [ ] Measure adapter overhead (should be < 1%)

- [ ] Optimize bus routing (AC: #2)
  - [ ] Review step() callback efficiency
  - [ ] Minimize memcpy operations
  - [ ] Consider DMA or optimized copy routines
  - [ ] Verify buffer alignment for SIMD
  - [ ] Measure routing overhead

- [ ] Test 32kHz mode (if implemented) (AC: #3)
  - [ ] Switch to 32kHz mode
  - [ ] Measure CPU usage in 32kHz mode
  - [ ] Compare to 48kHz mode
  - [ ] Verify ~15-20% CPU savings
  - [ ] Document 32kHz mode performance

- [ ] Measure optimized CPU usage (AC: #3, #6)
  - [ ] Deploy optimized build to hardware
  - [ ] Measure idle CPU
  - [ ] Measure synthesis CPU (typical patch)
  - [ ] Measure peak CPU (complex patch)
  - [ ] Verify < 30% target met
  - [ ] Compare to baseline measurements

- [ ] Test different configurations (AC: #6)
  - [ ] 48kHz mode, reverb on: measure CPU
  - [ ] 48kHz mode, reverb off: measure CPU
  - [ ] 32kHz mode, reverb on: measure CPU (if applicable)
  - [ ] 32kHz mode, reverb off: measure CPU (if applicable)
  - [ ] Document CPU for each configuration

- [ ] Validate audio quality after optimization (AC: #4)
  - [ ] Compare audio output before/after optimization
  - [ ] Verify no sonic degradation
  - [ ] Test reverb bypass (should be silent when bypassed)
  - [ ] Test -ffast-math impact (should be negligible)
  - [ ] Document any quality trade-offs

- [ ] Create performance documentation (AC: #6)
  - [ ] Document baseline vs optimized CPU usage
  - [ ] Document optimization techniques applied
  - [ ] Document configuration recommendations
  - [ ] Note performance headroom remaining
  - [ ] Provide user guidance on CPU management

- [ ] Stress test optimized build (AC: #3)
  - [ ] Run synthesis for 1+ hour
  - [ ] Monitor for CPU spikes or instability
  - [ ] Test with rapid parameter changes
  - [ ] Test with MIDI note bursts
  - [ ] Verify sustained < 30% CPU usage

## Dev Notes

### Performance Target

**Goal: < 30% CPU Usage**

**Why 30%:**
- Allows chaining with other NT algorithms (70% headroom)
- Typical NT algorithm targets: 20-40% CPU
- Provides performance margin for edge cases

**Measurement:**
Use disting NT built-in CPU display (most accurate for hardware).

**Expected Breakdown (Optimized):**
- Elements DSP: 15-20%
- Reverb: 5-8% (or 0% if bypassed)
- Adapter/routing: < 2%
- **Total: 20-30%**

### Compiler Optimizations

**Optimization Flags:**

**Current (Size Optimization):**
```makefile
CFLAGS += -Os  # Optimize for size
```

**Optimized (Speed):**
```makefile
CFLAGS += -O3  # Optimize for speed (aggressive)
CFLAGS += -ffast-math  # Fast floating-point (IEEE non-compliant)
CFLAGS += -funroll-loops  # Unroll loops (larger code, faster)
```

**Expected CPU Savings:**
- -O3 vs -Os: 10-20% faster
- -ffast-math: 5-10% faster (floating-point intensive code)
- -funroll-loops: 0-5% faster (loop-heavy code)

**Trade-offs:**
- Larger binary size (acceptable for NT)
- -ffast-math: Non-IEEE compliant (acceptable for audio DSP)
- Longer compile times (acceptable)

**Testing:**
Verify audio quality after enabling -ffast-math. Should be imperceptible differences.

### Reverb Optimization

**Reverb CPU Cost:**
Reverb is typically the most expensive component (~30-40% of total DSP).

**Optimization Strategy:**

**1. Bypass When Not Used:**
```cpp
void step(_NT_algorithm* self, ...) {
    _nt_elementsAlgorithm* pThis = (_nt_elementsAlgorithm*)self;

    // Process exciter + resonator
    pThis->elements_part->ProcessWithoutReverb(pThis->patch, input, output, numFrames);

    // Conditionally process reverb
    float reverb_amount = pThis->patch.reverb_amount;
    if (reverb_amount > 0.01f) {  // Only process if reverb is audible
        pThis->elements_part->ProcessReverb(output, numFrames);
    }
}
```

**Expected Savings:** 5-10% CPU when reverb bypassed

**2. Reduce Reverb Quality:**
If reverb alone exceeds budget, reduce quality:
- Shorten reverb buffer (smaller room size limit)
- Reduce diffusion stages
- Lower sample rate for reverb (if Elements supports)

**Note:** Check Elements code to see if reverb can be processed separately or bypassed.

### Profiling Hot Paths

**Hot Path Identification:**

**1. Elements Part::Process():**
- Likely 80-90% of CPU time
- Internal to Elements DSP (can't optimize without modifying submodule)
- Focus on optimizing around it

**2. Audio Buffer Copies:**
- memcpy() for bus routing
- Should be < 1% CPU with optimized memcpy
- Verify buffer alignment for efficiency

**3. Parameter Conversion:**
- parameterChanged() processing
- Should be negligible (called infrequently)
- Verify no expensive operations (sqrt, trig, etc.)

**4. Display Updates:**
- OLED rendering in draw()
- Should be < 2% CPU
- Verify dirty flag prevents unnecessary redraws

**Profiling Tools:**
Limited on embedded hardware. Use:
- CPU percentage display (coarse measurement)
- Timing code sections with cycle counters (if available)
- Binary search: disable components and measure CPU impact

### Memory Optimization

**Memory vs Performance:**
Optimized memory layout can improve cache performance.

**Current Memory Strategy (from architecture.md):**
- DTC: Hot state, parameters (ultra-fast)
- SRAM: Audio buffers, filter states (fast)
- DRAM: Reverb buffer (slower, larger)

**Verification:**
Ensure critical data in DTC/SRAM (not DRAM). This is already optimized in architecture.

### 32kHz Mode Performance

**If 32kHz Mode Implemented:**

**Expected Performance:**
- ~33% fewer samples to process (32k vs 48k)
- Proportional CPU reduction: ~20-30% savings
- Trade-off: Sample rate conversion overhead

**Measurement:**
- Measure CPU in 32kHz mode
- Compare to 48kHz mode
- Document savings

**User Guidance:**
"Use 32kHz mode for lower CPU usage when maximum NT compatibility is not required."

### Performance Documentation

**Documentation Format:**
```markdown
# nt_elements Performance Report

## Hardware Platform
- disting NT (ARM Cortex-M7 @ 480MHz)
- Firmware version: [version]

## Baseline Performance (Before Optimization)
- 48kHz mode, reverb on: 45% CPU
- 48kHz mode, reverb off: 38% CPU

## Optimization Techniques Applied
1. Compiler flags: -O3, -ffast-math, -funroll-loops
2. Reverb bypass when Amount = 0%
3. Memory layout optimization (DTC/SRAM/DRAM)

## Optimized Performance
- 48kHz mode, reverb on: 28% CPU ✓
- 48kHz mode, reverb off: 22% CPU ✓
- 32kHz mode, reverb on: 20% CPU ✓ (if implemented)
- 32kHz mode, reverb off: 15% CPU ✓ (if implemented)

## CPU Savings
- Compiler optimizations: ~12% reduction
- Reverb bypass: ~6% reduction (when not used)
- 32kHz mode: ~8% additional reduction (if implemented)

## Recommendations
- Use 48kHz mode for maximum NT compatibility (28% CPU)
- Disable reverb (set Amount to 0%) for lower CPU (22% CPU)
- Use 32kHz mode for lowest CPU usage (20% CPU) if available

## Performance Headroom
- Typical usage: ~28% CPU (72% headroom for other algorithms)
- Light usage (reverb off): ~22% CPU (78% headroom)
```

### Testing Strategy

**Measurement Procedure:**
1. Load plugin on disting NT
2. Configure test patch (typical parameters)
3. Trigger MIDI note (hold)
4. Observe CPU % on NT display (wait for stabilization)
5. Record measurement
6. Repeat for different configurations

**Test Configurations:**
- Idle (no notes)
- Single note sustained
- Rapid notes (stress test)
- With/without reverb
- With/without parameter changes
- 32kHz vs 48kHz mode (if applicable)

**Validation Criteria:**
- All configurations < 30% CPU (target met)
- No CPU spikes or instability
- Audio quality preserved
- No glitches or dropouts

### Performance Trade-offs

**Speed vs Size:**
-O3 produces larger code than -Os. Acceptable trade-off for performance.

**Speed vs Precision:**
-ffast-math sacrifices IEEE floating-point compliance. Acceptable for audio DSP (no precision-critical calculations).

**Features vs Performance:**
If 30% target not met:
- Reduce reverb quality (last resort)
- Disable 48kHz mode (force 32kHz)
- Simplify parameter smoothing (if implemented)

**Expected:** 30% target achievable without major compromises.

### Project Structure

**Files Modified:**
- Makefile - Update compiler optimization flags
- src/nt_elements.cpp - Add reverb bypass logic (if needed)

**Files Created:**
- docs/performance/optimization-report.md (new) - Performance documentation

### References

- [Source: docs/epics.md#Story 2.5] - Full story specification and acceptance criteria
- [Source: docs/PRD.md#NFR001] - Performance requirement (< 30% CPU)
- [Source: docs/architecture.md#Performance Considerations] - CPU budget and optimization strategies
- [Source: docs/architecture.md#Reverb] - Reverb inclusion, bypass logic, CPU savings
- [Source: docs/architecture.md#Sample Rate] - 32kHz mode for performance optimization

## Dev Agent Record

### Context Reference

- docs/stories/2-5-performance-optimization.context.md

### Agent Model Used

<!-- Model information will be filled in during implementation -->

### Debug Log References

### Completion Notes List

### File List
