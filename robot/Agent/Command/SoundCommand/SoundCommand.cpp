#include "SoundCommand.h"
#include <string.h>

SoundCommand::SoundCommand()
  : textToSay(),
    waveToPlay() {
}

SoundCommand::~SoundCommand() {
}

void SoundCommand::say(const std::string & newText) {
  strncpy(textToSay, newText.c_str(), MAX_SPEECH_LENGTH);
  textToSay[MAX_SPEECH_LENGTH - 1] = '\0';
}

void SoundCommand::playWavFile(const std::string & filename) {
  strncpy(waveToPlay, filename.c_str(), MAX_SPEECH_LENGTH);
  waveToPlay[MAX_SPEECH_LENGTH - 1] = '\0';
}

void SoundCommand::clear() {
  memset(textToSay,  0, MAX_SPEECH_LENGTH);
  memset(waveToPlay, 0, MAX_SPEECH_LENGTH);
}

const std::string SoundCommand::getTextToSay() const {
  return std::string(textToSay);
}

const std::string SoundCommand::getWaveToPlay() const {
  return std::string(waveToPlay);
}
