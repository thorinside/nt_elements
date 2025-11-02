/*
 * oled_display.h - OLED display rendering for nt_elements
 *
 * Provides display rendering functions for the disting NT OLED screen.
 * Displays current page name and parameter values for pots/encoders.
 *
 * Display Layout (128x64 pixels):
 * - Line 1: Page title (large font, 21pt)
 * - Lines 2-6: Parameter name/value pairs (normal font, 8pt)
 *
 * Copyright (c) 2025
 * Licensed under MIT License
 */

#ifndef NT_ELEMENTS_OLED_DISPLAY_H_
#define NT_ELEMENTS_OLED_DISPLAY_H_

#include <cstdint>
#include "distingnt/api.h"
#include "parameter_adapter.h"
#include "parameter_pages.h"
#include "nt_elements.h"

namespace oled_display {

// Display layout constants (128x64 pixel screen)
const int SCREEN_WIDTH = 256;   // 256x64 pixels (each byte = 2 pixels)
const int SCREEN_HEIGHT = 64;

// Layout positions
const int PAGE_TITLE_Y = 0;         // Page title at top
const int PARAM_START_Y = 14;       // Parameters start below title
const int PARAM_LINE_HEIGHT = 10;   // Space between parameter lines
const int PARAM_NAME_X = 2;         // Left margin for parameter names
const int PARAM_VALUE_X = 180;      // Right-aligned values

// Font sizes
const _NT_textSize TITLE_FONT = kNT_textLarge;   // 21pt for page title
const _NT_textSize PARAM_FONT = kNT_textNormal;  // 8pt for parameters

// Text colors (0-15, where 15 is white/brightest)
const int TEXT_COLOR = 15;
const int BG_COLOR = 0;

/**
 * Render the current page title at the top of the display
 * @param current_page Page index (0-3)
 */
void renderPageTitle(int current_page);

/**
 * Render parameter names and values for the current page
 * @param algo Algorithm instance containing current parameter values
 */
void renderParameters(nt_elementsAlgorithm* algo);

/**
 * Format a parameter value as a percentage string
 * @param value Parameter value (0-100)
 * @param buffer Output buffer (min 8 bytes)
 * @return Length of formatted string
 */
int formatPercentage(float value, char* buffer);

/**
 * Format a tuning value as semitones string
 * @param value Parameter value (0-100, maps to -12 to +12 semitones)
 * @param buffer Output buffer (min 8 bytes)
 * @return Length of formatted string
 */
int formatSemitones(float value, char* buffer);

/**
 * Format a tuning value as cents string
 * @param value Parameter value (0-100, maps to -50 to +50 cents)
 * @param buffer Output buffer (min 8 bytes)
 * @return Length of formatted string
 */
int formatCents(float value, char* buffer);

/**
 * Format geometry value as descriptive text
 * @param value Geometry parameter value (0-100)
 * @param buffer Output buffer (min 12 bytes)
 * @return Length of formatted string
 */
int formatGeometry(float value, char* buffer);

/**
 * Get abbreviated parameter name for display
 * @param param_index Parameter index from ParameterIndices enum
 * @return Abbreviated parameter name (fits display width)
 */
const char* getParamName(int param_index);

/**
 * Format a parameter value based on its type
 * @param param_index Parameter index from ParameterIndices enum
 * @param value Parameter value (0-100)
 * @param buffer Output buffer (min 12 bytes)
 * @return Length of formatted string
 */
int formatParamValue(int param_index, float value, char* buffer);

/**
 * Clear the display (set all pixels to black)
 */
void clearDisplay();

/**
 * Render the complete display for the current page
 * Called from draw() callback when display_dirty flag is set
 * @param algo Algorithm instance
 */
void renderDisplay(nt_elementsAlgorithm* algo);

} // namespace oled_display

#endif // NT_ELEMENTS_OLED_DISPLAY_H_
