#ifndef REMOTEMESSAGE_H_
#define REMOTEMESSAGE_H_

#include "shared/UsefulMacros.h"

/**
 * This is the superclass of all messages between the robot and a remote client.
 *
 */
class RemoteMessage {
public:

  /**
   * Constructor, which takes a size, a type, and a pointer to the data. The
   * data will be copied by this class.
   *
   * @param type type of the data
   * @param size size of the data
   * @param data pointer to the data that will be copied
   */
  RemoteMessage(int type, unsigned int size, void const * data);

  /**
   * Destructor.
   */
  virtual ~RemoteMessage();

  /**
   * Retrieves the size of the data.
   *
   * @return size of the data
   */
  unsigned int getSize() const;

  /**
   * Retrieves a pointer to the data.
   *
   * @return constant pointer to the data
   */
  void const * getData() const;

protected:
  RemoteMessage(int _type);

  /**
   * Retrieves the type of the data, as an int.
   *
   * @return type of the data (as an int)
   */
  int getTypeID() const;

  /**
   * Write an integer into a data buffer, in network format. 4 bytes will be
   * used.
   *
   * @param data pointer to the data buffer
   * @param value the value of the integer (in host format)
   */
  static void writeInt(char *data, int value);

  /**
   * Write a float into a data buffer. 4 bytes will be used.
   *
   * @param data pointer to the data buffer
   * @param value the value of the float
   */
  static void writeFloat(char *data, float value);

  /**
   * Read an integer from a data buffer, in network format. 4 bytes will be
   * read.
   *
   * @param data pointer to the data buffer
   * @return the value of the integer (in host format)
   */
  static int readInt(char const *data);

  /**
   * Read a float from a data buffer. 4 bytes will be read.
   *
   * @param data pointer to the data buffer
   * @return the value of the float
   */
  static float readFloat(char const *data);

  unsigned int  size;
  char         *data;

private:
  PREVENT_COPY_AND_ASSIGNMENT(RemoteMessage);

  int   type;
};

#endif /* REMOTEMESSAGE_H_ */
