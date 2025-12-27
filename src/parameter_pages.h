/*
 * parameter_pages.h - Parameter page organization for nt_elements
 *
 * Defines 4-page structure for organizing nt_elements parameters:
 * - Page 1 (Exciter): Excitation level and strength controls
 * - Page 2 (Timbre): Timbre/color controls for each exciter type
 * - Page 3 (Resonator): Resonator material and spatial controls
 * - Page 4 (Tuning): Global tuning, FM, and output controls
 *
 * Each page maps NT hardware controls (3 pots + 2 encoders) to specific parameters.
 * Layout mirrors the original Elements hardware panel organization.
 *
 * Copyright (c) 2025
 * Licensed under MIT License
 */

#ifndef NT_ELEMENTS_PARAMETER_PAGES_H_
#define NT_ELEMENTS_PARAMETER_PAGES_H_

#include "parameter_adapter.h"

// Page enumeration
enum PageIndex {
    kPageExciter = 0,
    kPageTimbre = 1,
    kPageResonator = 2,
    kPageTuning = 3,
    kNumPages = 4
};

// Page 1 - Exciter: Control how the resonator is excited (bowed, blown, struck)
// Physical modeling synthesis begins with excitation
namespace page_exciter {
    // Pot mappings (3 pots) - main exciter levels
    // Pot 1: Bow (bow exciter level)
    // Pot 2: Blow (blow exciter level)
    // Pot 3: Strike (strike exciter level)
    const int POT_MAPPING[3] = {
        kParamBowLevel,        // Bow Level
        kParamBlowLevel,       // Blow Level
        kParamStrikeLevel      // Strike Level
    };

    // Encoder mappings (2 encoders)
    // Encoder 1 (L): Strength (excitation intensity)
    // Encoder 2 (R): Contour (envelope shape for bow/blow)
    const int ENCODER_MAPPING[2] = {
        kParamStrength,        // Strength
        kParamExciterContour   // Contour
    };

    static const char* PAGE_NAME = "EXCITER";
}

// Page 2 - Timbre: Tonal character of each exciter type
// Shapes the frequency content and texture of excitation signals
namespace page_timbre {
    // Pot mappings (3 pots)
    // Pot 1: Bow Timbre (bow smoothness/granularity)
    // Pot 2: Blow Timbre (blow pitch/granulation rate)
    // Pot 3: Strike Timbre (strike brightness/speed)
    const int POT_MAPPING[3] = {
        kParamBowTimbre,       // Bow Timbre
        kParamBlowTimbre,      // Blow Timbre
        kParamStrikeTimbre     // Strike Timbre
    };

    // Encoder mappings (2 encoders)
    // Encoder 1: Flow (blow meta - air flow character)
    // Encoder 2: Mallet (strike meta - mallet type)
    const int ENCODER_MAPPING[2] = {
        kParamBlowFlow,        // Flow (blow meta)
        kParamStrikeMallet     // Mallet (strike meta)
    };

    static const char* PAGE_NAME = "TIMBRE";
}

// Page 3 - Resonator: The core of Elements synthesis
// Modal filters create the physical modeling character
namespace page_resonator {
    // Pot mappings (3 pots)
    // Pot 1: Geometry (structure shape)
    // Pot 2: Brightness (high frequency damping)
    // Pot 3: Damping (energy dissipation)
    const int POT_MAPPING[3] = {
        kParamGeometry,        // Geometry
        kParamBrightness,      // Brightness
        kParamDamping          // Damping
    };

    // Encoder mappings (2 encoders)
    // Encoder 1: Position (excitation point)
    // Encoder 2: Space (stereo width + reverb)
    const int ENCODER_MAPPING[2] = {
        kParamResonatorPosition,  // Position
        kParamReverbAmount        // Space (reverb amount)
    };

    static const char* PAGE_NAME = "RESONATOR";
}

// Page 4 - Tuning: Global parameters affecting overall synthesis
// Tuning, output, and modulation controls
namespace page_tuning {
    // Pot mappings (3 pots)
    // Pot 1: Coarse (semitone tuning)
    // Pot 2: Fine (cent tuning)
    // Pot 3: FM Amount (frequency modulation depth)
    const int POT_MAPPING[3] = {
        kParamCoarseTune,      // Coarse Tune
        kParamFineTune,        // Fine Tune
        kParamFMAmount         // FM Amount
    };

    // Encoder mappings (2 encoders)
    // Encoder 1: Output Level (master volume)
    // Encoder 2: Reverb Size (additional reverb control)
    const int ENCODER_MAPPING[2] = {
        kParamOutputLevel,     // Output Level
        kParamReverbSize       // Reverb Size
    };

    static const char* PAGE_NAME = "TUNING";
}

// Unified page access structure for generic page navigation
struct PageMapping {
    const int* pot_mapping;
    const int* encoder_mapping;
    const char* name;
};

// Page mapping table indexed by PageIndex (static to avoid duplicate symbols)
static const PageMapping PAGE_MAPPINGS[kNumPages] = {
    { page_exciter::POT_MAPPING, page_exciter::ENCODER_MAPPING, page_exciter::PAGE_NAME },
    { page_timbre::POT_MAPPING, page_timbre::ENCODER_MAPPING, page_timbre::PAGE_NAME },
    { page_resonator::POT_MAPPING, page_resonator::ENCODER_MAPPING, page_resonator::PAGE_NAME },
    { page_tuning::POT_MAPPING, page_tuning::ENCODER_MAPPING, page_tuning::PAGE_NAME }
};

#endif // NT_ELEMENTS_PARAMETER_PAGES_H_
