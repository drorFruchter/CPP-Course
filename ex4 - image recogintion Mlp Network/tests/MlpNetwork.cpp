#include "MlpNetwork.h"
#include "Activation.h"

MlpNetwork::MlpNetwork (Matrix *weights, Matrix *bias)
{
  for(int i = 0; i < MLP_SIZE; ++i)
  {
    _weights[i] = weights[i];
    _bias[i] = bias[i];
  }
}

Dense* MlpNetwork::init_dense() const
{
  Dense* all_dense = new Dense[MLP_SIZE];
  for (int i = 0;i < MLP_SIZE; ++i)
  {
    if (i == 3)
    {
      all_dense[i] = Dense(_weights[i], _bias[i], activation::softmax);
    }
    else
    {
      all_dense[i] = Dense(_weights[i], _bias[i], activation::relu);
    }
  }
  return all_dense;
}

Matrix run_network(Matrix img, Dense* all_dense)
{
  Matrix output_vec = Matrix();
  for (int i = 0;i < MLP_SIZE; ++i)
  {
    output_vec = all_dense[i](img);
    img = output_vec;
  }
  return output_vec;
}

digit MlpNetwork::operator()(Matrix img) const
{
  Dense* all_dense = init_dense();
  Matrix output_vec = run_network (img, all_dense);
  float max_prob = 0;
  unsigned int guess = 0;
  for (int i = 0; i < 10; ++i)
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