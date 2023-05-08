//MlpNetwork.h

#ifndef MLPNETWORK_H
#define MLPNETWORK_H

#include "Dense.h"

#define MLP_SIZE 4

/**
 * @struct digit
 * @brief Identified (by Mlp network) digit with
 *        the associated probability.
 * @var value - Identified digit value
 * @var probability - identification probability
 */
typedef struct digit {
	unsigned int value;
	float probability;
} digit;

const matrix_dims img_dims = {28, 28};
const matrix_dims weights_dims[] = {{128, 784},
									{64,  128},
									{20,  64},
									{10,  20}};
const matrix_dims bias_dims[] = {{128, 1},
								 {64,  1},
								 {20,  1},
								 {10,  1}};

class MlpNetwork
{
 private:
  Matrix _weights[4];
  Matrix _bias[4];
  Matrix run_network(Matrix img) const;

 public:
  /**
   * constructor
   * @param weights
   * @param bias
   */
  MlpNetwork(Matrix* weights, Matrix* bias);
  /**
   * running the network on the image
   * @param img
   * @return struct digit with the right guess and probability
   */
  digit operator()(const Matrix& img) const;
};

#endif // MLPNETWORK_H