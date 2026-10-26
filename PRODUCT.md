# Product

<!-- impeccable:product-schema 1 -->

## Platform

ti84-ce (native C program for the TI-84 Plus CE; not web, iOS or Android)

## Users
One IB student, the owner, using it for homework and revision. Not used in exams.

## Product Purpose
Type a math problem on the calculator and get the answer plus the worked steps to it, in the same way Photomath works but on the calculator. It covers algebra, trig and calculus (derivatives, integrals, limits). The final answer has to be readable at a glance, and the steps have to be easy to follow, because it is used both to check answers and to learn methods.

## Positioning
It runs on the calculator the student already has on the desk: offline, no phone, input with real math notation (fractions, exponents, integrals) and output drawn in 2D.

## Operating Context
Short sessions: type one problem, read the steps, maybe expand one, then quit with [clear]. The user knows the keys and doesn't need onboarding, but a short footer with the keys that matter on the current screen is wanted.

## Capabilities and Constraints
- 320×240 screen, 8-bit palette through graphx, keypad only (no touch), eZ80 CPU at 48 MHz.
- Program size is capped at about 64 KB per variable by the TI-OS, and static memory plus heap at 60 KB.
- Text is bitmap fonts compiled into the program with fontlibc: one body size and one small size.
- Keys: [clear] quits from any screen, [y=] goes back, [del] deletes, 2nd [del] clears the input, [sto→] types =, alpha [X,T,θ,n] inserts a fraction box, [math] opens templates, [mode] switches RAD/DEG.
- An answer is exact first, followed by a decimal approximation. Problems with no symbolic method get a numeric answer and no steps.

## Product Principles
- Answer first, method one keypress away: the answer is the first thing seen, and the steps read top to bottom like a worked solution on paper.
- Typing never surprises: nothing that is typed changes what is already on screen.
- Every screen says which keys do something on it, and nothing else.
- Readability beats density on a 320×240 screen: no tiny text, and no steps that need sideways scrolling to read.
