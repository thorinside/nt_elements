<story-context id="bmad/bmm/workflows/4-implementation/story-context/template" v="1.0">
  <metadata>
    <epicId>4</epicId>
    <storyId>5</storyId>
    <title>Reorganize Parameter Pages (Manual-Aligned Layout)</title>
    <status>drafted</status>
    <generatedAt>2025-12-26</generatedAt>
    <generator>BMAD Story Context Workflow</generator>
    <sourceStoryPath>docs/stories/4-5-reorganize-parameter-pages.md</sourceStoryPath>
  </metadata>

  <story>
    <asA>user</asA>
    <iWant>parameter pages organized to match the original Elements layout</iWant>
    <soThat>I can access all sound design controls intuitively</soThat>
    <tasks>
      - Task 1: Document current page layout
        - List current page structure from pageExciter[], pageResonator[], etc.
        - Identify parameters to move
      - Task 2: Redesign Page 1 - Exciter
        - Pot 1: Contour, Pot 2: Bow, Pot 3: Blow
        - Encoder 1: Strike, Encoder 2: Strength
      - Task 3: Redesign Page 2 - Timbre
        - Pot 1: Bow Timbre, Pot 2: Blow Timbre, Pot 3: Strike Timbre
        - Encoder 1: Flow, Encoder 2: Mallet
      - Task 4: Redesign Page 3 - Resonator
        - Pot 1: Geometry, Pot 2: Brightness, Pot 3: Damping
        - Encoder 1: Position, Encoder 2: Space
      - Task 5: Redesign Page 4 - Tuning
        - Pot 1: Coarse, Pot 2: Fine, Pot 3: FM Amount
        - Encoder 1: Output Level, Encoder 2: Reserved
      - Task 6: Update parameter page arrays in nt_elements.cpp
      - Task 7: Update OLED display labels
      - Task 8: Verify routing via NT firmware
    </tasks>
  </story>

  <acceptanceCriteria>
    1. Page 1 (Exciter): Contour, Bow, Blow on pots; Strike, Strength on encoders
    2. Page 2 (Timbre): Bow Timbre, Blow Timbre, Strike Timbre on pots; Flow, Mallet on encoders
    3. Page 3 (Resonator): Geometry, Brightness, Damping on pots; Position, Space on encoders
    4. Page 4 (Tuning): Coarse, Fine, FM Amount on pots; Output Level on encoder
    5. OLED display shows page names and parameter labels matching manual terminology
    6. Contour prominently placed as first pot on Exciter page (matches hardware layout)
    7. Routing parameters (buses, MIDI, CV) remain accessible via NT firmware UI
  </acceptanceCriteria>

  <artifacts>
    <docs>
      <doc>
        <path>docs/architecture.md</path>
        <title>Decision Architecture</title>
        <section>ADR-006: Four-Page Parameter Organization</section>
        <snippet>Organize into 4 pages: Exciter, Resonator, Space, Performance. 5 controls per page fits NT hardware perfectly. Encoder press navigation (Enc1=prev, Enc2=next).</snippet>
      </doc>
      <doc>
        <path>docs/epic-4-audio-quality-and-parameters.md</path>
        <title>Epic 4: Audio Quality and Missing Parameters</title>
        <section>Page Organization (Aligned with Elements Manual)</section>
        <snippet>Page 1: Exciter (Contour, Bow, Blow, Strike, Strength). Page 2: Timbre (timbres + Flow/Mallet). Page 3: Resonator (Geometry, Brightness, Damping, Position, Space). Page 4: Tuning.</snippet>
      </doc>
    </docs>
    <code>
      <file>
        <path>src/nt_elements.cpp</path>
        <kind>main plugin implementation</kind>
        <symbol>pageExciter[], pageResonator[], pageSpace[], pagePerformance[]</symbol>
        <lines>104-118</lines>
        <reason>Current page parameter arrays - need complete restructure to match new layout</reason>
      </file>
      <file>
        <path>src/parameter_pages.h</path>
        <kind>header</kind>
        <symbol>PAGE_MAPPINGS, PageMapping</symbol>
        <reason>Defines page structure - may need updates for new page names and parameter counts</reason>
      </file>
      <file>
        <path>src/oled_display.cpp</path>
        <kind>display implementation</kind>
        <symbol>PARAM_NAMES[], renderPageTitle()</symbol>
        <lines>16-57, 59-74</lines>
        <reason>Parameter name display strings and page title rendering - update for new layout</reason>
      </file>
      <file>
        <path>src/oled_display.h</path>
        <kind>header</kind>
        <symbol>oled_display namespace</symbol>
        <lines>24-121</lines>
        <reason>Display layout constants and function declarations</reason>
      </file>
    </code>
    <dependencies>
      <cpp>
        <package>distingNT_API</package>
        <version>v9</version>
        <purpose>NT OLED display functions, page navigation</purpose>
      </cpp>
    </dependencies>
  </artifacts>

  <constraints>
    - Prerequisite: Stories 4.2, 4.3, 4.4 must be complete (Flow, Mallet, Strength parameters must exist)
    - 4 custom pages matching original Elements hardware panel layout
    - Pots (3) + Encoders (2) = 5 controls per page
    - NT firmware UI handles routing parameters (buses, MIDI, CV) - not on custom pages
    - OLED 256x64 display with limited text width
    - Parameter name abbreviations may be needed for display fit
  </constraints>

  <interfaces>
    <interface>
      <name>PageMapping struct</name>
      <kind>data structure</kind>
      <signature>struct PageMapping { const char* name; const uint8_t* params; int count; }</signature>
      <path>src/parameter_pages.h</path>
    </interface>
    <interface>
      <name>NT_drawText()</name>
      <kind>NT API function</kind>
      <signature>void NT_drawText(int x, int y, const char* text, int color, _NT_textAlignment align, _NT_textSize size)</signature>
      <path>distingNT_API/include/distingNT.h</path>
    </interface>
  </interfaces>

  <tests>
    <standards>Desktop-first testing using VCV Rack with nt_emu module. Navigate through all pages, verify parameter assignments, test encoder navigation.</standards>
    <locations>
      - VCV Rack with nt_emu module (desktop testing)
      - distingNT hardware (final validation)
    </locations>
    <ideas>
      - AC1-4: Navigate to each page, verify correct parameters appear in correct pot/encoder positions
      - AC5: Verify page titles match "EXCITER", "TIMBRE", "RESONATOR", "TUNING"
      - AC6: Verify Contour is first pot on Exciter page
      - AC7: Access NT firmware settings, verify routing parameters still accessible
    </ideas>
  </tests>
</story-context>
