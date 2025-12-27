# Makefile for nt_elements - Mutable Instruments Elements port for disting NT
#
# Dual build targets:
#   make hardware - Build ARM .o for disting NT hardware
#   make test     - Build native .dylib/.so for desktop testing in VCV Rack nt_emu
#   make clean    - Clean build artifacts

# Project configuration
PROJECT = nt_elements
DISTINGNT_API = distingNT_API
ELEMENTS_ROOT = external/mutable-instruments/elements
STMLIB_ROOT = external/mutable-instruments/stmlib

# Version from git (tag or commit hash)
VERSION := $(shell git describe --tags --always --dirty 2>/dev/null || echo "v1.0.0-dev")

# Source files
# Note: Using resources_luts_only.cc (LUTs without sample data)
# Sample data now loaded dynamically via SampleManager from SD card
SOURCES = \
	src/nt_elements.cpp \
	src/oled_display.cpp \
	src/sample_manager.cpp \
	src/resources_luts_only.cc \
	external/mutable-instruments/elements/dsp/exciter.cc \
	external/mutable-instruments/elements/dsp/multistage_envelope.cc \
	external/mutable-instruments/elements/dsp/ominous_voice.cc \
	external/mutable-instruments/elements/dsp/part.cc \
	external/mutable-instruments/elements/dsp/resonator.cc \
	external/mutable-instruments/elements/dsp/string.cc \
	external/mutable-instruments/elements/dsp/tube.cc \
	external/mutable-instruments/elements/dsp/voice.cc \
	external/mutable-instruments/stmlib/utils/random.cc \
	external/mutable-instruments/stmlib/dsp/units.cc

# Using extracted LUTs only (no wavetable sample data)
# Full resources.cc = 372KB, extracted = all LUTs but stub wavetables
# Limitation: Wavetable oscillator mode unavailable, resonator modes should work

# Include paths and defines
INCLUDES = \
	-I$(DISTINGNT_API)/include \
	-Iexternal/mutable-instruments

DEFINES_COMMON = -DTEST -D_USE_MATH_DEFINES -include src/math_constants.h -DNT_ELEMENTS_VERSION=\"$(VERSION)\"
DEFINES_HARDWARE = $(DEFINES_COMMON)
DEFINES_TEST = $(DEFINES_COMMON) -DNT_EMU_DEBUG

# Compiler flags - common
CXXFLAGS_COMMON = -std=c++11 -Wall -Wextra -fno-rtti -fno-exceptions

# Compiler flags - hardware (ARM Cortex-M7)
CXX_ARM = arm-none-eabi-g++
CXXFLAGS_ARM = $(CXXFLAGS_COMMON) $(DEFINES_HARDWARE) \
	-mcpu=cortex-m7 \
	-mfpu=fpv5-d16 \
	-mfloat-abi=hard \
	-O3 \
	-ffast-math \
	-funroll-loops \
	-fdata-sections \
	-ffunction-sections

# Compiler flags - desktop test
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    CXX_TEST = clang++
    DYLIB_EXT = dylib
else
    CXX_TEST = g++
    DYLIB_EXT = so
endif

CXXFLAGS_TEST = $(CXXFLAGS_COMMON) $(DEFINES_TEST) -O2 -fPIC

# Output directories
PLUGINS_DIR = plugins
BUILD_DIR = build

# Patch management
PATCH_DIR = patches
PATCH_MARKER = external/mutable-instruments/elements/dsp/.nt_elements_patched

# Targets
.PHONY: all hardware test clean apply-patches extract-samples

all: apply-patches hardware test

# Apply patches to Elements DSP if not already applied
# Patch order: sample-rate first, then samples (samples patch depends on clean resources.h)
apply-patches:
	@if [ ! -f $(PATCH_MARKER) ]; then \
		echo "Applying Elements DSP patches..."; \
		cd external/mutable-instruments && \
		patch -p1 < ../../$(PATCH_DIR)/elements-dynamic-sample-rate.patch && \
		patch -p1 < ../../$(PATCH_DIR)/elements-dynamic-samples.patch && \
		touch elements/dsp/.nt_elements_patched && \
		echo "Patches applied successfully"; \
	fi

# Hardware target - ARM .o for disting NT
hardware: apply-patches $(PLUGINS_DIR)/$(PROJECT).o

# Generate object file list for hardware build
OBJS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(notdir $(filter %.cpp,$(SOURCES)))) \
       $(patsubst %.cc,$(BUILD_DIR)/%.o,$(notdir $(filter %.cc,$(SOURCES))))

$(BUILD_DIR)/%.o: src/%.cpp | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CXX_ARM) $(CXXFLAGS_ARM) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.o: src/%.cc | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CXX_ARM) $(CXXFLAGS_ARM) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.o: external/mutable-instruments/elements/dsp/%.cc | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CXX_ARM) $(CXXFLAGS_ARM) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.o: external/mutable-instruments/elements/%.cc | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CXX_ARM) $(CXXFLAGS_ARM) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.o: external/mutable-instruments/stmlib/utils/%.cc | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CXX_ARM) $(CXXFLAGS_ARM) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.o: external/mutable-instruments/stmlib/dsp/%.cc | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CXX_ARM) $(CXXFLAGS_ARM) $(INCLUDES) -c $< -o $@

$(PLUGINS_DIR)/$(PROJECT).o: $(OBJS) | $(PLUGINS_DIR)
	$(CXX_ARM) -r $(OBJS) -o $@
	@echo "Hardware build complete: $@"
	@ls -lh $@
	@echo "NOTE: Built with minimal wavetables (all LUTs + noise + 2 smallest wavetables): ~180KB vs 407KB full"

# Test target - native .dylib/.so for VCV Rack nt_emu
test: apply-patches $(PLUGINS_DIR)/$(PROJECT).$(DYLIB_EXT)

$(PLUGINS_DIR)/$(PROJECT).$(DYLIB_EXT): $(SOURCES) | $(PLUGINS_DIR) $(BUILD_DIR)
ifeq ($(UNAME_S),Darwin)
	$(CXX_TEST) $(CXXFLAGS_TEST) $(INCLUDES) -dynamiclib -undefined dynamic_lookup $(SOURCES) -o $@
else
	$(CXX_TEST) $(CXXFLAGS_TEST) $(INCLUDES) -shared $(SOURCES) -o $@
endif
	@echo "Desktop test build complete: $@"
	@ls -lh $@

# Create output directories
$(PLUGINS_DIR):
	mkdir -p $(PLUGINS_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean build artifacts
clean:
	rm -rf $(PLUGINS_DIR) $(BUILD_DIR)
	@echo "Build artifacts cleaned"

# Clean build artifacts AND remove patch marker (forces reapplication)
clean-all: clean
	rm -f $(PATCH_MARKER)
	@echo "Build artifacts and patch marker cleaned"

# Extract wavetable and noise samples from resources.cc into WAV files
# Samples are needed for dynamic sample loading (Epic 3)
SAMPLES_DIR = samples/elements
extract-samples:
	@if [ ! -f $(SAMPLES_DIR)/wavetable_00.wav ]; then \
		echo "Extracting samples from resources.cc..."; \
		python3 scripts/extract_samples.py; \
	else \
		echo "Samples already extracted in $(SAMPLES_DIR)"; \
		echo "Use 'python3 scripts/extract_samples.py --force' to regenerate"; \
	fi
