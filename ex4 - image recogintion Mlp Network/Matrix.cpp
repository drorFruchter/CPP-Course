#include "Matrix.h"

#define LENGTH_ERROR_MSG "The matrice/s are not the right size"
#define OUT_OF_RANGE_MSG "out of range error"
#define MIN_VALUE 0.1

void Matrix::delete_mat(int rows_to_delete)
{
  for (int i = 0; i < rows_to_delete; ++i)
  {
    delete[] (this->_mtr[i]);
  }
  delete[] (this->_mtr);
}

void Matrix::allocate_mat()
{
  try
  {
    this->_mtr = new float *[this->_rows];
  }
  catch (const std::bad_alloc &e)
  {
    delete[]this->_mtr;
  }
  for (int i = 0; i < this->_rows; ++i)
  {
    try
    {
      this->_mtr[i] = new float[this->_cols];
    }
    catch (const std::bad_alloc &e)
    {
      this->delete_mat(i);
      break;
    }
  }
}


Matrix::Matrix(int rows, int cols)
{
  if(rows < 1 || cols < 1)
  {
    throw std::length_error(LENGTH_ERROR_MSG);
  }
  _rows = rows;
  _cols = cols;
  this->allocate_mat ();
  for (int i = 0; i < this->_rows; ++i)
  {
    for (int j = 0; j < this->_cols; ++j)
    {
      _mtr[i][j] = 0;
    }
  }
}

Matrix::Matrix(): Matrix (1, 1) {}

Matrix::Matrix(const Matrix& m)
{
  _rows = m._rows;
  _cols = m._cols;
  this->allocate_mat ();
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
    delete[](_mtr[i]);
  }
  delete[](_mtr);
}

int Matrix::get_rows() const
{
  return _rows;
}

int Matrix::get_cols() const
{
  return _cols;
}

Matrix& Matrix::transpose()
{
  Matrix copy_mtr = Matrix(_cols, _rows);
  for (int i = 0; i < copy_mtr._rows; ++i)
  {
    for (int j = 0; j < copy_mtr._cols; ++j)
    {
      copy_mtr._mtr[i][j] = _mtr[j][i];
    }
  }
  Matrix temp = *this;
  (*this) = copy_mtr;
  return (*this);
}

Matrix& Matrix::vectorize()
{
  if (_cols == 1)
  {
    return (*this);
  }
  Matrix vec_mtr = Matrix (_rows * _cols, 1);
  for (int i = 0; i < vec_mtr._rows; i++)
  {
    vec_mtr[i] = (*this)[i];
  }
  (*this) = vec_mtr;
  return (*this);
}

void Matrix::plain_print()
{
  for (int i = 0; i < _rows; i++)
  {
    for (int j = 0; j < _cols; j++)
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

Matrix& Matrix::operator=(const Matrix& m) //TODO: FIX THIS UGLY FUNCTION
{
  if(this == &m)
  {
    return (*this);
  }
  this->delete_mat (this->_rows);
  _cols = m._cols;
  _rows = m._rows;
  this->allocate_mat();
  for (int i = 0; i < _rows; i++)
  {
    for (int j = 0; j < _cols; j++)
    {
      this->_mtr[i][j] = m._mtr[i][j];
    }
  }
  return (*this);
}

Matrix operator*(const Matrix& lm, const Matrix& rm)
{
  if(lm._cols != rm._rows)
  {
    throw std::length_error(LENGTH_ERROR_MSG);
  }
  Matrix new_m = Matrix(lm._rows, rm._cols);
  for (int i = 0; i < new_m._rows; ++i)
  {
    for (int j = 0; j < new_m._cols; ++j)
    {
      for (int k = 0; k < lm._cols; ++k)
      {

        new_m._mtr[i][j] += lm._mtr[i][k] * rm._mtr[k][j];
      }
    }
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
  if (i < 0 || i > (_rows * _cols)-1)
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
      if (m(i,j) > MIN_VALUE)
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
  Matrix data = Matrix(1, m._rows * m._cols);
  if(!s.read ((char*)(*(data._mtr)) ,m._rows * m._cols * sizeof (float)))
  {
    throw std::runtime_error ("reading error");
  }
  for (int i = 0; i < m._rows * m._cols; ++i)
  {
      m[i] = (float)data[i];
  }
  return s;
}