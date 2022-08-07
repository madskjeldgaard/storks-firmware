#pragma once
#ifndef STORKSCOMMON
#define STORKSCOMMON
#include <array>
#include "hardwareencoder.hpp"
#include "virtualencoder.hpp"

namespace storkspace {

static constexpr int numHardwareEncoders = 16;
static constexpr int numLayers = 4;
static constexpr int numChannels = 16;
static constexpr int numVirtualEncoders = numHardwareEncoders * numLayers * numChannels;
static constexpr int numHardwareButtons = 8;

} // namespace storkspace
#endif
