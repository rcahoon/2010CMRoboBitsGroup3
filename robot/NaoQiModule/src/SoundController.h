#ifndef SOUNDCONTROLLER_H_
#define SOUNDCONTROLLER_H_

#include <pthread.h>

#include <albroker.h>
#include <alptr.h>
#include <altexttospeechproxy.h>
#include <alaudioplayerproxy.h>

#include <map>
#include <string>

#include "../../Agent/Command/SoundCommand/SoundCommand.h"

class SoundController {
public:
  SoundController(AL::ALPtr<AL::ALBroker> broker);
  ~SoundController();
  void executeSoundCommand(const SoundCommand & soundCommand);

  static void* startSpeechThread(void *ptr);
  static void* startWaveThread(void *ptr);
  void runSpeech();
  void runWave();

private:
  AL::ALTextToSpeechProxy* speech;
  AL::ALAudioPlayerProxy* audioPlayer;

  bool isSpeaking, isPlaying;
  std::string textToSay;
  std::string waveToPlay;

  pthread_mutex_t speechMutex;
  pthread_mutex_t waveMutex;
  pthread_t speechThread;
  pthread_t waveThread;

  bool alive;

  std::map<std::string, int> loadedWaves;

  static const double volume;
};

#endif /* SOUNDCONTROLLER_H_ */
