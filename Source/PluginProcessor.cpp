// see header file for license 

#include <cstring> 
#include <stdexcept> 

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Percussa.h" 

QVCA::QVCA(): 
	inputNames  ({ "In1",  "In2",  "In3",  "In4",  "In5",  "In6",  "In7",  "In8"  }),  
	outputNames ({ "Out1", "Out2", "Out3", "Out4", "Out5", "Out6", "Out7", "Out8" })   
{
	std::memset(paramValues, 0, sizeof(paramValues)); 
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
    return numParams; 
}

float QVCA::getParameter (int index)
{
	if (index < Percussa::sspFirst) return 0.00f; 
	if (index >= Percussa::sspLast) return 0.00f; 

	return paramValues[index-Percussa::sspFirst];
}

void QVCA::setParameter (int index, float newValue)
{
	// The SSP's software uses VST parameters to pass the state of its hardware 
	// buttons and encoders to your plugin. See Percussa.h, these parameters 
	// begin at sspFirst and end at sspLast. There are also special parameters 
	// passed to indicate which of the inputs and outputs of the VST module 
	// (which hosts the plugin) are enabled. You can add your own VST parameters 
	// starting at sspLast, if you wish, so your plugin can also be used with 
	// desktop DAWs. 

	// Calls to the setParameter function of your VST plugin might be done from 
	// the ssp's software audio callback which means you cannot do any gui, 
	// file or other time consuming operations in your setParameter function. 

	// See the JUCE documentation and examples for AsyncUpdater, ChangeBroadcaster 
	// ... for suggestions on triggering updates on a diferent thread from within 
	// setParameter(). 

	// the paramValues array used below is used to store encoder/button data 
	// here such that it can be read back later using getParameter(), by the 
	// PluginEditor class, which displays the encoder/button data to help with 
	// debugging and development. 

	// Obviously in a real functional plugin with actual settings such as gain 
	// etc. you would use the encoder pulses passed via setParameter() to update 
	// real settings and store these, rather than storing the encoder/button 
	// data like we do here.   

	//Logger::writeToLog(String::formatted(
	//	"QVCA::setParameter: index=%d newValue=%4.2f", 
	//		index, newValue)); 

	if (index < Percussa::sspFirst) return; 
	if (index >= Percussa::sspLast) return; 

	switch(index) { 
		case Percussa::sspEnc1:
		case Percussa::sspEnc2: 
		case Percussa::sspEnc3:
		case Percussa::sspEnc4: 
		{
			if (newValue > 0.5) {  
				paramValues[index-Percussa::sspFirst]++; 
			} else if (newValue < 0.5) { 
				paramValues[index-Percussa::sspFirst]--; 
			}
			break; 
		}

		case Percussa::sspEncSw1: 
		case Percussa::sspEncSw2: 
		case Percussa::sspEncSw3: 
		case Percussa::sspEncSw4: 
		case Percussa::sspSw1: 
		case Percussa::sspSw2: 
		case Percussa::sspSw3: 
		case Percussa::sspSw4: 
		case Percussa::sspSw5: 
		case Percussa::sspSw6: 
		case Percussa::sspSw7: 
		case Percussa::sspSw8: 
		case Percussa::sspSwLeft: 
		case Percussa::sspSwRight: 
		case Percussa::sspSwUp: 
		case Percussa::sspSwDown: 
		case Percussa::sspSwShiftL:
		case Percussa::sspSwShiftR: 

		case Percussa::sspOutEn1: 
		case Percussa::sspOutEn2: 
		case Percussa::sspOutEn3: 
		case Percussa::sspOutEn4: 
		case Percussa::sspOutEn5: 
		case Percussa::sspOutEn6: 
		case Percussa::sspOutEn7: 
		case Percussa::sspOutEn8: 
		case Percussa::sspOutEn9: 
		case Percussa::sspOutEn10: 
		case Percussa::sspOutEn11: 
		case Percussa::sspOutEn12: 
		case Percussa::sspOutEn13: 
		case Percussa::sspOutEn14: 
		case Percussa::sspOutEn15: 
		case Percussa::sspOutEn16: 
		case Percussa::sspOutEn17: 
		case Percussa::sspOutEn18: 
		case Percussa::sspOutEn19: 
		case Percussa::sspOutEn20: 
		case Percussa::sspOutEn21: 
		case Percussa::sspOutEn22: 
		case Percussa::sspOutEn23: 
		case Percussa::sspOutEn24: 

		case Percussa::sspInEn1: 
		case Percussa::sspInEn2: 
		case Percussa::sspInEn3: 
		case Percussa::sspInEn4: 
		case Percussa::sspInEn5: 
		case Percussa::sspInEn6: 
		case Percussa::sspInEn7: 
		case Percussa::sspInEn8: 
		case Percussa::sspInEn9: 
		case Percussa::sspInEn10: 
		case Percussa::sspInEn11: 
		case Percussa::sspInEn12: 
		case Percussa::sspInEn13: 
		case Percussa::sspInEn14: 
		case Percussa::sspInEn15: 
		case Percussa::sspInEn16: 
		case Percussa::sspInEn17: 
		case Percussa::sspInEn18: 
		case Percussa::sspInEn19: 
		case Percussa::sspInEn20: 
		case Percussa::sspInEn21: 
		case Percussa::sspInEn22: 
		case Percussa::sspInEn23: 
		case Percussa::sspInEn24: 

		default:
		{
			paramValues[index-Percussa::sspFirst] = newValue;  
			break; 
		}
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
    return inputNames[channelIndex]; 
}

const String QVCA::getOutputChannelName (int channelIndex) const
{
    return outputNames[channelIndex]; 
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
	// Program numbers are not stored or recalled from SSP preset files and there is no way 
	// to change a program from within the SSP software. The SSP software which hosts your 
	// plugin will ONLY store and recall state using the functions below. 
}

const String QVCA::getProgramName (int index)
{
    return String("default");
}

void QVCA::changeProgramName (int index, const String& newName)
{
	// The SSP's software does not provide for changing program names so this is something 
	// you need to do (if you need it) in your own plugin GUI. Keep in mind the SSP software
	// does not set, store or recall program numbers and there is no way for you to change
	// a program number from within the SSP software.  
}

void QVCA::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	// Use this method as the place to do any pre-playback initialisation that you need..
	// this is called by the SSP's software right after loading the plugin and before 
	// it starts calling processBlock below  
	const ScopedLock sl(lock); 

	// allocate space in the input/output buffers for visualisation here, to make sure 
	// processBlock() does not do any allocations. make sure buffers are cleared at 
	// the same time (clearExtraSpace) 
	inBuffer.setSize(getNumInputChannels(), samplesPerBlock, false, true, false); 
	outBuffer.setSize(getNumOutputChannels(), samplesPerBlock, false, true, false); 
}

void QVCA::releaseResources()
{
	// when playback stops, you can use this as an opportunity to free up any memory. 
	// this is called when your plugin is being destroyed by the SSP host software. 
}

void QVCA::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
	// the QVCA plugin has 8 input and 8 output channels. 
	// the ssp's software treats modulation and audio signals equally in its patcher matrix 
	// so what you would typically do below is use one or more incoming signals to change 
	// internal parameters (e.g. an incoming signal can change the frequency of an oscillator) 
	// if you don't want to do audio rate modulation you'd process the changes at a lower 
	// control rate. 	

	// try to get lock and copy input buffer  
	if (lock.tryEnter()) {  
		for (int ch=0; ch<getNumInputChannels(); ch++) 
			inBuffer.copyFrom(ch, 0, buffer, ch, 0, buffer.getNumSamples()); 
		lock.exit(); 
	}

	// process signals 
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

	// try to get lock and copy output buffer  
	if (lock.tryEnter()) { 
		for (int ch=0; ch<getNumOutputChannels(); ch++) 
			outBuffer.copyFrom(ch, 0, buffer, ch, 0, buffer.getNumSamples()); 
		lock.exit();
	} 
}

bool QVCA::hasEditor() const
{
	// your plugin needs to provide an editor in any case, since the SSP host software 
	// requires one. so you cannot return false here. when the VST GUI is brought to 
	// the front all hardware controls are sent to the plugin as parameter changes 
	// (i.e. setParameter() calls). The SSP software will not get or set any parameters 
	// you added yourself (see Percussa.h for notes about that). The SSP software only 
	// uses the functions below to store and recall state of your plugin. The state 
	// is stored in the SSP preset file.  
	return true; 
}

AudioProcessorEditor* QVCA::createEditor()
{
	// this allocates your plugin editor. the SSP host software will take care of 
	// deleting the editor when it destorys your plugin. 
	return new QVCAEditor (*this);
}

void QVCA::getStateInformation (MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.

	// The SSP software will call this function to capture the state of your plugin
	// and will store the state inside its preset files when saved. 

	// IMPORTANT: as a plugin developer you are responsible for resizing the 
	// memory block passed by reference to this function, befor writing into it.
	// the memory block passed has zero size initially when entering this function.   
	// calling append() on the memory block will automatically grow the block. 

	// write header consisting of plugin name and version string 
	destData.append(JucePlugin_Name, sizeof(JucePlugin_Name)); 
	destData.append(JucePlugin_VersionString, sizeof(JucePlugin_VersionString)); 

	// obviously it makes no sense to store the button or encoder values from 
	// paramValues and in a real plugin you would store other settings, such as gain, 
	// cutoff, resonance etc. the QVCA doesn't have any settings at the moment, so 
	// we just store the data from paramValues as an example. 

	// write parameter value array 
	destData.append(paramValues, sizeof(paramValues)); 

	// write footer
	destData.append(JucePlugin_Name, sizeof(JucePlugin_Name)); 
}

void QVCA::setStateInformation (const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from the data buffer 
	// passed to this function, whose contents will have been created originally by the 
	// getStateInformation() call.

	// The SSP software will call this function after reading the state from a preset
	// file, when the user loads a new preset. 

	// code below can probably use additional error handling and bounds checking, 
	// it's just here as a general example and not guaranteed to be reliable. 

	// read and compare header consisting of plugin name and version 
	if (std::memcmp(data, JucePlugin_Name, sizeof(JucePlugin_Name)) != 0) 
		throw std::runtime_error("error reading plugin name"); 
	data += sizeof(JucePlugin_Name); 

	if (std::memcmp(data, JucePlugin_VersionString, sizeof(JucePlugin_VersionString)) != 0) 
		throw std::runtime_error("error reading plugin version string"); 
	data += sizeof(JucePlugin_VersionString); 

	// restore parameter values 
	std::memcpy(paramValues, data, sizeof(paramValues)); 	
	data += sizeof(paramValues); 

	// read and compare footer 
	if (std::memcmp(data, JucePlugin_Name, sizeof(JucePlugin_Name)) != 0) 
		throw std::runtime_error("error reading plugin name"); 
}

// this creates new instances of the plugin ...
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new QVCA();
}
