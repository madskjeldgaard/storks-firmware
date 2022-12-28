#pragma once
#ifndef STORKSMATHHH
#define STORKSMATHHH
#include <math.h>

namespace storkspace{

// Get the sign of a value
template <typename T> int get_sign(T val) {
  return (T(0) < val) - (val < T(0));
}

// Wrap double to max value
// From https://stackoverflow.com/questions/4633177/c-how-to-wrap-a-float-to-the-interval-pi-pi
/* wrap x -> [0,max) */
// double wrapMaxDouble(double x, double max)
// {
//     /* integer math: `(max + x % max) % max` */
//     return fmod(max + fmod(x, max), max);
// }

// Wrap double to min and max value
/* wrap x -> [min,max) */
// double wrapMinMaxDouble(double x, double min, double max)
// {
//     return min + wrapMaxDouble(x - min, max - min);
// }

// Wrap float to max value
inline float wrapMaxFloat(float x, float max)
{
    /* integer math: `(max + x % max) % max` */
    return fmod(max + fmodf(x, max), max);
}

// Wrap float to min and max value
/* wrap x -> [min,max) */
inline float wrapMinMaxFloat(float x, float min, float max)
{
    return min + wrapMaxFloat(x - min, max - min);
}

// Clip value
template <typename T> T clipValue(T value, T minValue, T maxValue) {
  if (value > maxValue) {
	return maxValue;
  } else if (value < minValue) {
	return minValue;
  } else {
	return value;
  }
}

// Wrap values
// Based on https://stackoverflow.com/a/707426
template <typename T> T wrapValue(T value, T minValue, T maxValue) {
  T range_size = maxValue - minValue + 1;

  if (value < minValue)
	value += range_size * ((minValue - value) / range_size + 1);

  return minValue + (value - minValue) % range_size;
}

} // namespace hakke

#endif
