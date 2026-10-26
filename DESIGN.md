---
name: MATH for the TI-84 Plus CE
description: Type a problem, read the answer, follow the worked steps, on a 320x240 calculator screen.
colors:
  teal: "#0B6E64"
  tint: "#DCEFEB"
  pale: "#EEF6F4"
  sub-rule: "#B9D9D3"
  paper: "#FBFAF6"
  ink: "#1D2433"
  secondary: "#5F6676"
  rule: "#D9DCD6"
  error: "#B02E3E"
typography:
  body:
    fontFamily: "DrSans (bitmap, fontlibc)"
    fontSize: "14px"
    fontWeight: 400
    lineHeight: "14px"
  label:
    fontFamily: "DrSans (bitmap, fontlibc)"
    fontSize: "10px"
    fontWeight: 400
    lineHeight: "10px"
rounded:
  none: "0px"
spacing:
  hair: "1px"
  gap: "2px"
  inset: "8px"
  sub-indent: "10px"
  footer: "14px"
  tab: "18px"
  row: "20px"
  strip: "20px"
components:
  title-strip:
    backgroundColor: "{colors.teal}"
    textColor: "{colors.paper}"
    typography: "{typography.body}"
    rounded: "{rounded.none}"
    height: "{spacing.strip}"
    padding: "0 8px"
  answer-box:
    backgroundColor: "{colors.tint}"
    textColor: "{colors.ink}"
    typography: "{typography.body}"
    rounded: "{rounded.none}"
    padding: "5px 8px 5px 10px"
  step-row:
    backgroundColor: "{colors.paper}"
    textColor: "{colors.ink}"
    typography: "{typography.body}"
    padding: "4px 0 3px 24px"
  step-row-selected:
    backgroundColor: "{colors.pale}"
    textColor: "{colors.ink}"
  step-tab:
    backgroundColor: "{colors.paper}"
    textColor: "{colors.teal}"
    typography: "{typography.label}"
    width: "{spacing.tab}"
  step-tab-selected:
    backgroundColor: "{colors.teal}"
    textColor: "{colors.paper}"
    typography: "{typography.label}"
    width: "{spacing.tab}"
  step-caption:
    textColor: "{colors.secondary}"
    typography: "{typography.label}"
  menu-panel:
    backgroundColor: "{colors.paper}"
    textColor: "{colors.secondary}"
    typography: "{typography.label}"
    rounded: "{rounded.none}"
  menu-row:
    backgroundColor: "{colors.paper}"
    textColor: "{colors.ink}"
    typography: "{typography.body}"
    height: "{spacing.row}"
    padding: "0 0 0 26px"
  menu-row-selected:
    backgroundColor: "{colors.pale}"
    textColor: "{colors.ink}"
  footer:
    backgroundColor: "{colors.paper}"
    textColor: "{colors.ink}"
    typography: "{typography.label}"
    height: "{spacing.footer}"
    padding: "0 6px"
  error-text:
    textColor: "{colors.error}"
    typography: "{typography.label}"
---

# Design System: MATH for the TI-84 Plus CE

## Overview

**Creative North Star: "The Worked Example"**

Every result is set like a worked example in a maths textbook: the problem sits in a teal strip at the top, the answer sits in a tinted box under it, and the method follows as numbered steps, each with a short caption above its line of maths. Any step that has smaller rewrites inside it opens in place to show them, indented behind a thin rule.

The screen is a sheet of warm paper with dark ink and one teal. Teal carries the structure (strip, step numbers, the answer box's left bar, the cursor, approximations, disclosure marks); everything else is ink, a muted secondary grey, or a 1 px rule. Hierarchy comes from two font sizes, colour and position, never from weight, boxes-in-boxes or shadow. The look rejects both the stock TI-OS look (grey bars, a full-width blue selection bar) and the phone-app look (a stack of rounded cards).

The platform sets hard limits: 320x240 pixels, an 8-bit palette, two bitmap fonts, keypad only. Density is moderate on purpose: the answer must read at a glance, and a step should fit on one line before any fallback kicks in.

**Key Characteristics:**
- Paper ground, ink text, one teal for structure.
- Two bitmap sizes, no bold.
- Square corners, 1 px rules, no shadows.
- One selection mark everywhere: the 18 px filled teal margin tab on a pale row.
- Maths drawn in 2D: real fractions, raised exponents, drawn radicals, integrals and stretched brackets.
- A footer that lists only the keys that act on the current screen.

## Colors

A warm paper and cool ink pair with a single deep teal and its two pale tints; red appears only for errors.

### Primary
- **Textbook Teal** (teal): the title strip and problem strip, step numbers, the filled tab on the selected step or menu row, the 3 px bar on the answer box's left edge, the text cursor, the outline of the empty slot the cursor is in, decimal approximations and their ≈ sign, the open/closed triangles after captions, and the "more below" triangle in the footer.
- **Answer Tint** (tint): the fill of the answer box only.
- **Selection Pale** (pale): the full-width fill behind the selected step and the selected menu row.
- **Sub-step Rule** (sub-rule): the 1 px vertical rule to the left of opened sub-steps.

### Neutral
- **Paper** (paper): the screen ground, menu and prompt panels, the footer, and text set on teal.
- **Ink** (ink): all maths, menu labels, and key names in the footer.
- **Secondary Grey** (secondary): step captions, panel titles, the "Type a problem" placeholder, the action words in the footer, the word "or" between solution branches, and the outline of an empty slot the cursor is not in.
- **Rule Grey** (rule): 1 px lines: the line under the input, the footer's top edge, and the frames of menu and interval panels.
- **Error Red** (error): the error message under the input or inside the interval prompt. Small font, nothing else red.

### Named Rules
**The One Teal Rule.** Teal is the only accent. Anything that marks structure, selection or a secondary reading of the answer is teal or one of its two tints; no second hue is added for emphasis.

**The Quiet Words Rule.** Words that frame the maths (captions, "or", footer actions, panel titles) are secondary grey; the maths itself is ink.

## Typography

**Body Font:** DrSans bitmap, 14 px line, 11 px ascent (fontlibc)
**Label Font:** DrSans bitmap, 10 px line, 8 px ascent (fontlibc)

**Character:** A plain, upright sans in two pixel sizes. Maths symbols (π, ≤, ±, ∞, →, ·, ÷, ×, the true minus sign, ≈ and the triangles) come from the font's own code page below 0x20 and in the high range, so every symbol shares the font's pixel grid.

### Hierarchy
- **Body** (regular, 14 px line): the problem in the strip, the answer, each step's maths, menu labels, the typed input, the editor title "Math".
- **Label** (regular, 10 px line): step captions, step numbers, panel titles, the footer, RAD/DEG and the 2nd/alpha indicator, the interval in the strip, approximations, error messages. Inside maths it sets exponents, integral and limit bounds, root indices and log bases.

### Named Rules
**The Two Sizes Rule.** There are exactly two sizes and one weight. A new kind of text takes one of the two sizes and is told apart by colour or position, not by a third size or bold.

**The Caption Above Rule.** A step's caption sits above its maths in the label size, secondary grey, the way a textbook writes the reason above the line.

## Layout

The screen is a fixed 320x240 canvas with an 8 px inset on both sides for panels, rules and the answer box.

- **Top:** a full-width teal strip, 20 px tall on the editor. On the steps screen it grows to fit the problem (20 to 60 px), dropping the problem to the label size when it would come within 70 px of the right edge. RAD/DEG is right-aligned 8 px from the edge; the 2nd/alpha indicator and the interval (lo ≤ x ≤ hi) sit to its left.
- **Bottom:** a 14 px footer with a 1 px rule on top. Hints start 6 px in; each key name is followed 3 px later by its action, and pairs are 9 px apart.
- **Steps screen:** the answer box starts 6 px under the strip; the step list starts 4 px under the answer box and is clipped above the footer. Steps sit to the right of an 18 px margin tab, text starting 6 px past it (x = 24). Sub-steps indent a further 10 px, with their rule 8 px past the tab. Inside a step: 4 px above the caption, 2 px between caption and maths, 2 px plus 3 px after the maths. The list scrolls so the selected step stays in view.
- **Editor:** the input baseline is at y = 92, 10 px from the left; a 1 px rule runs 8 px under the input's lowest point; an error sits 14 px under that rule.
- **Menus:** a framed panel centred in the space above the footer, rows 20 px tall, labels 26 px in from the panel's inner edge (past the tab plus 8 px).
- **Wide maths:** first falls back to the label size, then splits at "=" or "or" onto stacked lines, and only then pans with left/right on the selected step.

## Elevation & Depth

The system is flat. There are no shadows, gradients or overlapping layers apart from menus and the interval prompt, which are drawn over the editor as paper panels with a 1 px rule frame. Depth comes from fill alone: tint for the answer, pale for the selection, teal for the shell.

### Named Rules
**The Flat Paper Rule.** Nothing casts a shadow. A panel that sits over another screen is separated by its 1 px rule frame and nothing else.

## Shapes

All corners are square. Edges are filled rectangles and 1 px lines: the strip, the answer box with its 3 px teal left bar, the 18 px tab, the 1 px panel frame, the 1 px sub-step rule (inset 2 px top and bottom). Maths shapes are drawn with lines to match the content's height: a 1 px fraction bar 2 px clear of numerator and denominator, a radical with a hook and an overbar, an integral sign with curled ends, and brackets, bars and evaluation brackets that switch from font glyphs to drawn lines once the content is taller than a text line. An empty template slot is a small outlined rectangle (7 px wide in body, 5 px in label).

## Components

### Title Strip
The teal band at the top of every screen. Title or problem in paper at the body size, 8 px from the left; mode and shift indicators in paper at the label size on the right.

### Answer Box
The first thing read on the steps screen. Tint fill, 8 px inset each side, a 3 px teal bar down the left edge, maths in ink at the body size with 5 px above. A decimal approximation follows in teal at the label size with a ≈ sign, right-aligned on the answer's baseline; if it does not fit there, it moves to its own line inside the box. Several approximations are separated by commas.

### Step Row and Margin Tab
The signature. Each top-level step has a number in teal, label size, centred in the 18 px tab. The selected step fills its tab teal with the number in paper, and the whole row turns pale. A step with sub-steps has a teal triangle after its caption: pointing right when closed, down when open. Sub-steps have no number; they are body size like their parent, indented 10 px behind a 1 px sub-step rule.

### Menu Panel
Used for the action menu ("What to do", "Solve the equation") and the templates menu ("Insert a template"). A paper panel, 8 px inset, 1 px rule frame, the title in secondary at the label size, rows of body-size ink labels 20 px tall. The selected row is marked exactly like the selected step: pale row and 18 px filled teal tab. Up/down wrap around.

### Interval Prompt
A 70 px framed paper panel placed just under the input rule, so the equation stays visible above it. "Find the solutions in" in secondary at the label size, then lo ≤ x ≤ hi as an editable box at the body size, with errors in red at the bottom of the panel.

### Editor
The input line on paper, with a 1 px teal cursor the full height of the current row and a secondary placeholder when empty. Template slots show as outlined rectangles, teal where the cursor is.

### Footer
Key hints only. Key names in ink, actions in secondary, both at the label size. It lists only keys that do something on the current screen (enter is left out on a step with no sub-steps). On the steps screen a teal down-triangle at the right end means more steps below.

## Do's and Don'ts

### Do:
- **Do** mark every selection, in any list, with the pale row and the 18 px filled teal tab.
- **Do** set step captions above the maths, label size, secondary grey.
- **Do** show approximations in teal at the label size, after the exact answer.
- **Do** draw fractions, exponents, roots, integrals and limits in 2D, and stretch brackets with the content.
- **Do** list in the footer only the keys that act on the current screen.
- **Do** keep panels, boxes and rules on the 8 px side inset.

### Don't:
- **Don't** add a second accent colour, a third font size, or bold.
- **Don't** round corners or add shadows.
- **Don't** use a full-width solid selection bar or grey title bars (the stock TI-OS look).
- **Don't** stack results as separate rounded cards (the phone-app look).
- **Don't** list keys in the footer that do nothing on the screen shown.
