#include "Processor.h"
#include "BlazarEditor.h"
juce::AudioProcessorEditor* Processor::createEditor(){return new blazar::Editor(*this);}
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter(){return new Processor();}
