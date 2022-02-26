/*
	Copyright (c) 2022 - Bert Schiettecatte, Noisetron LLC. 

	This software is part of the Percussa SSP's software development kit (SDK). 
	For more info about Percussa or the SSP visit http://www.percussa.com/ 
	and our forum at http://forum.percussa.com/ 

	Permission is granted to use this software under the terms of either:
	a) the GPL v2 (or any later version)
	b) the Affero GPL v3

	Details of these licenses can be found at: www.gnu.org/licenses

	This software is distributed in the hope that it will be useful, but WITHOUT ANY
	WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
	A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Percussa.h"

#include <array>
#include <string>


namespace ID {
#define PARAMETER_ID(str) constexpr const char* str { #str };
PARAMETER_ID (gain1)
PARAMETER_ID (gain2)
PARAMETER_ID (gain3)
PARAMETER_ID (gain4)
#undef PARAMETER_ID
}


class PluginProcessor : public AudioProcessor {
public:
    PluginProcessor();
    ~PluginProcessor();

    const String getName() const override { return JucePlugin_Name; }

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(AudioSampleBuffer &, MidiBuffer &) override;
    void releaseResources() override;

    AudioProcessorEditor *createEditor() override;

    bool hasEditor() const override { return true; }

    void getStateInformation(MemoryBlock &destData) override;
    void setStateInformation(const void *data, int sizeInBytes) override;

    // buses are the CV/audio inputs/outputs
    static const String getInputBusName(int channelIndex);
    static const String getOutputBusName(int channelIndex);

    static BusesProperties getBusesProperties() {
        BusesProperties props;
        for (auto i = 0; i < I_MAX; i++) {
            props.addBus(true, getInputBusName(i), AudioChannelSet::mono());
        }
        for (auto i = 0; i < O_MAX; i++) {
            props.addBus(false, getOutputBusName(i), AudioChannelSet::mono());
        }
        return props;
    }

    // VST parameters
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();


    // these methods are required for a VST , but the SSP is not using
    bool acceptsMidi() const override { return false; }

    bool producesMidi() const override { return false; }

    bool silenceInProducesSilenceOut() const override { return false; }

    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }  // some hosts don't like 0, so use 1
    int getCurrentProgram() override { return 0; }

    void setCurrentProgram(int) override {}

    const String getProgramName(int) override { return String(); }

    void changeProgramName(int index, const String &newName) override {}

    static constexpr unsigned I_MAX = 8;
    static constexpr unsigned O_MAX = 8;
private:
    bool inputEnabled_[I_MAX]{false, false, false, false, false, false, false, false};
    bool outputEnabled_[O_MAX]{false, false, false, false, false, false, false, false};
    AudioProcessorValueTreeState apvts_;
public:
    void onInputChanged(int, bool);
    void onOutputChanged(int, bool);
    CriticalSection lock;
    AudioSampleBuffer inBuffer;
    AudioSampleBuffer outBuffer;

    struct PluginParams {
        using Parameter = juce::RangedAudioParameter;
        explicit PluginParams(juce::AudioProcessorValueTreeState &);
        // we could use an array, but let's keep this simple!
        Parameter &gain1;
        Parameter &gain2;
        Parameter &gain3;
        Parameter &gain4;
    } params_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};



