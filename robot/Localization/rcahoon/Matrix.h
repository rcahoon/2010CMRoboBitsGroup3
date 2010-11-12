#ifndef RCAHOON_MATRIX_H_
#define RCAHOON_MATRIX_H_

#include <assert.h>
#include <stdio.h>

namespace RCahoon {

class Matrix {
 private:
  float * const v;
  const unsigned mrows, mcols;
  
  // unsafe because of double-free
  //Matrix(float* _v, unsigned _rows, unsigned _cols) : v(_v), mrows(_rows), mcols(_cols) {}
 public:
  inline unsigned rows() const
  {
    return mrows;
  }
  inline unsigned cols() const
  {
    return mcols;
  }
  
  Matrix(unsigned _rows, unsigned _cols) : v(new float[_rows*_cols]), mrows(_rows), mcols(_cols) {
  	fill(0.0f);
  }
  Matrix(unsigned _rows, unsigned _cols, float* a) : v(new float[_rows*_cols]), mrows(_rows), mcols(_cols) {
  	copy(a);
  }

  Matrix(const Matrix &other) : v(new float[other.rows()*other.cols()]), mrows(other.rows()), mcols(other.cols()) {
    copy(other);
  }
  
  ~Matrix() {
    delete[] v;
  }
  
  inline float& operator ()(unsigned idx) {
    assert(idx < mrows*mcols);
    
    return v[idx];
  }
  inline const float operator()(unsigned idx) const {
    assert(idx < mrows*mcols);
    
    return v[idx];
  }
  inline float& operator ()(unsigned row, unsigned col) {
    assert(row < mrows);
    assert(col < mcols);
    
    return v[offset(row, col)];
  }
  inline const float operator ()(unsigned row, unsigned col) const {
    assert(row < mrows);
    assert(col < mcols);
    
    return v[offset(row, col)];
  }
  /*Matrix operator ()(unsigned rmin, unsigned rmax, unsigned cmin, unsigned cmax) {
    assert(rmax < mrows);
    assert(rmin < rmax);
    assert(cmax < mcols);
    assert(cmin < cmax);
    
    unsigned rsize = rmax - rmin;
    unsigned csize = cmax - cmin;
    return Matrix(&v[offset(rmin, cmin)], rsize, csize);
  }
  const Matrix operator ()(unsigned rmin, unsigned rmax, unsigned cmin, unsigned cmax) const {
    assert(rmax < mrows);
    assert(rmin < rmax);
    assert(cmax < mcols);
    assert(cmin < cmax);
    
    unsigned rsize = rmax - rmin;
    unsigned csize = cmax - cmin;
    return Matrix(&v[offset(rmin, cmin)], rsize, csize);
  }*/

  Matrix& operator =(float value) {
    fill(value);
    
    return *this;
  }
  Matrix& operator =(const Matrix& other) {
    copy(other);
    
    return *this;
  }
  Matrix& operator =(const float other[]) {
    copy(other);
    
    return *this;
  }

  Matrix& operator += (const Matrix& other) {
    assert(other.rows()==mrows);
    assert(other.cols()==mcols);

    for(unsigned i=0; i<mrows*mcols; i++)
      v[i] += other.v[i];
    
    return *this;
  }
  Matrix operator + (const Matrix& other) const {
    return Matrix(*this) += other;
  }

  Matrix& operator -=(const Matrix& other) {
    assert(other.rows()==mrows);
    assert(other.cols()==mcols);

    for(unsigned i=0; i<mrows*mcols; i++)
      v[i] -= other.v[i];
    
    return *this;
  }
  Matrix operator - (const Matrix& other) const {
    return Matrix(*this) -= other;
  }

  Matrix operator * (const Matrix& other) const {
    assert(mcols==other.rows());

    Matrix new_v(mrows, other.cols());

    for(unsigned nr=0; nr<new_v.rows(); nr++)
    for(unsigned nc=0; nc<new_v.cols(); nc++)
    {
      float sum = 0.0;
      for(unsigned i=0; i<mcols; i++)
        sum += v[offset(nr, i)] * other(i,nc);
      new_v(nr,nc) = sum;
    }
    
    return new_v;
  }

  // scaling operators
  Matrix& operator *= (float value) {
    for(unsigned i=0; i<mrows*mcols; i++)
      v[i] *= value;
    return *this;
  }
  Matrix operator * (float value) const {
    return Matrix(*this) *= value;
  }
  Matrix operator /= (float value) {
    for(unsigned i=0; i<mrows*mcols; i++)
      v[i] /= value;
    return *this;
  }
  Matrix operator / (float value) const {
    return Matrix(*this) /= value;
  }

  Matrix T() const {
    Matrix new_v(mcols, mrows);
    
    for(unsigned nr=0; nr<mrows; nr++)
    for(unsigned nc=0; nc<mcols; nc++)
      new_v(nc,nr) = v[offset(nr,nc)];
    
    return new_v;
  }
  
  // invert() originally from http://users.erols.com/mdinolfo/matrix.htm
  Matrix inv() const {
    assert(mrows==mcols);
    
    Matrix data(*this);
    
    if (mrows == 0)
      return data;
    if (mrows == 1)
    {
      data(0) = 1.0f/v[0];
      return data;
    }
    
    for (unsigned i=1; i < mrows; i++)
      data(i) /= data(0); // normalize row 0
    
    for (unsigned i=1; i < mrows; i++)
    {
      for (unsigned j=i; j < mrows; j++)
      {
        // do a column of L
        float sum = 0.0;
        for (unsigned k = 0; k < i; k++)  
          sum += data(j,k) * data(k,i);
        data(j,i) -= sum;
      }
      
      if (i == mrows-1) continue;
      
      for (unsigned j=i+1; j < mrows; j++)
      {
        // do a row of U
        float sum = 0.0;
        for (unsigned k = 0; k < i; k++)
            sum += data(i,k)*data(k,j);
        data(i,j) = (data(i,j)-sum) / data(i,i);
      }
    }
    for (unsigned i=0; i < mrows; i++)  // invert L
    {
      for (unsigned j=i; j < mrows; j++)
      {
        float x = 1.0;
        if ( i != j )
        {
          x = 0.0;
          for ( unsigned k = i; k < j; k++ ) 
            x -= data(j,k)*data(k,i);
        }
        data(j,i) = x / data(j,j);
      }
    }
    for (unsigned i = 0; i < mrows; i++)   // invert U
    {
      for ( unsigned j = i; j < mrows; j++ )
      {
        if ( i == j ) continue;
        float sum = 0.0;
        for ( unsigned k = i; k < j; k++ )
          sum += data(k,j)*( (i==k) ? 1.0 : data(i,k) );
        data(i,j) = -sum;
      }
    }
    for ( unsigned i = 0; i < mrows; i++ )   // final inversion
    {
      for ( unsigned j = 0; j < mrows; j++ )
      {
        float sum = 0.0;
        for ( unsigned k = ((i>j)?i:j); k < mrows; k++ )
          sum += ((j==k)?1.0:data(j,k))*data(k,i);
        data(j,i) = sum;
      }
    }
    
    return data;
  };
  
  float trace() const {
    float sum = 0.0f;  
    for(unsigned i=0; i<mrows; i++)
      sum += v[offset(i,i)];
    return sum;
  }

  void print() const {
    for(unsigned r=0; r<mrows; r++) {
      for(unsigned c=0; c<mcols; c++)
        printf("%0.4f ", v[offset(r, c)]);
      printf("\n");
    }
  }
  
  template<unsigned size>
  static Matrix I()
  {
    Matrix eye(size,size);
    for(unsigned i=0; i<size; i++)
      eye(i,i) = 1.0;
    return eye;
  }

 protected:
  inline unsigned offset(unsigned row, unsigned col) const {
    return row*mcols + col;
  }
  void copy(const Matrix& other)
  {
    assert(other.rows()==mrows);
    assert(other.cols()==mcols);
    
    for(unsigned i=0; i<mrows*mcols; i++)
      v[i] = other.v[i];
  }
  void copy(const float other[])
  {
    for(unsigned i=0; i<mrows*mcols; i++)
      v[i] = other[i];
  }
  void fill(float val)
  {
    for(unsigned i=0; i<mrows*mcols; i++)
      v[i] = val;
  }
};

}

#endif
