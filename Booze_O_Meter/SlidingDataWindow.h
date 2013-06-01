// Copyright (c) 2013 Mark Dyer. All rights reserved.
#ifndef SLIDING_DATA_WINDOW_H
#define SLIDING_DATA_WINDOW_H

namespace mdlib {
/**
 * A class for managing a set of samples and providing characteristics of the
 * sample set.
 *
 * This class is designed for use on Arduino, and therefore is implemented with
 * a fixed size sample pool.
 */
class SlidingDataWindow {
 public:
  SlidingDataWindow();
  ~SlidingDataWindow();

  // How many slots in the pool to actually use. For example, if you are
  // sampling at 5 samples per second and want a 5 second window, set
  // window size to 5 x 5 = 25
  // The default value is POOL_SIZE
  void SetWindowSize(int size);
  
  // If all sample values fall within the range defined by StableSize,
  // the window is considered stable.
  // The default value is 1.0
  void SetStableSize(float size);

  // clears all accumulated samples and statistics. Use this when you stop
  // sampling for awhile and are ready to resume.
  void Reset();

  void AddSample(float sample);

  // The SlidingDataWindow is 'ready' when it has enough samples
  // to begin categorizing the data
  bool IsReady() const;

  // functions that characterize the sample set
  bool IsRising() const;
  bool IsFalling() const;
  bool IsStable() const;

  // these functions are mostly for testing
  float Minimum() const;
  float Maximum() const;
  float Average() const;
  float Median() const;
  float StandardDeviation() const;

  void Dump();

 private:
  static const int POOL_SIZE = 100;
  int window_size_;
  float stable_size_;
  float* data_;
  float minimum_;
  float maximum_;
  float average_;
  float standard_deviation_;
  int next_;
  int count_;
};

} // namespace mdlib

#endif // SLIDING_DATA_WINDOW_H
