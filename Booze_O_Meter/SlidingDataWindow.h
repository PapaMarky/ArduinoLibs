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

  // Trends of the sample window such as 'stable', 'rising' and 'falling' 
  // require the 'trend' to be in effect for trend_sample_size_ samples.
  // In other words, a single 'stable' sample, will not cause the SlidingDataWindow
  // to return true for IsStable. We count how many stable samples we have, and
  // only report is stable if at least trend_sample_size_ samples have been
  // stable.
  // EX: If you are sampling at a rate of 5 samples per second, and you want the
  // data to be stable (or rising, or falling) for at least a second before seeing it
  // as stable, call SetTrendSampleSize(5)
  void SetTrendSampleSize(int size) {
    trend_sample_size_ = size;
  }

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

  float GetFirstSample() { return data_[next_]; }
  float GetLastSample() {
    return data_[(next_ == 0 ? window_size_ - 1 : next_)];
  }

 private:
  static const int POOL_SIZE = 25;
  int window_size_;
  float stable_size_;
  float data_[POOL_SIZE];
  float minimum_;
  float maximum_;
  float average_;
  float standard_deviation_;
  int next_;
  int count_;
  int trend_sample_size_;
  int stable_sample_count_;
  int rising_sample_count_;
  int falling_sample_count_;
};

} // namespace mdlib

#endif // SLIDING_DATA_WINDOW_H
