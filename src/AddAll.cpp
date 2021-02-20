#include "CAOPlugs.hpp"

#include <sstream>
#include <iomanip>
#include <math.h>

struct AddAll : Module {
    enum ParamIds {
	NUM_PARAMS = 0
    };

    enum InputIds {
	COMMON = 0,
	INPUT_0,
	INPUT_1,
	INPUT_2,
	INPUT_3,
	NUM_INPUTS
    };

    enum OutputIds {
	OUTPUT_0 = 0,
	OUTPUT_1,
	OUTPUT_2,
	OUTPUT_3,
	NUM_OUTPUTS
    };

    AddAll() {
	config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS);
    }

    void process(const ProcessArgs &args) override;
};

void AddAll::process(const ProcessArgs &args) {
    float commonV = inputs[COMMON].getVoltage();
    
    outputs[OUTPUT_0].setVoltage(inputs[INPUT_0].getVoltage() + commonV);
    outputs[OUTPUT_1].setVoltage(inputs[INPUT_1].getVoltage() + commonV);
    outputs[OUTPUT_2].setVoltage(inputs[INPUT_2].getVoltage() + commonV);
    outputs[OUTPUT_3].setVoltage(inputs[INPUT_3].getVoltage() + commonV);
};


struct AddAllWidget : ModuleWidget {
    AddAllWidget(AddAll *module);
};


#define ADD_SHIFTER(NUMBER, INPUT, OUTPUT)	\
    do {								\
	addInput(createInput<input_Port>(mm2px(Vec(3.4, NUMBER*24.3+26.3)), module, AddAll::INPUT)); \
	addOutput(createOutput<output_Port>(mm2px(Vec(3.4, NUMBER*24.3+36.3)), module, AddAll::OUTPUT)); \
    } while (0);


AddAllWidget::AddAllWidget(AddAll *module) {
    setModule(module);
    box.size = Vec(15 * 3, 380);

    setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/AddAll.svg")));
    addInput(createInput<input_Port>(mm2px(Vec(3.4, 9.7)), module, AddAll::COMMON));
    ADD_SHIFTER(0, INPUT_0, OUTPUT_0);
    ADD_SHIFTER(1, INPUT_1, OUTPUT_1);
    ADD_SHIFTER(2, INPUT_2, OUTPUT_2);
    ADD_SHIFTER(3, INPUT_3, OUTPUT_3);
}

Model *modelAddAll =
    createModel<AddAll, AddAllWidget>("AddAll");
