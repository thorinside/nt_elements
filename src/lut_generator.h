/*
 * lut_generator.h - Runtime LUT generation for nt_elements
 *
 * Generates all Elements lookup tables at runtime and stores them in
 * API-allocated static DRAM, eliminating ~33KB from .rodata.
 *
 * Copyright (c) 2025 Neal Sanche
 * Licensed under MIT License
 */

#ifndef NT_ELEMENTS_LUT_GENERATOR_H_
#define NT_ELEMENTS_LUT_GENERATOR_H_

#include <cstddef>
#include <cstdint>

// Returns total DRAM bytes needed for all LUTs (with alignment padding)
size_t lutGeneratorTotalBytes();

// Computes all LUTs into the DRAM block and sets the global pointer variables
void lutGeneratorInit(uint8_t* dram);

#endif // NT_ELEMENTS_LUT_GENERATOR_H_
