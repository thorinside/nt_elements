<story-context id="bmad/bmm/workflows/4-implementation/story-context/template" v="1.0">
  <metadata>
    <epicId>4</epicId>
    <storyId>9</storyId>
    <title>Add Emilie Gillet Attribution</title>
    <status>drafted</status>
    <generatedAt>2025-12-26</generatedAt>
    <generator>BMAD Story Context Workflow</generator>
    <sourceStoryPath>docs/stories/4-9-add-emilie-gillet-attribution.md</sourceStoryPath>
  </metadata>

  <story>
    <asA>user</asA>
    <iWant>to see "By Emilie Gillet" displayed on the OLED screen</iWant>
    <soThat>the original creator of Elements receives proper credit</soThat>
    <tasks>
      - Task 1: Locate display rendering code
        - Open src/oled_display.cpp
        - Find renderDisplay() or equivalent function
        - Identify where title/header is rendered
      - Task 2: Choose font and positioning
        - Review available NT OLED fonts
        - Select small font that is readable but subtle
        - Determine Y position below title
      - Task 3: Add attribution text
        - Add draw call for "By Emilie Gillet"
        - Position below main title
        - Use selected small font
      - Task 4: Ensure visibility on all pages
        - Verify attribution is part of common header section
        - Test all 4 parameter pages
      - Task 5: Verify readability
        - Test in nt_emu
        - Check text is legible
        - Verify no overlap with other UI elements
    </tasks>
  </story>

  <acceptanceCriteria>
    1. Text "By Emilie Gillet" displayed below the main title text
    2. Uses small/subtle font size (not competing with page title)
    3. Visible on all parameter pages
    4. Properly positioned and readable
  </acceptanceCriteria>

  <artifacts>
    <docs>
      <doc>
        <path>docs/epic-4-audio-quality-and-parameters.md</path>
        <title>Epic 4: Audio Quality and Missing Parameters</title>
        <section>Story 4.9: Add Emilie Gillet Attribution</section>
        <snippet>Emilie Gillet is the creator of Mutable Instruments and designed the original Elements module. This attribution honors her exceptional work in synthesis algorithm design.</snippet>
      </doc>
    </docs>
    <code>
      <file>
        <path>src/oled_display.cpp</path>
        <kind>display implementation</kind>
        <symbol>renderPageTitle(), renderDisplay()</symbol>
        <lines>59-74</lines>
        <reason>Page title rendering - add attribution line below title in common header</reason>
      </file>
      <file>
        <path>src/oled_display.h</path>
        <kind>header</kind>
        <symbol>layout constants</symbol>
        <lines>26-45</lines>
        <reason>Display layout constants - may need new Y position for attribution text</reason>
      </file>
    </code>
    <dependencies>
      <cpp>
        <package>distingNT_API</package>
        <version>v9</version>
        <purpose>NT OLED display functions (NT_drawText)</purpose>
      </cpp>
    </dependencies>
  </artifacts>

  <constraints>
    - Single file modification: src/oled_display.cpp
    - No parameter or logic changes
    - Add string constant for attribution text
    - Attribution should not compete visually with page title
    - Must be readable on 256x64 OLED display
    - Use small/tiny font (kNT_textTiny or kNT_textSmall)
  </constraints>

  <interfaces>
    <interface>
      <name>NT_drawText()</name>
      <kind>NT API function</kind>
      <signature>void NT_drawText(int x, int y, const char* text, int color, _NT_textAlignment align, _NT_textSize size)</signature>
      <path>distingNT_API/include/distingNT.h</path>
    </interface>
    <interface>
      <name>_NT_textSize enum</name>
      <kind>font size constants</kind>
      <signature>kNT_textTiny, kNT_textSmall, kNT_textNormal, kNT_textLarge</signature>
      <path>distingNT_API/include/distingNT.h</path>
    </interface>
  </interfaces>

  <tests>
    <standards>Desktop-first testing using VCV Rack with nt_emu module. Verify attribution displays correctly on all pages.</standards>
    <locations>
      - VCV Rack with nt_emu module (desktop testing)
      - distingNT hardware (final validation)
    </locations>
    <ideas>
      - AC1: Verify "By Emilie Gillet" text appears below title
      - AC2: Confirm font is smaller than page title, subtle but readable
      - AC3: Navigate through all 4 pages, verify attribution visible on each
      - AC4: Check text doesn't overlap with parameters, is properly positioned
    </ideas>
  </tests>
</story-context>
