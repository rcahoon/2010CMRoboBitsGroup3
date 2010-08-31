// This file is added only so that Eclipse can compile the robot code properly.

#include <string>
#include <iostream>
#include <fstream>
#include <sys/stat.h>

#include "Agent/Agent.h"
#include "Agent/RobotState.h"
#include "Agent/Command/Command.h"
#include "Factory/Factory.h"
#include "Log/Log.h"

char * readImage(std::string path) {
  char * image;

  struct stat results;
  if (stat(path.c_str(), &results) == 0) {
    image = new char[results.st_size];
  }
  else {
    printf("Error reading file size: %s\n", path.c_str());
    return NULL;
  }

  std::ifstream file(path.c_str(), std::ios::in | std::ios::binary);
  if (!file.read(image, results.st_size)) {
    printf("Error reading file: %s\n", path.c_str());
    delete [] image;
    return NULL;
  }

  return image;
}

int main(void) {
  Log & log = Factory::getDefaultFactory().getLog();

  Agent agent;

  char * image = readImage("/home/som/CMurfs/image1.yuv");

  RobotState robotState(log.getTimestamp() + 1,
                        false,
                        false,
                        false,
                        image,
                        153600);

  agent.run(robotState);

  return 0;
}
