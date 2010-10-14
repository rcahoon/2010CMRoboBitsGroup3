/* =============================================================================

Copyright 2009 Somchaya Liemhetcharat, Carnegie Mellon University

This file is part of ConfigFile.

ConfigFile is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

ConfigFile is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with ConfigFile.  If not, see <http://www.gnu.org/licenses/>.

============================================================================= */

#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#include "ConfigFile.h"

ConfigFile::ConfigFile(const std::string & _path, const std::string & _filename)
  : path(_path),
    filename(_filename),
    mapString(),
    mapInt(),
    mapFloat(),
    filesOpened(),
    useDefault(false) {
  // Ensure that the path ends with a / if it's not empty
  if (!path.empty() && (path[path.length() - 1] != '/')) {
    path.append("/");
  }

  reload();
}

ConfigFile::~ConfigFile() {
}

void ConfigFile::reload() {
  // Clear all the maps
  mapString.clear();
  mapInt.clear();
  mapFloat.clear();

  // Clear the list of opened files
  filesOpened.clear();

  // Reload the main file
  readFile(filename);
}

std::string ConfigFile::getString(const std::string & name, const std::string & defaultValue) {
  useDefault = false;

  std::map<std::string, std::string>::const_iterator it = mapString.find(name);
  if (it != mapString.end()) {
    return it->second;
  }

  std::cerr << name << " not found in ConfigFile." << std::endl;

  // Return an empty string by default
  useDefault = true;
  return defaultValue;
}

std::string ConfigFile::getPath(const std::string & name, const std::string & defaultValue) {
  std::string str = getString(name);
  if (!useDefault) {
    return path + str;
  }
  else {
    return path + defaultValue;
  }
}

int ConfigFile::getInt(const std::string & name, int defaultValue, bool override) {
  useDefault = false;

  // Check if the map to int has the key we need
  if (!override) {
    std::map<std::string, int>::const_iterator it = mapInt.find(name);
    if (it != mapInt.end()) {
      return it->second;
    }
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

  // Return default value
  useDefault = true;
  return defaultValue;
}

const int & ConfigFile::getIntReference(const std::string & name,
                                        int defaultValue) {
  // Try to read the value
  getInt(name, defaultValue);

  // Was the default value used?
  if (useDefault) {
    // Set the value in the map
    mapInt[name] = defaultValue;
  }

  return mapInt[name];
}

float ConfigFile::getFloat(const std::string & name, float defaultValue, bool override) {
  useDefault = false;

  // Check if the map to float has the key we need
  if (!override) {
    std::map<std::string, float>::const_iterator it = mapFloat.find(name);
    if (it != mapFloat.end()) {
      return it->second;
    }
  }

  // Check if the key exists in the map to string
  std::string strValue = getString(name);
  std::istringstream iss(strValue);
  float outputValue;
  if (iss >> outputValue) {
    mapFloat[name] = outputValue;
    return outputValue;
  }

  // Return default value
  useDefault = true;
  return defaultValue;
}

unsigned long ConfigFile::getMillisecondsFromSeconds(const std::string & name,
                                                     unsigned long defaultValue) {
  // Retrieve the value in the file (in seconds)
  float seconds = getFloat(name);
  // Convert it to milliseconds
  if (!useDefault) {
    return (unsigned long)(seconds * 1000.0);
  }
  else {
    return defaultValue;
  }
}

float ConfigFile::getRadiansFromDegrees(const std::string & name,
                                        float defaultValue) {
  // Retrieve the value in the file (in degrees)
  float degrees = getFloat(name);
  // Convert it to radians
  if (!useDefault) {
    return degrees / 180.0 * M_PI;
  }
  else {
    return defaultValue / 180.0 * M_PI;
  }
}

float ConfigFile::getCentimetersFromMeters(const std::string & name,
                                           float defaultValue) {
  // Retrieve the value in the file (in m)
  float meters = getFloat(name);
  // Convert it to centimeters
  if (!useDefault) {
    return meters * 100.0;
  }
  else {
    return defaultValue;
  }
}

float ConfigFile::getMetersFromCentimeters(const std::string & name,
                                           float defaultValue) {
  // Retrieve the value in the file (in cm)
  float centimeters = getFloat(name);
  // Convert it to meters
  if (!useDefault) {
    return centimeters / 100.0;
  }
  else {
    return defaultValue;
  }
}

bool ConfigFile::getBool(const std::string & name,
                         bool defaultValue) {
  // Retrieve the value in the file (as an int)
  int value = getInt(name);
  // Cast the int to a bool (0 = false, everything else = true)
  if (!useDefault) {
    return (bool)value;
  }
  else {
    return defaultValue;
  }
}

void ConfigFile::setString(const std::string & name,
                           const std::string & value) {
  bool hadInt(false), hadFloat(false);

  // Retrieve the previous int and float values (if any)
  hadInt   = (mapInt.find(name) != mapInt.end());
  hadFloat = (mapFloat.find(name) != mapFloat.end());

  // Set the value in the map of strings
  mapString[name] = value;

  // Replace the mapped values if they existed before
  if (hadInt) {
    getInt(name, mapInt[name], true);
  }
  if (hadFloat) {
    getFloat(name, mapFloat[name], true);
  }
}

void ConfigFile::readFile(const std::string & name) {
  std::ifstream inputFile((path + name).c_str());
  std::string line;

  filesOpened.push_back(name);

  // Check if file was opened successfully
  if (!inputFile.good()) {
    std::cerr << "Failed to open file: " << name << std::endl;
    return;
  }

  int lineNum = 0;

  while (getline(inputFile, line)) {

    lineNum++;

    if (!inputFile.good()) {
      std::cerr << "Error reading file: " << name << " at line number " << lineNum << std::endl;
    }

    // Trim off comments (starting with #) and spaces/tabs
    std::string trimmedLine = trimString(line);

    // Check if the line is empty
    if (trimmedLine.size() == 0) {
      continue;
    }

    // Check if we should include another file
    size_t importPos = trimmedLine.find("~INCLUDE ");
    if (importPos == 0) {
      // We should retrieve the filename by trimming off the command
      std::string filename = trimmedLine.substr(9); // 9 is the length of "~INCLUDE "
      filename = trimString(filename);

      // Make sure we have a filename
      if (filename.size() == 0) {
        std::cerr << "Error reading ~INCLUDE command in file: " << name << " at line number " << lineNum << std::endl;
        continue;
      }
      else {
        // Make sure we haven't already imported the file
        bool alreadyImported = false;
        for (std::vector<std::string>::const_iterator iter = filesOpened.begin();
             iter != filesOpened.end();
             iter++) {
          if (filename.compare(*iter) == 0) {
            std::cerr << "Repeated ~INCLUDE in file: " << name << " at line number " << lineNum << std::endl;
            alreadyImported = true;
            break;
          }
        }

        if (!alreadyImported) {
          readFile(filename);
        }
        continue;
      }
    }

    // Find the occurrence of =
    size_t equalPos = trimmedLine.find('=');
    if (equalPos == std::string::npos) {
      std::cerr << "Error parsing '=' file: " << name << " at line number " << lineNum << std::endl;
      continue;
    }

    std::string name = trimString(trimmedLine.substr(0, equalPos));
    std::string value = trimString(trimmedLine.substr(equalPos + 1));
    if ((name.size() == 0) || (value.size() == 0)) {
      std::cerr << "Error parsing name/value in file: " << name << " at line number " << lineNum << std::endl;
      continue;
    }

    mapString[name] = value;
  }
}

std::string ConfigFile::trimString(const std::string & str) {
  // Find the first character that is not whitespace
  size_t startPos = str.find_first_not_of(" \t");

  // Check if a comment marker (#) exists
  size_t commentPos = str.find_last_of("#");

  // Find the last character that is not whitespace, after comments
  size_t endPos;
  // If there are no comments, we just want the last non-whitespace character
  if (commentPos == std::string::npos) {
    endPos = str.find_last_not_of(" \t");
  }
  // If there are comments, we want the last non-whitespace before the comment
  else if (commentPos > 0) {
    endPos = str.find_last_not_of(" \t", commentPos - 1);
  }
  else {
    endPos = std::string::npos;
  }

  // If the string is entirely spaces or empty, return an empty string
  if ((startPos > endPos) ||
      (startPos == std::string::npos) ||
      (endPos == std::string::npos)) {
    return "";
  }
  // No comments, so just snip off whitespace
  else {
    return str.substr(startPos, endPos - startPos + 1);
  }
}
