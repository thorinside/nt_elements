/*
 * Math constants and sample rate injection for Elements DSP
 *
 * This file is force-included via Makefile (-include src/math_constants.h)
 * BEFORE any other headers, allowing us to inject NT's dynamic sample rate
 * into Elements' namespace without modifying the readonly submodule.
 */

#ifndef NT_ELEMENTS_MATH_CONSTANTS_H_
#define NT_ELEMENTS_MATH_CONSTANTS_H_

#include <cmath>

// Math constants (not guaranteed in all C++11 implementations)
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_SQRT2
#define M_SQRT2 1.41421356237309504880
#endif

// ============================================================================
// Sample Rate Note (Story 1.5)
// ============================================================================
// Elements DSP uses hardcoded: static const float kSampleRate = 32000.0f
// disting NT provides user-configurable sample rate via NT_globals.sampleRate
//
// RESOLUTION: Modify external/mutable-instruments/elements/dsp/dsp.h directly
// to use NT_globals.sampleRate. The MIT license permits this modification.
// The submodule is local to this project, not pushed upstream.
//
// See docs/sprint-change-proposal-sample-rate-fix-2025-11-02.md for details.
// ============================================================================

#endif // NT_ELEMENTS_MATH_CONSTANTS_H_
