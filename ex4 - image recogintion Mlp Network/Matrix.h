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
  /**
   * allocates the matrix according to the rows and cols
   */
  void allocate_mat();
  /**
   * deleting the matrix until the row i(in case of allocation error)
   * @param i
   */
  void delete_mat(int i);

 public:
  /**
   * default constructor
   */
  Matrix();
  /**
   * constructor of matrix of size rows multiple cols
   * @param rows
   * @param cols
   */
  Matrix(int rows, int cols);
  /**
   * copy constructor
   * @param m
   */
  Matrix(const Matrix& m);
  /**
   * destructor
   */
  ~Matrix();

  /**
   * @return rows value
   */
  int get_rows() const;
  /**
   * @return cols value
   */
  int get_cols() const;
  /**
   * transposes this matrix
   */
  Matrix& transpose();
  /**
   * make this matrix to vector(matrix with 1 col)
   */
  Matrix& vectorize();
  /**
   * printing the matrix according to the format
   */
  void plain_print();

  /**
   * multiple each coordinate between the matrices
   * @param m
   * @return new matrix after the multiplication
   */
  Matrix dot(Matrix& m) const;
  /**
   * norm this matrix
   * @return float number after the norm
   */
  float norm() const;

  /**
   * operators overload
   */
  friend Matrix operator+(const Matrix& lm, const Matrix& rm);
  Matrix& operator=(const Matrix& m);
  friend Matrix operator*(const Matrix& lm, const Matrix& rm);
  Matrix operator*(float c) const;
  friend Matrix operator*(float c, const Matrix& m);
  Matrix& operator+=(const Matrix& m);

  /**
   * assignment and approach to specific coordinate operators overload
   */
  float operator()(int i, int j) const;
  float& operator()(int i, int j);

  float operator[](int i) const;
  float& operator[](int i);

  /**
   * extraction and destruction operators
   */
  friend std::ostream& operator<<(std::ostream& s, const Matrix& m);
  friend std::istream& operator>>(std::istream& s, Matrix& m);
};
#endif //MATRIX_H