#include "Matrix.h"
#include "cmath"


#ifndef ACTIVATION_H
#define ACTIVATION_H

typedef Matrix (*Activation_f)(Matrix vec);

namespace activation
{
  Matrix relu(Matrix vec);
  Matrix softmax(Matrix vec);
}

#endif //ACTIVATION_H