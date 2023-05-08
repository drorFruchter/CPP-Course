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
  /**
   * constructor
   * @param weights
   * @param bias
   * @param act_f
   */
  Dense(const Matrix &weights, const Matrix &bias, Activation_f act_f);
  /**
   * getters
   * @return the value
   */
  const Matrix& get_weights() const;
  const Matrix& get_bias() const;
  Activation_f get_activation() const;
  /**
   * dense activator function
   * @param vec
   * @return matrix after the dense applied in vec
   */
  Matrix operator()(const Matrix& vec) const;
};

#endif //DENSE_H
