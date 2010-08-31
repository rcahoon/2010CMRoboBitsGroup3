#include <string>
#include <stdlib.h>
#include <iostream>
#include <cstring>

#include "Factory.h"
#include "Factory/Configurable/FactoryConfigurable.h"
#include "Factory/Null/NullFactory.h"

const std::string Factory::CONFIG_FILE("config.txt");

Factory & Factory::getDefaultFactory() {
  static FactoryConfigurable factory(getCMurfsPath(),
                                     CONFIG_FILE);

  return factory;
}

Factory & Factory::getNullFactory() {
  static NullFactory factory(getCMurfsPath(),
                             CONFIG_FILE);
  return factory;
}

std::string Factory::getCMurfsPath() {
  std::string path;
  char const * cmurfsPath = getenv("CMURFS_DIR");

  if ( cmurfsPath == NULL )
  {
    path = "/home/nao/config/";
  }
  else
  {
    path = cmurfsPath;
    path.append("/robot/config/");
  }

  return path;
}
