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

// Parameter name abbreviations for display (max ~12 chars to fit)
const char* PARAM_NAMES[kNumParams] = {
    // System parameters
    "Input",
    "Output",
    "Mode",

    // Page 1 - Exciter
    "Bow",
    "Blow",
    "Strike",
    "BowTim",
    "BlowTim",

    // Page 2 - Resonator
    "Geom",
    "Bright",
    "Damp",
    "Pos",
    "Inharm",

    // Page 3 - Space
    "Reverb",
    "Size",
    "RvDamp",

    // Page 4 - Performance
    "Coarse",
    "Fine",
    "Level",
    "FM Amt",
    "Contour"
};

void clearDisplay() {
    // Clear screen memory (256x64 pixels, each byte = 2 pixels)
    memset(NT_screen, 0, 128 * 64);
}

void renderPageTitle(int current_page) {
    // Get page name from page mappings
    if (current_page < 0 || current_page >= kNumPages) {
        return;  // Invalid page index
    }

    const char* page_name = PAGE_MAPPINGS[current_page].name;

    // Draw page title centered at top with large font
    NT_drawText(SCREEN_WIDTH / 2, PAGE_TITLE_Y, page_name, TEXT_COLOR, kNT_textCentre, TITLE_FONT);
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

    // Render pot parameters (3 pots)
    int y_pos = PARAM_START_Y;
    for (int i = 0; i < 3; ++i) {
        int param_index = page.pot_mapping[i];

        // Get parameter name
        const char* param_name = getParamName(param_index);

        // Get parameter value from NT parameter array
        float param_value = algo->v[param_index];

        // Format value based on parameter type
        char value_str[12];
        formatParamValue(param_index, param_value, value_str);

        // Draw parameter name (left-aligned)
        NT_drawText(PARAM_NAME_X, y_pos, param_name, TEXT_COLOR, kNT_textLeft, PARAM_FONT);

        // Draw parameter value (right-aligned)
        NT_drawText(PARAM_VALUE_X, y_pos, value_str, TEXT_COLOR, kNT_textRight, PARAM_FONT);

        y_pos += PARAM_LINE_HEIGHT;
    }

    // Render encoder parameters (2 encoders)
    for (int i = 0; i < 2; ++i) {
        int param_index = page.encoder_mapping[i];

        // Skip if encoder not mapped on this page
        if (param_index < 0) {
            continue;
        }

        // Get parameter name
        const char* param_name = getParamName(param_index);

        // Get parameter value from NT parameter array
        float param_value = algo->v[param_index];

        // Format value based on parameter type
        char value_str[12];
        formatParamValue(param_index, param_value, value_str);

        // Draw parameter name (left-aligned)
        NT_drawText(PARAM_NAME_X, y_pos, param_name, TEXT_COLOR, kNT_textLeft, PARAM_FONT);

        // Draw parameter value (right-aligned)
        NT_drawText(PARAM_VALUE_X, y_pos, value_str, TEXT_COLOR, kNT_textRight, PARAM_FONT);

        y_pos += PARAM_LINE_HEIGHT;
    }
}

void renderDisplay(nt_elementsAlgorithm* algo) {
    if (!algo) {
        return;
    }

    // Clear display
    clearDisplay();

    // Render page title
    renderPageTitle(algo->current_page);

    // Render parameters
    renderParameters(algo);
}

} // namespace oled_display
