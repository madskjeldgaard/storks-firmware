#pragma once
#ifndef ONEPOLESMOOTHER
#define ONEPOLESMOOTHER

namespace storkspace {

/**
 * @brief OnePoleSmoother
 * @author Mads Kjeldgaard
 * @date 09-10-2022
 * @details OnePole filter for smoothing values
 */

class OnePoleSmoother {
public:
  // OnePoleSmoother();
  // ~OnePoleSmoother();

  void setup(float initFc = 10) {
	setFc(initFc);
  }

  float loop(float in) { return z1 = in * a0 + z1 * b1; }

  void setFc(float Fc) {
    b1 = exp(-2.0 * M_PI * Fc);
    a0 = 1.0 - b1;
  }

private:
  // coeffecients
  float a0{0}, b1{0}, z1{0};
};

} // namespace storkspace
#endif
