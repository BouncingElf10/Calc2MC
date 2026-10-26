#include <stddef.h>
#include "content.h"

/* IB Physics HL: definitions in words. The first six pages are the
 * magnetic fields and induction test; their '=' lines are formulas the
 * data booklet does not give. The electrostatics and currents test before
 * it is condensed into Matter & heat and Fields; quoted definitions there
 * are the teacher's exact wording. */

/* ---- test unit: magnetic fields, motion in fields, induction ---- */

static const char *const magnet[] = {
    "!Field patterns and right-hand rules only: no formulas are needed for "
    "these. How to draw each pattern is on the Sketches page.",
    "#Magnetic fields",
    "@Magnetic field",
    "A region of space in which a moving charge, a current-carrying "
    "conductor or a magnetic material experiences a force.",
    "@Magnetic field lines",
    "Show the direction of the force on a north pole placed in the field. "
    "They run from north to south outside a magnet and form closed loops; "
    "they never cross, and closer lines mean a stronger field.",
    "@Magnetic poles",
    "The regions of a magnet where the field is strongest. Like poles repel, "
    "unlike poles attract. Poles always come in north-south pairs: cut a "
    "magnet in half and you get two smaller magnets.",
    "@Field of a bar magnet",
    "Lines leave the north pole, curve round and enter the south pole, then "
    "continue through the magnet from south to north to close the loop. "
    "Strongest, with the lines most crowded, at the poles.",
    "@Earth's magnetic field",
    "Like the field of a huge bar magnet whose south pole lies near the "
    "geographic North Pole. That is why the north pole of a compass points "
    "north.",
    "@Compass",
    "A small magnet free to turn. Its north pole points along the field "
    "lines, so it maps out the direction of the field.",
    "@Magnetic flux density",
    "The strength of a magnetic field: the force per unit length per unit "
    "current on a conductor placed at right angles to the field. Symbol B, "
    "unit the tesla. A vector, along the field lines.",
    "@Tesla",
    "The flux density that gives a force of one newton on each metre of a "
    "wire carrying one ampere at right angles to the field.",
    "#Fields from currents",
    "@Oersted's discovery",
    "An electric current in a wire produces a magnetic field around it. "
    "Oersted saw a compass needle near a wire swing when the current was "
    "switched on, and swing the other way when the current was reversed.",
    "!Every magnetic field comes from moving charge: a current in a wire, or "
    "electrons spinning and orbiting inside the atoms of a magnet.",
    "@Field of a straight wire",
    "Concentric circles centred on the wire, in planes at right angles to "
    "it. The circles get further apart with distance because the field gets "
    "weaker: it is proportional to the current and inversely proportional "
    "to the distance from the wire.",
    "@Right-hand grip rule",
    "For a straight wire: grip the wire with your right hand, thumb along "
    "the conventional current. Your curled fingers show the direction of the "
    "magnetic field lines.",
    "@Field of a solenoid",
    "Inside: strong, uniform, straight parallel lines along the axis. "
    "Outside: the same shape as a bar magnet's field, with one end acting as "
    "a north pole and the other as a south pole.",
    "@Right-hand rule for a coil",
    "For a solenoid or circular coil: curl the fingers of your right hand "
    "the way the conventional current goes round the turns. Your thumb "
    "points along the field inside, towards the north pole end.",
    "@Clock rule",
    "Look at one end of a solenoid. Current flowing anticlockwise: that end "
    "is a north pole. Clockwise: a south pole.",
    "@Field of a circular coil",
    "Near the wire, circles round each side of the coil, in opposite "
    "senses. At the centre the lines are straight and run along the axis. "
    "The whole pattern looks like the field of a short, flat bar magnet.",
    "@Air-core solenoid",
    "A solenoid with nothing inside it. More turns per unit length or more "
    "current make its field stronger; an iron core would make it much "
    "stronger.",
    "@Electromagnet",
    "A solenoid, usually with a soft iron core, whose field can be switched "
    "on and off and controlled by the current.",
    "#Two straight wires",
    "@Resultant field of two wires",
    "The fields of the two wires add as vectors at every point "
    "(superposition). Near each wire its own circles dominate; far away the "
    "pattern looks like the field of a single wire.",
    "@Currents in the same direction",
    "Between the wires their fields point opposite ways and partly cancel. "
    "For equal currents there is a neutral point midway, where the field is "
    "zero. Further out, field lines loop round both wires together.",
    "@Currents in opposite directions",
    "Between the wires their fields point the same way and add, so the "
    "field there is strong and the lines are crowded. Outside, the fields "
    "partly cancel.",
    "@Dot and cross convention",
    "A dot means a current or field coming out of the page, towards you "
    "(the tip of an arrow). A cross means going into the page (the tail "
    "feathers of an arrow).",
    NULL
};

static const char *const forces[] = {
    "!Your teacher uses the left-hand rule for every force direction and "
    "right-hand rules only for field directions.",
    "!Booklet: F = qvB sin theta, F = BIL sin theta and F/L = mu0 I1 I2 / "
    "(2 pi r). Theta is the angle between the velocity or current and the "
    "field.",
    "#Force on a moving charge",
    "@Lorentz force",
    "The force on a charge moving through a magnetic field. It is at right "
    "angles to both the velocity and the field.",
    "@Size of the force on a charge",
    "Proportional to the charge, its speed, the flux density and the sine "
    "of the angle between the velocity and the field. Greatest when the "
    "charge moves at right angles to the field; zero when it moves along the "
    "field.",
    "!A charge at rest, or moving parallel to the field lines, feels no "
    "magnetic force at all.",
    "@Fleming's left-hand rule",
    "Hold the thumb and first two fingers of your left hand at right angles. "
    "First finger: Field (north to south). seCond finger: Current "
    "(conventional). thuMb: the force, and so the Motion.",
    "@Force on a negative charge",
    "Use the left-hand rule with the second finger pointing OPPOSITE to the "
    "electron's velocity, because a moving negative charge is a current the "
    "other way. The force comes out reversed.",
    "@Magnetic force does no work",
    "The force is always at right angles to the velocity, so it changes the "
    "direction of motion but never the speed. The kinetic energy of a "
    "charged particle stays constant in a magnetic field.",
    "#The motor effect",
    "@Motor effect",
    "A current-carrying conductor in a magnetic field feels a force. The "
    "force acts on the moving charges inside the wire and is passed on to "
    "the wire itself.",
    "@Size of the force on a wire",
    "Proportional to the flux density, the current, the length of wire in "
    "the field and the sine of the angle between the wire and the field. "
    "Greatest when the wire is at right angles to the field; zero when it "
    "is parallel.",
    "@Direction of the force on a wire",
    "Fleming's left-hand rule with the second finger along the conventional "
    "current. Reverse the current or the field and the force reverses; "
    "reverse both and it does not.",
    "@Why the motor effect happens",
    "The field of the wire and the field of the magnet add on one side of "
    "the wire and partly cancel on the other. The wire is pushed from the "
    "strong-field side towards the weak-field side.",
    "@Measuring flux density",
    "Hold a known length of wire at right angles to the field, pass a known "
    "current and measure the force, for example with the magnet on a "
    "top-pan balance. The force per unit current per unit length is the "
    "flux density.",
    "@Electric motor",
    "A coil carrying current in a magnetic field: the forces on its two "
    "sides are equal and opposite and turn it. A split-ring commutator "
    "reverses the current every half turn to keep it turning the same way.",
    "#Parallel wires",
    "@Force between parallel wires",
    "Each wire sits in the magnetic field of the other, so each feels a "
    "motor-effect force. Currents in the same direction attract; currents in "
    "opposite directions repel.",
    "@Force per unit length between wires",
    "Directly proportional to the product of the two currents and inversely "
    "proportional to their separation, not its square. The constant is the "
    "permeability of free space divided by two pi.",
    "!The forces on the two wires are a Newton's third law pair: equal and "
    "opposite, even when one wire carries much more current than the other.",
    "@Finding the direction for two wires",
    "Use the right-hand grip rule to find the field of wire 1 where wire 2 "
    "is, then Fleming's left-hand rule on wire 2. It always gives: like "
    "currents attract, unlike currents repel.",
    "@Permeability of free space",
    "The constant that sets how strong a magnetic field a current makes in "
    "a vacuum: four pi times ten to the minus 7 tesla metres per ampere. "
    "Air has almost the same value.",
    "@Definition of the ampere",
    "One ampere is the constant current which, flowing in two straight, "
    "parallel, infinitely long wires of negligible cross-section placed one "
    "metre apart in a vacuum, produces a force of 2 times ten to the minus 7 "
    "newtons per metre of length on each wire.",
    "=F/L = mu0 (1)(1) / (2 pi " TIMES " 1) = 2 " TIMES " 10^-7 N/m",
    "!Learn every condition: straight, parallel, infinitely long, negligible "
    "cross-section, one metre apart, vacuum, per metre of length. Since 2019 "
    "the SI fixes the ampere through the elementary charge, but this is the "
    "definition the study guide asks for.",
    NULL
};

static const char *const motion[] = {
    "!Electric fields speed charges up or slow them down; magnetic fields "
    "only turn them.",
    "!Booklet: W = qV, E = V/d, F = qE, F = qvB sin theta, Ek = 1/2 mv^2, "
    "F = mv^2/r. The formulas below are not in it; the Derivations page "
    "shows where each comes from.",
    "#Getting charges moving",
    "@Accelerating a charge",
    "A charge moving through a potential difference gains kinetic energy "
    "equal to the work done on it: its charge times the p.d. Starting from "
    "rest, that is all its kinetic energy.",
    "@Electronvolt",
    "The energy gained by one electron when it is accelerated through a "
    "potential difference of one volt; about 1.6 times ten to the minus 19 "
    "joules.",
    "=1 eV = 1.6 " TIMES " 10^-19 J",
    "!Energy in electronvolts is the charge in units of e times the p.d. in "
    "volts: an electron through 500 V gains 500 eV; an alpha particle, with "
    "twice the charge, gains 1000 eV.",
    "@Electron gun",
    "A heated filament releases electrons (thermionic emission), and a large "
    "p.d. between the filament and a positive anode accelerates them. They "
    "pass through a hole in the anode as a narrow beam.",
    "@Speed from the accelerating p.d.",
    "Set the charge times the p.d. equal to half the mass times the speed "
    "squared. Doubling the p.d. multiplies the speed by the square root of "
    "two, not by two.",
    "=v = sqrt(2qV / m)",
    "#In a uniform electric field",
    "@Field between parallel plates",
    "Uniform between the plates: the same strength everywhere, equal to the "
    "p.d. across the plates divided by their separation. It points from the "
    "positive plate to the negative plate.",
    "@Force in a uniform electric field",
    "The charge times the field strength: the same size and direction "
    "everywhere between the plates, along the field lines for a positive "
    "charge and against them for a negative charge.",
    "@Parabolic motion",
    "A charge fired between plates at right angles to the field keeps a "
    "constant velocity along the plates and has a constant acceleration "
    "across them. Its path is a parabola, exactly like a horizontally thrown "
    "projectile.",
    "@Working out a deflection",
    "The time between the plates is the plate length divided by the entry "
    "speed. The acceleration across is the force over the mass. The "
    "sideways deflection is half the acceleration times the time squared.",
    "=t = L / v,   a = qE / m",
    "=y = 1/2 a t^2 = qEL^2 / (2mv^2)",
    "@After leaving the plates",
    "Outside the field there is no force, so the charge carries on in a "
    "straight line in the direction it had at the edge of the plates.",
    "@Charge fired along the field",
    "It speeds up or slows down in a straight line. An electron fired "
    "towards the negative plate slows down, stops and comes back if the p.d. "
    "is big enough.",
    "!Gravity on an electron or ion is far too small to matter next to the "
    "electric force. Ignore it unless told otherwise.",
    "#In a uniform magnetic field",
    "@Circular motion in a magnetic field",
    "A charge moving at right angles to a uniform field feels a force of "
    "constant size, always at right angles to its velocity. That force is "
    "the centripetal force, so the charge moves in a circle at constant "
    "speed.",
    "@Radius of the circle",
    "Set the magnetic force equal to the centripetal force. The radius is "
    "the momentum divided by the charge times the flux density: faster or "
    "heavier particles make bigger circles; a bigger charge or a stronger "
    "field makes smaller ones.",
    "=r = mv / (qB) = p / (qB)",
    "@Period in a magnetic field",
    "The time for one circle does not depend on the speed: a faster particle "
    "goes round a bigger circle in the same time. It depends only on the "
    "mass, the charge and the flux density.",
    "=T = 2 pi m / (qB)",
    "@Helical motion",
    "A charge moving at an angle to the field: split its velocity into "
    "parts along and across the field. The part across the field gives a "
    "circle; the part along the field is unaffected. Together they make a "
    "helix, or corkscrew, along the field lines.",
    "=along: v cos theta,   across: v sin theta",
    "=r = mv sin theta / (qB)",
    "@Pitch of a helix",
    "The distance moved along the field in one turn: the velocity component "
    "along the field times the period. The radius comes from the component "
    "across the field only.",
    "=pitch = v cos theta " TIMES " T = 2 pi mv cos theta / (qB)",
    "#Reading a particle's path",
    "@Sign of the charge from its path",
    "Use the left-hand rule for a positive charge at one point on the path. "
    "If the path curves the way the rule says, the particle is positive; if "
    "it curves the other way, negative.",
    "@Charge-to-mass ratio",
    "The charge of a particle divided by its mass, in coulombs per "
    "kilogram. Particles with the same speed in the same field move in "
    "circles with radius inversely proportional to it.",
    "@Charge-to-mass ratio from a circular path",
    "Accelerate the particles through a known p.d. into a known uniform "
    "magnetic field at right angles to it, and measure the radius of the "
    "circle. Combining the energy gained with the radius gives the ratio: "
    "two times the p.d. divided by the flux density squared times the "
    "radius squared.",
    "=q/m = 2V / (B^2 r^2)",
    "@Fine-beam tube",
    "The lab version for electrons: an electron gun fires a beam into a "
    "glass bulb of low-pressure gas, which glows along the path. Helmholtz "
    "coils give a uniform field, and the diameter of the glowing circle is "
    "read from a scale.",
    "@Spiral track",
    "A charged particle passing through matter or a gas loses energy, so its "
    "speed and its radius fall. A spiral track in a bubble chamber shows "
    "which way the particle was going.",
    "#Velocity selector",
    "!Both devices below use an electric and a magnetic field at right "
    "angles. The velocity selector is the first stage of the Bainbridge "
    "mass spectrometer.",
    "@Velocity selector",
    "A region where a uniform electric field and a uniform magnetic field "
    "are at right angles to each other and to the beam. Only particles with "
    "one particular speed go straight through.",
    "@Balanced forces in a selector",
    "The electric force and the magnetic force point in opposite "
    "directions. At one speed they are equal, there is no resultant force, "
    "and the particle goes straight through undeflected.",
    "@Selected speed",
    "The electric field strength divided by the magnetic flux density. It "
    "does not depend on the charge, its sign or the mass.",
    "=v = E / B",
    "!The charge cancels: the same selector passes electrons, protons and "
    "ions of the same speed, positive or negative.",
    "@Too fast or too slow",
    "Faster than the selected speed: the magnetic force wins and the "
    "particle is deflected the way the magnetic force points. Slower: the "
    "electric force wins and it is deflected the other way. Either way it "
    "misses the exit slit.",
    "@Choosing the field directions",
    "Pick the electric field direction and find the electric force on the "
    "charge, then use the left-hand rule to choose a magnetic field that "
    "pushes the opposite way.",
    "!The electric force does not depend on speed; the magnetic force grows "
    "with speed. That is why only one speed balances.",
    "#Bainbridge mass spectrometer",
    "@Mass spectrometer",
    "A device that separates ions by their charge-to-mass ratio, used to "
    "measure the masses of atoms and the abundance of isotopes.",
    "@Ion source",
    "The sample is vaporized and ionized, usually by knocking electrons off "
    "its atoms with an electron beam, and the positive ions are accelerated "
    "through a p.d. into the next stage.",
    "@Bainbridge stages",
    "Ions are accelerated, pass through a velocity selector so they all have "
    "the same speed, then enter a region of uniform magnetic field alone, at "
    "right angles to their velocity. They travel a semicircle and land on a "
    "detector or photographic plate.",
    "@Radius in the spectrometer",
    "With the speed fixed by the selector and the same deflecting field for "
    "every ion, the radius is directly proportional to the mass divided by "
    "the charge.",
    "=r = mv / (qB2)",
    "@Ratio from the spectrometer",
    "The charge-to-mass ratio equals the selected speed divided by the "
    "deflecting flux density times the radius. The ion lands one diameter, "
    "twice the radius, from the entry slit.",
    "=q/m = v / (B2 r) = E / (B1 B2 r)",
    "B1 is the selector's field, B2 the deflecting field.",
    "@Separating isotopes",
    "Isotopes of an element with the same charge leave the selector at the "
    "same speed but have different masses, so the heavier isotope makes the "
    "bigger semicircle and lands further from the slit. How strong each "
    "spot is shows how common that isotope is.",
    "=m1 / m2 = r1 / r2",
    "!An ion with twice the charge and the same mass lands at half the "
    "distance, so it can be mistaken for an ion of half the mass.",
    "@Why a velocity selector first",
    "Without it the ions would have a spread of speeds, and the radius "
    "depends on speed too, so ions of the same mass would land in different "
    "places and blur the result.",
    "@Vacuum in a spectrometer",
    "The whole spectrometer is evacuated so the ions do not hit air "
    "molecules, which would scatter them and change their speed.",
    NULL
};

static const char *const induct[] = {
    "!Faraday gives the size of the induced EMF, Lenz the direction. The "
    "coil-through-a-field problem is on the Maxi problem page.",
    "!Booklet: Phi = BA cos theta, emf = -N (change in Phi) / (change in t) "
    "and emf = BvL.",
    "#Magnetic flux",
    "@Magnetic flux",
    "The flux density times the area it passes through, times the cosine of "
    "the angle between the field and the normal to the area. Unit: the "
    "weber.",
    "!The angle is between the field and the NORMAL to the coil. Field "
    "straight through the coil face: maximum flux. Field along the plane of "
    "the coil: zero flux.",
    "@Weber",
    "The unit of magnetic flux: one tesla times one square metre.",
    "=1 Wb = 1 T m^2",
    "@Magnetic flux linkage",
    "The magnetic flux through a coil times its number of turns. Unit: the "
    "weber, or weber-turns.",
    "=N Phi = NBA cos theta",
    "@Changing flux linkage",
    "Flux linkage changes if the flux density changes, the area in the "
    "field changes, or the angle between the coil and the field changes.",
    "=field changing: emf = NA (change in B) / t",
    "=area changing: emf = NB (change in A) / t",
    "#Faraday and Lenz",
    "@Electromagnetic induction",
    "The production of an EMF in a conductor when the magnetic flux linking "
    "it changes, or when it cuts across magnetic field lines. If the circuit "
    "is complete, a current flows.",
    "@Faraday's law",
    "The induced EMF in a circuit is equal to the rate of change of magnetic "
    "flux linkage through the circuit.",
    "!Rate of change: the change in flux linkage divided by the time it "
    "takes. Twice as fast, twice the EMF. No change, no EMF, however strong "
    "the field.",
    "@Lenz's law",
    "The induced EMF acts in a direction such that any current it drives "
    "opposes the change in flux that produced it.",
    "@Lenz's law and conservation of energy",
    "If the induced current helped the change instead of opposing it, the "
    "change would speed itself up and make electrical energy from nothing. "
    "Because it opposes, work must be done against the opposing force, and "
    "that work becomes the electrical energy.",
    "!The minus sign in Faraday's law is Lenz's law.",
    "@Using Lenz's law",
    "Ask what is changing: more flux or less, and which way. The induced "
    "current makes its own field against an increase, or along the field to "
    "make up a decrease. The right-hand rule for a coil then gives the "
    "current direction.",
    "@Magnet pushed into a coil",
    "The end of the coil facing the magnet becomes a like pole and repels "
    "it, so you have to push. Pulling the magnet out, that end becomes an "
    "unlike pole and attracts it, so you have to pull.",
    "@Fleming's right-hand rule",
    "For a wire moving through a field: First finger Field, thuMb Motion of "
    "the wire, seCond finger the induced Current. It always agrees with "
    "Lenz's law.",
    "#Ways to induce an EMF",
    "@Magnet and coil",
    "A magnet moved into or out of a coil connected to a galvanometer gives "
    "a deflection. Faster movement, a stronger magnet or more turns give a "
    "bigger EMF; reversing the motion or the pole reverses it; no motion, "
    "no EMF.",
    "@Magnet oscillating on a spring",
    "A magnet bouncing on a spring in and out of a coil induces an "
    "alternating EMF. With the coil in a closed circuit the induced current "
    "opposes the motion, so the oscillation dies away faster: "
    "electromagnetic damping.",
    "@Coil moved into or out of a field",
    "While part of the coil is entering or leaving the field the flux "
    "linkage changes and an EMF is induced. Fully inside a uniform field, "
    "moving does not change the flux, so there is no EMF.",
    "@Time-varying magnetic field",
    "A coil held still near an electromagnet whose current changes, by "
    "switching on or off or with an alternating current, has a changing "
    "flux linkage and so an induced EMF. This is how a transformer works.",
    "@Coil rotating in a field",
    "Turning a coil in a uniform field keeps changing the angle, and so the "
    "flux linkage, inducing an alternating EMF. See the AC generator page.",
    "#Motional EMF",
    "@EMF in a moving straight conductor",
    "A straight conductor moving at right angles to a uniform field has an "
    "EMF equal to the flux density times its length times its speed.",
    "=emf = BLv,   a coil side of N turns: NBLv",
    "!You must be able to derive this: three ways are on the Derivations "
    "page.",
    "@Energy for a moving rod",
    "If the rod is part of a closed circuit, the induced current in the "
    "field gives a motor-effect force against the motion. The mechanical "
    "power needed to keep the rod moving at constant speed equals the "
    "electrical power produced.",
    "=I = BLv / R,   F = BIL = B^2 L^2 v / R",
    "=P = Fv = emf " TIMES " I = (BLv)^2 / R",
    "@Eddy currents",
    "Currents induced in a solid piece of metal moving in a field or sitting "
    "in a changing field. By Lenz's law they oppose the motion (magnetic "
    "braking), and they heat the metal.",
    "#Self-induction (qualitative)",
    "@Self-induction",
    "A changing current in a coil changes its own flux linkage, which "
    "induces an EMF in the same coil. By Lenz's law it opposes the change "
    "in current.",
    "@Back EMF",
    "The EMF from self-induction. When a circuit with a coil is switched "
    "on, it slows the rise of the current; when it is switched off, it tries "
    "to keep the current flowing and can make a spark at the switch.",
    NULL
};

static const char *const gen[] = {
    "!A rectangular coil turning at a steady rate in a uniform field gives a "
    "sine-wave EMF. The flux and EMF graphs are on the Sketches page.",
    "!None of the formulas on this page are in the booklet. The EMF is "
    "derived on the Derivations page.",
    "#The AC generator",
    "@Alternating current generator",
    "A coil of wire turned in a uniform magnetic field. As it turns, its "
    "flux linkage keeps changing, so an alternating EMF is induced.",
    "@Slip rings and brushes",
    "Each end of the coil connects to its own ring, which turns with it. "
    "Carbon brushes press on the rings and connect the turning coil to the "
    "outside circuit, so the output stays alternating.",
    "@Sinusoidal EMF",
    "The flux linkage varies as the cosine of the angle turned, so its rate "
    "of change, the EMF, varies as a sine. The EMF goes through one full "
    "cycle for each turn of the coil.",
    "=N Phi = BAN cos(omega t)",
    "=emf = BAN omega sin(omega t)",
    "@Peak EMF",
    "The flux density times the coil's area times the number of turns times "
    "the angular speed. Bigger in a stronger field, with a bigger coil, with "
    "more turns, or turning faster.",
    "=peak emf = BAN omega = 2 pi f BAN",
    "@Effect of rotation frequency",
    "Doubling the rotation frequency doubles the peak EMF, because the flux "
    "linkage changes twice as fast. It also doubles the frequency of the "
    "output, halving its period.",
    "!On the EMF-time graph, turning twice as fast gives peaks twice as tall "
    "and twice as close together. Examiners look for both.",
    "@Angular speed",
    "The angle turned per second, in radians per second: two pi times the "
    "rotation frequency.",
    "=omega = 2 pi f = 2 pi / T",
    "#Flux and EMF out of step",
    "@Coil face at right angles to the field",
    "Flux linkage is at its maximum. The sides of the coil move along the "
    "field lines instead of cutting them, so for that instant the flux is "
    "not changing and the EMF is zero.",
    "@Coil plane parallel to the field",
    "Flux linkage is zero. The sides of the coil cut straight across the "
    "field lines, so the flux is changing fastest and the EMF is at its "
    "peak.",
    "@Phase difference of flux and EMF",
    "The EMF is a quarter of a cycle, 90 degrees, out of phase with the flux "
    "linkage: the EMF is zero when the flux is greatest and greatest when "
    "the flux is zero.",
    "!The EMF is minus the gradient of the flux linkage-time graph. Where "
    "the flux graph is steepest the EMF peaks; at the top or bottom of the "
    "flux graph the EMF is zero.",
    "#Getting more out",
    "@Increasing a generator's output",
    "Use stronger magnets or an electromagnet, more turns, a bigger coil "
    "area or a soft iron core inside the coil, or turn it faster.",
    "@Generator and load",
    "With nothing connected no current flows and the coil turns freely. "
    "Draw a current and, by Lenz's law, the motor-effect force on the coil "
    "opposes its rotation, so more work is needed to turn it.",
    "!A generator converts kinetic energy into electrical energy; it does "
    "not create energy. A motor does the reverse.",
    "@Induction and industrialization",
    "Faraday's discovery in 1831 led to generators that turned the energy "
    "of steam turbines and falling water into electricity, and to "
    "transformers that sent it over long distances. Factories no longer had "
    "to sit beside a river or their own steam engine.",
    NULL
};

static const char *const derive[] = {
    "!Each one starts from booklet formulas. For the marks: write the "
    "starting equation, say why it holds, then rearrange.",
    "#Motion in fields",
    "@Kinetic energy in a magnetic field",
    "=W = Fs cos 90 = 0",
    "The magnetic force is always at right angles to the velocity, so it "
    "does no work. The kinetic energy, and so the speed, cannot change.",
    "@Speed after an accelerating p.d.",
    "=qV = 1/2 mv^2",
    "=v = sqrt(2qV / m)",
    "The work done by the field becomes kinetic energy, starting from rest.",
    "@Deflection between plates",
    "=a = F / m = qE / m = qV / (md)",
    "=t = L / v",
    "=y = 1/2 a t^2 = qEL^2 / (2mv^2)",
    "Constant velocity along the plates and constant acceleration across "
    "them, as for a horizontally thrown projectile. L is the plate length.",
    "@Radius of the circular path",
    "=qvB = mv^2 / r",
    "=r = mv / (qB)",
    "The magnetic force provides the centripetal force.",
    "@Period of the circular path",
    "=T = 2 pi r / v = 2 pi m / (qB)",
    "Put the radius into the time for one circumference: the speed cancels, "
    "so the period does not depend on it.",
    "@Radius and pitch of a helix",
    "=r = m (v sin theta) / (qB)",
    "=pitch = (v cos theta) T = 2 pi mv cos theta / (qB)",
    "Only the velocity component across the field sets the radius; the "
    "component along it carries the particle forward for one period per "
    "turn.",
    "@Charge-to-mass ratio from the circle",
    "=qV = 1/2 mv^2   and   r = mv / (qB)",
    "=v = qBr / m,  so  qV = q^2 B^2 r^2 / (2m)",
    "=q/m = 2V / (B^2 r^2)",
    "Remove the speed between the energy equation and the radius equation.",
    "@Speed through a velocity selector",
    "=qE = qvB",
    "=v = E / B = V / (Bd)",
    "Undeflected means the electric and magnetic forces are equal and "
    "opposite. The charge cancels, so the speed does not depend on it.",
    "@Charge-to-mass ratio in the spectrometer",
    "=v = E / B1   and   r = mv / (qB2)",
    "=q/m = v / (B2 r) = E / (B1 B2 r)",
    "The selector fixes the speed; the second field alone bends the ions "
    "into a semicircle.",
    "#Wires",
    "@Force between wires from the field of a wire",
    "=B1 = mu0 I1 / (2 pi r)",
    "=F = B1 I2 L,  so  F/L = mu0 I1 I2 / (2 pi r)",
    "Wire 2 sits in the field of wire 1, at right angles to it, so a length "
    "L of wire 2 feels the motor-effect force.",
    "@Ampere from the force between wires",
    "=F/L = (4 pi " TIMES " 10^-7)(1)(1) / (2 pi " TIMES " 1)",
    "=F/L = 2 " TIMES " 10^-7 N/m",
    "Two currents of one ampere, one metre apart in a vacuum.",
    "#Induction",
    "!The EMF of a moving rod is the one your teacher says to be able to "
    "derive. Any of the three ways earns the marks.",
    "@Motional EMF from Faraday's law",
    "=in time t the rod moves vt",
    "=area swept = Lvt,   flux cut = BLvt",
    "=emf = BLvt / t = BLv",
    "The rod sweeps out an area, the flux through that area is cut, and the "
    "EMF is the flux cut per second.",
    "@Motional EMF from the force on charges",
    "=magnetic force on an electron: evB",
    "=balanced by the electric force: eE = evB,  so  E = vB",
    "=emf = EL = BLv",
    "The free electrons move with the rod and are pushed along it. They "
    "pile up at one end, setting up an electric field, until the electric "
    "force balances the magnetic force. The EMF is that field times the "
    "length.",
    "@Motional EMF from energy",
    "=force to keep the rod moving: F = BIL",
    "=mechanical power: Fv = BILv",
    "=electrical power: emf " TIMES " I",
    "=emf " TIMES " I = BILv,  so  emf = BLv",
    "By Lenz's law the induced current opposes the motion, so work must be "
    "done to keep the rod moving. Energy conservation makes the two powers "
    "equal.",
    "@EMF of a coil entering a field",
    "=in time t the flux linkage grows by NBLvt",
    "=emf = NBLvt / t = NBLv",
    "=I = NBLv / R,   F = NBIL",
    "L is the side cutting the field. Only that side feels a force, and it "
    "opposes the motion.",
    "@EMF of a rotating coil",
    "=each long side (length a) is b/2 from the axis",
    "=its speed: v = omega b / 2",
    "=its speed across the field: v sin(omega t)",
    "=emf = 2 " TIMES " N B a (omega b / 2) sin(omega t)",
    "=emf = BAN omega sin(omega t),   A = ab",
    "Two long sides cut the field and their EMFs add round the coil; the "
    "short sides add nothing. Time is measured from the coil face being at "
    "right angles to the field, when the sides move along the field lines.",
    "!Turn twice as fast: omega doubles, so the peak EMF doubles and the "
    "period halves.",
    NULL
};

/* ---- sketches and the maxi problem ---- */

static const char *const sketch[] = {
    "!You must be able to draw these. Each entry says what the examiner "
    "looks for.",
    "#Rules for field lines",
    "@Field line direction",
    "Electric: the way a positive test charge would be pushed. Magnetic: the "
    "way a north pole would be pushed, north to south outside a magnet.",
    "@Field line spacing",
    "Lines closer together mean a stronger field. Parallel, equally spaced "
    "lines mean a uniform field.",
    "@Field lines never cross",
    "The field at any point has only one direction, so two lines can never "
    "pass through the same point.",
    "@Electric and magnetic lines",
    "Electric field lines start on positive charges and end on negative "
    "ones. Magnetic field lines have no start or end: they are closed loops.",
    "#Electric fields",
    "@Isolated point charge",
    "Straight radial lines, evenly spaced all round: arrows outwards for a "
    "positive charge, inwards for a negative one.",
    "@Positive and negative charge",
    "Lines leave the positive charge and curve round into the negative, "
    "most tightly packed on the line between them.",
    "@Two like charges",
    "Lines leave both charges (or enter both) and bend away from each "
    "other; none cross the gap. For equal charges there is a neutral point "
    "midway.",
    "@Plate field lines",
    "Straight, parallel, evenly spaced lines from the positive plate to the "
    "negative plate, at right angles to both, curving outwards beyond the "
    "ends of the plates.",
    "#Magnetic fields",
    "@Bar magnet field lines",
    "Closed loops from the north pole round to the south pole, symmetrical "
    "about the magnet's axis and most crowded at the poles. Arrows point "
    "away from N and towards S.",
    "@Straight wire field lines",
    "Concentric circles round the wire, spaced further apart going outwards. "
    "Current out of the page (dot): anticlockwise. Into the page (cross): "
    "clockwise.",
    "@Solenoid field lines",
    "Straight, parallel, evenly spaced lines inside along the axis; outside, "
    "loops from one end round to the other like a bar magnet. Mark N and S "
    "with the right-hand rule.",
    "@Circular coil field lines",
    "Drawn as a cross-section: a dot on one side and a cross on the other. "
    "Small circles round each side in opposite senses, merging into "
    "straight lines through the centre of the coil.",
    "@Two wires, same current direction",
    "Circles round each wire close in; an empty gap midway (the neutral "
    "point) for equal currents; further out, loops round both wires "
    "together.",
    "@Two wires, opposite currents",
    "Circles round each wire, squashed together and crowded in the region "
    "between the wires, where the fields add.",
    "@Field into or out of the page",
    "A grid of evenly spaced crosses (into the page) or dots (out of the "
    "page) for a uniform field.",
    "#Paths of charges",
    "@Path between plates",
    "A parabola curving towards the plate of opposite sign, then a straight "
    "line after leaving the plates. Electrons bend towards the positive "
    "plate.",
    "@Path in a magnetic field",
    "An arc of a circle at constant speed, bending the way the left-hand "
    "rule gives. Opposite charges curve opposite ways; a heavier particle "
    "with the same speed and charge makes a bigger arc.",
    "@Path at an angle to a magnetic field",
    "A helix wound round the field lines, its axis along the field.",
    "@Path in a velocity selector",
    "Straight through at the selected speed; curving one way if faster, the "
    "other way if slower.",
    "#Generator graphs",
    "@Flux linkage against time",
    "A cosine curve, starting at a maximum when the coil face is at right "
    "angles to the field.",
    "@EMF against time",
    "A sine curve, a quarter of a cycle behind the flux graph: zero where "
    "the flux peaks, peaking where the flux is zero.",
    "@Faster rotation on the graph",
    "Twice the rotation frequency: twice as many EMF cycles in the same time "
    "and peaks twice as tall. The flux linkage peaks stay the same height.",
    NULL
};

static const char *const maxi[] = {
    "!A rectangular coil moved at constant speed into, through and out of a "
    "region of uniform field. Know every stage and both graphs.",
    "#Set-up",
    "A flat rectangular coil of several turns moves at constant speed "
    "towards a region of uniform field wider than the coil. The field is at "
    "right angles to the coil, and the leading side is at right angles to "
    "the motion.",
    "#Stage by stage",
    "@Before the coil reaches the field",
    "No flux links the coil and nothing changes: no EMF, no current, no "
    "force.",
    "@Coil entering the field",
    "The area of coil inside the field grows steadily, so the flux linkage "
    "rises at a constant rate. Only the leading side cuts field lines. The "
    "EMF is constant: the number of turns times the flux density times the "
    "length of the leading side times the speed.",
    "=emf = NBLv",
    "@Coil completely inside the field",
    "The flux linkage stays at its maximum, the number of turns times the "
    "flux density times the coil's area, and does not change. No EMF: the "
    "EMFs in the leading and trailing sides are equal and cancel.",
    "=N Phi = NBA",
    "@Coil leaving the field",
    "The flux linkage falls at a constant rate. Only the trailing side cuts "
    "field lines. The EMF has the same size as on entry but the opposite "
    "direction, and so does the current.",
    "@Coil out of the field",
    "No flux, no change, no EMF.",
    "#Graphs",
    "@Flux linkage-time graph",
    "A trapezium: a straight ramp up while entering, flat while fully "
    "inside, a straight ramp down while leaving, zero before and after.",
    "@EMF-time graph",
    "Rectangular pulses: a constant EMF while entering, zero while fully "
    "inside, an equal and opposite constant EMF while leaving. Each pulse "
    "lasts the coil's length divided by its speed.",
    "=pulse time = coil length / v",
    "!The EMF graph is minus the gradient of the flux graph: ramps give flat "
    "steps, flat parts give zero. Steeper ramp, taller step.",
    "@Field region narrower than the coil",
    "The leading side leaves the field before the trailing side enters. In "
    "between, the whole field region is inside the coil, so the flux is "
    "constant and the EMF is zero.",
    "@Faster coil",
    "The ramps are steeper and shorter, so the EMF pulses are taller and "
    "narrower. The area under each EMF pulse, the change in flux linkage, "
    "stays the same.",
    "#Direction, current and force",
    "@Current direction on entry",
    "The flux through the coil is increasing, so by Lenz's law the induced "
    "current makes a field the opposite way inside the coil. Use the "
    "right-hand rule for a coil, or Fleming's right-hand rule on the leading "
    "side.",
    "@Induced current",
    "The EMF divided by the coil's resistance. It flows only while the coil "
    "is entering or leaving.",
    "=I = NBLv / R",
    "@Force on the coil",
    "The current in the side inside the field feels a motor-effect force: "
    "the number of turns times the flux density times the current times "
    "the side's length. By Lenz's law it opposes the motion, both entering "
    "and leaving.",
    "=F = NBIL = N^2 B^2 L^2 v / R",
    "@Energy in the maxi problem",
    "To keep the speed constant an external force must balance the opposing "
    "force. The work it does becomes electrical energy, then heat in the "
    "coil: the force times the speed equals the EMF times the current.",
    "=Fv = emf " TIMES " I = (NBLv)^2 / R",
    "@Coil left to move freely",
    "With no pushing force, the opposing force slows the coil while it "
    "enters and leaves the field, so the EMF falls during each stage. Fully "
    "inside, nothing opposes it.",
    "!Checklist: flux zero, rising, constant, falling, zero. EMF zero, "
    "constant, zero, reversed, zero. The current reverses; the force does "
    "not.",
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
    "#Current and circuits",
    "!The p.d., Ohm's law and EMF definitions are your teacher's exact "
    "words.",
    "@Electric current",
    "The rate of flow of charge past a point in a circuit. Unit: the ampere, "
    "one coulomb per second.",
    "@Conventional current",
    "The direction positive charge would flow: from the positive terminal "
    "round the circuit to the negative. Electrons drift the opposite way.",
    "@Potential difference",
    "The work done per unit charge in moving a small positive test charge "
    "between the two points.",
    "@Electrical resistance",
    "The ratio of the potential difference across a component to the "
    "current through it. Unit: the ohm.",
    "@Resistivity",
    "The resistance of a sample of the material of unit length and unit "
    "cross-sectional area. Unit: the ohm metre.",
    "@Ohm's law",
    "For an ohmic device, the potential difference across the device is "
    "directly proportional to the current through the device.",
    "@Filament lamp",
    "Non-ohmic: as the current rises the filament heats up and its "
    "resistance increases, so its V-I graph curves.",
    "@Thermistor",
    "A semiconductor resistor whose resistance falls as its temperature "
    "rises (NTC), because heating frees more charge carriers.",
    "@Light-dependent resistor (LDR)",
    "A semiconductor resistor whose resistance falls as the light on it gets "
    "brighter, because absorbed photons free more charge carriers.",
    "@Electrical power",
    "The rate at which electrical energy is transferred in a component: the "
    "p.d. across it times the current through it.",
    "@Kirchhoff's first law",
    "The sum of the currents entering a junction equals the sum of the "
    "currents leaving it. It is based on conservation of charge.",
    "@Kirchhoff's second law",
    "Around any closed loop in a circuit, the sum of the EMFs equals the sum "
    "of the potential differences. It is based on conservation of energy.",
    "@Ideal meters",
    "An ideal ammeter, in series, has zero resistance; an ideal voltmeter, "
    "in parallel, has infinite resistance.",
    "@Potential divider",
    "Resistors in series across a supply, sharing its p.d. in proportion to "
    "their resistances. With a thermistor or LDR in it, it is a sensor "
    "circuit.",
    "@EMF (electromotive force)",
    "The work done per unit charge in driving a quantity of charge "
    "completely around the circuit.",
    "@Internal resistance",
    "The resistance of the materials inside the cell itself, which wastes "
    "some energy as heat inside the cell.",
    "@Terminal potential difference",
    "The p.d. across a cell's terminals while it delivers current: the EMF "
    "minus the lost volts across the internal resistance.",
    "@Measuring EMF and internal resistance",
    "Vary the current with a variable resistor and plot terminal p.d. "
    "against current: a falling straight line. The intercept on the p.d. "
    "axis is the EMF; the gradient is minus the internal resistance.",
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
    "!Magnetic fields and induction are on the test pages. Electric fields "
    "follow the same logic as gravity, with charge in place of mass.",
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
    "#Electric fields",
    "!The field strength and potential definitions are your teacher's exact "
    "words.",
    "@Electric charge",
    "A property of matter that makes it experience a force in an electric "
    "field. Like charges repel, unlike charges attract.",
    "@Elementary charge",
    "The size of the charge on one proton or one electron, about 1.6 times "
    "ten to the minus 19 coulombs. All free charge comes in whole-number "
    "multiples of it.",
    "@Conservation of charge",
    "The total charge of an isolated system stays constant. Charge can be "
    "moved but never created or destroyed.",
    "@Coulomb's law",
    "The electrostatic force between two point charges is directly "
    "proportional to the product of the charges and inversely proportional "
    "to the square of their separation.",
    "@Charging by friction",
    "Rubbing two different insulators together moves electrons from one to "
    "the other, leaving them equally and oppositely charged.",
    "@Charging by contact",
    "Touching a charged object to a conductor shares the charge, so both end "
    "up with charge of the same sign.",
    "@Charging by induction",
    "Hold a charged object near a conductor, earth the conductor, break the "
    "earth connection, then take the object away. The conductor is left "
    "with the opposite sign.",
    "@Electric field",
    "A region of space in which a charge experiences an electric force.",
    "@Electric field strength",
    "The force per unit charge acting on a small positive test charge placed "
    "at that point.",
    "@Electric potential",
    "The work done per unit charge in bringing a small positive test charge "
    "from infinity to the point.",
    "@Electric potential energy",
    "The work done in bringing a charge from infinity to a point in the "
    "field of another charge.",
    "@Potential gradient",
    "The rate of change of potential with distance. The field strength is "
    "minus the potential gradient.",
    "@Equipotential surface",
    "A surface of constant potential, at right angles to the field lines. No "
    "work is done moving a charge along it.",
    "@Millikan's oil-drop experiment",
    "Charged oil drops are held still between horizontal plates by "
    "adjusting the p.d., so the electric force balances their weight. Every "
    "drop's charge was a whole-number multiple of the elementary charge: "
    "charge is quantized.",
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

static const char *const cmpEB[] = {
    "@Charge in an electric field vs in a magnetic field",
    "Both push on charges; they differ in the direction of the force, what "
    "it does to the speed, and the path.",
    "%Uniform electric field",
    "~Force along the field lines.",
    "~Acts on a charge whether it moves or not.",
    "~Does work: changes the speed and kinetic energy.",
    "~Path across the field: a parabola.",
    "%Uniform magnetic field",
    "-Force at right angles to both the field and the velocity.",
    "-Acts only on a charge moving across the field.",
    "-Does no work: constant speed and kinetic energy.",
    "-Path across the field: a circle.",
    "!Crossed at right angles and balanced, the two fields make a velocity "
    "selector.",
    NULL
};

static const char *const cmpHands[] = {
    "@Left-hand rule vs right-hand grip rule",
    "One gives the direction of a force, the other the direction of a field.",
    "%Fleming's left-hand rule",
    "~Gives the FORCE on a current or moving charge in a field.",
    "~First finger field, second finger current, thumb force.",
    "~Used for motors, beams in fields and parallel wires.",
    "%Right-hand grip rule",
    "-Gives the direction of the FIELD made by a current.",
    "-Wire: thumb along the current, fingers show the field.",
    "-Coil: fingers round with the current, thumb points to north.",
    "!Both use conventional current. For electrons, point the current "
    "finger opposite to their motion.",
    NULL
};

static const char *const cmpWires[] = {
    "@Currents in the same direction vs opposite directions",
    "Two long parallel wires, each sitting in the other's field.",
    "%Same direction",
    "~The wires attract.",
    "~Fields partly cancel between the wires.",
    "~A neutral point between them: midway for equal currents.",
    "~Further out, field lines loop round both wires.",
    "%Opposite directions",
    "-The wires repel.",
    "-Fields add between the wires.",
    "-Strongest field between them.",
    "-No neutral point between them.",
    "!Like currents attract and unlike currents repel: the opposite of "
    "charges and magnetic poles.",
    NULL
};

static const char *const cmpCircle[] = {
    "@Circular vs helical motion",
    "Both happen at constant speed in a uniform magnetic field; the angle "
    "between the velocity and the field decides which.",
    "%Circle",
    "~Velocity at right angles to the field.",
    "~All of the velocity is turned by the force.",
    "~Stays in one plane.",
    "%Helix",
    "-Velocity at an angle to the field.",
    "-Only the part across the field is turned; the part along it is "
    "unchanged.",
    "-Drifts along the field lines as it circles: a corkscrew.",
    "!Moving exactly along the field there is no force at all, so the path "
    "is a straight line.",
    NULL
};

static const char *const cmpMagnet[] = {
    "@Bar magnet vs solenoid",
    "Outside, their fields have the same shape.",
    "%Bar magnet",
    "~Permanent: cannot be switched off.",
    "~Fixed field strength.",
    "~Fixed poles.",
    "%Solenoid",
    "-Field only while a current flows.",
    "-Stronger with more current, more turns or an iron core.",
    "-Reverse the current and the poles swap.",
    "-Uniform field inside.",
    "!Find a solenoid's poles with the right-hand rule: fingers round with "
    "the current, thumb points to north.",
    NULL
};

static const char *const cmpFlux[] = {
    "@Magnetic flux vs flux linkage",
    "Flux is through one loop; flux linkage counts every turn of the coil.",
    "%Flux",
    "~Flux density times area times cosine of the angle to the normal.",
    "~Through one turn.",
    "~Weber.",
    "%Flux linkage",
    "-Flux times the number of turns.",
    "-Through the whole coil.",
    "-Weber, or weber-turns.",
    "!Faraday's law uses the rate of change of flux LINKAGE: forget the "
    "number of turns and the EMF comes out too small.",
    NULL
};

static const char *const cmpFaraday[] = {
    "@Faraday's law vs Lenz's law",
    "Two halves of one rule about induced EMF.",
    "%Faraday's law",
    "~Gives the SIZE of the induced EMF.",
    "~EMF equals the rate of change of flux linkage.",
    "~Faster change, bigger EMF.",
    "%Lenz's law",
    "-Gives the DIRECTION of the induced EMF.",
    "-The induced current opposes the change that caused it.",
    "-Follows from conservation of energy.",
    "!Lenz's law is the minus sign in Faraday's law.",
    NULL
};

static const char *const cmpMotor[] = {
    "@Motor effect vs electromagnetic induction",
    "The same parts, with the energy going opposite ways.",
    "%Motor effect",
    "~Current in, motion out.",
    "~Electrical energy to kinetic energy.",
    "~Fleming's LEFT-hand rule.",
    "~Used in motors and loudspeakers.",
    "%Induction",
    "-Motion or a changing field in, EMF out.",
    "-Kinetic energy to electrical energy.",
    "-Fleming's RIGHT-hand rule, or Lenz's law.",
    "-Used in generators, microphones and transformers.",
    "!A generator supplying a current also feels a motor-effect force, "
    "which opposes its rotation. That is Lenz's law at work.",
    NULL
};

static const char *const cmpPhase[] = {
    "@Maximum flux vs maximum EMF in a generator",
    "They happen a quarter of a turn apart.",
    "%Maximum flux linkage",
    "~Coil face at right angles to the field.",
    "~Coil sides move along the field lines.",
    "~Flux not changing at that instant.",
    "~EMF zero.",
    "%Maximum EMF",
    "-Coil plane parallel to the field.",
    "-Coil sides cut straight across the field lines.",
    "-Flux changing fastest.",
    "-Flux linkage zero.",
    "!90 degrees out of phase: the EMF graph is minus the gradient of the "
    "flux graph.",
    NULL
};

static const char *const cmpEntering[] = {
    "@Coil entering vs leaving a field",
    "The two stages of the maxi problem that induce an EMF.",
    "%Entering",
    "~Flux linkage rising.",
    "~The leading side cuts the field.",
    "~Constant EMF in one direction.",
    "~The force opposes the motion.",
    "%Leaving",
    "-Flux linkage falling.",
    "-The trailing side cuts the field.",
    "-Constant EMF of the same size, reversed.",
    "-The force still opposes the motion.",
    "!Fully inside a uniform field there is no EMF at all, even though the "
    "coil is moving.",
    NULL
};

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
    "!For an electron beam in a magnetic field, point the current finger of "
    "the left-hand rule opposite to the beam.",
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
    {"Magnetic fields", magnet},
    {"Magnetic forces", forces},
    {"Charges in fields", motion},
    {"Flux & Faraday", induct},
    {"AC generator", gen},
    {"Derivations", derive},
    {"Sketches", sketch},
    {"Maxi problem", maxi},
    {"Motion & energy", themeA},
    {"Matter & heat", themeB},
    {"Wave behaviour", themeC},
    {"Fields", themeD},
    {"Atoms & nuclei", themeE},
    {"Tools & tips", tools},
    {"E-field" VS "B-field", cmpEB},
    {"Left hand" VS "right hand", cmpHands},
    {"Same" VS "opposite currents", cmpWires},
    {"Circle" VS "helix", cmpCircle},
    {"Bar magnet" VS "solenoid", cmpMagnet},
    {"Flux" VS "flux linkage", cmpFlux},
    {"Faraday" VS "Lenz", cmpFaraday},
    {"Motor" VS "generator", cmpMotor},
    {"Max flux" VS "max EMF", cmpPhase},
    {"Entering" VS "leaving", cmpEntering},
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
    "MAGNETISM", NULL, NULL, NULL, NULL,
    "INDUCTION", NULL, NULL, NULL, NULL,
    "OTHER UNITS", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
};
