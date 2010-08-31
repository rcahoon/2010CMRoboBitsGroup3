#ifndef LOG_H_
#define LOG_H_

class ConfigFile;
class Messages;
class RobotState;
class SegmentedImage;
class Shape;
class GameState;

// Define log levels
#define LOG_LEVEL_TRACE 1
#define LOG_LEVEL_DEBUG 2
#define LOG_LEVEL_INFO  3
#define LOG_LEVEL_WARN  4
#define LOG_LEVEL_ERROR 5
#define LOG_LEVEL_NONE  6

// Define components
#define OTHERS          0
#define AGENT           1
#define BEHAVIORS       2
#define COMM            3
#define GAME_CONTROLLER 4
#define LOCALIZATION    5
#define LOG             6
#define VISION          7
#define WORLD_MODEL     8
#define MAIN_MODULE     9

/**
 * Base class of the logging system.
 */
class Log {
public:
  // The screen for shapes to appear
  enum Screen {
    OriginalImageScreen = 1,
    SegmentedImageScreen,
    Field,
    TextConsole,
  };

  Log(ConfigFile & configFile);

  virtual ~Log();

  /**
   * Gets any messages that the Log wants to transmit over the network.
   *
   * @param messages messages to be transmitted
   */
  virtual bool run(Messages & messages) = 0;

  /**
   * Marks the beginning of a log cycle.
   *
   * @param timestamp the current timestamp
   */
  virtual void logBegin(unsigned long timestamp) = 0;

  /**
   * Marks the end of a log cycle.
   */
  virtual void logEnd() = 0;

  /**
   * Log the state of the robot, including the original image.
   *
   * @param robotState state of the robot
   */
  virtual void logRobotState(const RobotState & robotState) = 0;

  /**
   * Log text output from the other components
   *
   * @param logLevel level of the text (trace, debug, info, warning, error)
   * @param component id of the component (Agent, Behaviors, etc)
   * @param className filled in by C++
   * @param functionName filled in by C++
   * @param format format of the text, like printf
   */
  void logText(int logLevel,
               int component,
               char const * className,
               char const * functionName,
               char const * format, ...);

  /**
   * Log text output from other components
   *
   * @param logLevel level of the text (trace, debug, info, warning, error)
   * @param component id of the component (Agent, Behaviors, etc)
   * @param className filled in by C++
   * @param text text to be logged
   */
  virtual void logTextFormatted(int logLevel,
                                int component,
                                char const * className,
                                char const * functionName,
                                char const * text,
                                int textLength) = 0;

  /**
   * Log the segmented image of the robot
   *
   * @param segmentedImage reference to the segmented image
   */
  virtual void logSegmentedImage(const SegmentedImage & segmentedImage) = 0;

  /**
   * Log a shape into a screen
   *
   * @param screen which screen to draw the shape in
   * @param shape the shape to draw
   */
  virtual void logShape(Screen screen, const Shape & shape) = 0;

  /**
   * Log the color of the team
   *
   * @param isBlue whether the robot is on the blue team
   */
  virtual void logTeamColor(bool isBlue) = 0;

  /**
   * Log the current game state
   *
   * @param gameState state of the game
   */
  virtual void logGameState(const GameState & gameState) = 0;

  unsigned long getTimestamp();

  /**
   * Informs Log component if there is a connection to a remote client,
   * i.e., Visualizer.
   *
   * @param connectedToClient whether there is a remote connection
   */
  virtual void setConnectedToClient(bool connectedToClient);

protected:
  bool isConnectedToClient() const;

private:
  unsigned int maxTextLength;
  char * textBuffer;

  bool connectedToClient;
};

#endif /* LOG_H_ */
