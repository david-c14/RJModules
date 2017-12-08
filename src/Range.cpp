#include "RJModules.hpp"

#include "dsp/digital.hpp"
#include <iostream>
#include <cmath>
#include <sstream>
#include <iomanip>

/*
Thanks to Strum for the display widget!
*/

struct Range: Module {
    enum ParamIds {
        CH1_PARAM,
        CH2_PARAM,
        CH3_PARAM,
        CH4_PARAM,
        NUM_PARAMS
    };
    enum InputIds {
        CH1_INPUT,
        NUM_INPUTS
    };
    enum OutputIds {
        CH1_OUTPUT,
        NUM_OUTPUTS
    };
    enum LightIds {
        NUM_LIGHTS
    };

    float display1_val;
    float display2_val;
    float display3_val;
    float display4_val;

    Range() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}
    void step() override;
};

struct SmallNumberDisplayWidgeter : TransparentWidget {

  float *value;
  std::shared_ptr<Font> font;

  SmallNumberDisplayWidgeter() {
    font = Font::load(assetPlugin(plugin, "res/Segment7Standard.ttf"));
  };

  void draw(NVGcontext *vg) override
  {
    // Background
    NVGcolor backgroundColor = nvgRGB(0xC0, 0xC0, 0xC0);
    nvgBeginPath(vg);
    nvgRoundedRect(vg, 0.0, 0.0, box.size.x, box.size.y, 4.0);
    nvgFillColor(vg, backgroundColor);
    nvgFill(vg);

    // text
    nvgFontSize(vg, 24);
    nvgFontFaceId(vg, font->handle);
    nvgTextLetterSpacing(vg, 2.5);

    std::stringstream to_display;
    to_display << std::setprecision(2) << std::setw(1) << *value;

    Vec textPos = Vec(16.0f, 33.0f);
    NVGcolor textColor = nvgRGB(0x00, 0x00, 0x00);
    nvgFillColor(vg, textColor);
    nvgText(vg, textPos.x, textPos.y, to_display.str().c_str(), NULL);
  }
};

void Range::step() {

    display1_val = params[CH1_PARAM].value;
    display2_val = params[CH2_PARAM].value;
    display3_val = params[CH3_PARAM].value;
    display4_val = params[CH4_PARAM].value;

    outputs[CH1_OUTPUT].value = 0;

}

RangeWidget::RangeWidget() {
    Range *module = new Range();
    setModule(module);
    box.size = Vec(15*10, 380);

    {
        SVGPanel *panel = new SVGPanel();
        panel->box.size = box.size;
        panel->setBackground(SVG::load(assetPlugin(plugin, "res/Range.svg")));
        addChild(panel);
    }

    addChild(createScrew<ScrewSilver>(Vec(15, 0)));
    addChild(createScrew<ScrewSilver>(Vec(box.size.x-30, 0)));
    addChild(createScrew<ScrewSilver>(Vec(15, 365)));
    addChild(createScrew<ScrewSilver>(Vec(box.size.x-30, 365)));


    SmallNumberDisplayWidgeter *display = new SmallNumberDisplayWidgeter();
    display->box.pos = Vec(23, 60);
    display->box.size = Vec(50, 40);
    display->value = &module->display1_val;
    addChild(display);
    addParam(createParam<RoundBlackKnob>(Vec(28, 105), module, Range::CH1_PARAM, -12.0, 12.0, -12.0));

    SmallNumberDisplayWidgeter *display2 = new SmallNumberDisplayWidgeter();
    display2->box.pos = Vec(83, 60);
    display2->box.size = Vec(50, 40);
    display2->value = &module->display2_val;
    addChild(display2);
    addParam(createParam<RoundBlackKnob>(Vec(88, 105), module, Range::CH2_PARAM, -12.0, 12.0, 12.0));

    SmallNumberDisplayWidgeter *display3 = new SmallNumberDisplayWidgeter();
    display3->box.pos = Vec(23, 160);
    display3->box.size = Vec(50, 40);
    display3->value = &module->display3_val;
    addChild(display3);
    addParam(createParam<RoundBlackKnob>(Vec(28, 205), module, Range::CH3_PARAM, -12.0, 12.0, -12.0));

    SmallNumberDisplayWidgeter *display4 = new SmallNumberDisplayWidgeter();
    display4->box.pos = Vec(83, 160);
    display4->box.size = Vec(50, 40);
    display4->value = &module->display4_val;
    addChild(display4);
    addParam(createParam<RoundBlackKnob>(Vec(88, 205), module, Range::CH4_PARAM, -12.0, 12.0, 12.0));




    // addInput(createInput<PJ301MPort>(Vec(35, 123), module, Range::CH1_INPUT));
    // addOutput(createOutput<PJ301MPort>(Vec(95, 123), module, Range::CH1_OUTPUT));

    // SmallNumberDisplayWidgeter *display2 = new SmallNumberDisplayWidgeter();
    // display2->box.pos = Vec(28, 160);
    // display2->box.size = Vec(100, 40);
    // display2->value = &module->display2_val;
    // addChild(display2);

    // addInput(createInput<PJ301MPort>(Vec(35, 223), module, Range::CH2_INPUT));
    // addOutput(createOutput<PJ301MPort>(Vec(95, 223), module, Range::CH2_OUTPUT));

    // SmallNumberDisplayWidgeter *display3 = new SmallNumberDisplayWidgeter();
    // display3->box.pos = Vec(28, 260);
    // display3->box.size = Vec(100, 40);
    // display3->value = &module->display3_val;
    // addChild(display3);

    addInput(createInput<PJ301MPort>(Vec(35, 323), module, Range::CH1_INPUT));
    addOutput(createOutput<PJ301MPort>(Vec(95, 323), module, Range::CH1_OUTPUT));

}