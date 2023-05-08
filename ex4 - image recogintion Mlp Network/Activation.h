#include "Matrix.h"
#include "cmath"


#ifndef ACTIVATION_H
#define ACTIVATION_H

/**
 * activation pointer type definition
 */
typedef Matrix (*Activation_f)(Matrix vec);

namespace activation
{
  Matrix relu(Matrix vec);
  Matrix softmax(Matrix vec);
}

#endif //ACTIVATION_H