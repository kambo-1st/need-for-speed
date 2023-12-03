#include "cosine_similarity.h"
#include <math.h>

double cosine_similarity(const double *array1, const double *array2, int size) {
   double dot_product = 0.0;
   double magnitude_a = 0.0;
   double magnitude_b = 0.0;
   int i;

   for (i = 0; i < size; ++i) {
       dot_product += array1[i] * array2[i];
       magnitude_a += array1[i] * array1[i];
       magnitude_b += array2[i] * array2[i];
   }

   return dot_product / (sqrt(magnitude_a) * sqrt(magnitude_b));
}