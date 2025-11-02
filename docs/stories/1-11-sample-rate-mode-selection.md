# Story 1.11: Sample Rate Mode Selection

Status: ready-for-dev

## Story

As a user,
I want to choose between 48kHz (converted) and 32kHz (authentic) operation modes,
So that I can optimize for either performance or compatibility with other NT algorithms.

## Acceptance Criteria

1. Mode selection parameter added (48kHz/32kHz toggle)
2. In 32kHz mode, Elements runs at native sample rate (no conversion overhead)
3. In 48kHz mode, converted coefficients used (from Story 1.5)
4. Mode switch does not cause audio glitches or pops
5. CPU usage in 32kHz mode measured and compared to 48kHz (expect ~15-20% savings)
6. User documentation notes trade-offs: 32kHz = lower CPU, 48kHz = better NT integration

## Tasks / Subtasks

- [ ] Add sample rate mode parameter (AC: #1)
  - [ ] Define kParamSampleRateMode enum value
  - [ ] Add parameter to NT parameter array
  - [ ] Set default mode (48kHz recommended for NT integration)
  - [ ] Define mode values (0 = 32kHz, 1 = 48kHz)
  - [ ] Add parameter metadata (name, range, display)

- [ ] Implement mode switching logic (AC: #2, #3)
  - [ ] Add mode state variable to algorithm structure
  - [ ] Implement parameterChanged() handler for mode parameter
  - [ ] Call elements_part->Init(32000) for 32kHz mode
  - [ ] Call elements_part->Init(48000) for 48kHz mode
  - [ ] Verify Elements reinitializes correctly on mode change

- [ ] Handle sample rate conversion in 32kHz mode (AC: #2)
  - [ ] Implement sample rate conversion for input buffer (48kHz → 32kHz)
  - [ ] Implement sample rate conversion for output buffer (32kHz → 48kHz)
  - [ ] Use simple decimation/interpolation or skip for now
  - [ ] Alternative: Disable 32kHz mode in Story 1.11, enable in Epic 2
  - [ ] Document conversion approach or mode limitation

- [ ] Implement glitch-free mode switching (AC: #4)
  - [ ] Mute output briefly during mode switch
  - [ ] Reinitialize Elements DSP
  - [ ] Clear audio buffers
  - [ ] Fade output back in smoothly
  - [ ] Verify no clicks or pops during switch

- [ ] Test mode switching in nt_emu (AC: #1, #4)
  - [ ] Load plugin in nt_emu
  - [ ] Start synthesis in 48kHz mode
  - [ ] Switch to 32kHz mode while note playing
  - [ ] Listen for glitches or artifacts
  - [ ] Switch back to 48kHz mode
  - [ ] Verify smooth transitions

- [ ] Measure CPU usage for both modes (AC: #5)
  - [ ] Deploy to hardware (disting NT)
  - [ ] Measure CPU % in 48kHz mode (idle and synthesis)
  - [ ] Measure CPU % in 32kHz mode (idle and synthesis)
  - [ ] Calculate CPU savings: (48kHz - 32kHz) / 48kHz
  - [ ] Verify ~15-20% savings in 32kHz mode

- [ ] Validate audio quality in both modes (AC: #2, #3)
  - [ ] Record synthesis in 48kHz mode
  - [ ] Record synthesis in 32kHz mode (same patch)
  - [ ] Perform spectral analysis comparison
  - [ ] Verify 32kHz mode sounds authentic (native Elements)
  - [ ] Verify 48kHz mode sounds identical (proper conversion)

- [ ] Create user documentation (AC: #6)
  - [ ] Document mode selection parameter
  - [ ] Explain 32kHz mode benefits (lower CPU, authentic)
  - [ ] Explain 48kHz mode benefits (NT integration, compatibility)
  - [ ] Provide guidance on when to use each mode
  - [ ] Add to README or user guide

- [ ] Test mode persistence in presets (AC: #1)
  - [ ] Save preset in 48kHz mode
  - [ ] Switch to 32kHz mode
  - [ ] Load preset (should restore 48kHz mode)
  - [ ] Verify preset system saves/restores mode correctly
  - [ ] Test with multiple presets

## Dev Notes

### Sample Rate Mode Architecture

**Two Operating Modes:**

**48kHz Mode (Default):**
- Elements runs at 48kHz (disting NT native sample rate)
- Filter coefficients scaled for 48kHz (Story 1.5)
- Best compatibility with other NT algorithms
- Slightly higher CPU usage

**32kHz Mode (Performance):**
- Elements runs at 32kHz (hardware Elements native rate)
- No coefficient conversion overhead
- 100% authentic Elements sound
- Lower CPU usage (~15-20% savings)
- Requires sample rate conversion for NT I/O

**Mode Selection:**
User chooses via parameter. Default to 48kHz for best NT integration.

### Sample Rate Conversion Challenge

**The Problem:**
disting NT audio bus system runs at 48kHz. If Elements runs at 32kHz internally, input/output buffers need sample rate conversion.

**Options:**

**Option A: Simple Decimation/Interpolation**
- Input: Decimate 48kHz → 32kHz (take every 3rd sample, drop 2)
- Output: Interpolate 32kHz → 48kHz (repeat samples or linear interpolation)
- Pros: Simple, low CPU overhead
- Cons: Potential aliasing, not high quality

**Option B: Proper Sample Rate Conversion**
- Use quality resampling algorithm (sinc interpolation)
- Pros: High quality, no aliasing
- Cons: Higher CPU overhead (may negate 32kHz mode savings)

**Option C: Disable 32kHz Mode for v1.0**
- Only support 48kHz mode in Story 1.11
- Defer 32kHz mode to Epic 2 or future version
- Pros: Simpler implementation, no conversion complexity
- Cons: No performance optimization option

**Recommended for Story 1.11:**
Option C (48kHz only) or Option A (simple conversion). Proper resampling can be added in Epic 2 if needed.

### Elements Reinitialization

**Mode Switch Procedure:**
```cpp
void switch_sample_rate_mode(int new_mode) {
    // 1. Mute output
    bool previous_gate = patch.gate;
    patch.gate = false;

    // 2. Reinitialize Elements at new sample rate
    if (new_mode == MODE_32KHZ) {
        elements_part->Init(32000);
    } else {
        elements_part->Init(48000);
    }

    // 3. Clear buffers (avoid artifacts)
    memset(input_buffer, 0, buffer_size * sizeof(float));
    memset(temp_output, 0, buffer_size * sizeof(float));

    // 4. Restore gate (unmute)
    patch.gate = previous_gate;

    // 5. Update mode state
    current_mode = new_mode;
}
```

**Glitch Prevention:**
- Briefly mute output during switch (set gate = false)
- Reinitialize Elements DSP
- Clear audio buffers to avoid residual artifacts
- Restore gate state after initialization

### CPU Usage Comparison

**Expected CPU Savings in 32kHz Mode:**

**48kHz Mode:**
- Filter processing: ~18-22% CPU
- Reverb: ~8-10% CPU
- Other: ~2-3% CPU
- **Total: ~30-35% CPU**

**32kHz Mode:**
- Filter processing: ~12-15% CPU (2/3 of 48kHz rate)
- Reverb: ~5-7% CPU (2/3 of 48kHz rate)
- Sample rate conversion: ~1-2% CPU
- Other: ~2-3% CPU
- **Total: ~20-27% CPU**

**CPU Savings: ~10% absolute, ~30% relative**

**Measurement:**
Measure on hardware (disting NT) using built-in CPU display. Desktop measurements not accurate for embedded ARM platform.

### Mode Selection Trade-offs

**32kHz Mode Benefits:**
- Lower CPU usage (~15-20% savings)
- 100% authentic Elements sound (no coefficient conversion)
- Matches hardware Elements sample rate exactly
- Better for standalone synthesis

**32kHz Mode Drawbacks:**
- Sample rate conversion overhead (slight quality degradation)
- May not integrate as cleanly with 48kHz NT algorithms
- Potential aliasing if using simple decimation/interpolation

**48kHz Mode Benefits:**
- Native NT sample rate (best integration)
- No sample rate conversion needed
- Clean routing to/from other 48kHz algorithms
- Better for complex patching with other NT algorithms

**48kHz Mode Drawbacks:**
- Slightly higher CPU usage (~15-20% more than 32kHz)
- Filter coefficients recalculated (minor deviation from hardware Elements)

**Recommendation:**
Default to 48kHz for most users. Offer 32kHz as power-user option for CPU optimization.

### User Documentation Content

**Mode Selection Parameter:**
```
Sample Rate Mode
- 48kHz (Default): Best NT integration, slightly higher CPU
- 32kHz: Lower CPU, authentic Elements sample rate

When to use 48kHz mode:
- Routing nt_elements with other NT algorithms
- Using external audio input from 48kHz sources
- Maximum compatibility with NT ecosystem

When to use 32kHz mode:
- Standalone synthesis (no routing to other algorithms)
- Need lower CPU usage for chaining algorithms
- Want authentic hardware Elements sample rate
```

### Preset System Integration

**Mode Parameter Serialization:**
Sample rate mode parameter is automatically saved/loaded with presets via NT preset system.

**Behavior:**
- User saves preset in 48kHz mode → mode stored in preset
- User switches to 32kHz mode globally
- User loads preset → mode switches back to 48kHz
- Each preset remembers its sample rate mode

**Testing:**
Create presets in both modes and verify mode switches correctly on preset load.

### Implementation Simplification Option

**Simplified Story 1.11 (Recommended):**
If sample rate conversion adds too much complexity, simplify Story 1.11 to only support 48kHz mode. Add 32kHz mode in Epic 2 (Story 2.5 performance optimization) if CPU headroom allows.

**Simplified Acceptance Criteria:**
1. ~~Mode selection parameter added~~ (deferred)
2. ~~32kHz mode support~~ (deferred)
3. 48kHz mode operational and validated
4. ~~Mode switching~~ (deferred)
5. CPU usage measured and documented in 48kHz mode
6. User documentation notes 48kHz operation

This simplification keeps Epic 1 focused on core functionality. Performance optimization (including 32kHz mode) addressed in Epic 2.

### Project Structure

**Files Modified:**
- src/nt_elements.cpp: Add mode parameter and switching logic
- src/parameter_adapter.h: Add kParamSampleRateMode
- README.md: Add mode selection documentation (if implemented)

### References

- [Source: docs/epics.md#Story 1.11] - Full story specification and acceptance criteria
- [Source: docs/architecture.md#ADR-002] - Runtime Sample Rate Mode Toggle decision
- [Source: docs/PRD.md#FR014] - 48kHz sample rate functional requirement
- [Source: docs/architecture.md#Sample Rate] - Dual mode support and implementation details
- [Source: docs/architecture.md#Performance Considerations] - CPU budget and optimization strategies

## Dev Agent Record

### Context Reference

- [1-11-sample-rate-mode-selection.context.md](./1-11-sample-rate-mode-selection.context.md)

### Agent Model Used

<!-- Model information will be filled in during implementation -->

### Debug Log References

### Completion Notes List

### File List
