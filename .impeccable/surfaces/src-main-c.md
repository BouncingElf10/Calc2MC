---
version: 1
slug: "src-main-c"
primary_target: "src/main.c"
related_targets: []
---

# MATH: all screens (editor, action menu, interval prompt, steps)

Scope: every screen of the TI-84 Plus CE program, 320x240, keypad only. Visitor mode: Operate.
Task: type a problem, choose an action, read the answer, then follow the steps (expanding the ones you need).

## Direction contract

FORM: seed:0e66df3c8c7c9cb6, the chosen card was the model's pick "Worked Example" (IB maths textbook Example/Solution boxes). Reference sketch: .impeccable/mocks/decision/model-pick.html.

THESIS: every answer is set like a worked example in a maths textbook: the problem in a teal strip, the result in a tinted box, then numbered steps with their caption above the line. It refuses the TI-OS look (grey bars, a full-width blue selection) and the phone-app look (a stack of rounded cards).

OWN-WORLD: paper-white ground #fbfaf6, ink #1d2433, one teal #0b6e64 that carries the shell (top strip, step-number tabs, answer box rule), tint #dcefeb for the answer box, a pale teal #eef6f4 for the selected step, secondary text #6b7383, 1 px rules #d9dcd6. Two bitmap fonts: body around 11 px and small around 8 px (captions, exponents, bounds, footer). No rounded corners, no shadows.

STORY: the answer is the first thing you read, under the problem; the method reads top to bottom like a solution on paper, and any step opens to show the smaller rewrites inside it.

FIRST VIEWPORT: steps screen: teal strip (problem, interval, RAD/DEG), tinted answer box (exact answer, ≈ decimals on the right), numbered steps with the selected one's number tab filled teal, sub-steps indented behind a 1 px teal-tint rule, footer listing only this screen's keys.

SIGNATURE: the step-number margin tab: a narrow teal column that numbers steps and fills solid on the selected step, the same tab marks the selected row in every menu.

RISK: familiar by design; it has to be executed with exact spacing and type to beat the category standard.
