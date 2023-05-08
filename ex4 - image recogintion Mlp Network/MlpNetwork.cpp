#include "MlpNetwork.h"
#include "Activation.h"

#define FINAL_VEC_SIZE 10

MlpNetwork::MlpNetwork (Matrix *weights, Matrix *bias)
{
  for(int i = 0; i < MLP_SIZE; ++i)
  {
    _weights[i] = weights[i];
    _bias[i] = bias[i];
  }
}

Matrix MlpNetwork::run_network(Matrix img) const
{
  Dense layer_i = Dense (_weights[0], _bias[0], activation::relu);
  Matrix output_vec = layer_i(img);
  img = output_vec;
  for (int i = 1;i < MLP_SIZE; ++i)
  {
    if (i != 3)
    {
      layer_i = Dense (_weights[i], _bias[i], activation::relu);
    }
    else
    {
      layer_i = Dense (_weights[i], _bias[i], activation::softmax);
    }
    output_vec = layer_i(img);
    img = output_vec;
  }
  return output_vec;
}

digit MlpNetwork::operator()(const Matrix& img) const
{
  Matrix output_vec = (*this).run_network (img);
  float max_prob = 0;
  unsigned int guess = 0;
  for (int i = 0; i < FINAL_VEC_SIZE; ++i)
  {
    if (output_vec[i] > max_prob)
    {
      max_prob = output_vec[i];
      guess = i;
    }
  }
  digit output = {guess, max_prob};
  return output;
}