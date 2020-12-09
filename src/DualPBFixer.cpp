#include "CAOPlugs.hpp"

#include <sstream>
#include <iomanip>
#include <math.h>

struct DualPBFixer : Module {
    enum ParamIds {
	LOWER_0 = 0,
	UPPER_0,
	LOWER_1,
	UPPER_1,
	NUM_PARAMS
    };

    enum InputIds {
	BEND_IN_0,
	BEND_IN_1,
	NUM_INPUTS
    };

    enum OutputIds {
	BEND_OUT_0 = 0,
	BEND_OUT_1,
	NUM_OUTPUTS
    };

    DualPBFixer() {
	config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS);
	configParam(UPPER_0, -24, 24, 0, "Upper limit");
	configParam(LOWER_0, -24, 24, 0, "Lower limit");
	configParam(UPPER_1, -24, 24, 0, "Upper limit");
	configParam(LOWER_1, -24, 24, 0, "Lower limit");
    }

    void process(const ProcessArgs &args) override;
    void do_scale(enum InputIds bend_in, enum OutputIds bend_out, enum ParamIds lower_range, enum ParamIds upper_range);
};

void DualPBFixer::do_scale(enum InputIds bend_in, enum OutputIds bend_out, enum ParamIds lower_range, enum ParamIds upper_range)
{
    float bend_val = inputs[bend_in].getVoltage(bend_in)/(5.0*12.0);
    enum ParamIds bend_type = bend_val < 0 ? lower_range : upper_range;
    outputs[bend_out].setVoltage(fabs(bend_val) * params[bend_type].getValue());
}

void DualPBFixer::process(const ProcessArgs &args) {
    float shift;

    do_scale(BEND_IN_0, BEND_OUT_0, LOWER_0, UPPER_0);
    do_scale(BEND_IN_0, BEND_OUT_0, LOWER_0, UPPER_0);
};


struct DualPBFixerWidget : ModuleWidget {
    DualPBFixerWidget(DualPBFixer *module);
};


#define ADD_SHIFTER(OFFSET, LOWER, UPPER, BEND_IN, BEND_OUT)			\
    do {								\
	addParam(createParam<white_Knob>(mm2px(Vec(2.5, OFFSET+7)), module, DualPBFixer::UPPER)); \
	addParam(createParam<white_Knob>(mm2px(Vec(2.5, OFFSET+24)), module, DualPBFixer::LOWER)); \
	addInput(createInput<input_Port>(mm2px(Vec(3.4, OFFSET+41.3)), module, DualPBFixer::BEND_IN)); \
	addOutput(createOutput<output_Port>(mm2px(Vec(3.4, OFFSET+51.3)), module, DualPBFixer::BEND_OUT)); \
    } while (0);


DualPBFixerWidget::DualPBFixerWidget(DualPBFixer *module) {
    setModule(module);
    box.size = Vec(15 * 3, 380);

    setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/DualPBFixer.svg")));

    ADD_SHIFTER(0, LOWER_0, UPPER_0, BEND_IN_0, BEND_OUT_0);
    ADD_SHIFTER(58, LOWER_1, UPPER_1, BEND_IN_1, BEND_OUT_1);
}

Model *modelDualPBFixer =
    createModel<DualPBFixer, DualPBFixerWidget>("DualPBFixer");
