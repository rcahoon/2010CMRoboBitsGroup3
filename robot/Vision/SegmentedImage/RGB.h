#ifndef RGB_H_
#define RGB_H_

class RGB {
public:
  RGB(unsigned char red = 0, unsigned char green = 0, unsigned char blue = 0);
  virtual ~RGB();

  void set(unsigned char red, unsigned char green, unsigned char blue);

  int getColor() const;

  unsigned char getRed() const;

  unsigned char getGreen() const;

  unsigned char getBlue() const;

private:
  unsigned char red, green, blue;

};

#endif /* RGB_H_ */
