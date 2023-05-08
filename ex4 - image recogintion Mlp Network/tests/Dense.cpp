#include "Dense.h"

Dense::Dense(): _weights(Matrix()), _bias(Matrix()),_act_f(nullptr) {}

Dense::Dense(const Matrix weights, const Matrix bias, Activation_f act_f):
_weights(weights), _bias(bias), _act_f(act_f) {}

const Matrix& Dense::get_weights() const
{
  return _weights;
}
const Matrix& Dense::get_bias() const
{
  return _bias;
}
Activation_f Dense::get_activation() const
{
  return _act_f;
}
Matrix Dense::operator()(Matrix vec) const
{
  return _act_f(vec);
}