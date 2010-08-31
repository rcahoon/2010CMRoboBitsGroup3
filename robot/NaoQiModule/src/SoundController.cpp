#include "SoundController.h"
#include "../../Agent/Command/SoundCommand/SoundCommand.h"
#include "../../shared/ConfigFile/ConfigFile.h"
#include "../../Factory/Factory.h"
// Define the logging constants
#define COMPONENT MAIN_MODULE
//#define CLASS_LOG_LEVEL LOG_LEVEL_TRACE
#include "../../Log/LogSettings.h"

// Convert from 0-100 to 0-1
const double SoundController::volume = Factory::getNullFactory().getConfigFile().getFloat("speech/Volume",80) / 100;

SoundController::SoundController(AL::ALPtr<AL::ALBroker> broker)
  : isSpeaking(false),
    isPlaying(false),
    textToSay(),
    waveToPlay(),
    speechMutex(),
    waveMutex(),
    speechThread(),
    waveThread(),
    alive(true),
    loadedWaves() {
  //Create a proxy to the text-to-speech and audio modules
  try {
    speech = new AL::ALTextToSpeechProxy(broker);
    speech->setVolume(volume);
    audioPlayer = new AL::ALAudioPlayerProxy(broker);
    audioPlayer->setVolume(volume);
  }
  catch(const AL::ALError&) {
    std::cout << "could not create a proxy to Speech or AudioPlayer" << std::endl;
  }

  // Create the speech mutex
  if (pthread_mutex_init(&speechMutex, NULL) < 0) {
    return;
  }
  // Create the wave mutex
  if (pthread_mutex_init(&waveMutex, NULL) < 0) {
    return;
  }


  if (pthread_create(&speechThread, NULL, &SoundController::startSpeechThread,this) < 0) {
      //LOG_ERROR("Error creating thread to speak.");
      return;
  }
  if (pthread_create(&waveThread, NULL, &SoundController::startWaveThread, this) < 0) {
      //LOG_ERROR("Error creating thread to speak.");
      return;
  }
}

SoundController::~SoundController() {
  // We need to kill the threads, or Naoqi doesn't terminate properly
  alive = false;
  pthread_join(speechThread, NULL);
  pthread_join(waveThread, NULL);
}

void SoundController::executeSoundCommand(const SoundCommand & soundCommand) {
  const std::string & text = soundCommand.getTextToSay();
  
  if (text.length() > 0) {
    pthread_mutex_lock(&speechMutex);
    // Use the speech command only if the robot is not already speaking
    if (!isSpeaking) {
      isSpeaking = true;
      textToSay = text;
    }
    pthread_mutex_unlock(&speechMutex);
  }

  const std::string & wave = soundCommand.getWaveToPlay();
  if (wave.length() > 0) {
    pthread_mutex_lock(&waveMutex);
    // If the robot is already playing a wave, discard this speech command
    if (!isPlaying) {
      isPlaying = true;
      waveToPlay = wave;
    }
    pthread_mutex_unlock(&waveMutex);
  }
}

void* SoundController::startSpeechThread(void* obj)
{
    reinterpret_cast<SoundController*>(obj)->runSpeech();
    return 0;
}

void* SoundController::startWaveThread(void* obj)
{
    reinterpret_cast<SoundController*>(obj)->runWave();
    return 0;
}

void SoundController::runSpeech() 
{

  while(alive)
  {
      if (textToSay.length() > 0) {
          speech->say(textToSay.c_str());
          textToSay = "";
      }

      pthread_mutex_lock(&speechMutex);
      isSpeaking = false;
      pthread_mutex_unlock(&speechMutex);

      usleep(1000);
  }

}

void SoundController::runWave() {
    
  while(alive)
  {
      try {
        if (waveToPlay.length() > 0) {
          int waveID = 0;

          // Has it been loaded into memory?
          std::map<std::string, int>::const_iterator it = loadedWaves.find(waveToPlay);
          if (it != loadedWaves.end()) {
            waveID = it->second;
          }
          else {
            waveID = audioPlayer->loadFile(waveToPlay);
            loadedWaves[waveToPlay] = waveID;
          }

          audioPlayer->play(waveID);

          waveToPlay = "";
        }
      }
      catch(const AL::ALError&) {
        std::cout << "could not play .wav file: '" << waveToPlay << "'" << std::endl;
      }

      pthread_mutex_lock(&waveMutex);
      isPlaying = false;
      pthread_mutex_unlock(&waveMutex);
      
      usleep(1000);
  }
}
