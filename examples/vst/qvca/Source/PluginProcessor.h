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

class PluginProcessor : public AudioProcessor {
public:
    PluginProcessor();
    ~PluginProcessor();

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock(AudioSampleBuffer &, MidiBuffer &) override;

    AudioProcessorEditor *createEditor() override;

    bool hasEditor() const override { return true; }

    const String getName() const override { return JucePlugin_Name; }

    void getStateInformation(MemoryBlock &destData) override;
    void setStateInformation(const void *data, int sizeInBytes) override;

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


public:
    static Percussa::SSP::PluginDescriptor *createDescriptor();
    void onInputChanged(int, bool);
    void onOutputChanged(int, bool);
    static constexpr unsigned I_MAX = 8;
    static constexpr unsigned O_MAX = 8;


    CriticalSection lock;
    AudioSampleBuffer inBuffer;
    AudioSampleBuffer outBuffer;

private:
    bool inputEnabled_[I_MAX] {false, false, false, false, false,false, false, false };
    bool oututEnabled_[O_MAX] {false, false, false, false, false,false, false, false };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};



