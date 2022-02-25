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
#include "PluginProcessor.h"
#include "Oscilloscope.h"
#include "Percussa.h"

class PluginEditor :
    public AudioProcessorEditor, public Timer {
public:
    static const int nScopes = 8;
    static const int keepout = 100;

    PluginEditor(PluginProcessor &);
    ~PluginEditor();

    void paint(Graphics &) override;
    void resized() override;
    void timerCallback() override;

    // SSP interface
    void onButton(int, bool) {}
    void onLeftButton(bool) {}
    void onRightButton(bool) {}
    void onUpButton(bool) {}
    void onDownButton(bool) {}
    void onLeftShiftButton(bool) {}
    void onRightShiftButton(bool) {}

    void onEncoder(int,float) {}
    void onEncoderSwitch(int,bool) {}

private:
    PluginProcessor &processor;
    OwnedArray<Oscilloscope> in;
    OwnedArray<Oscilloscope> out;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};


