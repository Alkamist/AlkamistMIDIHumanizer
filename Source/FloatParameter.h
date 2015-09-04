#ifndef FLOATPARAMETER_H_INCLUDED
#define FLOATPARAMETER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "LinearlySmoothedDouble.h"

class AlkamistMIDIHumanizerAudioProcessor;

class FloatParameter : public AudioProcessorParameter
{
public:

    FloatParameter (AlkamistMIDIHumanizerAudioProcessor* inputProcessor,
                    float defaultParameterValue,
                    float minimumParameterValue,
                    float maximumParameterValue,
                    const String& parameterName,
                    const String& parameterLabel,
                    double inputSampleRate,
                    int inputBlockSize);

    void processPerSample();
    void reset (double inputSampleRate, int inputBlockSize);
    inline void clearParameterChangeFlag() { mParameterChangeFlag = false; };

    // Getters
    inline float getDefaultValue() const override                           { return mDefaultValue; };
    inline String getLabel() const override                                 { return mLabel; };
    inline float getValueForText (const String& inputString) const override { return inputString.getFloatValue(); };
    inline float getValue() const override                                  { return mUnSmoothedParameterValue; };
    inline float getMinimum() const                                         { return mMinimumValue; };
    inline float getMaximum() const                                         { return mMaximumValue; };
    inline bool needsToChange() const                                       { return mParameterChangeFlag; };
    float getUnNormalizedSmoothedValue();
    float getUnNormalizedUnSmoothedValue();
    float getNormalizedSmoothedValue();
    String getName (int maximumStringLength) const override;
    String getText(float inputValue, int) const override;

    // Setters
    void setValue (float newValue) override;
    void setNormalizedValue (float nonNormalizedValue);

private:

    AlkamistMIDIHumanizerAudioProcessor* mParentProcessor;

    float mUnSmoothedParameterValue;
    float mDefaultValue;
    float mMinimumValue;
    float mMaximumValue;
    String mName;
    String mLabel;

    double mSampleRate;
    bool mParameterChangeFlag;

    LinearlySmoothedDouble mLinearlySmoothedDouble;
    NormalisableRange<float> mNormalizableRange;

};

#endif  // FLOATPARAMETER_H_INCLUDED
