// see header file for license 

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Percussa.h"


PluginProcessor::PluginProcessor() :
    AudioProcessor(getBusesProperties()),
    apvts_(*this, nullptr, "state", std::move(createParameterLayout())),
    params_(apvts_) {
#ifdef __APPLE__
    // for testing VSTs, where we arent going to be informed of IO connections.
    for (int i = 0; i < I_MAX; i++) inputEnabled_[i] = true;
    for (int i = 0; i < O_MAX; i++) outputEnabled_[i] = true;
#endif
}

PluginProcessor::~PluginProcessor() {
}

const String PluginProcessor::getInputBusName(int channelIndex) {
    static String inputNames[O_MAX] = {
        "In1", "In2", "In3", "In4", "In5", "In6", "In7", "In8"};
    if (channelIndex < I_MAX) { return inputNames[channelIndex]; }
    return "ZZIn-" + String(channelIndex);
}


const String PluginProcessor::getOutputBusName(int channelIndex) {
    static String outputNames[O_MAX] = {
        "Out1", "Out2", "Out3", "Out4", "Out5", "Out6", "Out7", "Out8"};
    if (channelIndex < O_MAX) { return outputNames[channelIndex]; }
    return "ZZOut-" + String(channelIndex);
}


void PluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    // Use this method as the place to do any pre-playback initialisation that you need.
    const ScopedLock sl(lock);

    // allocate space in the input/output buffers for visualisation here, to make sure
    // processBlock() does not do any allocations. make sure buffers are cleared at
    // the same time (clearExtraSpace)
    inBuffer.setSize(I_MAX, samplesPerBlock, false, true, false);
    outBuffer.setSize(O_MAX, samplesPerBlock, false, true, false);
}

void PluginProcessor::releaseResources() {
    // when playback stops, you can use this as an opportunity to free up any memory.
}

void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    // the PluginProcessor plugin has 8 input and 8 output channels.
    // the ssp's software treats modulation and audio signals equally in its patcher matrix
    // so what you would typically do below is use one or more incoming signals to change
    // internal parameters (e.g. an incoming signal can change the frequency of an oscillator)
    // if you don't want to do audio rate modulation you'd process the changes at a lower
    // control rate.

    // try to get lock and copy input buffer
    if (lock.tryEnter()) {
        for (int ch = 0; ch < I_MAX; ch++) {
            if (inputEnabled_[ch]) {
                // we only need to copy the input IF an input is connected
                inBuffer.copyFrom(ch, 0, buffer, ch, 0, buffer.getNumSamples());
            }
        }
        lock.exit();
    }

    // process signals


    // here do we use outputEnabled?
    // if we don't produce the output it wont appear on the SSP scopes
    // but on the other hand we do save values DSP time!

    auto n = buffer.getNumSamples();

    for (int i = 0; i < n; i++) {

        // multiply side by side channels
        float out1 = buffer.getSample(0, i) * buffer.getSample(1, i);
        float out2 = buffer.getSample(2, i) * buffer.getSample(3, i);
        float out3 = buffer.getSample(4, i) * buffer.getSample(5, i);
        float out4 = buffer.getSample(6, i) * buffer.getSample(7, i);

        // output multiplied signals and also output their inverted version
        buffer.setSample(0, i, out1);
        buffer.setSample(1, i, -out1);
        buffer.setSample(2, i, out2);
        buffer.setSample(3, i, -out2);
        buffer.setSample(4, i, out3);
        buffer.setSample(5, i, -out3);
        buffer.setSample(6, i, out4);
        buffer.setSample(7, i, -out4);
    }

    // now apply gain to output buffers
    float g1 = params_.gain1.convertFrom0to1(params_.gain1.getValue());
    float g2 = params_.gain2.convertFrom0to1(params_.gain2.getValue());
    float g3 = params_.gain3.convertFrom0to1(params_.gain3.getValue());
    float g4 = params_.gain4.convertFrom0to1(params_.gain4.getValue());
    buffer.applyGain(0, 0, n, g1);
    buffer.applyGain(1, 0, n, g1);
    buffer.applyGain(2, 0, n, g2);
    buffer.applyGain(3, 0, n, g2);
    buffer.applyGain(4, 0, n, g3);
    buffer.applyGain(5, 0, n, g3);
    buffer.applyGain(6, 0, n, g4);
    buffer.applyGain(7, 0, n, g4);

    // try to get lock and copy output buffer
    if (lock.tryEnter()) {
        for (int ch = 0; ch < O_MAX; ch++)
            outBuffer.copyFrom(ch, 0, buffer, ch, 0, buffer.getNumSamples());
        lock.exit();
    }

}

AudioProcessorEditor *PluginProcessor::createEditor() {
    return new PluginEditor(*this);
}

static const char *VST_XML_TAG = "VST";

void PluginProcessor::getStateInformation(MemoryBlock &destData) {
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    // IMPORTANT: as a plugin developer you are responsible for resizing the
    // memory block passed by reference to this function, befor writing into it.
    // the memory block passed has zero size initially when entering this function.
    // calling append() on the memory block will automatically grow the block.

    auto state = apvts_.copyState();
    std::unique_ptr<juce::XmlElement> xmlVst = std::make_unique<XmlElement>(VST_XML_TAG);

    // first save state tree
    std::unique_ptr<juce::XmlElement> xmlState(state.createXml());

    // clean this xml by removing SSP parameters (encoders and alike)
    std::unique_ptr<XmlElement> cleanStateXml = std::make_unique<XmlElement>(xmlState->getTagName());
    for (int i = 0; i < xmlState->getNumChildElements(); i++) {
        XmlElement *p = xmlState->getChildElement(i);
        XmlElement *ne = new XmlElement(*p);
        cleanStateXml->addChildElement(ne);
    }

    if (xmlState) xmlVst->addChildElement(cleanStateXml.release());
    copyXmlToBinary(*xmlVst, destData);
}

void PluginProcessor::setStateInformation(const void *data, int sizeInBytes) {
    // You should use this method to restore your parameters from the data buffer
    // passed to this function, whose contents will have been created originally by the
    // getStateInformation() call.

    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml.get() != nullptr) {
        if (xml->hasTagName(VST_XML_TAG)) {
            auto xmlState = xml->getChildByName(apvts_.state.getType()); //STATE
            if (xmlState != nullptr) apvts_.replaceState(juce::ValueTree::fromXml(*xmlState));
        }
    }
}

// called by the juce VST framework to instantiate the plugin. 
AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new PluginProcessor();
}


// SSP specifics
void PluginProcessor::onInputChanged(int i, bool v) {
    // we could use inputEnabled to optimise DSP processing
    // if we know an input is not connect.
    if (i < I_MAX) {
        if (v != inputEnabled_[i] && !v) {
            // input has been disabled, so zero buffer for scopes
            if (lock.tryEnter()) {
                inBuffer.applyGain(i, 0, inBuffer.getNumSamples(), 0.0f);
                lock.exit();
            }
        }

        inputEnabled_[i] = v;
    }
}

void PluginProcessor::onOutputChanged(int i, bool v) {
    // we could use outputEnabled to optimise DSP processing
    // if we know an output has not been connected to another modules
    if (i < O_MAX) outputEnabled_[i] = v;
}


AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout() {
    AudioProcessorValueTreeState::ParameterLayout params;
    params.add(std::make_unique<juce::AudioParameterFloat>(ID::gain1, "Gain 1", -2.0, 2.0f, 1.0f));
    params.add(std::make_unique<juce::AudioParameterFloat>(ID::gain2, "Gain 2", -2.0, 2.0f, 1.0f));
    params.add(std::make_unique<juce::AudioParameterFloat>(ID::gain3, "Gain 3", -2.0, 2.0f, 1.0f));
    params.add(std::make_unique<juce::AudioParameterFloat>(ID::gain4, "Gain 4", -2.0, 2.0f, 1.0f));
    return params;
}

PluginProcessor::PluginParams::PluginParams(AudioProcessorValueTreeState &apvt) :
    gain1(*apvt.getParameter(ID::gain1)),
    gain2(*apvt.getParameter(ID::gain2)),
    gain3(*apvt.getParameter(ID::gain3)),
    gain4(*apvt.getParameter(ID::gain4)) {
}


