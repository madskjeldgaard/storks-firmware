#pragma once
#ifndef SCALESTORK
#define SCALESTORK
#include <string>
#include <unordered_map>
#include <vector>
#include "common.hpp"

/**
 * @brief ScaleIntervals
 * @author Mads Kjeldgaard
 * @date date
 * @details Scale intervals for constraining midi notes to certain scale steps
 */

namespace storkspace {


class ScaleIntervals {
public:
  const DegreeArray *getDegrees(const ScaleName scalename) const {
    // if (degrees.contains(scalename)) {
    return &intervals.at(scalename);
    // } else {
    //   return &degrees.at("Chromatic");
    // }
  }

  /**
   * @brief Add scale interval to a midi note
   *
   * @param scalename string of scale name
   * @param midinote integer
   * @param step step in the scale
   * @return midinote with scale step applied
   */
  int midinoteWithScaleInterval(const int midinote, const ScaleName scalename,
                                 const std::size_t intervalstep) const {
    return midinote + intervals.at(scalename).at(intervalstep);
  }

private:
  const ScaleDegreeMap intervals{
      {"Major", DegreeArray{0, 2, 4, 5, 7, 9, 11, 12}},
      {"Minor", DegreeArray{0, 2, 3, 5, 7, 8, 10, 12}},
      {"Whole", DegreeArray{0, 2, 4, 6, 8, 10, 12}},
      {"Chromatic", DegreeArray{0, 1, 2, 3, 4, 5, 6, 7}},
      {"Dorian", DegreeArray{0, 2, 3, 5, 7, 9, 10, 12}},
  };
};

} // namespace storkspace
#endif
