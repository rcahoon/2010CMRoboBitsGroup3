#ifndef HEADSTIFFNESS_H_
#define HEADSTIFFNESS_H_

class HeadStiffness {
public:

  /**
   * Constructor to set the head stiffness.
   */
  HeadStiffness(float stiffness = 0.8f);

  /**
   * Destructor.
   */
  virtual ~HeadStiffness();

  /**
   * Retrieve the requested head stiffness.
   *
   * @return head stiffness
   */
  float getStiffness() const;

  /**
   * Set the head stiffness.
   *
   * @param stiffness head stiffness
   */
  void setStiffness(float stiffness);

private:
  float stiffness;

};

#endif /* HEADSTIFFNESS_H_ */
