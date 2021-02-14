#pragma once

#include <rack.hpp>
using namespace rack;

extern Plugin *pluginInstance;

extern Model *modelFIFO;
extern Model *modelCLatch;
extern Model *modelDualCVShift;
extern Model *modelDualVarShift;
extern Model *modelAddAll;
extern Model *modelDualPBFixer;

struct output_Port : app::SvgPort {
    output_Port() {
	setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/OutputPort.svg")));
    }
};

struct input_Port : app::SvgPort {
    input_Port() {
	setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/InputPort.svg")));
    }
};

struct white_Button: app::SvgSwitch {
    white_Button() {
	shadow->opacity = 0.0f;
	addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "./res/SqrButton-up.svg")));
	addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "./res/SqrButton-down.svg")));
    }
};

struct white_Knob : app::SvgKnob {
    white_Knob() {
	minAngle = -.9 * M_PI;
	maxAngle = 0.9 * M_PI;
	setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/KnobWhite.svg")));
    }
};

struct white_SnapKnob : app::SvgKnob {
    white_SnapKnob() {
	snap = true;
	minAngle = -.9 * M_PI;
	maxAngle = 0.9 * M_PI;
	setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/KnobWhite.svg")));
    }
};

struct white_Switch:  app::SVGSwitch {
    white_Switch() {
	addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "./res/SwitchWhite_0.svg")));
	addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "./res/SwitchWhite_1.svg")));
	addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "./res/SwitchWhite_2.svg")));
	addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "./res/SwitchWhite_3.svg")));
	};

    void onChange(const event::Change &e) override {
	SVGSwitch::onChange(e);
	int current = paramQuantity->getValue();
	paramQuantity->setValue(current);
    };
};
