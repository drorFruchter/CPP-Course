#include "Activation.h"
#include "cmath"

/**
 * get one value and doing relu function on it
 * @param x
 * @return float number after relu function
 */
float relu_one_val(float& x)
{
  if (std::isgreaterequal (x, 0.0))
  {
    return x;
  }
  return 0;
}

Matrix activation::relu(Matrix vec)
{
  Matrix new_vec = Matrix(vec.get_rows(), vec.get_cols());
  for (int i = 0; i < vec.get_rows() * vec.get_cols(); ++i)
  {
    new_vec[i] = relu_one_val (vec[i]);
  }
  return new_vec;
}

Matrix activation::softmax(Matrix vec)
{
  Matrix output_vec = Matrix(vec.get_rows(), vec.get_cols());
  float sum = 0;
  for (int i = 0; i < vec.get_rows() * vec.get_cols(); ++i)
  {
    sum += std::exp (vec[i]);
    output_vec[i] = std::exp (vec[i]);
  }
  return (1/sum)*output_vec;
}