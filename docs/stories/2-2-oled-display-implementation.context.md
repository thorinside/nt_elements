<story-context id="bmad/bmm/workflows/4-implementation/story-context/template" v="1.0">
  <metadata>
    <epicId>2</epicId>
    <storyId>2</storyId>
    <title>OLED Display Implementation</title>
    <status>ready-for-dev</status>
    <generatedAt>2025-10-26</generatedAt>
    <generator>BMAD Story Context Workflow</generator>
    <sourceStoryPath>docs/stories/2-2-oled-display-implementation.md</sourceStoryPath>
  </metadata>

  <story>
    <asA>a user</asA>
    <iWant>clear visual feedback on the OLED display</iWant>
    <soThat>I always know which page I'm on and current parameter values</soThat>
    <tasks>
      - Create OLED display header (AC: #1-4)
        - Create src/oled_display.h
        - Define display layout constants (positions, sizes)
        - Define font sizes for headers and values
        - Declare rendering functions
        - Document display architecture

      - Implement page title rendering (AC: #1)
        - Create render_page_title() function
        - Display current page name (EXCITER/RESONATOR/SPACE/PERF)
        - Use large/bold font for visibility
        - Position at top of display
        - Test all four page titles

      - Implement parameter name rendering (AC: #2)
        - Create render_parameter_names() function
        - Display names for active pots/encoders on current page
        - Use abbreviated names if needed (fit display width)
        - Position below page title
        - Test all parameter name displays

      - Implement parameter value rendering (AC: #2, #4)
        - Create render_parameter_values() function
        - Display numeric values aligned with parameter names
        - Format percentages (0-100%)
        - Format tuning values (+/-12 semitones, +/-50 cents)
        - Format descriptive text for geometry/mode parameters
        - Test all value formats

      - Implement display update logic (AC: #3)
        - Add draw() callback to NT_algorithm structure
        - Call rendering functions on parameter change
        - Call rendering functions on page change
        - Implement dirty flag to minimize redraws
        - Verify immediate updates (no lag)

      - Create display layouts for each page (AC: #1-2)
        - Page 1 layout: "EXCITER" + 5 parameter name/value pairs
        - Page 2 layout: "RESONATOR" + 5 parameter name/value pairs
        - Page 3 layout: "SPACE" + 3 parameter name/value pairs
        - Page 4 layout: "PERF" + 5 parameter name/value pairs
        - Test visual layout on each page

      - Implement value formatting helpers (AC: #4)
        - Format percentage: "50%"
        - Format semitones: "+2.0" or "-5.5"
        - Format cents: "+25c"
        - Format geometry: "String", "Tube", "Mixed"
        - Format mode: "32kHz", "48kHz"
        - Test all formatters

      - Optimize display performance (AC: #6)
        - Implement dirty flag (only redraw on change)
        - Minimize full-screen clears
        - Use partial updates when possible
        - Avoid expensive operations in draw()
        - Verify no flicker or artifacts

      - Test display readability (AC: #5)
        - View display in normal lighting
        - View display in dim lighting (Eurorack typical)
        - Verify contrast is sufficient
        - Check font sizes are readable from normal viewing distance
        - Adjust contrast/brightness if needed

      - Test display updates (AC: #3, #6)
        - Change parameters, verify immediate display update
        - Switch pages, verify display updates correctly
        - Rapid parameter changes, verify no flicker
        - Sustained operation, verify no display artifacts
        - Monitor for visual glitches

      - Desktop testing in nt_emu (AC: #1-6)
        - Load plugin in nt_emu
        - If nt_emu supports display emulation, test all criteria
        - If not, validate logic and defer visual testing to hardware
        - Document testing limitations

      - Hardware testing (AC: #1-6)
        - Deploy to disting NT hardware
        - Verify all display functionality on actual OLED
        - Test readability in Eurorack case
        - Verify no performance impact from display updates
        - Document final display validation
    </tasks>
  </story>

  <acceptanceCriteria>
    1. Page title displayed prominently (e.g., "EXCITER", "RESONATOR")
    2. Active parameter names and values shown for pots/encoders
    3. Display updates immediately when parameters change
    4. Value representation appropriate (percentages, descriptive text, numeric)
    5. Display remains readable in typical Eurorack lighting
    6. No flicker or visual artifacts during updates
  </acceptanceCriteria>

  <artifacts>
    <docs>
      <doc>
        <path>docs/PRD.md</path>
        <title>Product Requirements Document</title>
        <section>FR008: Display parameter values and page information on OLED screen</section>
        <snippet>Display parameter values and page information on OLED screen. OLED display always shows current page and parameter assignments, eliminating confusion about what controls do.</snippet>
      </doc>
      <doc>
        <path>docs/PRD.md</path>
        <title>Product Requirements Document</title>
        <section>User Interface Design Goals</section>
        <snippet>Hardware-based interface on disting NT with small OLED display (128x64 pixels typical). Display constraints: limited resolution (must fit information clearly), monochrome (no color coding), small physical size, must be readable from ~1 foot distance in dim lighting.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>UI/Display - OLED Strategy</section>
        <snippet>OLED display shows page name + parameter values. Display updates are event-driven (parameter change). Simple, clear, update on change only. No polling, no animations.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>ADR-006: Four-Page Parameter Organization</section>
        <snippet>Organize into 4 pages: Exciter, Resonator, Space, Performance. Logical grouping by synthesis function. 5 controls per page fits NT hardware perfectly. OLED must clearly show current page context.</snippet>
      </doc>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>Performance Considerations</section>
        <snippet>Display updates can consume 1-5% CPU. Minimize updates using dirty flag (only redraw on change), avoid animations or constant refreshing, keep draw() logic simple.</snippet>
      </doc>
      <doc>
        <path>docs/epics.md</path>
        <title>Epic Breakdown</title>
        <section>Story 2.2: OLED Display Implementation</section>
        <snippet>Clear visual feedback on OLED display. Page title displayed prominently, active parameter names and values shown, display updates immediately when parameters change, value representation appropriate, readable in typical Eurorack lighting, no flicker or visual artifacts.</snippet>
      </doc>
      <doc>
        <path>docs/stories/2-1-complete-parameter-page-system.md</path>
        <title>Story 2.1: Complete Parameter Page System</title>
        <section>OLED Display Integration</section>
        <snippet>Basic display updates show current page name and parameter values (simplified). Update on page change and parameter change. Full implementation deferred to Story 2.2.</snippet>
      </doc>
      <doc>
        <path>docs/research-technical-2025-10-26.md</path>
        <title>Technical Research Report</title>
        <section>distingNT Platform Analysis</section>
        <snippet>disting NT provides OLED display (128x64 pixels typical) for visual feedback. Display updates must be efficient to minimize CPU overhead. Platform provides drawing primitives through NT API.</snippet>
      </doc>
    </docs>
    <code>
      <artifact>
        <path>src/oled_display.h</path>
        <kind>header</kind>
        <symbol>N/A - to be created</symbol>
        <lines>N/A</lines>
        <reason>New header file to define OLED display rendering functions, layout constants, and display update interface for story implementation</reason>
      </artifact>
      <artifact>
        <path>src/oled_display.cpp</path>
        <kind>implementation</kind>
        <symbol>N/A - to be created</symbol>
        <lines>N/A</lines>
        <reason>New implementation file for OLED rendering functions including page title, parameter names/values, and formatting helpers</reason>
      </artifact>
      <artifact>
        <path>src/nt_elements.cpp</path>
        <kind>implementation</kind>
        <symbol>draw() callback</symbol>
        <lines>N/A - to be added</lines>
        <reason>Existing main plugin file that needs draw() callback implementation to handle OLED display updates. Will integrate with display rendering functions.</reason>
      </artifact>
      <artifact>
        <path>src/parameter_pages.h</path>
        <kind>header</kind>
        <symbol>Page definitions</symbol>
        <lines>N/A</lines>
        <reason>Existing file from Story 2.1 containing 4-page structure and parameter mappings. Display needs to reference page names and parameter labels.</reason>
      </artifact>
      <artifact>
        <path>src/parameter_adapter.h</path>
        <kind>header</kind>
        <symbol>Parameter enums</symbol>
        <lines>N/A</lines>
        <reason>Existing file containing parameter enumerations needed for identifying which parameters to display on each page</reason>
      </artifact>
    </code>
    <dependencies>
      <cpp>
        <package>distingNT_API</package>
        <version>latest submodule</version>
        <note>Provides NT_algorithm structure, display drawing primitives, and OLED interface callbacks</note>
      </cpp>
      <cpp>
        <package>C++ Standard Library</package>
        <version>C++11</version>
        <note>snprintf for string formatting, standard types</note>
      </cpp>
      <system>
        <package>ARM GCC Toolchain</package>
        <version>arm-none-eabi-gcc</version>
        <note>Cross-compiler for ARM Cortex-M7 with C++11 support</note>
      </system>
    </dependencies>
  </artifacts>

  <constraints>
    <constraint>Display resolution limited to 128x64 pixels (typical OLED) - must fit page title plus 5 parameter name/value pairs</constraint>
    <constraint>Monochrome display only - no color coding for visual differentiation</constraint>
    <constraint>Display updates must be event-driven (dirty flag pattern) to minimize CPU overhead (target &lt; 2% for display)</constraint>
    <constraint>Font sizes must balance readability with information density - large font for page title, smaller for parameters</constraint>
    <constraint>Must use NT API display drawing primitives - exact API to be determined from distingNT_API documentation</constraint>
    <constraint>Display must remain readable in dim Eurorack case lighting (typical performance environment)</constraint>
    <constraint>No animations or constant refreshing - only update when parameter or page changes</constraint>
    <constraint>Parameter names may need abbreviation to fit display width (e.g., "Brightness" → "Bright")</constraint>
    <constraint>Desktop testing with nt_emu may have limited display emulation - validate logic in code, defer visual testing to hardware if needed</constraint>
    <constraint>Follow Elements C++ coding style: snake_case for variables, camelCase for functions, k-prefix PascalCase for constants</constraint>
    <constraint>All memory allocation via NT_globals.getMemory() - no malloc/new allowed</constraint>
    <constraint>No exceptions allowed (-fno-exceptions compiler flag) - use defensive programming with bounds checking</constraint>
  </constraints>

  <interfaces>
    <interface>
      <name>draw() callback</name>
      <kind>function callback</kind>
      <signature>void draw(_NT_algorithm* self, /* display context */)</signature>
      <path>distingNT_API/include/distingNT.h</path>
      <note>Main OLED rendering callback provided by NT API. Called when display needs refresh. Must check dirty flag and render current page/parameters.</note>
    </interface>
    <interface>
      <name>NT_algorithm structure</name>
      <kind>C struct</kind>
      <signature>struct _NT_algorithm with display_dirty flag, current_page, parameter array</signature>
      <path>src/nt_elements.cpp</path>
      <note>Main plugin algorithm structure. Extended to include display_dirty flag for update tracking and current_page for page state.</note>
    </interface>
    <interface>
      <name>parameterChanged() callback</name>
      <kind>function callback</kind>
      <signature>void parameterChanged(_NT_algorithm* self, int p)</signature>
      <path>src/nt_elements.cpp</path>
      <note>Existing callback that handles parameter updates. Must set display_dirty flag when parameters change to trigger display refresh.</note>
    </interface>
    <interface>
      <name>NT Display API</name>
      <kind>C API</kind>
      <signature>NT_display_text(), NT_display_clear(), or similar drawing primitives</signature>
      <path>distingNT_API/include/distingNT.h</path>
      <note>Display drawing primitives provided by distingNT API. Exact API to be discovered from documentation. Likely includes text rendering, positioning, font selection.</note>
    </interface>
  </interfaces>

  <tests>
    <standards>
      Testing for this embedded audio plugin follows a desktop-first strategy with hardware validation. Desktop testing uses nt_emu VCV Rack module for rapid iteration. Hardware testing validates on actual disting NT for performance, display readability, and real-world usability. Display testing must verify: correct rendering (page titles, parameter names/values), immediate updates (no lag), readability (fonts, contrast, lighting), and performance (no flicker, &lt; 2% CPU overhead). Test both normal parameter changes and edge cases (rapid changes, page switching during updates). Visual validation requires hardware testing with actual OLED display in typical Eurorack case lighting conditions.
    </standards>
    <locations>
      No formal test framework for this project. Testing approach: Desktop iteration in nt_emu (plugins/nt_elements.dylib), hardware validation on disting NT (plugins/nt_elements.o). Display testing primarily hardware-based due to visual/readability requirements. Code logic can be validated on desktop, but final display appearance/readability must be verified on actual hardware OLED in Eurorack environment.
    </locations>
    <ideas>
      <test ac="1">Test page title rendering for all 4 pages (EXCITER, RESONATOR, SPACE, PERF). Verify large font, prominent positioning, correct text on each page.</test>
      <test ac="2">Test parameter name/value display for each page. Verify all 5 controls shown on pages 1,2,4 and 3 controls on page 3. Check alignment, abbreviations fit display.</test>
      <test ac="3">Test immediate display updates: change parameter values, verify display updates without delay. Switch pages, verify display updates immediately. Test dirty flag logic prevents unnecessary redraws.</test>
      <test ac="4">Test value formatting for all types: percentages (0-100%), tuning values (+/-12 semitones, +/-50 cents), descriptive text (geometry, mode). Verify format functions produce correct strings.</test>
      <test ac="5">Hardware test: Display readability in normal lighting and dim Eurorack case lighting. Verify contrast sufficient, fonts readable from 1 foot viewing distance. Adjust if needed.</test>
      <test ac="6">Test display performance: rapid parameter changes should not cause flicker. Sustained operation should show no visual artifacts. Monitor CPU impact (target &lt; 2%). Test dirty flag optimization prevents constant redraws.</test>
      <test edge-case="page-switching">Change parameters on page 1, switch to page 2, verify correct parameters shown. Parameter states should persist, only display changes.</test>
      <test edge-case="rapid-changes">Rapidly change multiple parameters, verify display updates smoothly without lag or glitches. Test at limits of human interaction speed.</test>
      <test edge-case="extreme-values">Test display with parameter values at 0%, 50%, 100%. Test tuning at extreme ranges (+12/-12 semitones). Verify formatting handles all cases.</test>
      <test edge-case="long-names">Test parameter names that might be too long for display width. Verify abbreviation strategy works, text doesn't overflow or corrupt display.</test>
    </ideas>
  </tests>
</story-context>
