#ifndef STOP_WATCH_H
#define STOP_WATCH_H

#include <chrono>

class stopWatch {
 public:
  void start() { start_time_ = std::chrono::high_resolution_clock::now(); }
  double stop() {
    stop_time_ = std::chrono::high_resolution_clock::now();
    delta_time_ = stop_time_ - start_time_;

    return delta_time_.count();
  }

 private:
  std::chrono::time_point<std::chrono::high_resolution_clock> start_time_;
  std::chrono::time_point<std::chrono::high_resolution_clock> stop_time_;
  std::chrono::duration<double> delta_time_;
};

#endif