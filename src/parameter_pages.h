/*
 * parameter_pages.h - Parameter page organization for nt_elements
 *
 * Defines 4-page structure for organizing nt_elements parameters:
 * - Page 1 (Exciter): Bow/Blow/Strike excitation controls
 * - Page 2 (Resonator): Resonator material and character controls
 * - Page 3 (Space): Reverb and ambience controls
 * - Page 4 (Performance): Global tuning, FM, and output controls
 *
 * Each page maps NT hardware controls (3 pots + 2 encoders) to specific parameters.
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
    kPageResonator = 1,
    kPageSpace = 2,
    kPagePerformance = 3,
    kNumPages = 4
};

// Page 1 - Exciter: Control how the resonator is excited (bowed, blown, struck)
// Physical modeling synthesis begins with excitation
namespace page_exciter {
    // Pot mappings (3 pots) - using hardcoded indices to match parameters array
    const int POT_MAPPING[3] = {
        5,   // Bow Level (kParamBowLevel)
        6,   // Blow Level (kParamBlowLevel)
        7    // Strike Level (kParamStrikeLevel)
    };

    // Encoder mappings (2 encoders)
    const int ENCODER_MAPPING[2] = {
        8,   // Bow Timbre (kParamBowTimbre)
        9    // Blow Timbre (kParamBlowTimbre)
    };

    static const char* PAGE_NAME = "EXCITER";
}

// Page 2 - Resonator: The core of Elements synthesis
// Modal filters create the physical modeling character
namespace page_resonator {
    // Pot mappings (3 pots) - using hardcoded indices to match parameters array
    const int POT_MAPPING[3] = {
        10,  // Geometry (kParamGeometry)
        11,  // Brightness (kParamBrightness)
        12   // Damping (kParamDamping)
    };

    // Encoder mappings (2 encoders)
    const int ENCODER_MAPPING[2] = {
        13,  // Position (kParamResonatorPosition)
        14   // Inharmonicity (kParamInharmonicity)
    };

    static const char* PAGE_NAME = "RESONATOR";
}

// Page 3 - Space: Reverb section adds spatial dimension
// Creates ambience and room character
namespace page_space {
    // Pot mappings (3 pots) - using hardcoded indices to match parameters array
    const int POT_MAPPING[3] = {
        15,  // Reverb Amount (kParamReverbAmount)
        16,  // Reverb Size (kParamReverbSize)
        17   // Reverb Damping (kParamReverbDamping)
    };

    // Encoder mappings (2 encoders) - reserved for future use
    const int ENCODER_MAPPING[2] = {
        -1,  // Encoder 1: Reserved
        -1   // Encoder 2: Reserved
    };

    static const char* PAGE_NAME = "SPACE";
}

// Page 4 - Performance: Global parameters affecting overall synthesis
// Tuning, output, and modulation controls
namespace page_performance {
    // Pot mappings (3 pots) - using hardcoded indices to match parameters array
    const int POT_MAPPING[3] = {
        18,  // Coarse Tune (kParamCoarseTune)
        19,  // Fine Tune (kParamFineTune)
        20   // Output Level (kParamOutputLevel)
    };

    // Encoder mappings (2 encoders)
    const int ENCODER_MAPPING[2] = {
        21,  // FM Amount (kParamFMAmount)
        22   // Exciter Contour (kParamExciterContour)
    };

    static const char* PAGE_NAME = "PERF";
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
    { page_resonator::POT_MAPPING, page_resonator::ENCODER_MAPPING, page_resonator::PAGE_NAME },
    { page_space::POT_MAPPING, page_space::ENCODER_MAPPING, page_space::PAGE_NAME },
    { page_performance::POT_MAPPING, page_performance::ENCODER_MAPPING, page_performance::PAGE_NAME }
};

#endif // NT_ELEMENTS_PARAMETER_PAGES_H_
