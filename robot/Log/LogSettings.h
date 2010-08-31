#ifndef LOGSETTINGS_H_
#define LOGSETTINGS_H_

#include "Log.h"

#define LOGGING

// Should we log shapes?
#define LOG_SHAPE_ENABLED

// Should we log RobotState?
#define LOG_ROBOTSTATE_ENABLED

// Should we log segmented images?
#define LOG_SEGMENTEDIMAGE_ENABLED

// Should we log team color?
#define LOG_TEAM_COLOR_ENABLED

// Should we log game state?
#define LOG_GAMESTATE_ENABLED

// Logging level for module
#define MODULE_LOG_LEVEL LOG_LEVEL_WARN

// Logging level for specific components
#define AGENT_LOG_LEVEL           LOG_LEVEL_NONE
#define BEHAVIORS_LOG_LEVEL       LOG_LEVEL_NONE
#define COMM_LOG_LEVEL            LOG_LEVEL_NONE
#define GAME_CONTROLLER_LOG_LEVEL LOG_LEVEL_NONE
#define LOCALIZATION_LOG_LEVEL    LOG_LEVEL_NONE
#define MOTION_LOG_LEVEL          LOG_LEVEL_NONE
#define SCREEN_DISPLAY_LOG_LEVEL  LOG_LEVEL_NONE
#define VISION_LOG_LEVEL          LOG_LEVEL_NONE
#define WORLD_MODEL_LOG_LEVEL     LOG_LEVEL_NONE
#define MAIN_MODULE_LOG_LEVEL     LOG_LEVEL_NONE

// Logging level for non-specified components
#define OTHER_COMPONENT_LOG_LEVEL LOG_LEVEL_NONE

// Include the macros for the Log (which uses the settings here)
#include "LogMacros.h"

#endif /* LOGSETTINGS_H_ */
