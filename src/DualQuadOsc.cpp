#include "CAOPlugs.hpp"

#include <sstream>
#include <iomanip>
#include <math.h>

struct DualQuadOsc : Module {
    enum ParamIds {
	FREQ_0,
	FREQ_1,
	NUM_PARAMS
    };

    enum InputIds {
	VOCT_0,
	PHASE_0,
	VOCT_1,
	PHASE_1,
	NUM_INPUTS
    };

    enum OutputIds {
	SIN_0,
	COS_0,
	SIN_1,
	COS_1,
	NUM_OUTPUTS
    };

    DualQuadOsc() {
	config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS);
	configParam(FREQ_0, -54.f, 54.f, 0.f, "Frequency", "Hz", dsp::FREQ_SEMITONE, dsp::FREQ_C4);
	configParam(FREQ_1, -54.f, 54.f, 0.f, "Frequency", "Hz", dsp::FREQ_SEMITONE, dsp::FREQ_C4);
    }

    void process(const ProcessArgs &args) override;
    float phase0=0, phase1=0;
};

void DualQuadOsc::process(const ProcessArgs &args) {
    float pitch, delta, theta;
    pitch = params[FREQ_0].getValue() / 12 + inputs[VOCT_0].getVoltage();
    delta = 261.6256 * powf(2, pitch) * args.sampleTime;
    phase0 += delta;
    if (phase0 > 1)
	phase0 -= 1;
    theta = 2 * M_PI * (phase0 + inputs[PHASE_0].getVoltage()/10);
    outputs[SIN_0].setVoltage(5*sinf(theta));
    outputs[COS_0].setVoltage(5*cosf(theta));

    float old_pitch = pitch;
    pitch = params[FREQ_1].getValue() / 12 + inputs[VOCT_1].getVoltage();
    delta = 261.6256 * powf(2, pitch) * args.sampleTime;
    // If the frequencies are the same
    if (old_pitch != pitch || phase0 == phase1)
	phase1 += delta;
    // If the frequencies are the same, slowly sync up phases.
    else if (phase0 != phase1) {
	phase1 += 0.995 * delta;
 	if (fabsf(phase1 - phase0) <= delta)
	    phase1 = phase0;
    }
	
    if (phase1 > 1)
	phase1 -= 1;

    
    theta = 2 * M_PI * (phase1 + inputs[PHASE_1].getVoltage()/10);
    outputs[SIN_1].setVoltage(5*sinf(theta));
    outputs[COS_1].setVoltage(5*cosf(theta));
};


struct DualQuadOscWidget : ModuleWidget {
    DualQuadOscWidget(DualQuadOsc *module);
};


#define ADD_QUADOSC(OFFSET, FREQ, PHASE, VOCT, SIN, COS)		\
    do {								\
	addParam(createParam<white_Knob>(mm2px(Vec(4.7, OFFSET+7)), module, DualQuadOsc::FREQ)); \
	addInput(createInput<input_Port>(mm2px(Vec(5.6, OFFSET+20.6)), module, DualQuadOsc::VOCT)); \
	addInput(createInput<input_Port>(mm2px(Vec(5.6, OFFSET+30.6)), module, DualQuadOsc::PHASE)); \
	addOutput(createOutput<output_Port>(mm2px(Vec(5.6, OFFSET+43.3)), module, DualQuadOsc::SIN)); \
	addOutput(createOutput<output_Port>(mm2px(Vec(5.6, OFFSET+53.3)), module, DualQuadOsc::COS)); \
    } while (0);


DualQuadOscWidget::DualQuadOscWidget(DualQuadOsc *module) {
    setModule(module);
    box.size = Vec(20 * 3, 380);

    setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/DualQuadOsc.svg")));

    ADD_QUADOSC(0, FREQ_0, PHASE_0, VOCT_0, SIN_0, COS_0);
    ADD_QUADOSC(58, FREQ_1, PHASE_1, VOCT_1, SIN_1, COS_1);
}

Model *modelDualQuadOsc =
    createModel<DualQuadOsc, DualQuadOscWidget>("DualQuadOsc");
