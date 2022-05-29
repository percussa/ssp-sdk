// see header file for license 

#include "PluginProcessor.h"
#include "PluginEditor.h"

PluginEditor::PluginEditor(PluginProcessor &p)
    : AudioProcessorEditor(&p), processor(p) {
    for (int i = 0; i < nScopes; i++) {
        Oscilloscope *o = new Oscilloscope(
            processor.inBuffer, processor.lock, i);
        o->setInfo(String("In") + String(i + 1));
        o->setInfoCol(Colours::white);
        addAndMakeVisible(o);
        in.add(o);
    }

    for (int i = 0; i < nScopes; i++) {
        Oscilloscope *o = new Oscilloscope(
            processor.outBuffer, processor.lock, i);
        o->setInfoCol(Colours::red);
        addAndMakeVisible(o);
        out.add(o);
    }

    out[0]->setInfo(String("Out1=In1*In2"));
    out[1]->setInfo(String("Out2=-In1*In2"));
    out[2]->setInfo(String("Out3=In3*In4"));
    out[3]->setInfo(String("Out4=-In3*In4"));
    out[4]->setInfo(String("Out5=In5*In6"));
    out[5]->setInfo(String("Out6=-In5*In6"));
    out[6]->setInfo(String("Out7=In7*In8"));
    out[7]->setInfo(String("Out8=-In7*In8"));

    setSize(1600, 480);
    startTimer(50);
}

PluginEditor::~PluginEditor() {
}

void PluginEditor::timerCallback() {
    for (int i = 0; i < nScopes; i++) {
        in[i]->repaint();
    }

    for (int i = 0; i < nScopes; i++) {
        out[i]->repaint();
    }

    // repaint our own canvas as well
    repaint();
}

void PluginEditor::paint(Graphics &g) {
    g.fillAll(Colours::black);

    // show plugin name in the background
    Font f(Font::getDefaultMonospacedFontName(),
           0.9f * getHeight(), Font::plain);
    g.setFont(f);
    g.setColour(Colour(30, 30, 30));

    int w = getWidth();
    int h = getHeight();

    g.drawFittedText(
        JucePlugin_Name,
        0, 0,
        w, h - 75,
        Justification::centred,
        1);

    // draw the parameters
    Font ft(Font::getDefaultMonospacedFontName(), 26, Font::plain);
    g.setFont(ft);
    g.setColour(Colours::red);
    int xt = 30, yt = 420, wt = 100, ht = 26, gapt = 230;
    g.drawText("Gain 1", xt, yt, wt, ht, Justification::centred);
    xt += gapt;
    g.drawText("Gain 2", xt, yt, wt, ht, Justification::centred);
    xt += gapt;
    g.drawText("Gain 3", xt, yt, wt, ht, Justification::centred);
    xt += gapt;
    g.drawText("Gain 4", xt, yt, wt, ht, Justification::centred);
    xt = 30;
    yt += 30;
    g.setColour(Colours::white);
    g.drawText(processor.params_.gain1.getCurrentValueAsText(), xt, yt, wt, ht, Justification::centred);
    xt += gapt;
    g.drawText(processor.params_.gain2.getCurrentValueAsText(), xt, yt, wt, ht, Justification::centred);
    xt += gapt;
    g.drawText(processor.params_.gain3.getCurrentValueAsText(), xt, yt, wt, ht, Justification::centred);
    xt += gapt;
    g.drawText(processor.params_.gain4.getCurrentValueAsText(), xt, yt, wt, ht, Justification::centred);
    xt += gapt;

}

void PluginEditor::resized() {
    int w = getWidth();
    int h = getHeight();

    int scopeWidth = w / nScopes;
    int scopeHeight = (h / 2) - 75;

    for (int col = 0; col < in.size(); col++) {

        Oscilloscope *o = in[col];
        assert(o);

        o->setBounds(
            col * scopeWidth,
            25,
            scopeWidth,
            scopeHeight);
    }

    for (int col = 0; col < out.size(); col++) {

        Oscilloscope *o = out[col];
        assert(o);

        o->setBounds(
            col * scopeWidth,
            25 + scopeHeight,
            scopeWidth,
            scopeHeight);
    }
}

inline float constrain(float v, float vMin, float vMax) {
    return std::max<float>(vMin, std::min<float>(vMax, v));
}

void incP(juce::RangedAudioParameter &p, float v) {
    float ov = p.convertFrom0to1(p.getValue());
    float nv = constrain(ov + v, -2.0f, 2.0f);
    if (nv != ov) {
        float pv = p.convertTo0to1(nv);
        p.beginChangeGesture();
        p.setValueNotifyingHost(pv);
        p.endChangeGesture();
    }
}

void setP(juce::RangedAudioParameter &p, float v) {
    float ov = p.convertFrom0to1(p.getValue());
    float nv = constrain(v, -2.0f, 2.0f);
    if (nv != ov) {
        float pv = p.convertTo0to1(nv);
        p.beginChangeGesture();
        p.setValueNotifyingHost(pv);
        p.endChangeGesture();
    }
}

void PluginEditor::onEncoder(int i, float v) {
    float inc = (v > 0.5 ? 0.1 : (v < 0.5 ? -0.1 : 0.0f));
    switch (i) {
        case 0:
            incP(processor.params_.gain1, inc);
            break;
        case 1:
            incP(processor.params_.gain2, inc);
            break;
        case 2:
            incP(processor.params_.gain3, inc);
            break;
        case 3:
            incP(processor.params_.gain4, inc);
            break;
        default:
            break;
    }
}

void PluginEditor::onEncoderSwitch(int i, bool v) {
    if (v) {
        switch (i) {
            case 0:
                setP(processor.params_.gain1, 1.0f);
                break;
            case 1:
                setP(processor.params_.gain2, 1.0f);
                break;
            case 2:
                setP(processor.params_.gain3, 1.0f);
                break;
            case 3:
                setP(processor.params_.gain4, 1.0f);
                break;
            default:
                break;
        }
    }
}
