DualCVShift:

The voltage on the Shift jack and the position of the knob are added.
That sum is scaled by 1, 10, 100, or 1000 depending on the position
of the scale switch.  This is the frequency displacement in Hz.  The V/Oct
silver jack is the input, and the shifted V/Oct comes out the gold.
Thus for a knob set to 4, with in shift voltage of -6, and the switch
in the third (100s) position, the frequency (V/Oct) is shifted by
100*(4-6) = -200Hz.

DualVarShift:

This is similar to DualCVShift, but there are no control inputs, but
there is a second knob which causes a variation in the beat by pitch.
If set to 0, it behave in there is no variation; if set to 2, then
the beat frequency doubles for each octave up like a pair of oscillators
with a fixed detuning.  For other positive values, the beats per octave
go up exponentially per octave for the given value.  If set to negative
values, the absolute value is used as the base, but exponent is negative.
I.e. for a value of say -2, the beat halves for each octave up.

AddAll:

Take a single voltage (Maybe a V/Oct), and produce up to four sums with
four independent voltages.
