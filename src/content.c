#include <stddef.h>
#include "content.h"

/* IB Physics HL: definitions in words, no equations. The first six pages
 * are the electrostatics and electric currents test; quoted definitions
 * there are the teacher's exact wording. */

/* ---- test unit: electrostatics and electric currents ---- */

static const char *const charge[] = {
    "!Electrostatics mirrors gravitation: swap mass for charge. The big "
    "difference is that charge comes in two signs.",
    "#Electric charge",
    "@Electric charge",
    "A property of matter that makes it experience a force in an electric "
    "field. It comes in two kinds, positive and negative: like charges repel, "
    "unlike charges attract.",
    "@Elementary charge",
    "The smallest amount of free charge: the size of the charge on one proton "
    "or one electron, about 1.6 times ten to the minus 19 coulombs.",
    "@Quantization of charge",
    "Charge only ever comes in whole-number multiples of the elementary "
    "charge. Millikan's oil-drop experiment is the evidence.",
    "@Conservation of charge",
    "The total charge of an isolated system stays constant. Charge can be "
    "moved from one object to another but never created or destroyed.",
    "@Coulomb",
    "The SI unit of charge: the charge that passes a point in one second when "
    "the current is one ampere.",
    "@Point charge",
    "A charge whose size is negligible compared with the distances involved. "
    "Outside a charged sphere, the field is the same as if all its charge "
    "were a point charge at the centre.",
    "#Coulomb's law",
    "@Coulomb's law",
    "The electrostatic force between two point charges is directly "
    "proportional to the product of the charges and inversely proportional "
    "to the square of their separation.",
    "!\"Point charges\" and \"square of the separation\" are both marks. The "
    "force acts along the line joining the two charges.",
    "@Electrostatic force",
    "The force between charges at rest. Each charge feels an equal and "
    "opposite force (a Newton's third law pair): attractive for unlike "
    "charges, repulsive for like charges.",
    "@Inverse-square law",
    "A quantity that falls with one over the distance squared: double the "
    "separation and the force drops to a quarter; triple it and it drops to "
    "a ninth.",
    "@Coulomb constant",
    "The constant of proportionality in Coulomb's law. It equals one divided "
    "by four pi times the permittivity of free space.",
    "@Permittivity",
    "A property of a medium that sets how strongly charges in it interact. "
    "The larger the permittivity, the weaker the force between two given "
    "charges.",
    "@Permittivity of free space",
    "The permittivity of a vacuum, a fundamental constant. Air has almost "
    "exactly the same value.",
    "#Methods of charging",
    "!Only electrons move. Protons are locked in nuclei, so an object becomes "
    "positive by losing electrons, never by gaining protons.",
    "@Charging by friction",
    "Rubbing two different insulators together transfers electrons from one "
    "surface to the other. The one that gains electrons becomes negative; "
    "the other is left equally positive.",
    "@Charging by contact",
    "Touching a charged object to a conductor lets charge flow between them, "
    "so both end up with charge of the same sign. Also called charging by "
    "conduction.",
    "@Charging by induction",
    "Bring a charged object near a neutral conductor without touching it, so "
    "the conductor's free electrons are pushed to the far side or pulled to "
    "the near side. Earth the conductor while the object is still near, "
    "break the earth connection, then take the object away. The conductor is "
    "left with charge opposite in sign to the charged object.",
    "!Order matters in induction: remove the earth connection BEFORE the "
    "charged object, or the charge just flows back.",
    "@Polarization of charge",
    "The separation of positive and negative charge within a neutral object "
    "caused by a nearby charge. The near side gains the opposite sign, which "
    "is why a charged object attracts a neutral one.",
    "@Earthing",
    "Connecting a conductor to the Earth, a huge reservoir of charge at zero "
    "potential, so electrons flow to or from it until the conductor is "
    "neutral.",
    NULL
};

static const char *const fields[] = {
    "!The field strength and potential definitions are your teacher's exact "
    "words. Learn them word for word.",
    "#Electric field",
    "@Electric field",
    "A region of space in which a charge experiences an electric force.",
    "@Electric field strength",
    "The force per unit charge acting on a small positive test charge placed "
    "at that point.",
    "A vector, pointing the way a positive charge would be pushed. Unit: "
    "newtons per coulomb, which is the same as volts per metre.",
    "@Test charge",
    "A charge small enough not to disturb the field it is placed in, and "
    "positive by convention, so the field's direction is the direction of "
    "the force on it.",
    "@Field of a point charge",
    "Directly proportional to the charge and inversely proportional to the "
    "square of the distance from it. It points radially away from a positive "
    "charge and towards a negative one.",
    "@Force on a charge in a field",
    "Proportional to both the charge and the field strength. A positive "
    "charge is pushed along the field direction; a negative charge, such as "
    "an electron, is pushed against it.",
    "@Superposition of fields",
    "The resultant field at a point is the vector sum of the fields from "
    "each charge. Draw each field as an arrow, then add them by components, "
    "taking direction into account.",
    "@Neutral point",
    "A point where the fields from two or more charges cancel, so the "
    "resultant field strength is zero. For two like charges it lies between "
    "them, nearer the smaller charge; for two unlike charges it lies outside "
    "them, beyond the smaller one.",
    "#Electric potential",
    "@Electric potential",
    "The work done per unit charge in bringing a small positive test charge "
    "from infinity to the point.",
    "A scalar, measured in volts (joules per coulomb), and zero at infinity. "
    "Positive around a positive charge, negative around a negative charge.",
    "@Potential of a point charge",
    "Directly proportional to the charge and inversely proportional to the "
    "distance from it: one over the distance, not the distance squared.",
    "@Adding potentials",
    "Potential is a scalar, so the potentials from several charges simply "
    "add, each with its sign. No components, no angles: much easier than "
    "adding fields.",
    "@Electric potential energy",
    "The work done in bringing a charge from infinity to a point in the "
    "field of another charge. It belongs to the pair of charges: positive "
    "when they repel, negative when they attract.",
    "@Potential difference in a field",
    "The work done per unit charge in moving a small positive test charge "
    "between two points in the field.",
    "@Electronvolt",
    "The energy gained by one electron when it is accelerated through a "
    "potential difference of one volt; about 1.6 times ten to the minus 19 "
    "joules.",
    "#Field and potential together",
    "@Potential gradient",
    "The rate of change of potential with distance. The electric field "
    "strength at a point equals minus the potential gradient there, so the "
    "field points the way potential falls fastest.",
    "@Equipotential surface",
    "A surface on which every point has the same potential. No work is done "
    "moving a charge along it, and it always meets field lines at right "
    "angles.",
    "@Work done moving a charge",
    "The charge multiplied by the potential difference between the start and "
    "end points. It depends only on those two points, not on the path taken, "
    "and is zero along an equipotential.",
    "@Charge released in a field",
    "Its electric potential energy is converted to kinetic energy. The "
    "kinetic energy gained equals the work done by the field: the charge "
    "times the potential difference it moves through.",
    "!Positive charges \"fall\" from high to low potential, like masses in "
    "gravity. Electrons do the opposite: they accelerate towards higher "
    "potential.",
    NULL
};

static const char *const plates[] = {
    "#Parallel plates",
    "@Uniform electric field",
    "A field whose strength and direction are the same at every point. Its "
    "field lines are straight, parallel and equally spaced.",
    "@Field between parallel plates",
    "Uniform between the plates: the same strength everywhere, directly "
    "proportional to the potential difference across the plates and "
    "inversely proportional to their separation.",
    "@Field lines between plates",
    "Straight, parallel and evenly spaced, running from the positive plate "
    "to the negative plate and meeting both at right angles. At the ends of "
    "the plates they bulge outwards.",
    "@Equipotentials between plates",
    "Flat planes parallel to the plates, equally spaced for equal steps of "
    "potential. The potential falls steadily from the positive plate to the "
    "negative plate.",
    "@Edge effect",
    "Near the ends of the plates the field lines curve outwards and spread "
    "apart, so the field there is weaker and no longer uniform.",
    "@Charged particle between plates",
    "Feels the same force everywhere between the plates, so it has a "
    "constant acceleration. Fired in at right angles to the field it follows "
    "a parabola, like a projectile in a gravitational field.",
    "#Millikan's oil-drop experiment",
    "@Aim of Millikan's experiment",
    "To measure the charge on individual oil drops. It showed that charge is "
    "quantized and gave the value of the elementary charge.",
    "@Millikan's method",
    "A fine mist of oil drops is sprayed between two horizontal parallel "
    "plates; the drops pick up charge by friction in the sprayer. Watching "
    "through a microscope, the p.d. across the plates is adjusted until a "
    "chosen drop hangs still.",
    "@Balanced drop",
    "When the drop is stationary, the upward electric force exactly balances "
    "its weight. Knowing the p.d., the plate separation and the drop's mass "
    "then gives its charge.",
    "@Finding the drop's mass",
    "Switch the field off and the drop falls at terminal velocity, where air "
    "resistance balances its weight. The terminal speed gives its radius, "
    "and the oil's density then gives its mass.",
    "@Millikan's result",
    "Every drop's charge was a whole-number multiple of the same smallest "
    "value, about 1.6 times ten to the minus 19 coulombs: the elementary "
    "charge. Charge is quantized.",
    "!Direction check: to hold up a negatively charged drop, the top plate "
    "must be positive. If the drop rises, the electric force is bigger than "
    "its weight, so lower the p.d.",
    NULL
};

static const char *const current[] = {
    "!Electric currents. The p.d. and Ohm's law definitions are your "
    "teacher's exact words.",
    "#Current",
    "@Electric current",
    "The rate of flow of charge past a point in a circuit. Unit: the ampere, "
    "one coulomb per second.",
    "@Ampere",
    "The SI base unit of current: a flow of one coulomb of charge per second.",
    "@Conventional current",
    "The direction positive charge would flow: out of the positive terminal, "
    "round the circuit, into the negative terminal. Electrons in a metal "
    "actually drift the opposite way.",
    "@Charge carrier",
    "A charged particle that is free to move and so carry a current: free "
    "electrons in metals, ions in electrolytes and ionized gases.",
    "@Drift speed",
    "The small average speed of the charge carriers along a conductor when a "
    "current flows, on top of their fast random thermal motion.",
    "#Conductors and insulators",
    "@Conductor",
    "A material with many free (delocalized) electrons that can move through "
    "it, so charge flows easily. Metals are good conductors.",
    "@Insulator",
    "A material in which electrons are tightly bound to their atoms, so "
    "there are almost no free charge carriers and charge cannot flow.",
    "@Semiconductor",
    "A material with far fewer free charge carriers than a metal. Heating it "
    "or shining light on it frees more carriers, so its resistance falls.",
    "#Potential difference",
    "@Potential difference",
    "The work done per unit charge in moving a small positive test charge "
    "between the two points.",
    "In a circuit: the electrical energy converted to other forms for each "
    "coulomb passing through a component.",
    "@Volt",
    "The potential difference between two points when one joule of work is "
    "done per coulomb of charge moved between them.",
    "#Resistance",
    "@Electrical resistance",
    "The ratio of the potential difference across a component to the "
    "current through it. Unit: the ohm.",
    "@Ohm",
    "The resistance of a component that carries a current of one ampere when "
    "the p.d. across it is one volt.",
    "@Cause of resistance",
    "Free electrons accelerated by the electric field collide with the "
    "vibrating positive ions of the metal lattice and pass energy to them. "
    "This limits the electrons' drift and heats the conductor.",
    "@Resistance and temperature",
    "Heating a metal makes its lattice ions vibrate more, so electrons "
    "collide more often and resistance rises. In a semiconductor the extra "
    "carriers freed by heating win, so resistance falls.",
    "@Resistivity",
    "A property of the material itself: the resistance of a sample of unit "
    "length and unit cross-sectional area. Unit: the ohm metre. A wire's "
    "resistance is proportional to its length and inversely proportional to "
    "its cross-sectional area.",
    "!Resistance belongs to an object; resistivity belongs to the material. "
    "A long thin wire has more resistance than a short thick one of the same "
    "metal, but the same resistivity.",
    "#Ohm's law",
    "@Ohm's law",
    "For an ohmic device, the potential difference across the device is "
    "directly proportional to the current through the device.",
    "!Say \"directly proportional\" and \"ohmic device\". For a metal wire, "
    "add \"at constant temperature\".",
    "@Ohmic device",
    "A component that obeys Ohm's law: its resistance stays constant "
    "whatever the current, so its V-I graph is a straight line through the "
    "origin. Example: a metal wire at constant temperature.",
    "@Non-ohmic device",
    "A component whose resistance changes with the current, so its V-I "
    "graph is curved. Example: a filament lamp.",
    "@Filament lamp",
    "As the current rises the filament gets hotter, its ions vibrate more "
    "and its resistance increases, so each extra volt gives less extra "
    "current.",
    "#Thermistors and LDRs",
    "@Thermistor",
    "A semiconductor resistor whose resistance falls as its temperature "
    "rises (NTC type).",
    "@Why a thermistor's resistance falls",
    "Heating gives more electrons enough energy to break free of their "
    "atoms, so the number of free charge carriers rises sharply. This "
    "outweighs the extra lattice vibration that raises a metal's resistance.",
    "@Thermistor resistance and temperature",
    "Not linear: the resistance falls steeply at low temperatures and "
    "levels off at high ones, never reaching zero. A small change in "
    "temperature gives a large change in resistance, so it is very "
    "sensitive.",
    "@PTC thermistor",
    "The less common type whose resistance rises as its temperature rises. "
    "Assume NTC unless told otherwise.",
    "@Thermistor as a component",
    "Non-ohmic: a larger current heats it, its resistance falls, so each "
    "extra volt gives more extra current than the last. Held at constant "
    "temperature it behaves ohmically.",
    "@Self-heating",
    "The current through a thermistor warms it, so it reads its own "
    "temperature and not just the surroundings'. Keep the current small in "
    "a sensor circuit.",
    "@Thermistor uses",
    "Temperature sensors: digital thermometers, thermostats, fire alarms, "
    "engine and battery temperature monitors.",
    "@Light-dependent resistor",
    "A semiconductor resistor (LDR) whose resistance falls as the intensity "
    "of light on it increases.",
    "@Why an LDR's resistance falls",
    "Photons absorbed by the semiconductor give electrons enough energy to "
    "break free, so brighter light means more free charge carriers and a "
    "lower resistance.",
    "@LDR resistance and light",
    "Not linear: the resistance falls steeply as light first increases, "
    "then levels off. Typically about a megohm in the dark, down to a few "
    "hundred ohms in bright light.",
    "@LDR as a component",
    "At a constant light intensity it behaves ohmically: straight V-I "
    "graph through the origin. It responds to changes in light slowly, over "
    "tens of milliseconds.",
    "@LDR uses",
    "Light sensors: street lights and night lights that switch on at dusk, "
    "camera light meters, burglar alarms, automatic screen brightness.",
    "!Both are semiconductors, and in both more energy (heat or light) "
    "frees more charge carriers, so resistance falls. Say \"more charge "
    "carriers\", not \"electrons move faster\".",
    "#Power and heating",
    "@Joule heating",
    "The heating of a conductor carrying a current, as colliding electrons "
    "pass energy to the lattice. Also called resistive or ohmic heating.",
    "@Joule's heating law",
    "The rate of heat production in a resistor is proportional to the square "
    "of the current through it, for a given resistance.",
    "!Double the current and the heating goes up four times.",
    "@Electrical power",
    "The rate at which electrical energy is transferred to other forms in a "
    "component: the p.d. across it times the current through it. Unit: the "
    "watt, one joule per second.",
    "@Power dissipation",
    "The conversion of electrical energy into thermal energy in a resistor, "
    "which then passes to the surroundings. Rated power is the most a "
    "component can dissipate safely.",
    "@Heating water electrically",
    "The electrical energy supplied (power times time) becomes thermal "
    "energy in the water (mass times specific heat capacity times "
    "temperature rise). Allow for any energy lost to the surroundings.",
    NULL
};

static const char *const circuits[] = {
    "#Series and parallel",
    "@Series circuit",
    "Components connected one after another in a single loop. The same "
    "current flows through each, their p.d.s add up to the total, and the "
    "total resistance is the sum of their resistances.",
    "@Parallel circuit",
    "Components connected across the same two points on separate branches. "
    "Each has the same p.d. across it, the branch currents add up to the "
    "total, and the reciprocal of the total resistance is the sum of the "
    "reciprocals.",
    "!A resistor added in series always raises the total resistance. One "
    "added in parallel always lowers it, below the smallest branch.",
    "@Equivalent resistance",
    "The single resistance that could replace a combination and draw the "
    "same current from the same p.d.",
    "#Meters",
    "@Ammeter",
    "Measures current. Connected in series with the component, so the same "
    "current flows through both.",
    "@Ideal ammeter",
    "An ammeter with zero resistance, so there is no p.d. across it and it "
    "does not change the current it measures.",
    "@Voltmeter",
    "Measures potential difference. Connected in parallel, across the "
    "component.",
    "@Ideal voltmeter",
    "A voltmeter with infinite resistance, so it draws no current and does "
    "not change the circuit it measures.",
    "!A real voltmeter draws a little current and a real ammeter drops a "
    "little p.d. The error is worst when the meter's resistance is close to "
    "the component's.",
    "#Kirchhoff's laws",
    "@Kirchhoff's first law",
    "The sum of the currents entering a junction equals the sum of the "
    "currents leaving it. It is based on conservation of charge.",
    "@Kirchhoff's second law",
    "Around any closed loop in a circuit, the sum of the EMFs equals the sum "
    "of the potential differences. It is based on conservation of energy.",
    "!Both are conservation laws: the first conserves charge, the second "
    "conserves energy. That link is often the mark.",
    "@Junction",
    "A point in a circuit where three or more conductors meet, so the "
    "current divides or combines.",
    "#Potential dividers",
    "@Potential divider",
    "Two or more resistors in series across a supply, sharing the supply "
    "p.d. in proportion to their resistances. The output taken across one "
    "resistor is a chosen fraction of the supply p.d.",
    "@Potentiometer",
    "A variable potential divider: a resistor with a sliding contact that "
    "gives any output from zero up to the full supply p.d.",
    "@Rheostat",
    "A variable resistor used in series to control the current. Unlike a "
    "potentiometer, it cannot bring the current down to zero.",
    "@Sensor circuit",
    "A potential divider with a sensor, such as a thermistor or an LDR, as "
    "one of its resistors. When temperature or light changes, the sensor's "
    "resistance changes, so its share of the p.d. and the output change.",
    "!Bigger resistance, bigger share of the p.d. As a thermistor warms, its "
    "resistance falls, so the p.d. across it falls and the p.d. across the "
    "fixed resistor rises.",
    "@Output that rises with temperature",
    "Thermistor at the top, next to the positive supply; output across the "
    "fixed resistor. Warmer: thermistor resistance falls, fixed resistor's "
    "share rises. Used for fire alarms and cooling fans.",
    "@Output that rises in the dark",
    "LDR at the bottom; output across the LDR. Darker: LDR resistance "
    "rises, its share of the p.d. rises. Used to switch on street lights.",
    "!Swap the sensor and the fixed resistor and the output does the "
    "opposite.",
    "@Choosing the fixed resistor",
    "Pick it close to the sensor's resistance in the middle of the range to "
    "be measured: the output then changes most for a given change in "
    "temperature or light.",
    "@Setting the trigger point",
    "Replace the fixed resistor with a variable resistor. Adjusting it sets "
    "the temperature or light level at which the output reaches the value "
    "that switches the next device.",
    "@Sensor in series with a lamp",
    "Warm the thermistor or light the LDR: its resistance falls, the total "
    "resistance falls, the current rises and the lamp gets brighter.",
    "@Loading a sensor circuit",
    "A device connected across the output is in parallel with that "
    "resistor and lowers its resistance, so the output falls. Its "
    "resistance should be much larger than the resistor it is across.",
    NULL
};

static const char *const cells[] = {
    "!The EMF definition is your teacher's exact words. The lab to find "
    "internal resistance is on its own page: EMF & r lab.",
    "#Cells",
    "@Electric cell",
    "A device that converts chemical energy into electrical energy, driving "
    "charge round a circuit. A battery is two or more cells joined together.",
    "@Primary cell",
    "A cell that cannot be recharged: its chemical reaction cannot be "
    "reversed, so it is thrown away when used up. Example: an alkaline cell.",
    "@Secondary cell",
    "A rechargeable cell: driving a current backwards through it reverses "
    "the chemical reaction and restores its stored energy. Example: a "
    "lithium-ion cell.",
    "@Cell capacity",
    "The total charge a cell can deliver before it is flat, usually given in "
    "amp-hours: the current it could supply for one hour.",
    "@Discharge characteristic",
    "The graph of terminal p.d. against time as a cell discharges: a quick "
    "initial drop, a long nearly flat stretch, then a rapid fall as the cell "
    "runs out.",
    "#EMF and internal resistance",
    "@EMF (electromotive force)",
    "The work done per unit charge in driving a quantity of charge "
    "completely around the circuit.",
    "The chemical energy converted to electrical energy per coulomb, in "
    "volts. Despite its name it is not a force.",
    "@Internal resistance",
    "The resistance of the materials inside the cell itself. Charge passing "
    "through the cell does work against it, so some energy is dissipated "
    "inside the cell as heat.",
    "@Terminal potential difference",
    "The p.d. across the cell's terminals while it delivers current: the "
    "energy per coulomb that actually reaches the external circuit. It is "
    "the EMF minus the lost volts.",
    "@Lost volts",
    "The p.d. across the internal resistance: the energy per coulomb wasted "
    "inside the cell. It is proportional to the current, so it grows as more "
    "current is drawn.",
    "@Open circuit",
    "No current flows, so there are no lost volts and the terminal p.d. "
    "equals the EMF. A high-resistance voltmeter across a cell on its own "
    "reads very nearly its EMF.",
    "@Short circuit",
    "The terminals joined by a wire of negligible resistance. The current is "
    "limited only by the internal resistance, all the energy is dissipated "
    "inside the cell, and the terminal p.d. is zero.",
    "@Charging a battery",
    "A supply forces current backwards through the cell. The terminal p.d. "
    "is then the EMF plus the lost volts, because the supply must also push "
    "the charge through the internal resistance.",
    "!Discharging, terminal p.d. is less than the EMF; charging, it is more. "
    "The data booklet only gives the normal, discharging case.",
    NULL
};

/* ---- sketches and the lab ---- */

static const char *const sketch[] = {
    "!You must be able to draw these. Each entry says what the examiner "
    "looks for.",
    "#Rules for field lines",
    "@Field line direction",
    "Arrows point the way a positive test charge would be pushed: away from "
    "positive charges, towards negative ones.",
    "@Field line spacing",
    "Lines closer together mean a stronger field. Parallel, equally spaced "
    "lines mean a uniform field.",
    "@Field lines never cross",
    "The field at any point has only one direction, so two lines can never "
    "pass through the same point.",
    "@Field lines at conductors",
    "Field lines meet the surface of a conductor at right angles.",
    "@Drawing equipotentials",
    "Always at right angles to the field lines. For equal steps of "
    "potential, closer equipotentials mean a stronger field.",
    "#Single charges",
    "@Isolated positive charge",
    "Straight radial lines, evenly spaced all round, arrows pointing "
    "outwards. Equipotentials are concentric circles, further apart the "
    "further out they are.",
    "@Isolated negative charge",
    "The same radial pattern with the arrows pointing inwards. "
    "Equipotentials are concentric circles with the same spacing rule, at "
    "negative potential.",
    "@Charged conducting sphere",
    "Outside, exactly like a point charge at the centre. Inside, no field at "
    "all, and the potential is constant, equal to its value at the surface.",
    "#Two charges",
    "@Positive and negative charge",
    "Lines leave the positive charge and curve round into the negative, "
    "most tightly packed on the line between them. For equal charges, the "
    "equipotential midway between them is a straight line at zero "
    "potential.",
    "@Two positive charges",
    "Lines leave both charges and bend away from each other; none cross the "
    "gap. For equal charges there is a neutral point midway. Close to each "
    "charge the equipotentials are loops round it; further out, one loop "
    "encloses both.",
    "@Two negative charges",
    "The same shape as two positive charges, with every arrow reversed.",
    "@Unequal charges",
    "The larger charge has more lines leaving or entering it, in proportion "
    "to its charge. For like charges the neutral point moves towards the "
    "smaller charge.",
    "#Parallel plates",
    "@Plate field lines",
    "Straight, parallel, evenly spaced lines from the positive plate to the "
    "negative plate, at right angles to both, curving outwards beyond the "
    "ends of the plates.",
    "@Plate equipotentials",
    "Straight lines parallel to the plates, evenly spaced between them, "
    "bending near the ends.",
    "#Graphs to sketch",
    "@Ohmic conductor V-I graph",
    "A straight line through the origin, continuing into the negative "
    "quadrant for reversed p.d. Constant gradient: constant resistance.",
    "@Filament lamp V-I graph",
    "Current against p.d. is an S-shape through the origin: straight near "
    "the origin, then flattening towards the p.d. axis as the filament heats "
    "and its resistance rises. Symmetrical for reversed p.d.",
    "!Check which way round the axes are. Current against p.d. flattens for "
    "a lamp; p.d. against current curves upwards.",
    "@Thermistor V-I graph",
    "Current against p.d. curves upwards, away from the p.d. axis, as the "
    "thermistor heats and its resistance falls. The opposite bend to a "
    "lamp; symmetrical for reversed p.d.",
    "@Thermistor resistance against temperature",
    "A curve falling steeply at first, then levelling off towards the "
    "temperature axis without touching it. Not a straight line.",
    "@LDR resistance against light intensity",
    "The same shape: steep fall at low intensity, levelling off in bright "
    "light, never reaching zero.",
    "@Field strength against distance",
    "For a point charge, a steep inverse-square curve falling towards zero.",
    "@Potential against distance",
    "For a positive charge, a positive curve falling towards zero, less "
    "steep than the field curve; for a negative charge, a negative curve "
    "rising towards zero. Its gradient is minus the field strength.",
    "@Potential across parallel plates",
    "A straight line falling steadily from the positive plate to the "
    "negative plate, because the field is uniform.",
    NULL
};

static const char *const lab[] = {
    "!Flagged as important for the test. Know the set-up, the graph, and what "
    "the graph tells you.",
    "#Set-up",
    "A cell in series with a variable resistor, an ammeter and a switch, "
    "with a voltmeter directly across the cell's terminals. A small fixed "
    "resistor in series stops the current getting dangerously large.",
    "#Method",
    "Close the switch, record the current and the terminal p.d., then open "
    "it. Change the variable resistor and repeat for at least six settings, "
    "over as wide a range of current as possible.",
    "#The graph",
    "^Terminal p.d. against current",
    ">y-axis\tterminal potential difference",
    ">x-axis\tcurrent",
    ">Shape\tstraight line, falling",
    ">y-intercept\tEMF",
    ">Gradient\tminus internal resistance",
    ">x-intercept\tshort-circuit current",
    "@Why it is a straight line",
    "The terminal p.d. is the EMF minus the lost volts, and the lost volts "
    "are proportional to the current. So the terminal p.d. falls steadily "
    "as the current rises.",
    "@Reading the EMF",
    "At zero current there are no lost volts, so where the line meets the "
    "p.d. axis the terminal p.d. equals the EMF.",
    "@Reading the internal resistance",
    "The line drops by the same number of volts for every extra ampere; that "
    "drop per ampere, the size of the gradient, is the internal resistance.",
    "#Getting full marks",
    "@Keep the current low and brief",
    "A large current heats the cell, changing its internal resistance, and "
    "runs it down so its EMF falls. Open the switch between readings.",
    "@Uncertainties in the lab",
    "Error bars come from the meters' resolution. Draw the steepest and "
    "shallowest lines through all the error bars: their gradients give the "
    "range of internal resistance, their intercepts the range of EMF.",
    "@Systematic errors in the lab",
    "A real voltmeter draws a small current and a real ammeter has some "
    "resistance. Poor contacts add resistance that is not the cell's own.",
    "@Other method",
    "With only a resistance box and an ammeter: plot the reciprocal of the "
    "current against the external resistance. The gradient is the "
    "reciprocal of the EMF, and the line cuts the resistance axis at minus "
    "the internal resistance.",
    "!EMF is the energy per coulomb the cell gives; terminal p.d. is what "
    "reaches the circuit; lost volts are what the internal resistance "
    "keeps.",
    NULL
};

/* ---- the IB themes ---- */

static const char *const themeA[] = {
    "#Kinematics",
    "@Distance",
    "The total length of path travelled, whatever the direction. A scalar.",
    "@Displacement",
    "The distance moved in a stated direction from a fixed point. A vector.",
    "@Speed",
    "The rate of change of distance. A scalar.",
    "@Velocity",
    "The rate of change of displacement. A vector.",
    "@Acceleration",
    "The rate of change of velocity. A vector: speeding up, slowing down and "
    "changing direction are all acceleration.",
    "@Instantaneous velocity",
    "The velocity at one instant: the gradient of the tangent to a "
    "displacement-time graph at that time.",
    "@Average velocity",
    "The total displacement over the total time taken.",
    "@Free fall",
    "Motion under gravity alone, with no air resistance. Every object in "
    "free fall has the same acceleration.",
    "@Projectile motion",
    "Motion under gravity alone after launch. The horizontal velocity stays "
    "constant while the vertical velocity changes at a constant rate; the "
    "two are independent and the path is a parabola.",
    "@Fluid resistance",
    "A drag force opposing the motion of an object through a fluid, which "
    "increases with speed.",
    "@Terminal velocity",
    "The constant velocity a falling object reaches when drag has grown to "
    "equal its weight, so the resultant force and acceleration are zero.",
    "#Forces and momentum",
    "@Newton's first law",
    "An object stays at rest or moves with constant velocity unless a "
    "resultant external force acts on it.",
    "@Newton's second law",
    "The resultant force on an object is proportional to its rate of change "
    "of momentum and acts in the same direction. For constant mass, the "
    "acceleration is proportional to the resultant force.",
    "@Newton's third law",
    "If body A exerts a force on body B, then B exerts a force on A that is "
    "equal in size, opposite in direction and of the same type.",
    "!A third-law pair acts on DIFFERENT bodies. The weight of a book and "
    "the table's push on it are not a pair.",
    "@Inertia",
    "The tendency of an object to resist changes to its motion, measured by "
    "its mass.",
    "@Mass",
    "The amount of matter in an object, and a measure of its inertia. A "
    "scalar, in kilograms.",
    "@Weight",
    "The gravitational force on an object's mass. A vector, in newtons, "
    "towards the centre of the planet.",
    "@Normal force",
    "The contact force a surface exerts on an object, perpendicular to the "
    "surface.",
    "@Friction",
    "A force that opposes relative motion, or attempted motion, between two "
    "surfaces in contact, acting parallel to the surfaces.",
    "@Static friction",
    "Friction while the surfaces are not sliding. It grows to match the "
    "applied force up to a maximum; beyond that, sliding starts.",
    "@Dynamic friction",
    "Friction while the surfaces slide over each other. Usually less than "
    "the maximum static friction.",
    "@Coefficient of friction",
    "The ratio of the friction force to the normal force for a pair of "
    "surfaces. It has no units.",
    "@Tension",
    "The pulling force in a stretched string, rope or cable, acting along "
    "it.",
    "@Hooke's law",
    "The extension of a spring is directly proportional to the force "
    "applied, provided the limit of proportionality is not exceeded.",
    "@Spring constant",
    "The force needed per unit extension of a spring: a measure of its "
    "stiffness.",
    "@Upthrust",
    "The upward force a fluid exerts on an object in it, equal to the weight "
    "of fluid displaced (Archimedes' principle). Also called buoyancy.",
    "@Translational equilibrium",
    "The state of an object when the resultant force on it is zero, so it is "
    "at rest or moving at constant velocity.",
    "@Linear momentum",
    "The product of an object's mass and its velocity. A vector.",
    "@Impulse",
    "The change in momentum of an object: the average resultant force times "
    "the time it acts for, or the area under a force-time graph.",
    "@Conservation of linear momentum",
    "The total momentum of a system stays constant provided no resultant "
    "external force acts on it.",
    "@Elastic collision",
    "A collision in which total kinetic energy is conserved, as well as "
    "momentum.",
    "@Inelastic collision",
    "A collision in which some kinetic energy is transferred to other forms, "
    "such as heat and sound. Momentum is still conserved.",
    "@Explosion",
    "Objects at rest push apart. Total momentum stays zero, so they move off "
    "with equal and opposite momenta; kinetic energy comes from stored "
    "energy.",
    "#Circular motion",
    "@Uniform circular motion",
    "Motion in a circle at constant speed. The direction, and so the "
    "velocity, keeps changing, so the object is accelerating.",
    "@Centripetal acceleration",
    "The acceleration of an object in circular motion, directed towards the "
    "centre of the circle.",
    "@Centripetal force",
    "The resultant force towards the centre that keeps an object moving in a "
    "circle. It is not an extra force: gravity, tension, friction or a "
    "normal force provides it.",
    "@Angular velocity",
    "The rate of change of the angle swept out by the radius, in radians per "
    "second.",
    "#Work, energy and power",
    "@Work done",
    "The product of a force and the displacement in the direction of the "
    "force. A scalar, in joules.",
    "@Joule",
    "The work done when a force of one newton moves its point of application "
    "one metre in the direction of the force.",
    "@Energy",
    "The capacity to do work. Work done is energy transferred.",
    "@Conservation of energy",
    "Energy cannot be created or destroyed, only transferred from one form "
    "to another. The total energy of an isolated system is constant.",
    "@Kinetic energy",
    "The energy an object has because of its motion.",
    "@Elastic potential energy",
    "The energy stored in a deformed elastic object, such as a stretched "
    "spring: the area under its force-extension graph.",
    "@Power",
    "The rate of doing work, or the rate of energy transfer. Unit: the watt, "
    "one joule per second.",
    "@Efficiency",
    "The ratio of useful energy (or power) output to total energy (or power) "
    "input, often as a percentage.",
    "@Energy density",
    "The energy released per unit volume of a fuel.",
    "@Specific energy",
    "The energy released per unit mass of a fuel.",
    "#Rigid bodies (HL)",
    "@Torque",
    "The turning effect of a force about an axis: the force times the "
    "perpendicular distance from the axis to its line of action.",
    "@Couple",
    "Two equal, opposite, parallel forces not along the same line. They "
    "produce a turning effect but no resultant force.",
    "@Rotational equilibrium",
    "The state of a body when the resultant torque on it is zero, so its "
    "angular velocity is constant.",
    "@Moment of inertia",
    "A body's resistance to changes in its rotation, depending on its mass "
    "and how that mass is spread about the axis.",
    "@Angular acceleration",
    "The rate of change of angular velocity.",
    "@Angular momentum",
    "The product of moment of inertia and angular velocity. Conserved when "
    "no resultant external torque acts.",
    "@Angular impulse",
    "The change in angular momentum: the torque times the time it acts for.",
    "#Relativity (HL)",
    "@Reference frame",
    "A coordinate system from which an observer measures the positions and "
    "times of events.",
    "@Inertial reference frame",
    "A frame that is not accelerating, in which Newton's first law holds.",
    "@Galilean relativity",
    "The classical view that time and distance are the same for all "
    "observers and velocities simply add. It fails near the speed of light.",
    "@Postulates of special relativity",
    "First: the laws of physics are the same in all inertial reference "
    "frames. Second: the speed of light in a vacuum is the same for all "
    "inertial observers.",
    "@Event",
    "Something that happens at a single point in space at a single instant.",
    "@Proper time",
    "The time between two events in the frame where they happen at the same "
    "place. It is the shortest time any observer measures.",
    "@Proper length",
    "The length of an object in the frame where it is at rest. It is the "
    "longest length any observer measures.",
    "@Time dilation",
    "A clock moving relative to an observer is measured to run slow: the "
    "time between two events is longest outside their rest frame.",
    "@Length contraction",
    "An object moving relative to an observer is measured to be shorter "
    "along its direction of motion than its proper length.",
    "@Relativity of simultaneity",
    "Two events at different places that are simultaneous in one frame are "
    "not simultaneous in a frame moving relative to it.",
    "@Spacetime interval",
    "A combination of the time and space separations of two events that has "
    "the same value in every inertial frame.",
    "@Invariant quantity",
    "A quantity all inertial observers agree on: the speed of light, proper "
    "time, proper length, rest mass and the spacetime interval.",
    NULL
};

static const char *const themeB[] = {
    "!Electric current and circuits are on the test pages: Current & Ohm, "
    "Circuits & meters, Cells & EMF.",
    "#Thermal energy",
    "@Temperature",
    "A measure of the average kinetic energy of the particles of a "
    "substance. It decides which way thermal energy flows.",
    "@Internal energy",
    "The total of the random kinetic energies and intermolecular potential "
    "energies of all the particles in a substance.",
    "@Thermal energy",
    "Energy transferred from a region of higher temperature to one of lower "
    "temperature because of the temperature difference. Also called heat.",
    "@Thermal equilibrium",
    "Two objects in thermal contact are at the same temperature, so there is "
    "no net transfer of thermal energy between them.",
    "@Absolute zero",
    "The lowest possible temperature, zero kelvin (about minus 273 degrees "
    "Celsius), where particles have minimum kinetic energy.",
    "@Kelvin",
    "The SI unit of temperature on the absolute scale, which starts at "
    "absolute zero. A change of one kelvin equals a change of one degree "
    "Celsius.",
    "@Density",
    "Mass per unit volume.",
    "@Specific heat capacity",
    "The energy needed to raise the temperature of one kilogram of a "
    "substance by one kelvin.",
    "@Specific latent heat",
    "The energy needed to change the phase of one kilogram of a substance at "
    "constant temperature.",
    "@Latent heat of fusion",
    "The energy needed to melt one kilogram of a solid into liquid at "
    "constant temperature.",
    "@Latent heat of vaporization",
    "The energy needed to turn one kilogram of a liquid into gas at constant "
    "temperature.",
    "@Phase change",
    "While a substance melts or boils, the energy supplied raises the "
    "intermolecular potential energy, not the kinetic energy, so the "
    "temperature stays constant.",
    "@Conduction",
    "Transfer of thermal energy through a material by collisions between "
    "neighbouring particles, and in metals by free electrons, without the "
    "material itself moving.",
    "@Convection",
    "Transfer of thermal energy by bulk movement of a fluid: warmer, less "
    "dense fluid rises and cooler, denser fluid sinks.",
    "@Thermal radiation",
    "Transfer of energy by electromagnetic waves, mainly infrared. It needs "
    "no medium.",
    "#Radiation and climate",
    "@Black body",
    "A perfect absorber and emitter of radiation: it absorbs all radiation "
    "falling on it and emits the most possible for its temperature.",
    "@Emissivity",
    "The power radiated per unit area by a surface as a fraction of the "
    "power radiated per unit area by a black body at the same temperature.",
    "@Stefan-Boltzmann law",
    "The total power radiated by a black body is proportional to its surface "
    "area and to the fourth power of its absolute temperature.",
    "@Wien's displacement law",
    "The wavelength at which a black body emits most strongly is inversely "
    "proportional to its absolute temperature: hotter bodies peak at shorter "
    "wavelengths.",
    "@Luminosity",
    "The total power radiated by a star.",
    "@Apparent brightness",
    "The power from a star received per unit area at the observer, "
    "perpendicular to the direction of the light.",
    "@Albedo",
    "The total power reflected or scattered by a surface as a fraction of "
    "the total power falling on it.",
    "@Solar constant",
    "The power of the Sun's radiation per unit area arriving at the top of "
    "the Earth's atmosphere, perpendicular to the rays.",
    "@Greenhouse effect",
    "Greenhouse gases absorb infrared radiation emitted by the Earth's "
    "surface and re-emit it in all directions, some back to the surface, "
    "warming it above what it would otherwise be.",
    "@Greenhouse gas",
    "A gas whose molecules absorb infrared because it matches their natural "
    "vibration frequencies: carbon dioxide, methane, water vapour, nitrous "
    "oxide.",
    "@Enhanced greenhouse effect",
    "The extra warming caused by higher concentrations of greenhouse gases "
    "from human activity, such as burning fossil fuels.",
    "#Gases",
    "@Pressure",
    "The force acting normally per unit area. Unit: the pascal, one newton "
    "per square metre.",
    "@Mole",
    "The amount of substance that contains exactly Avogadro's number of "
    "particles.",
    "@Avogadro constant",
    "The number of particles in one mole: about 6.02 times ten to the 23.",
    "@Molar mass",
    "The mass of one mole of a substance.",
    "@Ideal gas",
    "A gas that obeys the ideal gas law exactly at all pressures, volumes "
    "and temperatures, with no forces between its molecules except during "
    "collisions.",
    "@Kinetic model assumptions",
    "Molecules are identical, in random motion, and of negligible volume "
    "compared with the container. Collisions are elastic and take negligible "
    "time. No intermolecular forces except during collisions.",
    "@Gas pressure explained",
    "Molecules hit the walls and change momentum. The rate of change of "
    "momentum is a force on the walls, and force per unit area is pressure.",
    "@Boyle's law",
    "For a fixed mass of gas at constant temperature, the pressure is "
    "inversely proportional to the volume.",
    "@Charles's law",
    "For a fixed mass of gas at constant pressure, the volume is directly "
    "proportional to the absolute temperature.",
    "@Pressure law",
    "For a fixed mass of gas at constant volume, the pressure is directly "
    "proportional to the absolute temperature.",
    "@Real gas",
    "Behaves like an ideal gas at low pressure and high temperature, but not "
    "at high pressure or low temperature, when molecular volume and "
    "intermolecular forces matter.",
    "#Thermodynamics (HL)",
    "@First law of thermodynamics",
    "The thermal energy transferred to a system equals the increase in its "
    "internal energy plus the work done by the system. It is conservation "
    "of energy.",
    "@Second law of thermodynamics",
    "The total entropy of an isolated system never decreases. Equivalently, "
    "thermal energy cannot flow by itself from a colder to a hotter body, "
    "and no heat engine can turn all the heat it takes in into work.",
    "@Entropy",
    "A measure of the disorder of a system: of how many ways its particles "
    "and energy can be arranged.",
    "@Isovolumetric process",
    "A change at constant volume, so no work is done by or on the gas.",
    "@Isobaric process",
    "A change at constant pressure.",
    "@Isothermal process",
    "A change at constant temperature, so the internal energy of an ideal "
    "gas does not change.",
    "@Adiabatic process",
    "A change with no thermal energy transferred into or out of the gas. "
    "Compression heats it; expansion cools it.",
    "@Heat engine",
    "A device that takes thermal energy from a hot reservoir, turns part of "
    "it into work and passes the rest to a cold reservoir, in a repeating "
    "cycle.",
    "@Carnot cycle",
    "The most efficient possible heat engine cycle: two isothermal and two "
    "adiabatic processes. Its efficiency depends only on the two reservoir "
    "temperatures.",
    NULL
};

static const char *const themeC[] = {
    "#Simple harmonic motion",
    "@Oscillation",
    "A repeated back-and-forth motion about an equilibrium position.",
    "@Simple harmonic motion",
    "Oscillation in which the acceleration is directly proportional to the "
    "displacement from the equilibrium position and always directed towards "
    "it.",
    "@Equilibrium position",
    "The position where the resultant force on the oscillating object is "
    "zero.",
    "@Restoring force",
    "The force that always acts to return an oscillating object to its "
    "equilibrium position.",
    "@Amplitude",
    "The maximum displacement from the equilibrium position.",
    "@Period",
    "The time taken for one complete oscillation.",
    "@Frequency",
    "The number of complete oscillations per unit time. Unit: the hertz, one "
    "per second.",
    "@Phase difference",
    "How far one oscillation is ahead of or behind another, given as a "
    "fraction of a cycle or an angle.",
    "@Isochronous",
    "Describes an oscillation whose period does not depend on its amplitude, "
    "as in simple harmonic motion.",
    "@Energy in SHM",
    "Energy moves back and forth between kinetic and potential. Kinetic is "
    "greatest at equilibrium, potential at maximum displacement; without "
    "damping the total is constant.",
    "#Travelling waves",
    "@Wave",
    "A disturbance that transfers energy from one place to another without "
    "transferring matter.",
    "@Transverse wave",
    "A wave in which the particles of the medium oscillate perpendicular to "
    "the direction of energy transfer. Example: light.",
    "@Longitudinal wave",
    "A wave in which the particles of the medium oscillate parallel to the "
    "direction of energy transfer. Example: sound.",
    "@Compression",
    "A region of a longitudinal wave where particles are closer together "
    "than normal, at higher pressure.",
    "@Rarefaction",
    "A region of a longitudinal wave where particles are further apart than "
    "normal, at lower pressure.",
    "@Wavelength",
    "The shortest distance between two points on a wave that oscillate in "
    "phase, such as crest to crest.",
    "@Wave speed",
    "The distance a wave travels per unit time: the speed at which it "
    "transfers energy.",
    "@Wave displacement",
    "The distance of a particle of the medium from its equilibrium position.",
    "@Wavefront",
    "A line or surface joining points of a wave that are in phase, such as "
    "all the crests.",
    "@Ray",
    "A line showing the direction of energy transfer, always perpendicular "
    "to the wavefronts.",
    "@Intensity",
    "The power per unit area received by a surface perpendicular to the "
    "wave. It is proportional to the amplitude squared.",
    "@Electromagnetic wave",
    "A transverse wave of oscillating electric and magnetic fields at right "
    "angles to each other. It travels through a vacuum at the speed of "
    "light.",
    "@Electromagnetic spectrum",
    "In order of increasing wavelength: gamma rays, X-rays, ultraviolet, "
    "visible light, infrared, microwaves, radio waves.",
    "#Wave phenomena",
    "@Reflection",
    "A wave bouncing back from a boundary into the medium it came from. The "
    "angle of incidence equals the angle of reflection.",
    "@Refraction",
    "The change in direction of a wave crossing a boundary between media at "
    "an angle, caused by the change in its speed.",
    "@Refractive index",
    "The ratio of the speed of light in a vacuum to the speed of light in "
    "the medium.",
    "@Snell's law",
    "For a wave crossing a boundary, the ratio of the sine of the angle of "
    "incidence to the sine of the angle of refraction is constant.",
    "@Critical angle",
    "The angle of incidence in the denser medium for which the angle of "
    "refraction is 90 degrees.",
    "@Total internal reflection",
    "All of a wave reflects back into the denser medium when it meets the "
    "boundary at more than the critical angle.",
    "@Diffraction",
    "The spreading of a wave as it passes through a gap or round an "
    "obstacle; greatest when the gap is about the size of the wavelength.",
    "@Principle of superposition",
    "When two or more waves meet at a point, the resultant displacement is "
    "the sum of their individual displacements.",
    "@Interference",
    "The superposition of waves from coherent sources, giving regions of "
    "reinforcement and cancellation.",
    "@Coherent sources",
    "Sources with the same frequency and a constant phase difference.",
    "@Path difference",
    "The difference in the distances two waves travel from their sources to "
    "a given point.",
    "@Constructive interference",
    "Waves arrive in phase, with a path difference of a whole number of "
    "wavelengths, giving maximum amplitude.",
    "@Destructive interference",
    "Waves arrive in antiphase, with a path difference of an odd number of "
    "half wavelengths, giving minimum amplitude.",
    "@Double-slit interference",
    "Coherent light through two narrow slits gives equally spaced bright and "
    "dark fringes on a screen.",
    "@Single-slit diffraction",
    "Light through one narrow slit gives a broad bright central maximum, "
    "twice as wide as the dimmer maxima either side.",
    "@Diffraction grating",
    "Many equally spaced parallel slits. It gives very sharp, bright, widely "
    "spaced maxima, used to measure wavelengths accurately.",
    "#Standing waves and resonance",
    "@Standing wave",
    "The superposition of two waves of the same frequency and amplitude "
    "travelling in opposite directions. It stores energy instead of "
    "transferring it.",
    "@Node",
    "A point on a standing wave where the displacement is always zero.",
    "@Antinode",
    "A point on a standing wave where the amplitude is greatest.",
    "@Fundamental",
    "The lowest frequency at which a standing wave forms on a string or in a "
    "pipe; the first harmonic.",
    "@Harmonics",
    "The frequencies at which standing waves can form: whole-number "
    "multiples of the fundamental, only odd ones for a pipe closed at one "
    "end.",
    "@Natural frequency",
    "The frequency at which a system oscillates when displaced and left to "
    "oscillate freely.",
    "@Forced oscillation",
    "Oscillation driven by a periodic external force, at the driving "
    "frequency.",
    "@Resonance",
    "The large increase in amplitude when the driving frequency equals the "
    "natural frequency of the system.",
    "@Damping",
    "The loss of energy from an oscillating system to its surroundings, "
    "usually by friction or drag, so the amplitude falls.",
    "@Light damping",
    "The system still oscillates, but its amplitude falls gradually.",
    "@Critical damping",
    "Just enough damping to return the system to equilibrium in the "
    "shortest time without oscillating.",
    "@Heavy damping",
    "So much damping that the system returns to equilibrium slowly, without "
    "oscillating.",
    "#Doppler effect",
    "@Doppler effect",
    "The change in the observed frequency and wavelength of a wave when the "
    "source and the observer move relative to each other.",
    "@Redshift",
    "The increase in wavelength of light from a source moving away. "
    "Blueshift is the decrease from a source moving closer.",
    NULL
};

static const char *const themeD[] = {
    "!Electric fields are on the test pages. Gravitational fields follow the "
    "same logic with mass in place of charge.",
    "#Gravitational fields",
    "@Kepler's first law",
    "Each planet moves in an ellipse with the Sun at one focus.",
    "@Kepler's second law",
    "The line joining a planet to the Sun sweeps out equal areas in equal "
    "times.",
    "@Kepler's third law",
    "The square of a planet's orbital period is proportional to the cube of "
    "its mean distance from the Sun.",
    "@Newton's law of gravitation",
    "Every point mass attracts every other point mass with a force directly "
    "proportional to the product of their masses and inversely proportional "
    "to the square of their separation.",
    "@Gravitational field",
    "A region of space in which a mass experiences a gravitational force.",
    "@Gravitational field strength",
    "The force per unit mass acting on a small test mass placed at that "
    "point.",
    "@Gravitational potential",
    "The work done per unit mass in bringing a small test mass from infinity "
    "to that point. Always negative, because gravity attracts.",
    "@Gravitational potential energy",
    "The work done in bringing a mass from infinity to a point in the field "
    "of another mass. Negative, rising to zero at infinity. Near the "
    "Earth's surface its change is weight times change in height.",
    "@Gravitational equipotential",
    "A surface of constant gravitational potential, at right angles to the "
    "field lines: concentric spheres round a planet.",
    "@Escape speed",
    "The minimum speed an object needs at a planet's surface to escape to "
    "infinity with no further propulsion, ignoring air resistance.",
    "@Orbital speed",
    "The speed of a satellite in a circular orbit, where gravity provides "
    "exactly the centripetal force needed.",
    "@Geostationary orbit",
    "An orbit above the equator with a period of one day, in the direction "
    "the Earth turns, so the satellite stays above one point.",
    "@Weightlessness",
    "The feeling in free fall, such as in orbit. Gravity still acts, but "
    "nothing provides a normal force because everything falls together.",
    "#Magnetic fields",
    "@Magnetic field",
    "A region of space in which a moving charge, a current-carrying "
    "conductor or a magnetic material experiences a force.",
    "@Magnetic field lines",
    "Show the way a north pole would be pushed: from north to south outside "
    "a magnet. They never cross and are closer where the field is stronger.",
    "@Field of a straight wire",
    "Concentric circles round the wire, further apart with distance, in the "
    "direction given by the right-hand grip rule.",
    "@Field of a solenoid",
    "Strong, uniform and parallel inside; outside, like a bar magnet's, with "
    "the ends acting as north and south poles.",
    "@Right-hand grip rule",
    "Grip the wire with the right thumb along the conventional current; the "
    "curled fingers show the direction of the magnetic field.",
    "@Magnetic flux density",
    "The force per unit length per unit current on a conductor at right "
    "angles to the field. Unit: the tesla.",
    "@Tesla",
    "The flux density that gives a force of one newton on each metre of a "
    "wire carrying one ampere at right angles to the field.",
    "#Motion in fields",
    "@Force on a current-carrying conductor",
    "A wire carrying current across a magnetic field feels a force at right "
    "angles to both the current and the field: greatest when perpendicular, "
    "zero when parallel.",
    "@Force on a moving charge",
    "A charge moving across a magnetic field feels a force at right angles "
    "to both its velocity and the field, proportional to its charge, its "
    "speed and the flux density.",
    "@Direction of magnetic force",
    "Given by a right-hand rule (or Fleming's left-hand rule) using "
    "conventional current. For a negative charge, reverse it.",
    "@Circular motion in a magnetic field",
    "The magnetic force is always at right angles to the velocity, so it "
    "does no work: the speed stays constant and the particle moves in a "
    "circle.",
    "@Charge in a uniform electric field",
    "The force is along the field lines and constant, so the charge follows "
    "a parabola and gains or loses kinetic energy.",
    "@Force between parallel wires",
    "Parallel currents in the same direction attract; in opposite "
    "directions they repel. Each wire sits in the other's magnetic field.",
    "#Induction (HL)",
    "@Magnetic flux",
    "The product of the magnetic flux density and the area at right angles "
    "to the field it passes through. Unit: the weber.",
    "@Magnetic flux linkage",
    "The magnetic flux through a coil times its number of turns.",
    "@Electromagnetic induction",
    "The production of an EMF in a conductor when the magnetic flux linking "
    "it changes.",
    "@Faraday's law",
    "The induced EMF equals the rate of change of magnetic flux linkage.",
    "@Lenz's law",
    "The induced EMF acts in the direction that makes any induced current "
    "oppose the change that caused it. It follows from conservation of "
    "energy.",
    "@Alternating current generator",
    "A coil turning in a magnetic field has a constantly changing flux "
    "linkage, which induces an alternating EMF. Faster turning gives a "
    "bigger EMF at a higher frequency.",
    "@Weber",
    "The unit of magnetic flux: one tesla times one square metre.",
    NULL
};

static const char *const themeE[] = {
    "#The atom",
    "@Nucleus",
    "The small, dense, positively charged centre of an atom, holding almost "
    "all its mass; made of protons and neutrons.",
    "@Nucleon",
    "A proton or a neutron: a particle in the nucleus.",
    "@Proton number",
    "The number of protons in a nucleus; it identifies the element. Also "
    "called atomic number.",
    "@Nucleon number",
    "The total number of protons and neutrons in a nucleus. Also called mass "
    "number.",
    "@Isotopes",
    "Atoms of the same element, with the same number of protons but "
    "different numbers of neutrons.",
    "@Nuclide",
    "A particular nucleus, identified by its proton number and nucleon "
    "number.",
    "@Rutherford scattering",
    "Alpha particles fired at thin gold foil mostly passed straight through, "
    "but a few bounced back through large angles. So the atom is mostly "
    "empty space with a tiny, dense, positive nucleus.",
    "@Energy level",
    "One of the discrete energies an electron in an atom is allowed to have.",
    "@Ground state",
    "The lowest energy level of an atom.",
    "@Photon",
    "A discrete packet of electromagnetic energy, whose energy is "
    "proportional to its frequency.",
    "@Emission spectrum",
    "Bright lines at specific wavelengths, emitted when electrons fall from "
    "higher to lower energy levels, each releasing a photon.",
    "@Absorption spectrum",
    "Dark lines in a continuous spectrum where a cool gas has absorbed "
    "photons of exactly the energy needed to lift electrons to higher "
    "levels.",
    "@Ionization energy",
    "The minimum energy needed to remove an electron from an atom in its "
    "ground state.",
    "#Quantum physics (HL)",
    "@Photoelectric effect",
    "The emission of electrons from a metal surface when electromagnetic "
    "radiation above a certain frequency falls on it.",
    "@Work function",
    "The minimum energy needed to remove an electron from the surface of a "
    "metal.",
    "@Threshold frequency",
    "The minimum frequency of radiation that releases electrons from a given "
    "metal surface.",
    "@Evidence for photons",
    "Below the threshold frequency no electrons are released however bright "
    "the light; emission is instant; the electrons' maximum kinetic energy "
    "depends on frequency, not intensity. Waves cannot explain this.",
    "@Stopping potential",
    "The p.d. needed to just stop the most energetic photoelectrons.",
    "@Wave-particle duality",
    "Light and matter show both wave properties (diffraction, interference) "
    "and particle properties (the photoelectric effect, collisions).",
    "@De Broglie wavelength",
    "The wavelength of a moving particle, inversely proportional to its "
    "momentum.",
    "@Electron diffraction",
    "Electrons passing through a thin crystal form a diffraction pattern, "
    "showing that particles behave as waves.",
    "@Compton scattering",
    "A photon scattered by an electron loses energy, so its wavelength "
    "increases. Evidence that photons carry momentum.",
    "#Radioactive decay",
    "@Radioactive decay",
    "The spontaneous and random emission of radiation from an unstable "
    "nucleus as it becomes more stable.",
    "@Random decay",
    "It is impossible to predict which nucleus will decay next or when; only "
    "the probability of decay per unit time is fixed.",
    "@Spontaneous decay",
    "Decay is not affected by outside conditions such as temperature, "
    "pressure or chemical state.",
    "@Alpha particle",
    "A helium nucleus: two protons and two neutrons. Strongly ionizing; "
    "stopped by paper or a few centimetres of air.",
    "@Beta-minus particle",
    "A fast electron emitted when a neutron in the nucleus turns into a "
    "proton, with an antineutrino. Stopped by a few millimetres of "
    "aluminium.",
    "@Beta-plus particle",
    "A positron emitted when a proton in the nucleus turns into a neutron, "
    "with a neutrino.",
    "@Gamma ray",
    "A high-energy photon emitted by a nucleus losing energy. Weakly "
    "ionizing; reduced by thick lead or concrete.",
    "@Neutrino",
    "A neutral particle of almost zero mass emitted in beta decay. It "
    "explains why beta particles have a range of energies.",
    "@Half-life",
    "The time taken for half the radioactive nuclei in a sample to decay, or "
    "for its activity to fall to half.",
    "@Activity",
    "The number of decays per second in a sample. Unit: the becquerel, one "
    "decay per second.",
    "@Decay constant",
    "The probability per unit time that a given nucleus will decay.",
    "@Ionizing radiation",
    "Radiation with enough energy to knock electrons out of atoms, creating "
    "ions.",
    "@Background radiation",
    "Ionizing radiation that is always present, from rocks, radon gas and "
    "cosmic rays, and from sources such as medical use.",
    "#Binding energy",
    "@Strong nuclear force",
    "The force between nucleons that holds the nucleus together: attractive "
    "and very short range, repulsive at very small separations.",
    "@Mass defect",
    "The total mass of the separate nucleons minus the mass of the nucleus "
    "they form.",
    "@Binding energy",
    "The energy needed to separate a nucleus completely into its nucleons, "
    "or the energy released when it forms. It is the energy equivalent of "
    "the mass defect.",
    "@Binding energy per nucleon",
    "The binding energy shared over the nucleons; a measure of stability. "
    "It peaks near iron.",
    "@Mass-energy equivalence",
    "Mass and energy are equivalent: a change in one is a change in the "
    "other, linked by the speed of light squared.",
    "@Unified atomic mass unit",
    "One twelfth of the mass of a carbon-12 atom.",
    "#Fission",
    "@Nuclear fission",
    "A heavy nucleus splitting into two smaller nuclei of similar mass, "
    "releasing energy and a few neutrons, usually after absorbing a neutron.",
    "@Chain reaction",
    "Neutrons released by one fission cause further fissions, so the "
    "reaction keeps itself going.",
    "@Critical mass",
    "The minimum mass of fissile material that can sustain a chain reaction.",
    "@Moderator",
    "A material such as water or graphite that slows fast neutrons by "
    "collisions, so they are more likely to cause fission.",
    "@Control rods",
    "Rods of a neutron-absorbing material such as boron, moved in or out to "
    "control the rate of fission.",
    "@Heat exchanger",
    "Passes thermal energy from the reactor's coolant to water, making steam "
    "to drive the turbines.",
    "@Shielding",
    "Thick concrete and steel round the reactor that absorbs radiation and "
    "neutrons.",
    "#Fusion and stars",
    "@Nuclear fusion",
    "Two light nuclei joining to form a heavier nucleus, releasing energy. "
    "It needs very high temperature and density to overcome electrostatic "
    "repulsion.",
    "@Stellar equilibrium",
    "In a stable star, the inward pull of gravity is balanced by the outward "
    "radiation and gas pressure from fusion.",
    "@Main sequence",
    "The band on the HR diagram where stars spend most of their lives, "
    "fusing hydrogen into helium in their cores.",
    "@Hertzsprung-Russell diagram",
    "A plot of stars' luminosity against surface temperature, with "
    "temperature decreasing to the right.",
    "@Red giant",
    "A large, cool, luminous star that has used up the hydrogen in its core "
    "and fuses helium and heavier elements.",
    "@White dwarf",
    "The small, hot, dense core left by a low-mass star, no longer fusing, "
    "held up by electron degeneracy pressure.",
    "@Chandrasekhar limit",
    "The largest possible mass of a white dwarf, about 1.4 solar masses.",
    "@Supernova",
    "The explosive death of a massive star, briefly outshining its galaxy "
    "and scattering heavy elements.",
    "@Neutron star",
    "The extremely dense remnant of a massive star after a supernova, held "
    "up by neutron degeneracy pressure.",
    "@Black hole",
    "The remnant of a very massive star, whose gravity is so strong that "
    "nothing, not even light, can escape.",
    "@Stellar parallax",
    "The apparent shift of a nearby star against distant stars as the Earth "
    "orbits the Sun, used to find its distance.",
    "@Parsec",
    "The distance at which a star has a parallax angle of one arcsecond; "
    "about 3.26 light-years.",
    "@Light-year",
    "The distance light travels in a vacuum in one year.",
    "@Astronomical unit",
    "The mean distance from the Earth to the Sun.",
    NULL
};

static const char *const tools[] = {
    "#Measurement",
    "@SI base units",
    "The seven base units: metre, kilogram, second, ampere, kelvin, mole and "
    "candela. Every other unit is built from them.",
    "@Derived unit",
    "A unit made from base units, such as the newton: a kilogram metre per "
    "second squared.",
    "@Scalar",
    "A quantity with size only, such as mass, distance, energy or potential.",
    "@Vector",
    "A quantity with size and direction, such as displacement, force or "
    "field strength.",
    "@Random error",
    "Unpredictable scatter of readings either side of the true value. "
    "Reduced by repeating readings and averaging.",
    "@Systematic error",
    "An error that shifts every reading the same way, such as a zero error "
    "or a badly calibrated meter. Repeating does not reduce it.",
    "@Zero error",
    "An instrument reading something other than zero when it should read "
    "zero.",
    "@Accuracy",
    "How close a measurement is to the true value.",
    "@Precision",
    "How close repeated measurements are to each other: how small the "
    "random error is.",
    "@Absolute uncertainty",
    "The range either side of a measured value within which the true value "
    "is expected to lie, in the same units as the value.",
    "@Fractional uncertainty",
    "The absolute uncertainty as a fraction of the measured value.",
    "@Percentage uncertainty",
    "The fractional uncertainty given as a percentage.",
    "@Propagating uncertainties",
    "Adding or subtracting quantities: add the absolute uncertainties. "
    "Multiplying or dividing: add the percentage uncertainties. A power: "
    "multiply the percentage uncertainty by the power.",
    "@Significant figures",
    "Give an answer to the same number of significant figures as the least "
    "precise data. Give uncertainties to one significant figure.",
    "@Error bars",
    "Bars on a plotted point showing the absolute uncertainty in its value.",
    "@Line of best fit",
    "A straight line or smooth curve passing as close as possible to all the "
    "points, through the error bars, with points evenly spread either side.",
    "@Gradient uncertainty",
    "Draw the steepest and shallowest lines that still pass through all the "
    "error bars; half the difference in their gradients is the "
    "uncertainty.",
    "@Independent variable",
    "The variable deliberately changed in an experiment.",
    "@Dependent variable",
    "The variable measured as a result.",
    "@Control variable",
    "A variable kept constant so it does not affect the result.",
    "#Writing definitions",
    "@Command terms",
    "Define: give the precise meaning. State: a name, value or short answer. "
    "Outline: a brief account. Describe: a detailed account. Explain: give "
    "the reasons or causes.",
    "@Per unit",
    "\"Per unit charge\", \"per unit mass\" and \"per unit area\" carry the "
    "mark in most field and electricity definitions. Leave them out and the "
    "mark goes.",
    "@Small positive test charge",
    "Field strength and potential definitions need it: small, so it does not "
    "disturb the field; positive, to fix the direction.",
    "@From infinity",
    "Potential definitions start at infinity, where the potential is zero, "
    "and bring the test charge or mass TO the point.",
    "!What earns the marks: say what kind of quantity it is (a force, a work "
    "done, a ratio), then per unit of what, then under what conditions.",
    NULL
};

/* ---- easily confused pairs ---- */

static const char *const cmpGravity[] = {
    "@Electric field vs gravitational field",
    "Both are inverse-square fields defined the same way: field strength is "
    "force per unit charge or mass, potential is work per unit charge or "
    "mass from infinity.",
    "%Electric field",
    "~Acts on charge.",
    "~Attractive or repulsive: charge comes in two signs.",
    "~Potential can be positive or negative.",
    "~Very strong: dominates between particles.",
    "%Gravitational field",
    "-Acts on mass.",
    "-Always attractive: mass has only one sign.",
    "-Potential is always negative.",
    "-Very weak: only noticeable for huge masses.",
    "!Turn an electric definition into a gravitational one by swapping "
    "\"charge\" for \"mass\" and dropping \"positive\" from the test charge.",
    NULL
};

static const char *const cmpFieldPot[] = {
    "@Electric field strength vs electric potential",
    "Two ways to describe the same field: one with forces, one with energy.",
    "%Field strength",
    "~Force per unit charge on a small positive test charge.",
    "~A vector: add with directions, by components.",
    "~Newtons per coulomb, or volts per metre.",
    "~Falls with distance squared for a point charge.",
    "%Potential",
    "-Work per unit charge bringing a small positive test charge from "
    "infinity.",
    "-A scalar: just add, keeping signs.",
    "-Volts, or joules per coulomb.",
    "-Falls with distance, not squared, for a point charge.",
    "!Field strength is minus the potential gradient: where equipotentials "
    "crowd together, the field is strong. Midway between equal positive "
    "charges the field is zero but the potential is not.",
    NULL
};

static const char *const cmpPotEnergy[] = {
    "@Electric potential vs electric potential energy",
    "Potential describes a point in the field; potential energy describes a "
    "charge placed there.",
    "%Potential",
    "~A property of a point in the field.",
    "~Per unit charge: exists with no charge there.",
    "~Measured in volts.",
    "%Potential energy",
    "-A property of a charge at that point, or of the pair of charges.",
    "-Total energy: depends on the size and sign of the charge placed there.",
    "-Measured in joules.",
    "!Potential energy is the charge times the potential. A negative charge "
    "at a positive potential has negative potential energy.",
    NULL
};

static const char *const cmpLines[] = {
    "@Field lines vs equipotentials",
    "The two sets of lines you draw for every field. They always cross at "
    "right angles.",
    "%Field lines",
    "~Show the direction of the force on a positive charge.",
    "~Closer together: stronger field.",
    "~Start on positive charges, end on negative charges.",
    "~Never cross each other.",
    "%Equipotentials",
    "-Join points at the same potential.",
    "-Closer together, for equal steps: stronger field.",
    "-Always cross field lines at right angles.",
    "-No work is done moving a charge along one.",
    "!Point charge: radial lines and concentric circles. Parallel plates: "
    "parallel lines and planes parallel to the plates.",
    NULL
};

static const char *const cmpInduction[] = {
    "@Charging by contact vs charging by induction",
    "Both charge a conductor; the difference is whether the charged object "
    "touches it.",
    "%By contact",
    "~The objects touch.",
    "~Electrons pass directly between the two objects.",
    "~The conductor ends with the SAME sign as the charged object.",
    "~The charged object loses some of its charge.",
    "%By induction",
    "-The charged object never touches the conductor.",
    "-Electrons flow to or from earth through a temporary earth connection.",
    "-The conductor ends with the OPPOSITE sign to the charged object.",
    "-The charged object keeps all its charge.",
    "!Friction charges two insulators equally and oppositely; contact shares "
    "charge; induction needs earthing. In all three, only electrons move.",
    NULL
};

static const char *const cmpConductor[] = {
    "@Conductor vs insulator",
    "The difference is whether there are free charge carriers.",
    "%Conductor",
    "~Many free, delocalized electrons.",
    "~Charge flows easily; low resistivity.",
    "~Charge put on it spreads over its surface.",
    "~Examples: metals, graphite.",
    "%Insulator",
    "-Electrons bound to their atoms.",
    "-Almost no current flows; very high resistivity.",
    "-Charge put on it stays where it is put.",
    "-Examples: plastic, glass, rubber.",
    "!That is why insulators are charged by friction, and conductors by "
    "contact or induction.",
    NULL
};

static const char *const cmpResistivity[] = {
    "@Resistance vs resistivity",
    "Resistance belongs to one object; resistivity belongs to the material "
    "it is made of.",
    "%Resistance",
    "~A property of a particular component.",
    "~Depends on length, cross-sectional area, material and temperature.",
    "~Measured in ohms.",
    "~The ratio of p.d. to current.",
    "%Resistivity",
    "-A property of the material alone.",
    "-The same for any size or shape of sample at the same temperature.",
    "-Measured in ohm metres.",
    "-The resistance of a sample of unit length and unit cross-sectional "
    "area.",
    "!Double the length: the resistance doubles. Double the area: it halves. "
    "The resistivity stays the same either way.",
    NULL
};

static const char *const cmpOhmic[] = {
    "@Ohmic vs non-ohmic device",
    "The difference is whether the resistance stays constant as the current "
    "changes.",
    "%Ohmic",
    "~Obeys Ohm's law: p.d. directly proportional to current.",
    "~Constant resistance.",
    "~V-I graph: a straight line through the origin.",
    "~Example: a metal wire at constant temperature.",
    "%Non-ohmic",
    "-Does not obey Ohm's law.",
    "-Resistance changes with the current.",
    "-V-I graph: curved.",
    "-Example: a filament lamp.",
    "!For a non-ohmic device, the resistance at a point is the p.d. over the "
    "current at that point, never the gradient of the graph.",
    NULL
};

static const char *const cmpSeries[] = {
    "@Series vs parallel",
    "Two ways to connect components, with opposite rules for current and "
    "p.d.",
    "%Series",
    "~One path: the same current through every component.",
    "~The p.d.s add up to the supply p.d.",
    "~Total resistance is the sum: more than any one resistor.",
    "~One break stops everything.",
    "%Parallel",
    "-Separate branches: the branch currents add up to the total.",
    "-The same p.d. across every branch.",
    "-Total resistance is less than the smallest branch.",
    "-Each branch works independently.",
    "!Both follow from Kirchhoff's laws: currents at a junction (charge) and "
    "p.d.s round a loop (energy).",
    NULL
};

static const char *const cmpMeters[] = {
    "@Ammeter vs voltmeter",
    "Opposite connections and opposite ideal resistances.",
    "%Ammeter",
    "~Measures current.",
    "~Connected in series.",
    "~Ideal: zero resistance.",
    "~Must not change the current it measures.",
    "%Voltmeter",
    "-Measures potential difference.",
    "-Connected in parallel, across the component.",
    "-Ideal: infinite resistance.",
    "-Must draw no current from the circuit.",
    "!Swap them and the circuit fails: a voltmeter in series stops the "
    "current; an ammeter in parallel short-circuits the component.",
    NULL
};

static const char *const cmpEmf[] = {
    "@EMF vs terminal potential difference",
    "Both are energy per coulomb in volts; the difference is the lost volts "
    "inside the cell.",
    "%EMF",
    "~Work per unit charge driving charge completely round the circuit.",
    "~Energy converted INTO electrical energy per coulomb.",
    "~Fixed by the cell's chemistry.",
    "~Equal to the terminal p.d. when no current flows.",
    "%Terminal p.d.",
    "-Energy per coulomb delivered to the external circuit.",
    "-Energy converted FROM electrical energy outside the cell.",
    "-Falls as more current is drawn.",
    "-The EMF minus the lost volts.",
    "!When a battery is being charged, the terminal p.d. is bigger than the "
    "EMF, not smaller.",
    NULL
};

static const char *const cmpCells[] = {
    "@Primary cell vs secondary cell",
    "The difference is whether the chemical reaction can be reversed.",
    "%Primary",
    "~Single use: cannot be recharged.",
    "~The reaction cannot be reversed.",
    "~Thrown away when used up.",
    "~Example: an alkaline cell.",
    "%Secondary",
    "-Rechargeable many times.",
    "-The reaction is reversed by driving current backwards through it.",
    "-Charged from a supply whose p.d. exceeds its EMF.",
    "-Example: a lithium-ion or lead-acid cell.",
    "!Both have an EMF and an internal resistance, and both show the same "
    "shape of discharge curve.",
    NULL
};

static const char *const cmpConventional[] = {
    "@Conventional current vs electron flow",
    "The same current described in two opposite directions.",
    "%Conventional current",
    "~The direction positive charge would flow.",
    "~Positive terminal to negative, round the external circuit.",
    "~Used for all circuit rules and right-hand rules.",
    "%Electron flow",
    "-The actual movement of electrons in a metal.",
    "-Negative terminal to positive, round the external circuit.",
    "-Opposite to conventional current.",
    "!In a magnetic force question with an electron beam, reverse the "
    "direction your right-hand rule gives.",
    NULL
};

static const char *const cmpDisplacement[] = {
    "@Distance vs displacement",
    "A scalar and its vector partner. Speed and velocity split the same way.",
    "%Distance",
    "~Total length of the path travelled.",
    "~Scalar.",
    "~Can never decrease.",
    "~Rate of change: speed.",
    "%Displacement",
    "-Straight-line distance from the start, in a stated direction.",
    "-Vector.",
    "-Can be zero after a round trip.",
    "-Rate of change: velocity.",
    "!After one lap of a circular track, the distance is the circumference "
    "but the displacement is zero.",
    NULL
};

static const char *const cmpWeight[] = {
    "@Mass vs weight",
    "Mass is how much matter; weight is the pull of gravity on it.",
    "%Mass",
    "~Amount of matter; measures inertia.",
    "~Scalar, in kilograms.",
    "~The same everywhere.",
    "%Weight",
    "-The gravitational force on the mass.",
    "-Vector, in newtons, towards the planet's centre.",
    "-Changes with the gravitational field strength.",
    "!On the Moon an astronaut has the same mass but about a sixth of the "
    "weight.",
    NULL
};

static const char *const cmpCollision[] = {
    "@Elastic vs inelastic collision",
    "Momentum is conserved in both; only kinetic energy tells them apart.",
    "%Elastic",
    "~Momentum conserved.",
    "~Kinetic energy conserved.",
    "~The objects bounce apart.",
    "~Example: ideal gas molecules.",
    "%Inelastic",
    "-Momentum conserved.",
    "-Kinetic energy NOT conserved: some becomes heat, sound or deformation.",
    "-Totally inelastic: the objects stick together.",
    "-Example: a car crash.",
    "!Total energy is conserved in every collision. Only kinetic energy can "
    "be lost.",
    NULL
};

static const char *const cmpHeat[] = {
    "@Temperature vs thermal energy",
    "Temperature is about the average particle; thermal energy is energy "
    "moving because of a temperature difference.",
    "%Temperature",
    "~Measures the average kinetic energy of the particles.",
    "~Does not depend on the amount of substance.",
    "~Kelvin or degrees Celsius.",
    "~Decides which way thermal energy flows.",
    "%Thermal energy",
    "-Energy transferred because of a temperature difference.",
    "-Depends on the amount of substance.",
    "-Joules.",
    "-Flows from hot to cold until the temperatures are equal.",
    "!A warm bath holds far more internal energy than a spark, though the "
    "spark is much hotter.",
    NULL
};

static const char *const cmpWaves[] = {
    "@Transverse vs longitudinal wave",
    "The difference is which way the medium oscillates compared with the way "
    "the energy travels.",
    "%Transverse",
    "~Oscillation perpendicular to the energy transfer.",
    "~Crests and troughs.",
    "~Can be polarized.",
    "~Examples: light and all EM waves, waves on a string.",
    "%Longitudinal",
    "-Oscillation parallel to the energy transfer.",
    "-Compressions and rarefactions.",
    "-Cannot be polarized.",
    "-Examples: sound, seismic P-waves.",
    "!Both transfer energy without transferring matter.",
    NULL
};

static const char *const cmpFission[] = {
    "@Fission vs fusion",
    "Both release energy by moving nuclei towards higher binding energy per "
    "nucleon.",
    "%Fission",
    "~A heavy nucleus splits into two lighter ones.",
    "~Triggered by absorbing a neutron.",
    "~Releases neutrons, so a chain reaction is possible.",
    "~Used in nuclear power stations.",
    "%Fusion",
    "-Light nuclei join into a heavier one.",
    "-Needs extreme temperature and density to overcome electrostatic "
    "repulsion.",
    "-Powers the stars.",
    "-More energy per kilogram of fuel.",
    "!On the binding energy per nucleon curve, fission moves down from the "
    "heavy end and fusion up from the light end, both towards iron.",
    NULL
};

static const char *const cmpErrors[] = {
    "@Random error vs systematic error",
    "Random errors spoil precision; systematic errors spoil accuracy.",
    "%Random error",
    "~Scatters readings either side of the true value.",
    "~Reduced by repeating and averaging.",
    "~Affects precision.",
    "~Shown by the spread of points and the size of error bars.",
    "%Systematic error",
    "-Shifts every reading the same way.",
    "-Not reduced by repeating.",
    "-Affects accuracy.",
    "-Shown by a graph that misses the expected intercept.",
    "!A zero error is systematic: a line that should pass through the origin "
    "is shifted up or down.",
    NULL
};

#define VS " vs "

const section_t sections[N_SECTIONS] = {
    {"Charge & Coulomb", charge},
    {"Field & potential", fields},
    {"Plates & Millikan", plates},
    {"Current & Ohm", current},
    {"Circuits & meters", circuits},
    {"Cells & EMF", cells},
    {"Sketches", sketch},
    {"EMF & r lab", lab},
    {"Motion & energy", themeA},
    {"Matter & heat", themeB},
    {"Wave behaviour", themeC},
    {"Fields", themeD},
    {"Atoms & nuclei", themeE},
    {"Tools & tips", tools},
    {"Electric" VS "gravity", cmpGravity},
    {"Field strength" VS "potential", cmpFieldPot},
    {"Potential" VS "PE", cmpPotEnergy},
    {"Field lines" VS "equipotentials", cmpLines},
    {"Contact" VS "induction", cmpInduction},
    {"Conductor" VS "insulator", cmpConductor},
    {"Resistance" VS "resistivity", cmpResistivity},
    {"Ohmic" VS "non-ohmic", cmpOhmic},
    {"Series" VS "parallel", cmpSeries},
    {"Ammeter" VS "voltmeter", cmpMeters},
    {"EMF" VS "terminal p.d.", cmpEmf},
    {"Primary" VS "secondary cell", cmpCells},
    {"Conventional" VS "electron flow", cmpConventional},
    {"Distance" VS "displacement", cmpDisplacement},
    {"Mass" VS "weight", cmpWeight},
    {"Elastic" VS "inelastic", cmpCollision},
    {"Temperature" VS "heat", cmpHeat},
    {"Transverse" VS "longitudinal", cmpWaves},
    {"Fission" VS "fusion", cmpFission},
    {"Random" VS "systematic error", cmpErrors},
};

/* the topic each comparison starts, NULL to continue the previous one */
const char *const compareTopic[N_COMPARE] = {
    "ELECTROSTATICS", NULL, NULL, NULL, NULL,
    "ELECTRIC CURRENTS", NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    "OTHER UNITS", NULL, NULL, NULL, NULL, NULL, NULL,
};
