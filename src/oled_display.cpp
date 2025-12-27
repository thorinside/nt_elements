/*
 * oled_display.cpp - OLED display rendering implementation for nt_elements
 *
 * Copyright (c) 2025 Neal Sanche
 * Licensed under MIT License - see LICENSE file for details
 */

#include "oled_display.h"
#include "nt_elements.h"
#include <cstring>
#include <cstdio>

namespace oled_display {

// Attribution for original Elements creator
const char* ATTRIBUTION_TEXT = "By Emilie Gillet";

// Parameter name abbreviations for display (max ~12 chars to fit)
const char* PARAM_NAMES[kNumParams] = {
    // System parameters (routing)
    "BlowIn",     // kParamBlowInputBus
    "StrikeIn",   // kParamStrikeInputBus
    "Output",     // kParamOutputBus
    "Mode",       // kParamOutputMode
    "AuxOut",     // kParamAuxOutputBus
    "AuxMode",    // kParamAuxOutputMode

    // Exciter parameters
    "Bow",        // kParamBowLevel
    "Blow",       // kParamBlowLevel
    "Strike",     // kParamStrikeLevel
    "BowTim",     // kParamBowTimbre
    "BlowTim",    // kParamBlowTimbre
    "StrTim",     // kParamStrikeTimbre
    "Flow",       // kParamBlowFlow
    "Mallet",     // kParamStrikeMallet

    // Resonator parameters
    "Geom",       // kParamGeometry
    "Bright",     // kParamBrightness
    "Damp",       // kParamDamping
    "Pos",        // kParamResonatorPosition
    "Inharm",     // kParamInharmonicity

    // Reverb/Space parameters
    "Reverb",     // kParamReverbAmount
    "Size",       // kParamReverbSize
    "RvDamp",     // kParamReverbDamping

    // Performance/Tuning parameters
    "Coarse",     // kParamCoarseTune
    "Fine",       // kParamFineTune
    "Level",      // kParamOutputLevel
    "FM Amt",     // kParamFMAmount
    "Contour",    // kParamExciterContour
    "Strength",   // kParamStrength

    // Routing parameters (MIDI + CV)
    "MIDI Ch",    // kParamMidiChannel
    "V/Oct",      // kParamVOctCV
    "Gate",       // kParamGateCV
    "FM CV",      // kParamFMCV
    "BrightCV",   // kParamBrightnessCV
    "Expr"        // kParamExpressionCV
};

// Page title display timing constants (assuming ~60 FPS draw rate)
const uint32_t PAGE_NAME_DISPLAY_FRAMES = 300;  // Show page name for ~5 seconds (300 frames @ 60Hz)
const uint32_t FADE_DURATION_FRAMES = 30;       // Fade transition ~500ms (30 frames @ 60Hz)

void renderPageTitle(nt_elementsAlgorithm* algo) {
    if (!algo) return;

    int current_page = algo->current_page;

    // Get page name from page mappings
    if (current_page < 0 || current_page >= kNumPages) {
        return;  // Invalid page index
    }

    const PageMapping& page = PAGE_MAPPINGS[current_page];
    if (!page.name) return;

    // Increment frame counter
    algo->draw_frame_count++;

    // Detect page change
    if (current_page != algo->last_displayed_page) {
        algo->last_displayed_page = current_page;
        algo->page_change_frame = algo->draw_frame_count;
    }

    // Calculate frames since page change
    uint32_t elapsed_frames = algo->draw_frame_count - algo->page_change_frame;

    // Determine what to display and at what brightness
    const char* title_text;
    int brightness;

    if (elapsed_frames < PAGE_NAME_DISPLAY_FRAMES) {
        // Show page name at full brightness
        title_text = page.name;
        brightness = TEXT_COLOR;
    } else if (elapsed_frames < PAGE_NAME_DISPLAY_FRAMES + FADE_DURATION_FRAMES) {
        // Fade from page name to "Elements"
        uint32_t fade_progress = elapsed_frames - PAGE_NAME_DISPLAY_FRAMES;
        float fade_ratio = static_cast<float>(fade_progress) / FADE_DURATION_FRAMES;

        // Cross-fade: page name fades out, "Elements" fades in
        int page_brightness = static_cast<int>((1.0f - fade_ratio) * TEXT_COLOR);
        int elements_brightness = static_cast<int>(fade_ratio * TEXT_COLOR);

        // Draw page name fading out
        if (page_brightness > 0) {
            NT_drawText(SCREEN_WIDTH / 2, PAGE_TITLE_Y, page.name, page_brightness, kNT_textCentre, TITLE_FONT);
        }
        // Draw "Elements" fading in
        if (elements_brightness > 0) {
            NT_drawText(SCREEN_WIDTH / 2, PAGE_TITLE_Y, "Elements", elements_brightness, kNT_textCentre, TITLE_FONT);
        }
        return;
    } else {
        // Show "Elements" at full brightness
        title_text = "Elements";
        brightness = TEXT_COLOR;
    }

    // Draw title at calculated brightness
    NT_drawText(SCREEN_WIDTH / 2, PAGE_TITLE_Y, title_text, brightness, kNT_textCentre, TITLE_FONT);
}

const char* getParamName(int param_index) {
    if (param_index < 0 || param_index >= kNumParams) {
        return "???";
    }
    return PARAM_NAMES[param_index];
}

int formatPercentage(float value, char* buffer) {
    // Format as integer percentage: "75%"
    int percent = static_cast<int>(value + 0.5f);  // Round to nearest int
    return snprintf(buffer, 8, "%d%%", percent);
}

int formatSemitones(float value, char* buffer) {
    // Map 0-100 to -12 to +12 semitones
    float semitones = (value - 50.0f) * 0.24f;

    // Format with sign and one decimal place: "+2.5" or "-5.0"
    return snprintf(buffer, 8, "%+.1f", semitones);
}

int formatCents(float value, char* buffer) {
    // Map 0-100 to -50 to +50 cents
    float cents = (value - 50.0f) * 1.0f;

    // Format with sign and 'c' suffix: "+25c" or "-12c"
    int cents_int = static_cast<int>(cents + 0.5f);
    return snprintf(buffer, 8, "%+dc", cents_int);
}

int formatGeometry(float value, char* buffer) {
    // Map geometry value to descriptive names
    const char* geom_name;

    if (value < 25.0f) {
        geom_name = "String";
    } else if (value < 50.0f) {
        geom_name = "Tube";
    } else if (value < 75.0f) {
        geom_name = "Plate";
    } else {
        geom_name = "Bell";
    }

    return snprintf(buffer, 12, "%s", geom_name);
}

int formatParamValue(int param_index, float value, char* buffer) {
    switch (param_index) {
        // Tuning parameters
        case kParamCoarseTune:
            return formatSemitones(value, buffer);

        case kParamFineTune:
            return formatCents(value, buffer);

        // Geometry parameter (descriptive text)
        case kParamGeometry:
            return formatGeometry(value, buffer);

        // All other parameters: percentage
        default:
            return formatPercentage(value, buffer);
    }
}

void renderParameters(nt_elementsAlgorithm* algo) {
    if (!algo) {
        return;
    }

    // Get current page mapping
    int current_page = algo->current_page;
    if (current_page < 0 || current_page >= kNumPages) {
        return;
    }

    const PageMapping& page = PAGE_MAPPINGS[current_page];

    // Safety check for null pointers in page mapping
    if (!page.pot_mapping || !page.encoder_mapping) {
        return;
    }

    // Render pot parameters (3 pots)
    int y_pos = PARAM_START_Y;
    for (int i = 0; i < 3; ++i) {
        // Bounds check for Y position
        if (y_pos >= SCREEN_HEIGHT) {
            break;
        }

        int param_index = page.pot_mapping[i];

        // Bounds check for parameter index
        if (param_index < 0 || param_index >= kNumParams) {
            y_pos += PARAM_LINE_HEIGHT;
            continue;
        }

        // Get parameter name with null check
        const char* param_name = getParamName(param_index);
        if (!param_name) {
            y_pos += PARAM_LINE_HEIGHT;
            continue;
        }

        // Get parameter value from NT parameter array
        float param_value = algo->v[param_index];

        // Format value based on parameter type with guaranteed null termination
        char value_str[12];
        value_str[0] = '\0';  // Ensure null termination
        formatParamValue(param_index, param_value, value_str);
        value_str[11] = '\0';  // Ensure null termination at end

        // Draw parameter name (left-aligned)
        NT_drawText(PARAM_NAME_X, y_pos, param_name, TEXT_COLOR, kNT_textLeft, PARAM_FONT);

        // Draw parameter value (right-aligned)
        NT_drawText(PARAM_VALUE_X, y_pos, value_str, TEXT_COLOR, kNT_textRight, PARAM_FONT);

        y_pos += PARAM_LINE_HEIGHT;
    }

    // Render encoder parameters (2 encoders)
    for (int i = 0; i < 2; ++i) {
        // Bounds check for Y position
        if (y_pos >= SCREEN_HEIGHT) {
            break;
        }

        int param_index = page.encoder_mapping[i];

        // Skip if encoder not mapped on this page
        if (param_index < 0) {
            continue;
        }

        // Bounds check for parameter index
        if (param_index >= kNumParams) {
            y_pos += PARAM_LINE_HEIGHT;
            continue;
        }

        // Get parameter name with null check
        const char* param_name = getParamName(param_index);
        if (!param_name) {
            y_pos += PARAM_LINE_HEIGHT;
            continue;
        }

        // Get parameter value from NT parameter array
        float param_value = algo->v[param_index];

        // Format value based on parameter type with guaranteed null termination
        char value_str[12];
        value_str[0] = '\0';  // Ensure null termination
        formatParamValue(param_index, param_value, value_str);
        value_str[11] = '\0';  // Ensure null termination at end

        // Draw parameter name (left-aligned)
        NT_drawText(PARAM_NAME_X, y_pos, param_name, TEXT_COLOR, kNT_textLeft, PARAM_FONT);

        // Draw parameter value (right-aligned)
        NT_drawText(PARAM_VALUE_X, y_pos, value_str, TEXT_COLOR, kNT_textRight, PARAM_FONT);

        y_pos += PARAM_LINE_HEIGHT;
    }
}

void renderSampleWarning(nt_elementsAlgorithm* algo) {
    const int BOTTOM_Y = SCREEN_HEIGHT - 8;  // Bottom of screen

    // Check if samples are loaded using the sample manager
    if (!algo->sample_manager.isLoaded()) {
        // Get loading state for more specific message
        SampleManager::LoadState state = algo->sample_manager.getLoadState();
        const char* message;

        switch (state) {
            case SampleManager::LoadState::IDLE:
            case SampleManager::LoadState::VALIDATING:
            case SampleManager::LoadState::LOADING_WAVETABLE:
            case SampleManager::LoadState::LOADING_NOISE:
                message = "Loading samples...";
                break;
            case SampleManager::LoadState::FAILED:
                message = "Samples not found";
                break;
            default:
                message = "No samples";
                break;
        }

        NT_drawText(SCREEN_WIDTH / 2, BOTTOM_Y, message, TEXT_COLOR, kNT_textCentre, kNT_textTiny);
    } else {
        // Show attribution for original Elements creator at bottom of screen
        NT_drawText(SCREEN_WIDTH / 2, BOTTOM_Y, ATTRIBUTION_TEXT, TEXT_COLOR, kNT_textCentre, kNT_textTiny);
    }
}

void renderDisplay(nt_elementsAlgorithm* algo) {
    if (!algo) {
        return;
    }

    // Render version string at top-left
#ifdef NT_ELEMENTS_VERSION
    NT_drawText(PARAM_NAME_X, VERSION_Y, "nt_elements " NT_ELEMENTS_VERSION, TEXT_COLOR, kNT_textLeft, VERSION_FONT);
#endif

    // Render page title (shows page name for 5s, then fades to "Elements")
    renderPageTitle(algo);

    // Render parameters
    renderParameters(algo);

    // Show warning or attribution at bottom of screen
    renderSampleWarning(algo);
}

} // namespace oled_display
