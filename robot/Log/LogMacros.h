#ifndef LOGMACROS_H_
#define LOGMACROS_H_

// Disable logging if needed
#ifdef DISABLE_LOGGING
  #ifdef LOGGING
    #undef LOGGING
  #endif
#endif

// Disable log shape if needed
#ifdef DISABLE_LOG_SHAPE
  #ifdef LOG_SHAPE_ENABLED
    #undef LOG_SHAPE_ENABLED
  #endif
#endif

#ifndef CLASS_LOG_LEVEL
  #define CLASS_LOG_LEVEL LOG_LEVEL_NONE
#endif

// Check that the class log level is valid
#if (CLASS_LOG_LEVEL != LOG_LEVEL_NONE) && (CLASS_LOG_LEVEL != LOG_LEVEL_TRACE) && (CLASS_LOG_LEVEL != LOG_LEVEL_DEBUG) && (CLASS_LOG_LEVEL != LOG_LEVEL_INFO) && (CLASS_LOG_LEVEL != LOG_LEVEL_WARN) && (CLASS_LOG_LEVEL != LOG_LEVEL_ERROR)
  // Create a compile error
  #error CLASS_LOG_LEVEL is invalid
#endif

// Component logging level
#ifndef COMPONENT
  #define COMPONENT OTHERS
  #define COMPONENT_LOG_LEVEL LOG_LEVEL_NONE
#else
  #if COMPONENT == AGENT
    #define COMPONENT_LOG_LEVEL AGENT_LOG_LEVEL
  #elif COMPONENT == BEHAVIORS
    #define COMPONENT_LOG_LEVEL BEHAVIORS_LOG_LEVEL
  #elif COMPONENT == COMM
    #define COMPONENT_LOG_LEVEL COMM_LOG_LEVEL
  #elif COMPONENT == GAME_CONTROLLER
    #define COMPONENT_LOG_LEVEL GAME_CONTROLLER_LOG_LEVEL
  #elif COMPONENT == LOCALIZATION
    #define COMPONENT_LOG_LEVEL LOCALIZATION_LOG_LEVEL
  #elif COMPONENT == VISION
    #define COMPONENT_LOG_LEVEL VISION_LOG_LEVEL
  #elif COMPONENT == WORLD_MODEL
    #define COMPONENT_LOG_LEVEL WORLD_MODEL_LOG_LEVEL
  #elif COMPONENT == MAIN_MODULE
    #define COMPONENT_LOG_LEVEL MAIN_MODULE_LOG_LEVEL
  #else
    #define COMPONENT_LOG_LEVEL OTHER_COMPONENT_LOG_LEVEL
  #endif
#endif

// Check that the component log level is valid
#if (COMPONENT_LOG_LEVEL != LOG_LEVEL_NONE) && (COMPONENT_LOG_LEVEL != LOG_LEVEL_TRACE) && (COMPONENT_LOG_LEVEL != LOG_LEVEL_DEBUG) && (COMPONENT_LOG_LEVEL != LOG_LEVEL_INFO) && (COMPONENT_LOG_LEVEL != LOG_LEVEL_WARN) && (COMPONENT_LOG_LEVEL != LOG_LEVEL_ERROR)
  // Create a compile error
  #error COMPONENT_LOG_LEVEL is invalid
#endif

#ifndef MODULE_LOG_LEVEL
  #define MODULE_LOG_LEVEL LOG_LEVEL_NONE
#endif

// Check that the module log level is valid
#if (MODULE_LOG_LEVEL != LOG_LEVEL_NONE) && (MODULE_LOG_LEVEL != LOG_LEVEL_TRACE) && (MODULE_LOG_LEVEL != LOG_LEVEL_DEBUG) && (MODULE_LOG_LEVEL != LOG_LEVEL_INFO) && (MODULE_LOG_LEVEL != LOG_LEVEL_WARN) && (MODULE_LOG_LEVEL != LOG_LEVEL_ERROR)
  // Create a compile error
  #error MODULE_LOG_LEVEL is invalid
#endif

// Overall Logging level
// (takes the min of module, component and class log levels)
#if MODULE_LOG_LEVEL <= CLASS_LOG_LEVEL
  #if MODULE_LOG_LEVEL <= COMPONENT_LOG_LEVEL
    #define OVERALL_LOG_LEVEL MODULE_LOG_LEVEL
  #else
    #define OVERALL_LOG_LEVEL COMPONENT_LOG_LEVEL
  #endif
#else
  #if CLASS_LOG_LEVEL <= COMPONENT_LOG_LEVEL
    #define OVERALL_LOG_LEVEL CLASS_LOG_LEVEL
  #else
    #define OVERALL_LOG_LEVEL COMPONENT_LOG_LEVEL
  #endif
#endif
#ifndef OVERALL_LOG_LEVEL
  #define OVERALL_LOG_LEVEL LOG_LEVEL_NONE
#endif

#ifdef LOGGING
  #if OVERALL_LOG_LEVEL <= LOG_LEVEL_TRACE
    #define LOG_TRACE_ACTIVE
    #define LOG_TRACE(format, ...) log.logText(LOG_LEVEL_TRACE, COMPONENT, __FILE__, __FUNCTION__, format, ## __VA_ARGS__);
  #else
    #define LOG_TRACE(format, ...)
  #endif

  #if OVERALL_LOG_LEVEL <= LOG_LEVEL_DEBUG
    #define LOG_DEBUG_ACTIVE
    #define LOG_DEBUG(format, ...) log.logText(LOG_LEVEL_DEBUG, COMPONENT, __FILE__, __FUNCTION__, format, ## __VA_ARGS__);
  #else
    #define LOG_DEBUG(format, ...)
  #endif

  #if OVERALL_LOG_LEVEL <= LOG_LEVEL_INFO
    #define LOG_INFO_ACTIVE
    #define LOG_INFO(format, ...) log.logText(LOG_LEVEL_INFO, COMPONENT, __FILE__, __FUNCTION__, format, ## __VA_ARGS__);
  #else
    #define LOG_INFO(format, ...)
  #endif

  #if OVERALL_LOG_LEVEL <= LOG_LEVEL_WARN
    #define LOG_WARN_ACTIVE
    #define LOG_WARN(format, ...) log.logText(LOG_LEVEL_WARN, COMPONENT, __FILE__, __FUNCTION__, format, ## __VA_ARGS__);
  #else
    #define LOG_WARN(format, ...)
  #endif

  #if OVERALL_LOG_LEVEL <= LOG_LEVEL_ERROR
    #define LOG_ERROR_ACTIVE
    #define LOG_ERROR(format, ...) log.logText(LOG_LEVEL_ERROR, COMPONENT, __FILE__, __FUNCTION__, format, ## __VA_ARGS__);
  #else
    #define LOG_ERROR(format, ...)
  #endif

  #define LOG_BEGIN(timestamp) log.logBegin(timestamp);
  #define LOG_END() log.logEnd();

  #ifdef LOG_SHAPE_ENABLED
    #define LOG_SHAPE(screen, shape) log.logShape(screen, shape);
  #else
    #define LOG_SHAPE(screen, shape)
  #endif

  #ifdef LOG_ROBOTSTATE_ENABLED
    #define LOG_ROBOTSTATE(robotState) log.logRobotState(robotState);
  #else
    #define LOG_ROBOTSTATE(robotState)
  #endif

  #ifdef LOG_SEGMENTEDIMAGE_ENABLED
    #define LOG_SEGMENTEDIMAGE(segmentedImage) log.logSegmentedImage(segmentedImage);
  #else
    #define LOG_SEGMENTEDIMAGE(segmentedImage)
  #endif

  #ifdef LOG_TEAM_COLOR_ENABLED
    #define LOG_TEAM_COLOR(isBlue) log.logTeamColor(isBlue);
  #else
    #define LOG_TEAM_COLOR(isBlue)
  #endif

  #ifdef LOG_GAMESTATE_ENABLED
    #define LOG_GAMESTATE(gameState) log.logGameState(gameState);
  #else
    #define LOG_GAMESTATE(gameState)
  #endif

#else
  #define LOG_TRACE(format, ...)
  #define LOG_DEBUG(format, ...)
  #define LOG_INFO(format, ...)
  #define LOG_WARN(format, ...)
  #define LOG_ERROR(format, ...)

  #define LOG_BEGIN(timestamp)
  #define LOG_END()

  #define LOG_SHAPE(screen, shape)

  #define LOG_ROBOTSTATE(robotState)

  #define LOG_SEGMENTEDIMAGE(segmentedImage)

  #define LOG_TEAM_COLOR(isBlue)

  #define LOG_GAMESTATE(gameState)
#endif

#endif /* LOGMACROS_H_ */
