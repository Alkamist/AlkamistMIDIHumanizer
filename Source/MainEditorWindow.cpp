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

    addAndMakeVisible (timingRangeSlider = new Slider ("Timing Range Slider"));
    timingRangeSlider->setRange (0, 15, 0.01);
    timingRangeSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    timingRangeSlider->setTextBoxStyle (Slider::TextBoxAbove, false, 80, 20);
    timingRangeSlider->setColour (Slider::thumbColourId, Colour (0xff909090));
    timingRangeSlider->setColour (Slider::trackColourId, Colour (0x7fdddddd));
    timingRangeSlider->setColour (Slider::rotarySliderFillColourId, Colour (0x7fdddddd));
    timingRangeSlider->setColour (Slider::rotarySliderOutlineColourId, Colour (0x66dddddd));
    timingRangeSlider->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    timingRangeSlider->setColour (Slider::textBoxBackgroundColourId, Colour (0xff4a4a4a));
    timingRangeSlider->setColour (Slider::textBoxHighlightColourId, Colour (0x40e4e4e4));
    timingRangeSlider->addListener (this);

    addAndMakeVisible (timingRangeLabel = new Label ("Timing Range Label",
                                                     TRANS("Timing Range (ms)")));
    timingRangeLabel->setFont (Font (15.00f, Font::plain));
    timingRangeLabel->setJustificationType (Justification::centred);
    timingRangeLabel->setEditable (false, false, false);
    timingRangeLabel->setColour (Label::textColourId, Colour (0xffdddddd));
    timingRangeLabel->setColour (TextEditor::textColourId, Colours::black);
    timingRangeLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (velocityRangeSlider = new Slider ("Velocity Range Slider"));
    velocityRangeSlider->setRange (0, 32, 0.01);
    velocityRangeSlider->setSliderStyle (Slider::RotaryVerticalDrag);
    velocityRangeSlider->setTextBoxStyle (Slider::TextBoxAbove, false, 80, 20);
    velocityRangeSlider->setColour (Slider::thumbColourId, Colour (0xff909090));
    velocityRangeSlider->setColour (Slider::trackColourId, Colour (0x7fdddddd));
    velocityRangeSlider->setColour (Slider::rotarySliderFillColourId, Colour (0x7fdddddd));
    velocityRangeSlider->setColour (Slider::rotarySliderOutlineColourId, Colour (0x66dddddd));
    velocityRangeSlider->setColour (Slider::textBoxTextColourId, Colour (0xffdddddd));
    velocityRangeSlider->setColour (Slider::textBoxBackgroundColourId, Colour (0xff4a4a4a));
    velocityRangeSlider->setColour (Slider::textBoxHighlightColourId, Colour (0x40e4e4e4));
    velocityRangeSlider->addListener (this);

    addAndMakeVisible (velocityRangeLabel = new Label ("Velocity Range Label",
                                                       TRANS("Velocity Range")));
    velocityRangeLabel->setFont (Font (15.00f, Font::plain));
    velocityRangeLabel->setJustificationType (Justification::centred);
    velocityRangeLabel->setEditable (false, false, false);
    velocityRangeLabel->setColour (Label::textColourId, Colour (0xffdddddd));
    velocityRangeLabel->setColour (TextEditor::textColourId, Colours::black);
    velocityRangeLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]
    setAllSliderRanges();
    //[/UserPreSize]

    setSize (240, 128);


    //[Constructor] You can add your own custom stuff here..

    timingRangeSlider->setVelocityModeParameters(0.45, 1, 0.05, true);
    timingRangeSlider->setDoubleClickReturnValue(true, 0.0);

    velocityRangeSlider->setVelocityModeParameters(0.45, 1, 0.05, true);
    velocityRangeSlider->setDoubleClickReturnValue(true, 0.0);

    updateAllSliderValues();

    startTimerHz(60);

    //[/Constructor]
}

MainEditorWindow::~MainEditorWindow()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    timingRangeSlider = nullptr;
    timingRangeLabel = nullptr;
    velocityRangeSlider = nullptr;
    velocityRangeLabel = nullptr;


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

    timingRangeSlider->setBounds (8, 40, 104, 88);
    timingRangeLabel->setBounds (0, 0, 120, 40);
    velocityRangeSlider->setBounds (128, 40, 104, 88);
    velocityRangeLabel->setBounds (120, 0, 120, 40);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void MainEditorWindow::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    setNormalizedValueFromSlider (sliderThatWasMoved);
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == timingRangeSlider)
    {
        //[UserSliderCode_timingRangeSlider] -- add your slider handling code here..
        //[/UserSliderCode_timingRangeSlider]
    }
    else if (sliderThatWasMoved == velocityRangeSlider)
    {
        //[UserSliderCode_velocityRangeSlider] -- add your slider handling code here..
        //[/UserSliderCode_velocityRangeSlider]
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
    setSliderRange (timingRangeSlider);
    setSliderRange (velocityRangeSlider);
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
    updateSliderValue (timingRangeSlider);
    updateSliderValue (velocityRangeSlider);
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
    if (slider == timingRangeSlider)
        return mParentProcessor->timingRange;

    if (slider == velocityRangeSlider)
        return mParentProcessor->velocityRange;

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
                 fixedSize="1" initialWidth="240" initialHeight="128">
  <BACKGROUND backgroundColour="ff323232"/>
  <SLIDER name="Timing Range Slider" id="2a2a0656b43acdbd" memberName="timingRangeSlider"
          virtualName="" explicitFocusOrder="0" pos="8 40 104 88" thumbcol="ff909090"
          trackcol="7fdddddd" rotarysliderfill="7fdddddd" rotaryslideroutline="66dddddd"
          textboxtext="ffdddddd" textboxbkgd="ff4a4a4a" textboxhighlight="40e4e4e4"
          min="0" max="15" int="0.01" style="RotaryVerticalDrag" textBoxPos="TextBoxAbove"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="Timing Range Label" id="a78ae72ed3b4912c" memberName="timingRangeLabel"
         virtualName="" explicitFocusOrder="0" pos="0 0 120 40" textCol="ffdddddd"
         edTextCol="ff000000" edBkgCol="0" labelText="Timing Range (ms)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="36"/>
  <SLIDER name="Velocity Range Slider" id="5524489392e19ee9" memberName="velocityRangeSlider"
          virtualName="" explicitFocusOrder="0" pos="128 40 104 88" thumbcol="ff909090"
          trackcol="7fdddddd" rotarysliderfill="7fdddddd" rotaryslideroutline="66dddddd"
          textboxtext="ffdddddd" textboxbkgd="ff4a4a4a" textboxhighlight="40e4e4e4"
          min="0" max="32" int="0.01" style="RotaryVerticalDrag" textBoxPos="TextBoxAbove"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="Velocity Range Label" id="d53ddcf2c0e714ef" memberName="velocityRangeLabel"
         virtualName="" explicitFocusOrder="0" pos="120 0 120 40" textCol="ffdddddd"
         edTextCol="ff000000" edBkgCol="0" labelText="Velocity Range"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="36"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
