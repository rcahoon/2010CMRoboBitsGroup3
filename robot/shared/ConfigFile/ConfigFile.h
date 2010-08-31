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

#ifndef CONFIGFILE_H_
#define CONFIGFILE_H_

#include <map>
#include <string>
#include <vector>

/**
 * This class reads one or more text files to generate name-value pairs that can
 * be used to load constants or initial values of other classes. The file(s) is
 * opened only during the construction of the object, and when reload() is
 * called. Otherwise, other functions return values that were read when the
 * file(s) was opened.
 *
 * The text file should have the following format:
 *   name1 = value1
 *   name2 = value2
 * Names and values are stored as strings until another function is called to
 * process the value, e.g., getInt()
 *
 * Comments in the text file should use the # character, e.g.
 *   # This is a valid comment
 *   name1 = value1 # this is a valid comment too
 *
 * To read other files as input, use the ~INCLUDE command, e.g.
 *   ~INCLUDE other_file.txt
 * This will cause the ConfigFile class to load "other_file.txt" and process it
 * as well.
 *
 */
class ConfigFile {
public:

  /**
   * Constructor that reads from a given filename. The files will be actually
   * opened and read during the construction of the object.
   *
   * @param path root directory to search for files
   * @param filename the path to the file that should be read (from the root directory specified)
   */
  ConfigFile(const std::string & path, const std::string & filename);

  /**
   * Destructor.
   */
  ~ConfigFile();

  /**
   * Returns the value associated with the name. If the name does not exist in
   * the files, then an empty string is returned.
   *
   * @param name name of the value in the file(s)
   *
   * @return value as a string
   */
  std::string getString(const std::string & name,
                        const std::string & defaultValue = "");

  /**
   * Returns the path associated with the name. The value is read from the file,
   * and pre-pended by the path defined in the config file.
   *
   * For example, if the path is /home/nao/ and the value in the file is a.txt,
   * then getPath will return /home/nao/a.txt
   *
   * @param name name of the value in the file(s)
   *
   * @return path as a string
   */
  std::string getPath(const std::string & name,
                      const std::string & defaultValue = "");

  /**
   * Returns the value associated with the name. If the name does not exist in
   * the files, then the default value is returned.
   *
   * @param name name of the value to in the file(s)
   * @param defaultValue default value to be returned if name does not exist
   *
   * @return value as an integer
   */
  int getInt(const std::string & name,
             int defaultValue = 0);

  /**
   * Returns the value associated with the name. If the name does not exist in
   * the files, then the default value is returned.
   *
   * @param name name of the value in the file(s)
   * @param defaultValue default value to be returned if name does not exist
   *
   * @return value as a float
   */
  float getFloat(const std::string & name,
                 float defaultValue = 0);

  /**
   * Returns the value associated with the name. If the name does not exist in
   * the files, then the default value is returned. This function assumes that
   * the value in the file(s) is in seconds, and will convert it to
   * milliseconds.
   *
   * @param name name of the value (in seconds) in the file(s)
   * @param defaultValue default value to be returned if name does not exist
   *
   * @return value (milliseconds) as an unsigned long
   */
  unsigned long getMillisecondsFromSeconds(const std::string & name,
                                           unsigned long defaultValue = 0);

  /**
   * Returns the value associated with the name. If the name does not exist in
   * the files, then the default value is returned. This function assumes that
   * the value in the file(s) is in degrees, and will convert it to radians.
   *
   * @param name name of the value (in degrees) in the file(s)
   * @param defaultValue default value (in degrees) to be returned if name does not exist
   *
   * @return value (in radians) as a float
   */
  float getRadiansFromDegrees(const std::string & name,
                              float defaultValue = 0);

  /**
   * Returns the value associated with the name. If the name does not exist in
   * the files, then the default value is returned. This function assumes that
   * the value in the file(s) is in meters, and will convert it to centimeters.
   *
   * @param name name of the value (in meters) in the file(s)
   * @param defaultValue default value to be returned if name does not exist
   *
   * @return value (in centimeters) as a float
   */
  float getCentimetersFromMeters(const std::string & name,
                                 float defaultValue = 0);

  /**
   * Returns the value associated with the name. If the name does not exist in
   * the files, then the default value is returned. This function assumes that
   * the value in the file(s) is in centimeters, and will convert it to meters.
   *
   * @param name name of the value (in centimeters) in the file(s)
   * @param defaultValue default value to be returned if name does not exist
   *
   * @return value (in meters) as a float
   */
  float getMetersFromCentimeters(const std::string & name,
                                 float defaultValue = 0);

  /**
   * Returns the value associated with the name. If the name does not exist in
   * the files, then the default value is returned. This function assumes that
   * 0 means false, and any other integer means true.
   *
   * @param name name of the value (0 or 1) in the file(s)
   * @param defaultValue default value to be returned if name does not exist
   *
   * @return value as a bool
   */
  bool getBool(const std::string & name,
               bool defaultValue = false);

  /**
   * Reloads the file, and all included files (if any).
   */
  void reload();

  /**
   * Sets a name/value pair in the config file.
   * Warning: this overrides what is in the config file.
   *
   * @param name name in the config file
   * @param value new value it should take
   */
  void setString(const std::string & name,
                 const std::string & value);

  /**
   * Trim whitespace from a string, and any comments (starting with #)
   *
   * @param str string to trim
   */
  static std::string trimString(const std::string & str);

private:
  // Prevent copy constructor, and assignment
  ConfigFile(const ConfigFile &);
  ConfigFile & operator=(const ConfigFile &);

  void readFile(const std::string & name);

  std::string path, filename;

  std::map<std::string, std::string> mapString;
  std::map<std::string, int> mapInt;
  std::map<std::string, float> mapFloat;

  std::vector<std::string> filesOpened;

  bool useDefault;
};

#endif /* CONFIGFILE_H_ */
