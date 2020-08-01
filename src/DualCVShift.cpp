#include "CAOPlugs.hpp"

#include <sstream>
#include <iomanip>
#include <math.h>

struct DualCVShift : Module {
    enum ParamIds {
	SHIFT_0 = 0,
	SCALE_0,
	SHIFT_1,
	SCALE_1,
	NUM_PARAMS
    };

    enum InputIds {
	SHIFT_CV_0 = 0,
	VOCT_0,
	SHIFT_CV_1,
	VOCT_1,
	NUM_INPUTS
    };

    enum OutputIds {
	OUTPUT_0 = 0,
	OUTPUT_1,
	NUM_OUTPUTS
    };

    DualCVShift() {
	config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS);
	configParam(SHIFT_0, -10, 10, 0, "Shift", "Hz");
	configParam(SCALE_0, 0, 3, 0, "Scale","",10);
	configParam(SHIFT_1, -10, 10, 0, "Shift", "Hz");
	configParam(SCALE_1, 0, 3, 0, "Scale","",10);
    }

    void process(const ProcessArgs &args) override;
};

// Frequency corresponding to -10V
static float min_freq = 440*exp2f(-10.75);

void DualCVShift::process(const ProcessArgs &args) {
    float shift;

    int channels = inputs[VOCT_0].getChannels();
    if (channels > 0) {
	shift = params[SHIFT_0].getValue() + inputs[SHIFT_CV_0].getVoltage();
	switch ((int)params[SCALE_0].getValue()) {
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
	outputs[OUTPUT_0].setChannels(channels);

	for (int channel = 0; channel < channels; channel++) {
	    float freq = shift +
		440*exp2f(inputs[VOCT_0].getVoltage(channel) - 0.75);

	    float volts = freq <= min_freq ? -10 : log2f(freq/440)+0.75;
	    outputs[OUTPUT_0].setVoltage(volts, channel);
	}
    }	
};


struct DualCVShiftWidget : ModuleWidget {
    DualCVShiftWidget(DualCVShift *module);
};

DualCVShiftWidget::DualCVShiftWidget(DualCVShift *module) {
    setModule(module);
    box.size = Vec(15 * 3, 380);

    setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/DualCVShift.svg")));

    addParam(createParam<white_Knob>(mm2px(Vec(2.5, 8)), module, DualCVShift::SHIFT_0));
    addInput(createInput<input_Port>(mm2px(Vec(3.4, 21.9)), module, DualCVShift::SHIFT_CV_0));
    addParam(createParam<white_Switch>(mm2px(Vec(1.8, 36)), module, DualCVShift::SCALE_0));
    addInput(createInput<input_Port>(mm2px(Vec(3.4, 43.3)), module, DualCVShift::VOCT_0));
    addOutput(createOutput<output_Port>(mm2px(Vec(3.4, 53.3)), module, DualCVShift::OUTPUT_0));

#define NEXT 59.6
    
    addParam(createParam<white_Knob>(mm2px(Vec(2.5, NEXT+8)), module, DualCVShift::SHIFT_1));
    addInput(createInput<input_Port>(mm2px(Vec(3.4, NEXT+21.9)), module, DualCVShift::SHIFT_CV_1));
    addParam(createParam<white_Switch>(mm2px(Vec(1.8, NEXT+36)), module, DualCVShift::SCALE_1));
    addInput(createInput<input_Port>(mm2px(Vec(3.4, NEXT+43.3)), module, DualCVShift::VOCT_1));
    addOutput(createOutput<output_Port>(mm2px(Vec(3.4, NEXT+53.3)), module, DualCVShift::OUTPUT_1));
}

Model *modelDualCVShift =
    createModel<DualCVShift, DualCVShiftWidget>("DualCVShift");
