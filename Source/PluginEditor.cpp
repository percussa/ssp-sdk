
// see header file for license 

#include "PluginProcessor.h"
#include "PluginEditor.h"

QVCAEditor::QVCAEditor (QVCA& p)
    : AudioProcessorEditor (&p), processor (p)
{
	// set this to true to see the parameter values update 
	// in the vst plugin GUI (editor) when turning encoders/
	// pushing buttons 
	showParamValues = true; 

	setSize (1600, 480);

	for (int i=0; i<nScopes; i++) { 
		addAndMakeVisible(in[i]); 
		in[i].setInfo(String("In")+String(i+1)); 
		in[i].setInfoCol(Colours::white); 
		in[i].start(inBuffer, inLock, i);  
	}

	for (int i=0; i<nScopes; i++) { 
		addAndMakeVisible(out[i]); 
		out[i].setInfoCol(Colours::red); 
		out[i].start(outBuffer, outLock, i); 
	}

	out[0].setInfo(String("Out1=In1*In2")); 
	out[1].setInfo(String("Out2=-In1*In2")); 
	out[2].setInfo(String("Out3=In3*In4")); 
	out[3].setInfo(String("Out4=-In3*In4")); 
	out[4].setInfo(String("Out5=In5*In6")); 
	out[5].setInfo(String("Out6=-In5*In6")); 
	out[6].setInfo(String("Out7=In7*In8")); 
	out[7].setInfo(String("Out8=-In7*In8")); 

	startTimer(50); 
}

QVCAEditor::~QVCAEditor()
{
	stopTimer(); 

	inLock.enter(); 
	for (int i=0; i<nScopes; i++) { 
		in[i].reset(); 
	}
	inLock.exit(); 

	outLock.enter(); 
	for (int i=0; i<nScopes; i++) { 
		out[i].reset(); 
	}
	outLock.exit(); 
}

void QVCAEditor::timerCallback() 
{ 
	for (int i=0; i<nScopes; i++) { 
		in[i].repaint(); 
	}

	for (int i=0; i<nScopes; i++) { 
		out[i].repaint(); 
	}

	// repaint our own canvas as well 
	repaint(); 
}

void QVCAEditor::paint(Graphics& g)
{
	g.fillAll (Colours::black);

	// show plugin name in the background 
	Font f(Font::getDefaultMonospacedFontName(), 
		0.9f*getHeight(), Font::plain);
	g.setFont(f);
	g.setColour(Colour(30, 30, 30));

	int w=getWidth(); 
	int h=getHeight();

	if (showParamValues) { 
		h -= keepout; 
	}

	g.drawFittedText(
		JucePlugin_Name,
		0, 0,
		w, h,
		Justification::centred,
		1);

	// paint parameter values 
	if (showParamValues) { 

		Font f(Font::getDefaultMonospacedFontName(), 
			24, Font::plain);
		g.setFont(f);
		g.setColour(Colours::white); 

		// first row 
		int x = 10; 
		for (int i=Percussa::sspEnc1; i<Percussa::sspSw1; i++) { 
			g.drawSingleLineText(
				String::formatted("%4.2f", processor.getParameter(i)), 
				x, getHeight()-keepout+20); 
			x += 150;  
		}

		// second row 
		x = 10; 
		for (int i=Percussa::sspSw1; i<Percussa::sspSwLeft; i++) { 
			g.drawSingleLineText(
				String::formatted("%4.2f", processor.getParameter(i)), 
				x, getHeight()-keepout+40); 
			x += 150;  
		}

		// third row 
		x = 10; 
		for (int i=Percussa::sspSwLeft; i<Percussa::sspOutEn1; i++) { 
			g.drawSingleLineText(
				String::formatted("%4.2f", processor.getParameter(i)), 
				x, getHeight()-keepout+60); 
			x += 150;  
		}

		// fourth row 
		x = 10; 
		for (int i=Percussa::sspOutEn1; i<Percussa::sspInEn1; i++) { 
			g.drawSingleLineText(
				String::formatted("%1.1f", processor.getParameter(i)), 
				x, getHeight()-keepout+80); 
			x += 50;  
		}

		// fifth row 
		x = 10; 
		for (int i=Percussa::sspInEn1; i<Percussa::sspLast; i++) { 
			g.drawSingleLineText(
				String::formatted("%1.1f", processor.getParameter(i)), 
				x, getHeight()-keepout+100); 
			x += 50;  
		}
	} 
}

void QVCAEditor::updateInputScopes(AudioSampleBuffer& asb) {  
	bool haveLock = inLock.tryEnter(); 
	if (!haveLock) return; 
	inBuffer = asb; 
	inLock.exit(); 
}

void QVCAEditor::updateOutputScopes(AudioSampleBuffer& asb) {  
	bool haveLock = outLock.tryEnter(); 
	if (!haveLock) return; 
	outBuffer = asb; 
	outLock.exit(); 
}
	
void QVCAEditor::resized()
{
	int w=getWidth(); 
	int h=getHeight();

	if (showParamValues) { 
		h -= keepout; 
	}

	int scopeWidth=w/(nScopeCols*2); 
	int scopeHeight=h/nScopeRows; 

	for (int col=0; col<nScopeCols; col++) { 
		for (int row=0; row<nScopeRows; row++) { 
			in[col*nScopeRows+row].setBounds(
				col*scopeWidth*2, 
				row*scopeHeight, 
				scopeWidth, 
				scopeHeight); 
		}	
	}

	for (int col=0; col<nScopeCols; col++) { 
		for (int row=0; row<nScopeRows; row++) { 
			out[col*nScopeRows+row].setBounds(
				col*scopeWidth*2+scopeWidth, 
				row*scopeHeight, 
				scopeWidth, 
				scopeHeight); 
		}	
	}
}
