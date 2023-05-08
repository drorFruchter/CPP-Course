// Matrix.h
#include "iostream"
#include <cmath>
#ifndef MATRIX_H
#define MATRIX_H

/**
 * @struct matrix_dims
 * @brief Matrix dimensions container. Used in MlpNetwork.h and main.cpp
 */
typedef struct matrix_dims
{
	int rows, cols;
} matrix_dims;

class Matrix
{
 private:
  float** _mtr;
  int _rows;
  int _cols;

 public:
  Matrix();
  Matrix(int rows, int cols);
  Matrix(const Matrix& m);
  ~Matrix();

  int get_rows() const;
  int get_cols() const;
  Matrix transpose();
  Matrix vectorize();
  void plain_print();

  Matrix dot(Matrix& m) const;
  float norm() const;

  friend Matrix operator+(const Matrix& lm, const Matrix& rm);
  Matrix& operator=(const Matrix& m);
  friend Matrix operator*(const Matrix& lm, const Matrix& rm);
  Matrix operator*(float c) const;
  friend Matrix operator*(float c, const Matrix& m);
  Matrix& operator+=(const Matrix& m);

  float operator()(int i, int j) const;
  float& operator()(int i, int j);

  float operator[](int i) const;
  float& operator[](int i);


  friend std::ostream& operator<<(std::ostream& s, const Matrix& m);
  friend std::istream& operator>>(std::istream& s, Matrix& m);
};





#endif //MATRIX_H