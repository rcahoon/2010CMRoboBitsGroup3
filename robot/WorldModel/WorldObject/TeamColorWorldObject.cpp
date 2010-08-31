#include "TeamColorWorldObject.h"

// Define the logging constants
#define COMPONENT WORLD_MODEL
//#define CLASS_LOG_LEVEL LOG_LEVEL_INFO
#include "Log/LogSettings.h"

TeamColorWorldObject::TeamColorWorldObject(Log & _log,
                                           WorldObject::Type _type,
                                           bool _teamIsBlue)
  : WorldObject(_log, _type),
    teamIsBlue(_teamIsBlue) {
}

TeamColorWorldObject::TeamColorWorldObject(const TeamColorWorldObject & other)
  : WorldObject(other),
    teamIsBlue(other.teamIsBlue) {
}

TeamColorWorldObject::~TeamColorWorldObject() {
}

bool TeamColorWorldObject::isTeamBlue() const {
  return teamIsBlue;
}

void TeamColorWorldObject::setTeamColor(const bool _teamIsBlue) {
  teamIsBlue = _teamIsBlue;
}

void TeamColorWorldObject::clear() {
  WorldObject::clear();

  teamIsBlue = true;
}

void TeamColorWorldObject::copyFrom(const WorldObject & other) {
  WorldObject::copyFrom(other);

  // Try to cast the other object as a team-colored world object
  TeamColorWorldObject const *otherTeamColor = dynamic_cast<TeamColorWorldObject const *>(&other);
  if (otherTeamColor != NULL) {
    teamIsBlue = otherTeamColor->teamIsBlue;
  }
  // Default values, but give a warning
  else {
    teamIsBlue = true;

    LOG_WARN("Couldn't cast world object as a team-colored world object.");
  }
}

