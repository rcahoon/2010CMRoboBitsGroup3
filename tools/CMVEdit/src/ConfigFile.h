#ifndef CONFIGFILE_H_
#define CONFIGFILE_H_

#include <map>
#include <string>

#define CONFIG_FILE "config.txt"

class ConfigFile {
public:

  static ConfigFile & getSingleton() {
    static ConfigFile singleton(CONFIG_FILE);
    return singleton;
  }

  ~ConfigFile();

  std::string getString(const std::string & name) const;

  int getInt(const std::string & name);

  double getDouble(const std::string & name);

  std::string getPath(const std::string & name);

  unsigned long getMillisecondsFromSeconds(const std::string & name);

  double getRadiansFromDegrees(const std::string & name);

  double getCentimetersFromMeters(const std::string & name);

  double getMetersFromCentimeters(const std::string & name);

  bool getBool(const std::string & name);

  void reload();

private:
  // Prevent copy constructor, and assignment
  ConfigFile(const ConfigFile &);
  ConfigFile & operator=(const ConfigFile &);

  ConfigFile(std::string _filename);

  void readFile(const std::string & name);

  std::string trimString(std::string str);

  std::map<std::string, std::string> mapString;
  std::map<std::string, int> mapInt;
  std::map<std::string, double> mapDouble;

  std::string configDir, filename;
};


#endif /* CONFIGFILE_H_ */
