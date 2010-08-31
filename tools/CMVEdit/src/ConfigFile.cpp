#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <math.h>

#include <unistd.h>

#include "ConfigFile.h"


// Config file is initialized before logging is ready to be used

ConfigFile::ConfigFile(std::string _filename)
  : configDir("config/"),
    filename(_filename) {
  mapString["configDir"] = configDir;

  reload();
}

ConfigFile::~ConfigFile() {
}

void ConfigFile::reload() {
  printf("Reading config file\n");
  // Read the general config file
  readFile(configDir + filename);
}

std::string ConfigFile::getString(const std::string & name) const {
  std::map<std::string, std::string>::const_iterator it = mapString.find(name);
  if (it != mapString.end()) {
    return it->second;
  }

  std::cout << name << " not found in config file." << std::endl;

  return std::string();
}

int ConfigFile::getInt(const std::string & name) {
  // Check if the map to int has the key we need
  std::map<std::string, int>::const_iterator it = mapInt.find(name);
  if (it != mapInt.end()) {
    return it->second;
  }

  // Check if the key exists in the map to string
  std::string strValue = getString(name);
  std::istringstream iss(strValue);
  int outputValue;

  // Check if string is a hex value
  unsigned int found = 0;
  found = strValue.find("0x");
  if (found != std::string::npos) {
    if (iss >> std::hex >> outputValue) {
      mapInt[name] = outputValue;
      return outputValue;
    }
  }
  else if (iss >> outputValue) {
    mapInt[name] = outputValue;
    return outputValue;
  }

  return 0;
}

double ConfigFile::getDouble(const std::string & name) {
  // Check if the map to double has the key we need
  std::map<std::string, double>::const_iterator it = mapDouble.find(name);
  if (it != mapDouble.end()) {
    return it->second;
  }

  // Check if the key exists in the map to string
  std::string strValue = getString(name);
  std::istringstream iss(strValue);
  double outputValue;
  if (iss >> outputValue) {
    mapDouble[name] = outputValue;
    return outputValue;
  }

  return 0;
}

std::string ConfigFile::getPath(const std::string & name) {
  // Retrieve the filename
  std::string filename = getString(name);
  // Return the path and filename
  return configDir + filename;
}

unsigned long ConfigFile::getMillisecondsFromSeconds(const std::string & name) {
  // Retrieve the value in the file (in seconds)
  double seconds = getDouble(name);
  // Convert it to milliseconds
  return (unsigned long)(seconds * 1000.0);
}

double ConfigFile::getRadiansFromDegrees(const std::string & name) {
  // Retrieve the value in the file (in degrees)
  double degrees = getDouble(name);
  // Convert it to radians
  return degrees / 180.0 * M_PI;
}

double ConfigFile::getCentimetersFromMeters(const std::string & name) {
  // Retrieve the value in the file (in m)
  double meters = getDouble(name);
  // Convert it to centimeters
  return meters * 100.0;
}

double ConfigFile::getMetersFromCentimeters(const std::string & name) {
  // Retrieve the value in the file (in cm)
  double centimeters = getDouble(name);
  // Convert it to meters
  return centimeters / 100.0;
}

bool ConfigFile::getBool(const std::string & name) {
  // Cast the int to a bool (0 = false, everything else = true)
  return (bool)getInt(name);
}

void ConfigFile::readFile(const std::string & name) {
  // TODO: check for failure
  std::ifstream inputFile(name.c_str());
  std::string line;

  while (getline(inputFile, line)) {

    std::string trimmedLine = trimString(line);

    // Check if the line starts with a #
    if ((trimmedLine.size() == 0) || (trimmedLine[0] == '#')) {
      continue;
    }

    // Find the occurance of =
    size_t equalPos = trimmedLine.find('=');
    if (equalPos == std::string::npos) {
      std::cout << "Error parsing '=' in: " << line << std::endl;
      continue;
    }

    std::string name = trimString(trimmedLine.substr(0, equalPos));
    std::string value = trimString(trimmedLine.substr(equalPos + 1));
    if ((name.size() == 0) || (value.size() == 0)) {
      std::cout << "Error parsing name/value in: " << line << std::endl;
      continue;
    }

    mapString[name] = value;
  }
}

std::string ConfigFile::trimString(std::string str) {
  // Trim both leading and trailing spaces
  size_t startpos = str.find_first_not_of(" \t"); // Find the first character position after excluding leading blank spaces
  size_t endpos   = str.find_last_not_of(" \t");  // Find the first character position from reverse

  // if all spaces or empty return an empty string
  if ((startpos == std::string::npos) || (endpos == std::string::npos)) {
    return "";
  }
  else {
    return str.substr(startpos, endpos - startpos + 1);
  }
}
