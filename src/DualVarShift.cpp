#include "CAOPlugs.hpp"

#include <sstream>
#include <iomanip>
#include <math.h>

struct DualVarShift : Module {
    enum ParamIds {
	SHIFT_0 = 0,
	SHIFT_BASE_0,
	SCALE_0,
	SHIFT_1,
	SHIFT_BASE_1,
	SCALE_1,
	NUM_PARAMS
    };

    enum InputIds {
	VOCT_0,
	VOCT_1,
	NUM_INPUTS
    };

    enum OutputIds {
	OUTPUT_0 = 0,
	OUTPUT_1,
	NUM_OUTPUTS
    };

    DualVarShift() {
	config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS);
	configParam(SHIFT_0, -10, 10, 0, "Shift", "Hz");
	configParam(SHIFT_BASE_0, -10, 10, 0, "Shift Base");
	configParam(SCALE_0, 0, 3, 0, "Scale","",10);
	configParam(SHIFT_1, -10, 10, 0, "Shift", "Hz");
	configParam(SHIFT_BASE_1, -10, 10, 0, "Shift Base");
	configParam(SCALE_1, 0, 3, 0, "Scale","",10);
    }

    void process(const ProcessArgs &args) override;
    void do_shift(enum InputIds voct_in, enum OutputIds voct_out, enum ParamIds shift_knob, enum ParamIds shift_base_knob, enum ParamIds scale);
};

// Frequency corresponding to -10V
static float min_freq = 440*exp2f(-10.75);

void DualVarShift::do_shift(enum InputIds voct_in, enum OutputIds voct_out, enum ParamIds shift_knob, enum ParamIds shift_base_knob, enum ParamIds scale)
{
    int channels = inputs[voct_in].getChannels();
    if (channels > 0) {
	float shift = params[shift_knob].getValue();
	float shift_base = params[shift_base_knob].getValue();
	bool shift_sense = shift_base < 0;
	shift_base = 1 + (shift_sense ? -0.9 : 0.9) * shift_base;

	switch ((int)params[scale].getValue()) {
	case 1:
	    shift *= 10;
	    break;
	case 2:
	    shift *= 100;
	    break;
	case 3:
	    shift *= 1000;
	    break;
	}
	outputs[voct_out].setChannels(channels);

	for (int channel = 0; channel < channels; channel++) {
	    float voct = inputs[voct_in].getVoltage(channel) -0.75;
	    float freq = shift * powf(shift_base, shift_sense ? -voct : voct) +
		440*exp2f(voct);

	    float volts = freq <= min_freq ? -10 : log2f(freq/440)+0.75;
	    outputs[voct_out].setVoltage(volts, channel);
	}
    }	
}

void DualVarShift::process(const ProcessArgs &args) {
    do_shift(VOCT_0, OUTPUT_0, SHIFT_0, SHIFT_BASE_0, SCALE_0);
    do_shift(VOCT_1, OUTPUT_1, SHIFT_1, SHIFT_BASE_1, SCALE_1);
};


struct DualVarShiftWidget : ModuleWidget {
    DualVarShiftWidget(DualVarShift *module);
};


#define ADD_SHIFTER(OFFSET, SHIFT, SHIFT_BASE, SCALE, VOCT, OUTPUT)	\
    do {								\
	addParam(createParam<white_Knob>(mm2px(Vec(2.5, OFFSET+7)), module, DualVarShift::SHIFT)); \
	addParam(createParam<white_Knob>(mm2px(Vec(2.5, OFFSET+19)), module, DualVarShift::SHIFT_BASE)); \
	addParam(createParam<white_Switch>(mm2px(Vec(1.8, OFFSET+34)), module, DualVarShift::SCALE)); \
	addInput(createInput<input_Port>(mm2px(Vec(3.4, OFFSET+41.3)), module, DualVarShift::VOCT)); \
	addOutput(createOutput<output_Port>(mm2px(Vec(3.4, OFFSET+51.3)), module, DualVarShift::OUTPUT)); \
    } while (0);


DualVarShiftWidget::DualVarShiftWidget(DualVarShift *module) {
    setModule(module);
    box.size = Vec(15 * 3, 380);

    setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/DualVarShift.svg")));

    ADD_SHIFTER(0, SHIFT_0, SHIFT_BASE_0, SCALE_0, VOCT_0, OUTPUT_0);
    ADD_SHIFTER(58, SHIFT_1, SHIFT_BASE_1, SCALE_1, VOCT_1, OUTPUT_1);
}

Model *modelDualVarShift =
    createModel<DualVarShift, DualVarShiftWidget>("DualVarShift");
