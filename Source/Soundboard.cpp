//
// Created by Hannah Schellekens on 2021-10-26.
//


#include "Soundboard.h"
#include <utility>

SoundSample::SoundSample(String newName, String newFilePath, bool newLoop, int buttonColour)
        : name(std::move(newName)), filePath(std::move(newFilePath)), loop(newLoop), buttonColour(buttonColour)
{}

String SoundSample::getName() const
{
    return this->name;
}

void SoundSample::setName(String newName)
{
    this->name = std::move(newName);
}

String SoundSample::getFilePath() const
{
    return this->filePath;
}

void SoundSample::setFilePath(String newFilePath)
{
    this->filePath = std::move(newFilePath);
}

bool SoundSample::isLoop() const
{
    return loop;
}

void SoundSample::setLoop(bool newLoop)
{
    loop = newLoop;
}

int SoundSample::getButtonColour() const
{
    return buttonColour;
}

void SoundSample::setButtonColour(int newRgb)
{
    buttonColour = newRgb;
}

ValueTree SoundSample::serialize() const
{
    ValueTree tree(SAMPLE_KEY);
    tree.setProperty(NAME_KEY, name, nullptr);
    tree.setProperty(FILE_PATH_KEY, filePath, nullptr);
    tree.setProperty(LOOP_KEY, loop, nullptr);
    tree.setProperty(BUTTON_COLOUR_KEY, buttonColour, nullptr);

    return tree;
}

SoundSample SoundSample::deserialize(const ValueTree tree)
{
    SoundSample soundSample(
        tree.getProperty(NAME_KEY),
        tree.getProperty(FILE_PATH_KEY),
        tree.getProperty(LOOP_KEY, false),
        tree.getProperty(BUTTON_COLOUR_KEY, SonoPlaybackProgressButton::DEFAULT_BUTTON_COLOUR)
    );

    return soundSample;
}

Soundboard::Soundboard(String newName)
        : name(std::move(newName)), samples(std::vector<SoundSample>())
{}

String Soundboard::getName() const
{
    return this->name;
}

void Soundboard::setName(String newName)
{
    this->name = std::move(newName);
}

std::vector<SoundSample>& Soundboard::getSamples()
{
    return this->samples;
}

ValueTree Soundboard::serialize() const
{
    ValueTree tree(SOUNDBOARD_KEY);

    tree.setProperty(NAME_KEY, name, nullptr);

    ValueTree samplesTree(SAMPLES_KEY);

    tree.addChild(samplesTree, 0, nullptr);

    int i = 0;
    for (const auto &sample : samples) {
        samplesTree.addChild(sample.serialize(), i++, nullptr);
    }

    return tree;
}

Soundboard Soundboard::deserialize(ValueTree tree)
{
    Soundboard soundboard(tree.getProperty(NAME_KEY));

    auto samplesTree = tree.getChildWithName(SAMPLES_KEY);
    auto& samples = soundboard.getSamples();
    for( int i = 0; i < samplesTree.getNumChildren(); ++i )
    {
        samples.emplace_back(SoundSample::deserialize(samplesTree.getChild(i)));
    }

    return soundboard;
}