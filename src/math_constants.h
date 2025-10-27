/*
 * Math constants and sample rate override for Elements compilation
 * M_PI and related constants not guaranteed in all C++11 implementations
 * kSampleRate overridden from Elements default 32kHz to disting NT's 48kHz
 */

#ifndef NT_ELEMENTS_MATH_CONSTANTS_H_
#define NT_ELEMENTS_MATH_CONSTANTS_H_

#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_SQRT2
#define M_SQRT2 1.41421356237309504880
#endif

// Story 1.5 Note: Sample Rate Override
// Elements uses `static const float kSampleRate = 32000.0f` which cannot be overridden
// without modifying the Elements source (readonly git submodule).
// The plugin currently runs Elements at its native 32kHz internally.
// Future Story 1.11 will implement runtime sample rate mode toggle with proper resampling.
// For now, Elements runs at 32kHz with audio resampling handled by NT hardware (if needed)

#endif // NT_ELEMENTS_MATH_CONSTANTS_H_
