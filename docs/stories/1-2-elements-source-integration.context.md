<story-context id="1-2-elements-source-integration" v="1.0">
  <metadata>
    <epicId>1</epicId>
    <storyId>2</storyId>
    <title>Elements Source Integration</title>
    <status>drafted</status>
    <generatedAt>2025-10-26</generatedAt>
    <generator>BMAD Story Context Workflow</generator>
    <sourceStoryPath>docs/stories/1-2-elements-source-integration.md</sourceStoryPath>
  </metadata>

  <story>
    <asA>developer</asA>
    <iWant>Elements DSP source code integrated into the project structure</iWant>
    <soThat>I can access and compile the synthesis algorithms</soThat>
    <tasks>
      - Verify Elements submodule structure (AC: #1, #2)
        - Confirm external/mutable-instruments/elements/dsp/ exists
        - Document Elements source version/commit hash
        - Verify all required DSP files present (part.cc, voice.cc, exciter.cc, resonator.cc, string.cc, tube.cc, multistage_envelope.cc, ominous_voice.cc)
      - Create stmlib stub implementations (AC: #3)
        - Create src/stmlib_stubs/ directory
        - Implement dsp.h stubs (DSP utility functions used by Elements)
        - Implement utils.h stubs (utility functions)
        - Add stub directory to include paths in Makefile
      - Update Makefile with Elements source files (AC: #3, #5)
        - Add Elements .cc files to SOURCES list
        - Add include paths: `-I./external/mutable-instruments`
        - Add stmlib stubs path: `-I./src/stmlib_stubs`
        - Maintain separation between src/ (adapter) and external/ (Elements)
      - Test compilation with Elements source (AC: #3)
        - Run `make test` and verify desktop build succeeds
        - Run `make hardware` and verify ARM build succeeds
        - Check for any compilation warnings or errors
        - Document any build issues and resolutions
      - Verify MIT license compliance (AC: #4)
        - Confirm MIT license header present in all Elements source files
        - Add Mutable Instruments attribution to README
        - Create LICENSE file with MIT text and Elements attribution
        - Document license compliance approach
      - Create .gitignore for build artifacts (AC: #5)
        - Add build/ directory to .gitignore
        - Add plugins/*.dylib and plugins/*.so (keep plugins/ directory)
        - Add .DS_Store and other platform-specific files
        - Ensure submodules are NOT ignored
    </tasks>
  </story>

  <acceptanceCriteria>
    1. Elements repository cloned as git submodule at correct version
    2. Elements DSP directory structure (dsp/) copied into project
    3. Project builds with Elements source files included
    4. MIT license notices preserved in all Elements files
    5. Build configuration separates Elements code from adapter code
  </acceptanceCriteria>

  <artifacts>
    <docs>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>ADR-001: Use Header-Only Integration for Elements DSP</section>
        <snippet>Header-only integration (compile Elements .cc files directly with plugin). Enables link-time optimization across adapter/Elements boundary, simplifies debugging in nt_emu, allows flexible sample rate mode implementation, and preserves original Elements source as submodule.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Project Structure</section>
        <snippet>Elements source remains in git submodule at external/mutable-instruments/elements/dsp/ - read-only, never modified. Clear separation enables easy upstream updates, no merge conflicts with Elements development, and clear ownership boundaries.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Build Pattern</section>
        <snippet>Makefile SOURCES section explicitly lists all Elements .cc files: part.cc, voice.cc, exciter.cc, resonator.cc, string.cc, tube.cc, multistage_envelope.cc, ominous_voice.cc. Include paths allow Elements code to find its own headers AND our stubs.</snippet>
      </doc>
      <doc>
        <path>docs/PRD.md</path>
        <title>Product Requirements Document</title>
        <section>Functional Requirements FR001-FR005</section>
        <snippet>FR001: Port complete Elements DSP engine including all three exciter modes. FR002: Implement full 64-filter modal resonator. FR003: Include string and tube modeling algorithms. FR004: Integrate multistage envelope generators. FR005: Implement reverb section.</snippet>
      </doc>
      <doc>
        <path>docs/epics.md</path>
        <title>Epic Breakdown</title>
        <section>Story 1.2: Elements Source Integration</section>
        <snippet>Elements repository cloned as git submodule at correct version. Elements DSP directory structure (dsp/) copied into project. Project builds with Elements source files included. MIT license notices preserved. Build configuration separates Elements code from adapter code.</snippet>
      </doc>
      <doc>
        <path>docs/stories/1-2-elements-source-integration.md</path>
        <title>Story 1.2 Details</title>
        <section>Dev Notes - Elements DSP Architecture</section>
        <snippet>Elements consists of 8 primary DSP source files compiled directly with the adapter using header-only integration pattern (ADR-001). All files are in external/mutable-instruments/elements/dsp/.</snippet>
      </doc>
      <doc>
        <path>docs/stories/1-2-elements-source-integration.md</path>
        <title>Story 1.2 Details</title>
        <section>Dev Notes - stmlib Dependencies</section>
        <snippet>Elements depends on Mutable Instruments' stmlib utility library. We provide minimal stubs for required functions: DSP utilities (interpolation, clamping, soft-clipping), Math utilities (fast approximations), Utility functions (ONE_POLE macro, buffer operations). Stubs implement only what Elements actually uses.</snippet>
      </doc>
      <doc>
        <path>docs/stories/1-2-elements-source-integration.md</path>
        <title>Story 1.2 Details</title>
        <section>Dev Notes - License Compliance Strategy</section>
        <snippet>Elements is MIT licensed, allowing this port with proper attribution. Preserve all MIT headers in Elements source files (don't modify submodule), add clear attribution in README, include MIT license text in project LICENSE file, credit Émilie Gillet (Elements author). This is a derivative work which is explicitly permitted by MIT license.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Project Initialization</section>
        <snippet>Foundation commands include: git init, git submodule add for distingNT_API and eurorack repos. Architecture pattern: Clean Adapter Layer with Elements Source as git submodule at external/mutable-instruments/elements/dsp/ preserved as-is, read-only.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Technology Stack Details - DSP Source</section>
        <snippet>Mutable Instruments Elements - Modal synthesis engine. Repository: https://github.com/pichenettes/eurorack. License: MIT. Version: Git submodule (fixed commit). Native sample rate: 32kHz.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Code Organization</section>
        <snippet>Elements DSP: NEVER modify files in external/mutable-instruments/. Read-only submodule. All customization in adapter layer. File naming: snake_case for filenames like parameter_adapter.h, oled_display.h.</snippet>
      </doc>
    </docs>

    <code>
      <artifact>
        <path>external/mutable-instruments/elements/dsp/</path>
        <kind>submodule</kind>
        <symbol>Elements DSP files (part.cc, voice.cc, exciter.cc, resonator.cc, string.cc, tube.cc, multistage_envelope.cc, ominous_voice.cc)</symbol>
        <lines>N/A - will be added as submodule</lines>
        <reason>Core Elements synthesis engine source files that need to be integrated as git submodule and compiled with the plugin</reason>
      </artifact>
      <artifact>
        <path>Makefile</path>
        <kind>build system</kind>
        <symbol>SOURCES, INCLUDES</symbol>
        <lines>N/A - will be created</lines>
        <reason>Build configuration that needs to be updated to include Elements .cc files and proper include paths for both Elements headers and stmlib stubs</reason>
      </artifact>
      <artifact>
        <path>src/stmlib_stubs/dsp.h</path>
        <kind>header stub</kind>
        <symbol>DSP utility functions</symbol>
        <lines>N/A - will be created</lines>
        <reason>Stub implementations for stmlib DSP utilities that Elements depends on (interpolation, clamping, soft-clipping)</reason>
      </artifact>
      <artifact>
        <path>src/stmlib_stubs/utils.h</path>
        <kind>header stub</kind>
        <symbol>Utility functions</symbol>
        <lines>N/A - will be created</lines>
        <reason>Stub implementations for stmlib utility functions that Elements depends on (ONE_POLE macro, buffer operations, math utilities)</reason>
      </artifact>
      <artifact>
        <path>.gitignore</path>
        <kind>configuration</kind>
        <symbol>Build artifacts exclusions</symbol>
        <lines>N/A - will be created</lines>
        <reason>Prevent build artifacts from being committed while ensuring submodules are tracked properly</reason>
      </artifact>
      <artifact>
        <path>LICENSE</path>
        <kind>legal</kind>
        <symbol>MIT license text</symbol>
        <lines>N/A - will be created</lines>
        <reason>Required for MIT license compliance - must include Elements attribution and Émilie Gillet credit</reason>
      </artifact>
      <artifact>
        <path>README.md</path>
        <kind>documentation</kind>
        <symbol>Mutable Instruments attribution</symbol>
        <lines>N/A - will be created or updated</lines>
        <reason>Must clearly attribute "Based on Mutable Instruments Elements" for license compliance</reason>
      </artifact>
    </code>

    <dependencies>
      <toolchain>
        <tool>arm-none-eabi-gcc</tool>
        <version>latest</version>
        <purpose>ARM cross-compiler for Cortex-M7 (hardware builds)</purpose>
      </toolchain>
      <toolchain>
        <tool>git</tool>
        <version>2.x+</version>
        <purpose>Version control and submodule management</purpose>
      </toolchain>
      <toolchain>
        <tool>make</tool>
        <version>GNU Make</version>
        <purpose>Build system</purpose>
      </toolchain>
      <repositories>
        <repo>
          <name>mutable-instruments/eurorack</name>
          <url>https://github.com/pichenettes/eurorack</url>
          <path>external/mutable-instruments</path>
          <type>git submodule</type>
          <license>MIT</license>
        </repo>
        <repo>
          <name>distingNT_API</name>
          <url>https://github.com/expertsleepersltd/distingNT_API.git</url>
          <path>distingNT_API</path>
          <type>git submodule</type>
          <notes>Required from Story 1.1</notes>
        </repo>
      </repositories>
      <compiler_flags>
        <flag>-std=c++11</flag>
        <flag>-mcpu=cortex-m7</flag>
        <flag>-mfpu=fpv5-d16</flag>
        <flag>-mfloat-abi=hard</flag>
        <flag>-Os</flag>
        <flag>-fno-rtti</flag>
        <flag>-fno-exceptions</flag>
      </compiler_flags>
    </dependencies>
  </artifacts>

  <constraints>
    <constraint>NEVER modify files in external/mutable-instruments/ - this is a read-only git submodule. All customization must be in the adapter layer.</constraint>
    <constraint>Follow header-only integration pattern (ADR-001) - compile Elements .cc files directly with the plugin, do not create static libraries.</constraint>
    <constraint>Maintain clear separation between src/ (adapter code) and external/ (Elements source). This enables easy upstream updates and prevents merge conflicts.</constraint>
    <constraint>Preserve all MIT license headers in Elements source files exactly as-is. Do not modify or remove license notices from submodule files.</constraint>
    <constraint>Use snake_case for all new filenames (e.g., parameter_adapter.h, oled_display.h) following project convention.</constraint>
    <constraint>Include paths must support both Elements finding its own headers AND the project finding stmlib stubs: -I./external/mutable-instruments and -I./src/stmlib_stubs</constraint>
    <constraint>Build system must support dual targets: hardware (.o) and test (.dylib/.so) builds with same source files.</constraint>
    <constraint>All 8 Elements DSP .cc files must be explicitly listed in Makefile SOURCES: part.cc, voice.cc, exciter.cc, resonator.cc, string.cc, tube.cc, multistage_envelope.cc, ominous_voice.cc</constraint>
    <constraint>stmlib stubs should implement ONLY what Elements actually uses - not the full stmlib API. Keep stubs minimal.</constraint>
    <constraint>.gitignore must exclude build artifacts (build/, plugins/*.dylib, plugins/*.so) but NOT exclude submodule directories or the plugins/ directory itself.</constraint>
  </constraints>

  <interfaces>
    <interface>
      <name>Elements DSP Source Files</name>
      <kind>C++ source files</kind>
      <signature>8 .cc files in external/mutable-instruments/elements/dsp/:
- part.cc/h: Main synthesis engine coordinator
- voice.cc/h: Voice management and note control
- exciter.cc/h: Bow/blow/strike excitation sources
- resonator.cc/h: 64-filter modal resonator
- string.cc/h: String resonator modeling
- tube.cc/h: Tube resonator modeling
- multistage_envelope.cc/h: Envelope generators
- ominous_voice.cc/h: Alternative voice mode</signature>
      <path>external/mutable-instruments/elements/dsp/</path>
    </interface>
    <interface>
      <name>stmlib DSP Utilities</name>
      <kind>header stubs</kind>
      <signature>src/stmlib_stubs/dsp.h:
- Interpolation functions (Crossfade, Interpolate)
- Clamping utilities (Clip, Clamp)
- Soft clipping functions
- ONE_POLE macro
Expected by Elements DSP code but not provided by distingNT API</signature>
      <path>src/stmlib_stubs/dsp.h</path>
    </interface>
    <interface>
      <name>stmlib Utility Functions</name>
      <kind>header stubs</kind>
      <signature>src/stmlib_stubs/utils.h:
- Buffer operations
- Fast math approximations (exp, log)
- Utility macros
Expected by Elements DSP code but not provided by distingNT API</signature>
      <path>src/stmlib_stubs/utils.h</path>
    </interface>
    <interface>
      <name>Makefile Build Configuration</name>
      <kind>GNU Makefile</kind>
      <signature>SOURCES = src/nt_elements.cpp \
          external/mutable-instruments/elements/dsp/part.cc \
          external/mutable-instruments/elements/dsp/voice.cc \
          [... all 8 .cc files]
INCLUDES = -I./src \
           -I./distingNT_API/include \
           -I./external/mutable-instruments \
           -I./src/stmlib_stubs
Targets: hardware (ARM .o), test (native .dylib/.so)</signature>
      <path>Makefile</path>
    </interface>
    <interface>
      <name>Git Submodule Configuration</name>
      <kind>git configuration</kind>
      <signature>[submodule "external/mutable-instruments"]
    path = external/mutable-instruments
    url = https://github.com/pichenettes/eurorack
[submodule "distingNT_API"]
    path = distingNT_API
    url = https://github.com/expertsleepersltd/distingNT_API.git</signature>
      <path>.gitmodules</path>
    </interface>
  </interfaces>

  <tests>
    <standards>This is a foundation story establishing the build system and source integration. Testing focuses on compilation success and build artifact generation rather than runtime behavior. Primary validation is: (1) Desktop build succeeds with `make test` producing plugins/nt_elements.dylib, (2) Hardware build succeeds with `make hardware` producing plugins/nt_elements.o, (3) No compilation warnings or errors, (4) Git submodule structure is correct and trackable, (5) License compliance is verifiable through file headers and LICENSE file. Runtime testing (loading in nt_emu or on hardware) is deferred to Story 1.3 (Minimal Plugin Stub).</standards>

    <locations>
      - Build artifacts: plugins/ directory (created by build system)
      - Submodule verification: external/mutable-instruments/elements/dsp/
      - License verification: LICENSE file, README.md, Elements source headers
      - Build configuration: Makefile, .gitignore
    </locations>

    <ideas>
      <test id="AC1" criteria="Elements repository cloned as git submodule at correct version">
        - Verify git submodule status shows correct path and commit hash
        - Confirm .gitmodules file contains correct repository URL
        - Test: git submodule status | grep external/mutable-instruments
      </test>
      <test id="AC2" criteria="Elements DSP directory structure exists">
        - Verify external/mutable-instruments/elements/dsp/ directory exists
        - Confirm all 8 required .cc/.h files are present: part, voice, exciter, resonator, string, tube, multistage_envelope, ominous_voice
        - Test: ls external/mutable-instruments/elements/dsp/*.cc | wc -l (expect 8)
      </test>
      <test id="AC3" criteria="Project builds with Elements source files included">
        - Run make test and verify successful desktop build (.dylib/.so output)
        - Run make hardware and verify successful ARM build (.o output)
        - Check for zero compilation errors and warnings
        - Verify stmlib stub headers compile correctly
        - Test both clean build and incremental rebuild
      </test>
      <test id="AC4" criteria="MIT license notices preserved in all Elements files">
        - Verify MIT license header present in Elements source files (grep for "MIT" or "Copyright")
        - Confirm LICENSE file exists with full MIT text and Mutable Instruments attribution
        - Verify README contains "Based on Mutable Instruments Elements" attribution
        - Verify Émilie Gillet credited as original author
      </test>
      <test id="AC5" criteria="Build configuration separates Elements code from adapter code">
        - Verify Makefile SOURCES lists Elements files with external/ prefix
        - Verify adapter code listed with src/ prefix
        - Confirm include paths maintain separation: -I./external/mutable-instruments and -I./src
        - Verify .gitignore excludes build/ and plugins/*.dylib but NOT external/ or distingNT_API/
        - Test that git status shows submodules as clean (no untracked files in submodule directories)
      </test>
    </ideas>
  </tests>
</story-context>
