#ifndef SOUNDCOMMAND_H_
#define SOUNDCOMMAND_H_

#include <string>

#include "shared/UsefulMacros.h"

//TODO these constants should be defined somewhere in the config file somehow
#define MAX_SPEECH_LENGTH 255

class SoundCommand {
public:
  SoundCommand();

  ~SoundCommand();

  void say(const std::string & newText);

  void playWavFile(const std::string & filename);

  void clear();

  const std::string getTextToSay() const;

  const std::string getWaveToPlay() const;

private:
  PREVENT_COPY_AND_ASSIGNMENT(SoundCommand);

  char textToSay[MAX_SPEECH_LENGTH];
  char waveToPlay[MAX_SPEECH_LENGTH];

};

#endif /* SOUNDCOMMAND_H_ */
