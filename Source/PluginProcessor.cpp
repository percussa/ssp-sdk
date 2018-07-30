
// see header file for license 

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Percussa.h" 

QVCA::QVCA()
{
	memset(paramValues, 0, sizeof(paramValues)); 
}

QVCA::~QVCA()
{
}

const String QVCA::getName() const
{
    return JucePlugin_Name;
}

int QVCA::getNumParameters()
{
    return Percussa::sspLast;
}

float QVCA::getParameter (int index)
{
	if (index < Percussa::sspFirst) return 0.00f; 
	if (index >= Percussa::sspLast) return 0.00f; 

	// add offset so index starts from 0 
	return paramValues[index-Percussa::sspFirst];
}

void QVCA::setParameter (int index, float newValue)
{
	//Logger::writeToLog(String::formatted("QVCA::setParameter: index=%d newValue=%4.2f", index, newValue)); 

	// see Percussa.h for more info about the parameters below 

	// The SSP's software does not provide access to your 
	// parameters from outside your plugin code, 
	// so your plugin's editor (GUI) is what is used to 
	// see and change parameters using the SSP's hardware 
	// controls (provided directly to you via the VST parameter
	// interface here). 

	// Calls to the setParameter function of your VST plugin 
	// might be done from the ssp's software audio callback 
	// which means you cannot do any gui, file or other time 
	// consuming operations in your setParameter function. 

	// See the JUCE documentation and examples for 
	// AsyncUpdater, ChangeBroadcaster ... for suggestions 
	// on triggering updates on a diferent thread from within 
	// setParameter(). 

	// if not an ssp parameter then return 
	if (index < Percussa::sspFirst) return; 
	if (index >= Percussa::sspLast) return; 

	// add offset so index starts from 0 
	paramValues[index-Percussa::sspFirst] = newValue;  

	// use this if you need to do something special 
	// to process parameter  
	switch(index) { 
		case Percussa::sspEnc1:
			break; 
		case Percussa::sspEnc2: 
			break; 
		case Percussa::sspEnc3:
			break; 
		case Percussa::sspEnc4: 
			break; 
		case Percussa::sspEncSw1: 
			break; 
		case Percussa::sspEncSw2: 
			break; 
		case Percussa::sspEncSw3: 
			break; 
		case Percussa::sspEncSw4: 
			break; 
		case Percussa::sspSw1: 
			break; 
		case Percussa::sspSw2: 
			break; 
		case Percussa::sspSw3: 
			break; 
		case Percussa::sspSw4: 
			break; 
		case Percussa::sspSw5: 
			break; 
		case Percussa::sspSw6: 
			break; 
		case Percussa::sspSw7: 
			break; 
		case Percussa::sspSw8: 
			break; 
		case Percussa::sspSwLeft: 
			break; 
		case Percussa::sspSwRight: 
			break; 
		case Percussa::sspSwUp: 
			break; 
		case Percussa::sspSwDown: 
			break; 
		case Percussa::sspSwShiftL:
			break; 
		case Percussa::sspSwShiftR: 
			break; 
		default:
			break; 
	}
}

const String QVCA::getParameterName (int index)
{
    return String();
}

const String QVCA::getParameterText (int index)
{
    return String();
}

const String QVCA::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String QVCA::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

bool QVCA::isInputChannelStereoPair (int index) const
{
    return false;
}

bool QVCA::isOutputChannelStereoPair (int index) const
{
    return false;
}

bool QVCA::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool QVCA::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool QVCA::silenceInProducesSilenceOut() const
{
    return false;
}

double QVCA::getTailLengthSeconds() const
{
    return 0.0;
}

int QVCA::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int QVCA::getCurrentProgram()
{
    return 0;
}

void QVCA::setCurrentProgram (int index)
{
	// see below (functions for storing/recalling state info) 
	// for more info about what the SSP's software stores and recalls for your plugin 
}

const String QVCA::getProgramName (int index)
{
    return String("default");
}

void QVCA::changeProgramName (int index, const String& newName)
{
	// The SSP's software does not provide for changing program 
	// names so this is something you need to do (if you need it) 
	// in your own plugin GUI. 
}

void QVCA::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	// Use this method as the place to do any pre-playback
	// initialisation that you need..
	// this is called by the SSP's software right after loading 
	// the plugin and before it starts calling processBlock below  
}

void QVCA::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
}

void QVCA::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
	// plugin is 8ch in, 8ch out 
	// the ssp's software treats modulation and audio 
	// signals equally in its patcher matrix 
	// so what you would typically do below is use one or more 
	// incoming signals to change internal parameters 
	// (e.g. an incoming signal can change the frequency of an oscillator) 
	// if you don't want to do audio rate modulation you'd process the 
	// changes at a lower control rate. 	

	if (QVCAEditor* editor = (QVCAEditor*)getActiveEditor()) { 
		editor->updateInputScopes(buffer); 
	} 

	for (int i=0; i<buffer.getNumSamples(); i++) { 

		// multiply side by side channels 	
		float out1=buffer.getSample(0, i)*buffer.getSample(1, i); 
		float out2=buffer.getSample(2, i)*buffer.getSample(3, i); 
		float out3=buffer.getSample(4, i)*buffer.getSample(5, i); 
		float out4=buffer.getSample(6, i)*buffer.getSample(7, i); 

		// output multiplied signals and also output their inverted version 
		buffer.setSample(0, i,  out1); 
		buffer.setSample(1, i, -out1); 
		buffer.setSample(2, i,  out2); 
		buffer.setSample(3, i, -out2); 
		buffer.setSample(4, i,  out3); 
		buffer.setSample(5, i, -out3); 
		buffer.setSample(6, i,  out4); 
		buffer.setSample(7, i, -out4); 
	
	}

	if (QVCAEditor* editor = (QVCAEditor*)getActiveEditor()) { 
		editor->updateOutputScopes(buffer); 
	} 
}

bool QVCA::hasEditor() const
{
	// (change this to false if you choose to not supply an editor)
	// please note that you pretty much need an editor at the moment 
	// because the ssp's software does not provide a macro user interface 
	// to expose VST parameters for editing, outside the VST's GUI.
	// the moment the VST GUI is brought to front all hardware controls 
	// are sent to the plugin as parameter changes (i.e. setParameter() 
	// calls). The SSP software will not get or set any parameters you 
	// might choose to implement starting from 0.  
 
	return true; 
}

AudioProcessorEditor* QVCA::createEditor()
{
    return new QVCAEditor (*this);
}

void QVCA::getStateInformation (MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.

	// the ssp's software currently does not store any info in its presets 
	// about plugins, only the plugin file and program number 
	// this means that this function won't be called. 
}

void QVCA::setStateInformation (const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.

	// the ssp's software currently does not store any info in its presets 
	// about plugins, only the plugin file and program number 
	// this means that this function won't be called. 

	// this means you have to provide your own way of storing your VST plugin 
	// presets as files and recalling them. The SSP's software will call get/set 
	// program and related above functions to let the user choose programs in the 
	// VST module UI. 
}

// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new QVCA();
}
