#include <iostream>
#include <signal.h>
#include <fstream>
#include <sstream>
#include <boost/archive/binary_iarchive.hpp>

#include "Agent/Agent.h"
#include "Agent/RobotState.h"
#include "Factory/Factory.h"
#include "shared/ConfigFile/ConfigFile.h"

void _terminationHandler(int signum) {
  exit(0);
}

int main(int argc, const char* argv[]) {
  if (argc < 2) {
    printf("Usage: %s [name of log file] [[starting frame number]]\n", argv[0]);
    return 0;
  }

  // Set up signal handler (to capture ctrl-c)
  struct sigaction new_action;
  new_action.sa_handler = _terminationHandler;
  sigemptyset(&new_action.sa_mask);
  new_action.sa_flags = 0;
  sigaction(SIGINT, &new_action, NULL);

  // Open the log file for reading
  std::ifstream fileStream(argv[1], std::ios::in | std::ios::binary);

  // Which factory are we using?
  Factory & factory = Factory::getDefaultFactory();

  // Get an instance of the ConfigFile
  ConfigFile & configFile = factory.getConfigFile();
  // We don't want to log RobotState
  configFile.setString("log/logRemote/logRobotStatePeriod", "0");
  // We always want to log images
  configFile.setString("log/logRemote/logOriginalImagePeriod", "1");
  configFile.setString("log/logRemote/logSegmentedImagePeriod", "1");
  configFile.setString("configurable/log/useLogToRemote", "1");

  //Set it to AlwaysPlaying for gameController
  configFile.setString("configurable/gameController", "AlwaysPlaying");

  // Create an instance of Agent
  Agent agent(factory);
 
  int logFrameCount = 0;
  bool done = false;
  
  if ( argc == 3 )
  {
    for ( int i = 0; i < atoi(argv[2]); i++ )
    {
        // Read the size of the serialized data      
        char sizeBytes[4];
        fileStream.read(sizeBytes, 4);
        if (!fileStream.good()) {
          done = true;
          break;
        }
        int size = ((sizeBytes[0] << 24) & 0xFF000000)
                 | ((sizeBytes[1] << 16) & 0x00FF0000)
                 | ((sizeBytes[2] << 8)  & 0x0000FF00)
                 | ( sizeBytes[3]        & 0x000000FF);

        // Read the bytes of the serialized RobotState
        char *data = new char[size];
        fileStream.read(data, size);
        if (!fileStream.good()) {
          delete [] data;
          done = true;
          break;
        }

        RobotState robotState;

        // De-serialize it using Boost
        std::string robotStateStr(data, size);
        std::istringstream iss(robotStateStr, std::stringstream::in | std::stringstream::binary);
        boost::archive::binary_iarchive inputArchive(iss);
        inputArchive & robotState;

        // Read the image
        if (robotState.getRawImageSize() > 0) {
          // Set the RobotState's image
          char *image = data + size - robotState.getRawImageSize();
          robotState.setRawImage(image);
        }
        else {
          printf("RobotState has an invalid raw image size.\n");
          delete [] data;
          done = true;
          break;
        }

        logFrameCount++;

        delete [] data;
    }
  }
  
  done = false;
  while (!done) {



    // Read the size of the serialized data      
    char sizeBytes[4];
    fileStream.read(sizeBytes, 4);
    if (!fileStream.good()) {
      done = true;
      break;
    }
    int size = ((sizeBytes[0] << 24) & 0xFF000000)
             | ((sizeBytes[1] << 16) & 0x00FF0000)
             | ((sizeBytes[2] << 8)  & 0x0000FF00)
             | ( sizeBytes[3]        & 0x000000FF);

    // Read the bytes of the serialized RobotState
    char *data = new char[size];
    fileStream.read(data, size);
    if (!fileStream.good()) {
      delete [] data;
      done = true;
      break;
    }

    RobotState robotState;

    // De-serialize it using Boost
    std::string robotStateStr(data, size);
    std::istringstream iss(robotStateStr, std::stringstream::in | std::stringstream::binary);
    boost::archive::binary_iarchive inputArchive(iss);
    inputArchive & robotState;

    // Read the image
    if (robotState.getRawImageSize() > 0) {
      // Set the RobotState's image
      char *image = data + size - robotState.getRawImageSize();
      robotState.setRawImage(image);
    }
    else {
      printf("RobotState has an invalid raw image size.\n");
      delete [] data;
      done = true;
      break;
    }

    // Call Agent's run
    printf("Frame: %d\n", logFrameCount);
    agent.run(robotState);

    // Clean up the allocated memory
    delete [] data;

    logFrameCount++;

    //printf("Press a key to continue.\n");
    getchar();
  }

  printf("Reached end of log file.\n");
  fileStream.close();

  return 0;
}
