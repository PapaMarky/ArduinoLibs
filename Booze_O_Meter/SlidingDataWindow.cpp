#include <Arduino.h>
#include "SlidingDataWindow.h"

namespace mdlib {

SlidingDataWindow::SlidingDataWindow()
  : window_size_(POOL_SIZE),
    stable_size_(0.01f),
    trend_sample_size_(10),
    stable_sample_count_(0),
    rising_sample_count_(0),
    falling_sample_count_(0)
{
}

SlidingDataWindow::~SlidingDataWindow() {
}

void SlidingDataWindow::SetStableSize(float size) {
  stable_size_ = size;
}

void SlidingDataWindow::Reset() {
  next_ = count_ = 0;
  stable_sample_count_ =
    rising_sample_count_ =
    falling_sample_count_ = 0;
}

void SlidingDataWindow::Dump() {
  char buffer[256];
  //  Serial.print("NXT: ");
  //  Serial.print(next_);
  Serial.print(", MIN: ");
  Serial.print(minimum_, 2);
  Serial.print(", MAX: ");
  Serial.print(maximum_, 2);
  Serial.print(", AVE: ");
  Serial.print(average_, 2);
  Serial.print(", MED: ");
  Serial.print(Median(), 2);
  Serial.print(", DEV: ");
  Serial.print(standard_deviation_);
  if (IsStable()) {
    Serial.print(" -- (");
    Serial.print(stable_size_);
    Serial.print(") -- STABLE");
  }
  Serial.println("");
}

void SlidingDataWindow::AddSample(float sample) {
  data_[next_] = sample;
  next_++;
  if (next_ >= window_size_)
    next_ = 0;

  if (count_ < window_size_)
    count_++;

  if (IsReady()) {
    float total = 0.0;
    float variance = 0.0;
    float *d = data_;
    minimum_ = maximum_ = *d;
    for (int i = 0; i < window_size_; i++) {
      float dd = *d;
      if (dd < minimum_) {
	minimum_ = dd;
      }
      if (data_[i] > maximum_)
	maximum_ = dd;

      total += dd;

      d++;
    }
    average_ = total/(float)window_size_;
    for (int i = 0; i < window_size_; i++) {
      int v = data_[i] - average_;
      variance += (v*v);
    }
    standard_deviation_ = sqrt(variance/(float)window_size_);

    if (standard_deviation_ < stable_size_) {
      stable_sample_count_++;
      //      Serial.print("stable count: ");
      //      Serial.print(stable_sample_count_);
      //      if (IsStable())
      //	Serial.print(" -- STABLE");
      //      Serial.println("");
      rising_sample_count_ = 0;
      falling_sample_count_ = 0;
    }
    else {
      float first = GetFirstSample();
      float last = sample;
      if (first < average_ && average_ < last) {
	rising_sample_count_++;
	falling_sample_count_ = 0;
	stable_sample_count_ = 0;
	//	Serial.print("rising count: ");
	//	Serial.print(rising_sample_count_);
	//	if (IsRising())
	//	  Serial.print(" -- RISING");
	//	Serial.println("");
      }
      else if (last < average_ && average_ < first) {
	  falling_sample_count_++;
	  rising_sample_count_ = 0;
	  stable_sample_count_ = 0;
      }
      else {
	stable_sample_count_ = 0;
	rising_sample_count_ = 0;
	falling_sample_count_ = 0;
      }
    }
  }
}

bool SlidingDataWindow::IsReady() const {
  return (count_ >= window_size_);
}

bool SlidingDataWindow::IsRising() const {
  return IsReady() && rising_sample_count_ >= trend_sample_size_;
}

bool SlidingDataWindow::IsFalling() const {
  return IsReady() && falling_sample_count_ >= trend_sample_size_;
}

bool SlidingDataWindow::IsStable() const {
  return IsReady() && stable_sample_count_ >= trend_sample_size_;
}

float SlidingDataWindow::Minimum() const {
  return minimum_;
}

float SlidingDataWindow::Maximum()  const {
  return maximum_;
}

float SlidingDataWindow::Average()  const {
  return average_;
}

float SlidingDataWindow::Median()  const {
  return minimum_ + (maximum_ - minimum_) / 2.0f;
}

float SlidingDataWindow::StandardDeviation()  const {
  return standard_deviation_;
}


} // namespace mdlib

