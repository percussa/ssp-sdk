// see header file for license 

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Percussa.h"


PluginProcessor::PluginProcessor() : AudioProcessor(getBusesProperties()) {
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
    // Use this method as the place to do any pre-playback initialisation that you need..
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
            if(inputEnabled_[ch]) {
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

    for (int i = 0; i < buffer.getNumSamples(); i++) {

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

void PluginProcessor::getStateInformation(MemoryBlock &destData) {
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    // IMPORTANT: as a plugin developer you are responsible for resizing the
    // memory block passed by reference to this function, befor writing into it.
    // the memory block passed has zero size initially when entering this function.
    // calling append() on the memory block will automatically grow the block.
}

void PluginProcessor::setStateInformation(const void *data, int sizeInBytes) {
    // You should use this method to restore your parameters from the data buffer
    // passed to this function, whose contents will have been created originally by the
    // getStateInformation() call.
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
                inBuffer.applyGain(i, inBuffer.getNumSamples(), 0.0f);
                lock.exit();
            }
        }

        inputEnabled_[i] = v;
    }
}

void PluginProcessor::onOutputChanged(int i, bool v) {
    // we could use outputEnabled to optimise DSP processing
    // if we know an output has not been connected to another modules
    if (i < O_MAX) oututEnabled_[i] = v;
}


Percussa::SSP::PluginDescriptor *PluginProcessor::createDescriptor() {
    auto desc = new Percussa::SSP::PluginDescriptor;

    desc->name = JucePlugin_Name;
    desc->descriptiveName = JucePlugin_Desc;
    desc->manufacturerName = JucePlugin_Manufacturer;
    desc->version = JucePlugin_VersionString;
    desc->uid = (int) JucePlugin_VSTUniqueID;
    for (int i = 0; i < I_MAX; i++) {
        desc->inputChannelNames.push_back(getInputBusName(i).toStdString());
    }
    for (int i = 0; i < O_MAX; i++) {
        desc->outputChannelNames.push_back(getOutputBusName(i).toStdString());
    }
    return desc;
}

