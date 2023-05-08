#include "Matrix.h"

#define LENGTH_ERROR_MSG "The matrice/s are not the right size"
#define OUT_OF_RANGE_MSG "out of range error"

Matrix::Matrix(int rows, int cols)
{
  _rows = rows;
  _cols = cols;
  _mtr = new float*[rows];
  for (int i = 0; i < rows; ++i)
  {
    _mtr[i] = new float[cols];
    for (int j = 0; j < cols; ++j)
    {
      _mtr[i][j] = 0;
    }
  }
}

Matrix::Matrix(): Matrix (1, 1) {};

Matrix::Matrix(const Matrix& m)
{
  _rows = m._rows;
  _cols = m._cols;
  _mtr = new float*[_rows];
  for (int i = 0; i < _rows; ++i)
  {
    _mtr[i] = new float[_cols];
  }
  for (int i = 0; i < _rows; ++i)
  {
    for (int j = 0; j < _cols; ++j)
    {
      _mtr[i][j] = m._mtr[i][j];
    }
  }
}

Matrix::~Matrix()
{
  for (int i = 0; i < _rows; ++i)
  {
    delete(_mtr[i]);
  }
  delete(_mtr);
}

int Matrix::get_rows() const
{
  return _rows;
}

int Matrix::get_cols() const
{
  return _cols;
}

Matrix Matrix::transpose()
{
  Matrix copy_mtr = Matrix(*this);
  for (int i = 0; i < _rows; ++i)
  {
    for (int j = 0; j < _cols; ++j)
    {
      _mtr[j][i] = copy_mtr._mtr[i][j];
    }
  }
  return (*this);
}

Matrix Matrix::vectorize()
{
  Matrix vec_mtr = Matrix (_rows*_cols, 1);
  for (int i = 0; i < _rows; ++i)
  {
    for (int j = 0; j < _cols; ++j)
    {
      vec_mtr._mtr[i*_cols + j][0] = _mtr[i][j];
    }
  }
  _mtr = vec_mtr._mtr;
  _rows = vec_mtr._rows;
  _cols = vec_mtr._cols;
  return (*this);
}

void Matrix::plain_print()
{
  for (int i = 0; i < _rows; ++i)
  {
    for (int j = 0; j < _cols; ++j)
    {
      std::cout << _mtr[i][j] << " ";
    }
    std::cout << std::endl;
  }
}

Matrix Matrix::dot(Matrix& m) const
{
  if(_rows != m._rows || _cols != m._cols)
  {
    throw std::length_error(LENGTH_ERROR_MSG);
  }
  Matrix multi = Matrix(_rows, _cols);
  for (int i = 0; i < multi._rows; ++i)
  {
    for (int j = 0; j < multi._cols; ++j)
    {
      multi._mtr[i][j] = _mtr[i][j] * m._mtr[i][j];
    }
  }
  return multi;
}

float Matrix::norm() const
{
  float sum = 0;
  for (int i = 0; i < _rows; ++i)
  {
    for (int j = 0; j < _cols; ++j)
    {
      sum += powf(_mtr[i][j],2);
    }
  }
  return sqrtf (sum);
}

Matrix operator+(const Matrix& lm, const Matrix& rm)
{
  if(lm._cols != rm._cols || lm._rows != rm._rows)
  {
    throw std::length_error(LENGTH_ERROR_MSG);
  }
  Matrix new_m = Matrix(lm._rows,lm._cols);
  for (int i = 0; i < lm._rows; ++i)
  {
    for (int j = 0; j < lm._cols; ++j)
    {
      new_m._mtr[i][j] = lm._mtr[i][j] + rm._mtr[i][j];
    }
  }
  return new_m;
}

Matrix& Matrix::operator=(const Matrix& m) = default;

Matrix operator*(const Matrix& lm, const Matrix& rm)
{
  if(lm._cols != rm._rows)
  {
    throw std::length_error(LENGTH_ERROR_MSG);
  }
  Matrix new_m = Matrix(lm._rows, rm._cols);
  int cur_col = 0;
  float sum = 0;
  for (int i = 0; i < rm._rows; ++i)
  {
    for (int j = 0; j < lm._cols; ++j)
    {
      sum += lm._mtr[i][j] * rm._mtr[j][i];
    }
    new_m._mtr[i][cur_col] = sum;
    ++cur_col;
    sum = 0;
  }
  return new_m;
}

Matrix Matrix::operator*(float const c) const
{
  Matrix new_m = Matrix(_rows, _cols);
  for (int i = 0; i < _rows; ++i)
  {
    for (int j = 0; j < _cols; ++j)
    {
      new_m._mtr[i][j] = c*_mtr[i][j];
    }
  }
  return new_m;
}

Matrix operator*(const float c, const Matrix& m)
{
  return m * c;
}

Matrix& Matrix::operator+=(const Matrix& m)
{
  if(_rows != m._rows || _cols != m._cols)
  {
    throw std::length_error(LENGTH_ERROR_MSG);
  }
  for (int i = 0; i < _rows; ++i)
  {
    for (int j = 0; j < _cols; ++j)
    {
      _mtr[i][j] += m._mtr[i][j];
    }
  }
  return (*this);
}

float Matrix::operator()(int i, int j) const
{
  if (i < 0 || i > _rows - 1 || j < 0 || j > _cols - 1)
  {
    throw std::out_of_range(OUT_OF_RANGE_MSG);
  }
  return _mtr[i][j];
}

float& Matrix::operator()(int i, int j)
{
  if (i < 0 || i > _rows - 1 ||j < 0 || j > _cols - 1)
  {
    throw std::out_of_range(OUT_OF_RANGE_MSG);
  }
  return _mtr[i][j];
}

float Matrix::operator[](int i) const
{
  if (i < 0 || i > (_rows * _cols)-1)
  {
    throw std::out_of_range(OUT_OF_RANGE_MSG);
  }
  int right_row = i / _cols;
  int right_col = i % _cols;
  return _mtr[right_row][right_col];
}

float& Matrix::operator[](int i)
{
  if (i < 0 || i > _rows * _cols)
  {
    throw std::out_of_range(OUT_OF_RANGE_MSG);
  }
  int right_row = i / _cols;
  int right_col = i % _cols;
  return _mtr[right_row][right_col];
}

std::ostream& operator<<(std::ostream& s, const Matrix& m)
{
  for (int i = 0; i < m._rows; ++i)
  {
    for (int j = 0;j < m._cols; ++j)
    {
      if (m(i,j) > 0.1)
      {
        s << "**";
      }
      else
      {
        s << "  ";
      }
    }
    s << std::endl;
  }
  return s;
}

std::istream& operator>>(std::istream& s, Matrix& m)
{
  if(!s.read ((char*)m._mtr,m._rows * m._cols * sizeof (float)))
  {
    throw std::runtime_error ("reading error");
  }
  for (int i = 0; i < m._rows; ++i)
  {
    for (int j = 0;j < m._cols; ++j)
    {
      if(!(s >> m._mtr[i][j]))
      {
        return s;
      }
    }
  }
  return s;
}