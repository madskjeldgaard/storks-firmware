#pragma once
#ifndef STORKSCOMMON
#define STORKSCOMMON
#include "hardwareencoder.hpp"
#include "virtualencoder.hpp"
#include <array>
#include <unordered_map>
#include <vector>

namespace storkspace {

//------------------------------------------------------------------//
//                         Global variables                         //
//------------------------------------------------------------------//

static constexpr int numHardwareEncoders = 16;
static constexpr int numLayers = 4;
static constexpr int numChannels = 16;
static constexpr int numVirtualEncoders =
    numHardwareEncoders * numLayers * numChannels;
static constexpr int numHardwareButtons = 8;

//------------------------------------------------------------------//
//                           Type aliases                           //
//------------------------------------------------------------------//

// Storks only has 8 keys
constexpr auto numDegrees = numHardwareButtons;
using ScaleName = std::string;
using DegreeArray = std::array<int, numDegrees>;
using ScaleDegreeMap = std::unordered_map<ScaleName, DegreeArray>;


using ChordVector = std::unique_ptr<std::vector<int>>;

/**
 * @brief Creates a chord which is a std::vector of ints wrapped in a unique_ptr
 *
 * @return chord
 */
static ChordVector makeChordVector(){
  return std::make_unique<std::vector<int>>();
}

} // namespace storkspace
#endif
