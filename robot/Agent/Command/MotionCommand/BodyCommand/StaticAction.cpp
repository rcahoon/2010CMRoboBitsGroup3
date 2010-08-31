#include "StaticAction.h"

StaticAction::StaticAction(StaticActionType _actionID)
  : actionID(_actionID){
}

StaticAction::~StaticAction() {
}

StaticAction::StaticActionType StaticAction::getActionID() const {
  return actionID;
}

void StaticAction::setActionID(StaticActionType _actionID) {
  actionID = _actionID;
}
