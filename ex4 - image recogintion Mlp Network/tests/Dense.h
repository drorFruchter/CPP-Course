#ifndef DENSE_H
#define DENSE_H

#include "Activation.h"

class Dense
{
 private:
  Matrix _weights;
  Matrix _bias;
  Activation_f _act_f;

 public:
  Dense();
  Dense(Matrix weights, Matrix bias, Activation_f act_f);
  const Matrix& get_weights() const;
  const Matrix& get_bias() const;
  Activation_f get_activation() const;
  Matrix operator()(Matrix vec) const;
};

#endif //DENSE_H
