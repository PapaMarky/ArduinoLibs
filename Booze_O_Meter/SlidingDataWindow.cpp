#include "SlidingDataWindow.h"

namespace mdlib {

SlidingDataWindow::SlidingDataWindow()
  : window_size_(POOL_SIZE),
    stable_size_(1.0)
{
}

SlidingDataWindow::~SlidingDataWindow() {}

void SlidingDataWindow::SetWindowSize(int size) {
  window_size_ = size;
}

void SlidingDataWindow::SetStableSize(float size) {
  stable_size_ = size;
}

void SlidingDataWindow::Reset() {
  // TODO: write me
}

void SlidingDataWindow::AddSample(float sample) {
  // TODO: write me
}

bool SlidingDataWindow::IsReady() const {
  // TODO: write me
}

bool SlidingDataWindow::IsRising() const {
  // TODO: write me
}

bool SlidingDataWindow::IsFalling() const {
  // TODO: write me
}
bool SlidingDataWindow::IsStable() const {
  // TODO: write me
}

float SlidingDataWindow::Minimum() const {
  // TODO: write me
}

float SlidingDataWindow::Maximum()  const {
  // TODO: write me
}

float SlidingDataWindow::Average()  const {
  // TODO: write me
}

float SlidingDataWindow::Mean()  const {
  // TODO: write me
}

float SlidingDataWindow::StandardDeviation()  const {
  // TODO: write me
}


} // namespace mdlib

