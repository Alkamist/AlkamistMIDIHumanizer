/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 3.2.0

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "PluginProcessor.h"
#include "FloatParameter.h"
//[/Headers]

#include "MainEditorWindow.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
MainEditorWindow::MainEditorWindow (AlkamistMIDIHumanizerAudioProcessor& inputProcessor)
    : mParentProcessor (&inputProcessor)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (timingStandardDeviationSlider = new Slider ("Timing Standard Deviation Slider"));
    timingStandardDeviationSlider->setRange (0, 30, 0.01);
    timingStandardDeviationSlider->setSliderStyle (Slider::LinearHorizontal);
    timingStandardDeviationSlider->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    timingStandardDeviationSlider->setColour (Slider::thumbColourId, Colour (0xff909090));
    timingStandardDeviationSlider->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    timingStandardDeviationSlider->setColour (Slider::textBoxBackgroundColourId, Colour (0xff4a4a4a));
    timingStandardDeviationSlider->setColour (Slider::textBoxHighlightColourId, Colour (0x40e4e4e4));
    timingStandardDeviationSlider->addListener (this);

    addAndMakeVisible (timingStandardDeviationLabel = new Label ("Timing Standard Deviation Label",
                                                                 TRANS("Timing Standard\n"
                                                                 "Deviation (ms)")));
    timingStandardDeviationLabel->setFont (Font (15.00f, Font::plain));
    timingStandardDeviationLabel->setJustificationType (Justification::centredLeft);
    timingStandardDeviationLabel->setEditable (false, false, false);
    timingStandardDeviationLabel->setColour (Label::textColourId, Colour (0xffdddddd));
    timingStandardDeviationLabel->setColour (TextEditor::textColourId, Colours::black);
    timingStandardDeviationLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]
    setAllSliderRanges();
    //[/UserPreSize]

    setSize (390, 100);


    //[Constructor] You can add your own custom stuff here..

    timingStandardDeviationSlider->setVelocityModeParameters(0.45, 1, 0.05, true);
    timingStandardDeviationSlider->setDoubleClickReturnValue(true, 0.0);

    updateAllSliderValues();

    startTimerHz(60);

    //[/Constructor]
}

MainEditorWindow::~MainEditorWindow()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    timingStandardDeviationSlider = nullptr;
    timingStandardDeviationLabel = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void MainEditorWindow::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323232));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void MainEditorWindow::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    timingStandardDeviationSlider->setBounds (8, 8, 264, 24);
    timingStandardDeviationLabel->setBounds (272, 0, 119, 40);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void MainEditorWindow::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    setNormalizedValueFromSlider (sliderThatWasMoved);
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == timingStandardDeviationSlider)
    {
        //[UserSliderCode_timingStandardDeviationSlider] -- add your slider handling code here..
        //[/UserSliderCode_timingStandardDeviationSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void MainEditorWindow::setSliderRange (Slider* slider)
{
    if (FloatParameter* parameter = getParameterFromSlider(slider))
    {
        double dittoInterval = slider->getInterval();

        slider->setRange (parameter->getMinimum(), parameter->getMaximum(), dittoInterval);
    }
}

void MainEditorWindow::setAllSliderRanges()
{
    setSliderRange (timingStandardDeviationSlider);
}

void MainEditorWindow::setNormalizedValueFromSlider(Slider* slider)
{
    if (FloatParameter* parameter = getParameterFromSlider(slider))
    {
        parameter->setNormalizedValue ((float) slider->getValue());
    }
}

void MainEditorWindow::updateSliderValue(Slider* slider)
{
    if (FloatParameter* parameter = getParameterFromSlider(slider))
    {
        float unNormalizedParameterValue = parameter->getUnNormalizedUnSmoothedValue();

        slider->setValue (unNormalizedParameterValue);
    }
}

void MainEditorWindow::updateAllSliderValues()
{
    updateSliderValue (timingStandardDeviationSlider);
}

void MainEditorWindow::timerCallback()
{
    updateAllSliderValues();
}

void MainEditorWindow::sliderDragStarted (Slider* slider)
{
    if (FloatParameter* parameter = getParameterFromSlider (slider))
    {
        parameter->beginChangeGesture();
    }
}

void MainEditorWindow::sliderDragEnded (Slider* slider)
{
    if (FloatParameter* parameter = getParameterFromSlider (slider))
    {
        parameter->endChangeGesture();
    }
}

FloatParameter* MainEditorWindow::getParameterFromSlider (const Slider* slider) const
{
    if (slider == timingStandardDeviationSlider)
        return mParentProcessor->timingStandardDeviation;

    return nullptr;
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MainEditorWindow" componentName=""
                 parentClasses="public Component, public Timer" constructorParams="AlkamistMIDIHumanizerAudioProcessor&amp; inputProcessor"
                 variableInitialisers="mParentProcessor (&amp;inputProcessor)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="390" initialHeight="100">
  <BACKGROUND backgroundColour="ff323232"/>
  <SLIDER name="Timing Standard Deviation Slider" id="2a2a0656b43acdbd"
          memberName="timingStandardDeviationSlider" virtualName="" explicitFocusOrder="0"
          pos="8 8 264 24" thumbcol="ff909090" textboxtext="ffdddddd" textboxbkgd="ff4a4a4a"
          textboxhighlight="40e4e4e4" min="0" max="30" int="0.01" style="LinearHorizontal"
          textBoxPos="TextBoxLeft" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <LABEL name="Timing Standard Deviation Label" id="a78ae72ed3b4912c"
         memberName="timingStandardDeviationLabel" virtualName="" explicitFocusOrder="0"
         pos="272 0 119 40" textCol="ffdddddd" edTextCol="ff000000" edBkgCol="0"
         labelText="Timing Standard&#10;Deviation (ms)" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
