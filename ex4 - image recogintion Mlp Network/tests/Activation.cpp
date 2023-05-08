#include "Activation.h"

int relu_one_val(float& x)
{
  if (x > 0.0)
  {
    return x;
  }
  return 0;
}

Matrix activation::relu(Matrix vec)
{
  if (vec.get_cols() > 1)
  {
    throw std::runtime_error("this is not vector");
  }
  Matrix new_vec = Matrix(vec.get_rows(), 1);
  for (int i = 0; i < vec.get_rows(); ++i)
  {
    new_vec[i] = relu_one_val (vec[i]);
  }
  return new_vec;
}

Matrix activation::softmax(Matrix vec)
{
  if (vec.get_cols() > 1)
  {
    throw std::runtime_error("this is not vector");
  }
  Matrix output_vec = Matrix(vec.get_rows(), 1);
  float sum = 0;
  for (int i = 0; i < vec.get_rows(); ++i)
  {
    sum += std::exp (vec[i]);
    output_vec[i] = std::exp (vec[i]);
  }
  return (1/sum)*output_vec;
}