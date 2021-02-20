#include "CAOPlugs.hpp"

#include <sstream>
#include <iomanip>
#include <math.h>

struct CLatch : Module {
    enum ParamIds {
	LATCH_SW,
	NUM_PARAMS
    };

    enum InputIds {
	LATCH_IN,
	POLY_VOCT_IN,
	POLY_GATE_IN,
	TRANSPOSE_VOCT_IN,
	TRANSPOSE_GATE_IN,
	NUM_INPUTS
    };

    enum OutputIds {
	POLY_VOCT_OUT,
	POLY_GATE_OUT,
	NUM_OUTPUTS
    };

    int OutVOctCount = 0, OutGateCount = 0;
    int TransposeCtrl = 0;
    float LatchedGates[PORT_MAX_CHANNELS];
    float LatchedVOcts[PORT_MAX_CHANNELS];

    CLatch() {
	config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS);
    }

    void process(const ProcessArgs &args) override;
};

void CLatch::process(const ProcessArgs &args) {
    bool latched = params[LATCH_SW].value;
    if (inputs[LATCH_IN].isConnected()) {
	latched = inputs[LATCH_IN].getVoltage() > 0;
	params[LATCH_SW].value = latched;
    }

    if (not latched) {
	int channels = inputs[POLY_GATE_IN].getChannels();
	OutGateCount = channels;
	outputs[POLY_GATE_OUT].setChannels(channels);

	for (int channel = 0; channel < channels; channel++) {
	    LatchedGates[channel] = inputs[POLY_GATE_IN].getVoltage(channel);
	    outputs[POLY_GATE_OUT].setVoltage(LatchedGates[channel], channel);
	}

	channels = inputs[POLY_VOCT_IN].getChannels();
	OutVOctCount = channels;
	outputs[POLY_VOCT_OUT].setChannels(channels);

	float BottomNote = 1000;
	TransposeCtrl = 0;
	for (int channel = 0; channel < channels; channel++) {
	    if (channel >= OutGateCount || !LatchedGates[channel])
		continue;
	    float thisNote = inputs[POLY_VOCT_IN].getVoltage(channel);
	    if (thisNote < BottomNote)
		BottomNote = thisNote;
	    LatchedVOcts[channel] = inputs[POLY_VOCT_IN].getVoltage(channel);
	    outputs[POLY_VOCT_OUT].setVoltage(thisNote, channel);
	}
	for (int channel = 0; channel < channels; channel++)
	    LatchedVOcts[channel] -= BottomNote;

    } else {
	bool monoGateOn = inputs[TRANSPOSE_GATE_IN].getVoltage() > 0;
	if (!monoGateOn && TransposeCtrl == 0)
	    TransposeCtrl = 1;
	else if (monoGateOn && TransposeCtrl == 1)
	    TransposeCtrl = 2;

	outputs[POLY_VOCT_OUT].setChannels(OutVOctCount);
	if (TransposeCtrl == 2) {
	    float TransposeVOct = inputs[TRANSPOSE_VOCT_IN].getVoltage();
	    for (int channel = 0; channel < OutVOctCount; channel++) {
		float newVOct = LatchedVOcts[channel] + TransposeVOct;
		outputs[POLY_VOCT_OUT].setVoltage(newVOct, channel);
	    }
	}
	outputs[POLY_GATE_OUT].setChannels(OutGateCount);
	if (monoGateOn)
	    for (int channel = 0; channel < OutGateCount; channel++)
		outputs[POLY_GATE_OUT].setVoltage(LatchedGates[channel],
						  channel);
	else
	    for (int channel = 0; channel < OutGateCount; channel++)
		outputs[POLY_GATE_OUT].setVoltage(0, channel);
    }
}


struct CLatchWidget : ModuleWidget {
    CLatchWidget(CLatch *module);
};

CLatchWidget::CLatchWidget(CLatch *module) {
    setModule(module);
    box.size = Vec(15 * 3, 380);

    setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/CLatch.svg")));

    addInput(createInput<input_Port>(mm2px(Vec(5.8, 9.5)), module, CLatch::LATCH_IN));
    addParam(createParam<white_Button>(mm2px(Vec(6.6, 21)), module, CLatch::LATCH_SW));

#define SPACING(X,Y) (41+28*X+10*Y)

    addInput(createInput<input_Port>(mm2px(Vec(5.8, SPACING(0,0))), module, CLatch::POLY_VOCT_IN));
    addInput(createInput<input_Port>(mm2px(Vec(5.8, SPACING(0,1))), module, CLatch::POLY_GATE_IN));

    addInput(createInput<input_Port>(mm2px(Vec(5.8, SPACING(1,0))), module, CLatch::TRANSPOSE_VOCT_IN));
    addInput(createInput<input_Port>(mm2px(Vec(5.8, SPACING(1,1))), module, CLatch::TRANSPOSE_GATE_IN));

    addOutput(createOutput<output_Port>(mm2px(Vec(5.8, SPACING(2,0))), module, CLatch::POLY_VOCT_OUT));
    addOutput(createOutput<output_Port>(mm2px(Vec(5.8, SPACING(2,1))), module, CLatch::POLY_GATE_OUT));

}



Model *modelCLatch =
    createModel<CLatch, CLatchWidget>("CLatch");
