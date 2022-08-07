#pragma once
#ifndef HAKKEGLOBALS
#define HAKKEGLOBALS

namespace hakke {
enum class MIDIMODE { MODE_NORMAL, MODE_14BIT };
enum class ButtonSetup { KeyBoard, Midi };

// This will calculate the correct midi cc number based on the current page,
// layer and encoder
inline int pageToMidiNum(int encNum, int page, int numEncoders) {
  int pageNum = page;
  auto ccNum = encNum + (pageNum * numEncoders);
  return ccNum;
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
} // namespace hakke

// Wrap values
// Based on https://stackoverflow.com/a/707426
template <typename T> T wrapValue(T value, T minValue, T maxValue) {
  T range_size = maxValue - minValue + 1;

  if (value < minValue)
    value += range_size * ((minValue - value) / range_size + 1);

  return minValue + (value - minValue) % range_size;
}

#endif
