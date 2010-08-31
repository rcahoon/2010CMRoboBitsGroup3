#ifndef HMATRIX_H_
#define HMATRIX_H_

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <vector>
#include <boost/serialization/utility.hpp>

#include "shared/Vector/Vector3D.h"

/* A quick and dirty matrix class. */

#define FIXED_MATRIX_SIZE 16

class FixedMatrix {

 public:
  int rows;
  int cols;

 private:
  float v[FIXED_MATRIX_SIZE];

 public:

  FixedMatrix() :
    rows(0), cols(0) {
	assert(rows * cols <= FIXED_MATRIX_SIZE);
  }

  FixedMatrix(int _rows, int _cols) :
    rows(_rows), cols(_cols) {

    assert(rows > 0);
    assert(cols > 0);
	assert(rows * cols <= FIXED_MATRIX_SIZE);
  }

  FixedMatrix(const FixedMatrix &other) :
    rows(other.rows), cols(other.cols) {

    assert(rows > 0);
    assert(cols > 0);
	assert(rows * cols <= FIXED_MATRIX_SIZE);

    for(int i=0; i<rows*cols; i++)
      v[i] = other.v[i];
  }

  ~FixedMatrix() {
  }

  float get(int r, int c) const {
    assert(r>=0 && r < rows);
    assert(c>=0 && c < cols);

    return v[offset(r, c)];
  }

  void set(int r, int c, float value) {
    assert(r>=0 && r < rows);
    assert(c>=0 && c < cols);

    v[offset(r, c)] = value;
  }

  void setAll(float value) {
    for(int i=0; i<rows*cols; i++)
      v[i] = value;
  }

  void setFrom(const FixedMatrix *other) {
    assert(rows==other->rows);
    assert(cols==other->cols);

    for(int i=0; i<rows*cols; i++)
      v[i] = other->v[i];
  }

  FixedMatrix *getRow(int r) {
    assert(r >= 0 && r < rows);
    assert(rows > 0 && cols > 0);

    FixedMatrix *retval = new FixedMatrix(1, cols);
    for(int c=0; c<cols; c++)
      retval->set(0, c, get(r, c));

    return retval;
  }

  FixedMatrix *getCol(int c) {
    assert(c >= 0 && c < cols);
    assert(rows > 0 && cols > 0);

    FixedMatrix *retval = new FixedMatrix(rows, 1);
    for(int r=0; r<rows; r++)
      retval->set(r, 0, get(r, c));

    return retval;
  }

  /* Returns a num_r by num_c matrix starting with the
     entry at (r, c) inclusive.
  */
  FixedMatrix *getSubMatrix(int r, int c,
			     int num_r, int num_c) {

    assert(num_r > 0 && num_c > 0);
    FixedMatrix *retval = new FixedMatrix(num_r, num_c);

    for(int new_r=0; new_r<num_r; new_r++)
      for(int new_c=0; new_c<num_c; new_c++)
	retval->set(new_r, new_c, get(r + new_r, c + new_c));

    return retval;
  }

  void setSubMatrix(int r, int c, const FixedMatrix *other) {
    for(int nr=r; nr<r+other->rows; nr++)
      for(int nc=c; nc<c+other->cols; nc++)
	set(nr, nc, other->get(nr - r, nc - c));
  }

  void zero() {
    setAll(0.0);
  }

  void identity() {
    assert(rows==cols);

    setAll(0.0);
    for(int i=0; i<rows; i++)
      set(i, i, 1.0);
  }

  void add(const FixedMatrix *other) {
    assert(other->rows==rows);
    assert(other->cols==cols);

    for(int i=0; i<rows*cols; i++)
      v[i] += other->v[i];
  }

  void sub(const FixedMatrix *other) {
    assert(other->rows==rows);
    assert(other->cols==cols);

    for(int i=0; i<rows*cols; i++)
      v[i] -= other->v[i];
  }

  void mul(const FixedMatrix *other) {
    assert(rows > 0 && cols > 0);
    assert(other->rows > 0 && other->cols > 0);
    assert(cols==other->rows);

    int new_rows = rows;
    int new_cols = other->cols;
    float new_v[FIXED_MATRIX_SIZE];

    mulIntoBuffer(other, new_v);

    memcpy(v, new_v, FIXED_MATRIX_SIZE * sizeof(float));
    rows = new_rows;
    cols = new_cols;
  }

  /* Do a multiplication, but return a new matrix rather than
     messing with this one.
  */
  FixedMatrix *mulNew(const FixedMatrix *other) const {
    assert(rows > 0 && cols > 0);
    assert(other->rows > 0 && other->cols > 0);
    assert(cols==other->rows);

    FixedMatrix *retval = new FixedMatrix(rows, other->cols);

    mulIntoBuffer(other, retval->v);

    return retval;
  }

  void scale(float value) {
    for(int i=0; i<rows*cols; i++)
      v[i] *= value;
  }

  void transpose() {
    // There's got to be a way of doing this without a
    // scratch buffer, but I'm going for correct rather
    // than fast. (And probably missing correct regardless)

    float new_v[FIXED_MATRIX_SIZE];
    memset(new_v, 0, FIXED_MATRIX_SIZE * sizeof(float));

    int new_rows = cols;
    int new_cols = rows;

    for(int nr=0; nr<new_rows; nr++) {
      for(int nc=0; nc<new_cols; nc++) {
	int off = nr*new_cols + nc;
	new_v[off] = get(nc, nr); // deliberately transpose order in get
      }
    }

    memcpy(v, new_v, FIXED_MATRIX_SIZE * sizeof(float));
    rows = new_rows;
    cols = new_cols;
  }

  void print() {
    for(int r=0; r<rows; r++) {
      for(int c=0; c<cols; c++)
	printf("%0.4f ", get(r, c));
      printf("\n");
    }
  }

  void printToFile(const char *fname) const {
		FILE *f = fopen(fname,"w+");
    for(int r=0; r<rows; r++) {
      for(int c=0; c<cols; c++)
				fprintf(f,"%f ", get(r, c));
      	fprintf(f,"\n");
    }
		fclose(f);
  }

  // We assume the size of the current matrix is the same as the
  // matrix stored in the file.
  bool readFromFile(const char *file_name) {

    FILE *infile = fopen(file_name, "rt");
    if(!infile)
      return false;

    for(int r=0; r<rows; r++) {
      for(int c=0; c<cols; c++) {
	float v;
	if(fscanf(infile, " %f ", &v)!=1) {
	  zero(); // restore to a consistent state
	  printf("Unable to read values from %s\n", file_name);
	  return false;
	}

	set(r, c, v);
      }
    }

    fclose(infile);

    return true;
  }

 protected:
  inline int offset(int r, int c) const {
    return r*cols + c;
  }

  /* Compute the product of this * other and place the
     result into buffer. We assume that buffer is of
     the appropriate size.
  */
  void mulIntoBuffer(const FixedMatrix *other,
		     float *buffer) const {

    assert(rows > 0 && cols > 0);
    assert(other->rows > 0 && other->cols > 0);
    assert(cols==other->rows);

    int new_rows = rows;
    int new_cols = other->cols;

    for(int nr=0; nr<new_rows; nr++) {
      for(int nc=0; nc<new_cols; nc++) {
	// We can't use our utility function to compute the
	// offset as it's specific to our size
	int off = nr*new_cols + nc;

	buffer[off] = 0.0;
	for(int i=0; i<cols; i++)
	  buffer[off] += get(nr, i) *
	    other->get(i, nc);
      }
    }
  }
};

class HMatrix : public FixedMatrix {

 public:

  HMatrix() : FixedMatrix(4, 4) {
    identity();
  }

  HMatrix(int _rows, int _cols) : FixedMatrix(_rows, _cols) {
    assert(_rows==4);
    assert(_cols==4);
  }

  HMatrix(const HMatrix *other) : FixedMatrix(4, 4) {

    for(int r=0; r<rows; r++)
      for(int c=0; c<cols; c++)
	set(r, c, other->get(r, c));
  }

  HMatrix(const float data[3][4]) : FixedMatrix(4, 4) {
    identity();
    for(int r=0; r<3; r++)
      for(int c=0; c<4; c++)
	set(r, c, data[r][c]);
  }

  HMatrix(const std::vector<float> &data) : FixedMatrix(4, 4) {

    assert(data.size()==16);

    identity();
    size_t offset = 0;
    for(int r=0; r<4 && offset < data.size(); r++)
      for(int c=0; c<4 && offset < data.size(); c++)
	set(r, c, data[offset++]);
  }

  void initTranslation(float dx, float dy, float dz) {
    identity();

    set(0, 3, dx);
    set(1, 3, dy);
    set(2, 3, dz);
  }

  void initRotX(float theta_in_rad) {
    identity();
    set(1, 1,  cos(theta_in_rad));
    set(2, 1,  sin(theta_in_rad));
    set(1, 2, -sin(theta_in_rad));
    set(2, 2,  cos(theta_in_rad));
  }

  void initRotY(float theta_in_rad) {
    identity();
    set(0, 0,  cos(theta_in_rad));
    set(0, 2,  sin(theta_in_rad));
    set(2, 0, -sin(theta_in_rad));
    set(2, 2,  cos(theta_in_rad));
  }

  void initRotZ(float theta_in_rad) {
    identity();
    set(0, 0,  cos(theta_in_rad));
    set(1, 0,  sin(theta_in_rad));
    set(0, 1,  -sin(theta_in_rad));
    set(1, 1,  cos(theta_in_rad));
  }

  void mul(const float data[3][4]) {
    FixedMatrix *other = new HMatrix(data);
    FixedMatrix::mul(other);
    delete other;
  }

  void mul(const vector3d &pt) {
    FixedMatrix *other = vect2matrix(pt);
    FixedMatrix::mul(other);
    delete other;
  }

  void mul(const HMatrix *other) {
    FixedMatrix::mul(other);
  }

  Vector3D mulNew(const Vector3D &pt) const {
    FixedMatrix *other = vect2matrix(pt);

    FixedMatrix *result = FixedMatrix::mulNew(other);
    Vector3D retval = matrix2vect(result);

    delete other;
    delete result;
    return retval;
  }

  Vector3D getVector() {
    return matrix2vect(this);
  }

  void reverse() {

    FixedMatrix *temp = new FixedMatrix(4, 4);
    temp->identity();

    // Get the origin of this frame
    FixedMatrix *ab_origin = getSubMatrix(0, 3, /* pull matrix start at posn */
					   3, 1  /* pull 3 rows and 1 column of data */);

    // Get the rotation portion of this matrix
    FixedMatrix *a_to_b_rot_transposed = getSubMatrix(0, 0, /* take start at origin */
						       3, 3  /* pull a 3x3 */);
    // Take it's transpose
    a_to_b_rot_transposed->transpose();

    // This is all we need for the rotation portion of b_to_a
    temp->setSubMatrix(0, 0, a_to_b_rot_transposed);

    // Now we need -a_to_b_rot_transposed*a_origin
    a_to_b_rot_transposed->scale(-1.0);
    a_to_b_rot_transposed->mul(ab_origin);

    // Store our new origin transform in temp
    temp->setSubMatrix(0, 3, a_to_b_rot_transposed);

    // Copy our new transformation matrix into ourself
    setFrom(temp);

    delete ab_origin;
    delete a_to_b_rot_transposed;
    delete temp;
  }

  HMatrix & operator = (const HMatrix & other) {
    if (this != &other) {
      for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
          set(r, c, other.get(r, c));
        }
      }
    }

    return *this;
  }

private:
  FixedMatrix *vect2matrix(const Vector3D &pt) const {
    FixedMatrix *retval = new FixedMatrix(4, 1);
    retval->set(0, 0, pt.x);
    retval->set(1, 0, pt.y);
    retval->set(2, 0, pt.z);
    retval->set(3, 0, 1.0);
    return retval;
  }

  Vector3D matrix2vect(FixedMatrix *v) const {
    assert(v->rows==4 || v->rows==3);
    assert(v->cols==1);

    Vector3D retval;
    retval.set(v->get(0,0), v->get(1,0), v->get(2,0));

    return retval;
  }

  /**
   * Allow access for Boost serialization
   */
  friend class boost::serialization::access;
  BOOST_SERIALIZATION_SPLIT_MEMBER()

  template<class Archive>
  void save(Archive & ar, const unsigned int version) const {
    for (int r = 0; r < rows; r++) {
      for (int c = 0; c < cols; c++) {
        float value = get(r, c);
        ar & value;
      }
    }
  }

  template<class Archive>
  void load(Archive & ar, const unsigned int version) {
    identity();
    for (int r = 0; r < rows; r++) {
      for (int c = 0; c < cols; c++) {
        float value;
        ar & value;
        set(r, c, value);
      }
    }
  }

};

#endif
