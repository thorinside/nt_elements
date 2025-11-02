# Known Issues - nt_elements v1.0.0

## Overview

This document lists known limitations and issues in nt_elements v1.0.0. These items do not prevent normal use but are documented for user awareness.

## Issues

### Issue 1: Manual Hardware Testing Required

**Severity:** Note
**Description:** Stories 1.9 (Desktop Testing) and 1.10 (Hardware Deployment) require manual testing with physical hardware and VCV Rack. Automated testing cannot verify all functionality without human interaction with the disting NT hardware and desktop testing environment.

**Workaround:** None needed - functionality is expected to work based on code analysis and build verification.

**Planned Fix:** Manual testing by users will validate these acceptance criteria post-release.

### Issue 2: Plugin Size Optimization

**Severity:** Note  
**Description:** The final plugin binary (207KB) uses minimal wavetables to stay within disting NT plugin scanner limits. Full wavetable data would result in a 407KB plugin that exceeds scanner limits.

**Impact:** Wavetable synthesis modes are limited. Only bow/blow/strike exciter modes with modal resonator are available.

**Workaround:** None needed - the implemented modes provide full Elements functionality for modal synthesis.

**Planned Fix:** v1.1+ may implement runtime LUT generation to reduce binary size while supporting more modes.

### Issue 3: Build Warnings from Upstream Code

**Severity:** Low
**Description:** Build process produces harmless warnings from upstream Mutable Instruments Elements code:
- Unused parameter warnings in template functions
- Unused typedef warnings in static assertions
- Missing field initializer warnings in factory struct

**Impact:** None - these are expected warnings from the original Elements codebase and do not affect functionality.

**Workaround:** None needed.

**Planned Fix:** Not planned - these are in upstream code and do not indicate actual issues.

## Platform-Specific Notes

### disting NT Hardware
- Requires firmware v1.0 or later
- SD card must be FAT32 formatted
- Plugin appears in file browser after copying .o file to SD card
- CPU usage typically < 30% in normal operation

### Desktop Testing (VCV Rack)
- Requires VCV Rack v2.x with nt_emu module
- Desktop builds (.dylib on macOS) are for testing only, not for release
- Memory usage well within hardware limits (72-76KB total)

## Not Bugs

The following are expected behavior, not bugs:

### Monophonic Operation
nt_elements is monophonic by design (matching hardware Elements behavior). Multiple simultaneous MIDI notes are handled with last-note priority - the most recent note-on takes over synthesis.

### Sample Rate Mode Removed
The sample rate selection feature from Story 1.11 was removed in the final build. The plugin operates at 48kHz to match disting NT's native sample rate for optimal integration.

## Reporting Issues

If you encounter issues not listed here:

1. Check that you're using the correct firmware version
2. Verify SD card is FAT32 formatted
3. Ensure plugin file is not corrupted (207KB file size)
4. Try reloading the plugin
5. Open an issue on GitHub with:
   - disting NT firmware version
   - Steps to reproduce
   - Expected vs actual behavior

## Future Improvements

Items planned for future versions (not bugs):

- Extended stability testing (4+ hours) - planned manual testing
- Stress testing (rapid parameter changes, MIDI bursts) - planned manual testing
- A/B sound quality validation vs hardware Elements - planned manual testing
- Additional factory presets
- Performance optimizations for lower CPU usage
- Alternative exciter/resonator modes

---

Last updated: 2025-11-02
Version: v1.0.0
