#pragma once

#include <chrono>

class Timer {
    long long time;
    long long getTime() {
        return std::chrono::steady_clock::now().time_since_epoch().count();
    }
public:
    Timer() {
        start();
    }
    void reset() {
        time = getTime();
    }
    void start() {
        reset();
    }
    double elapsed(bool _reset = false) {
        double ret = double(getTime() - time) / 1e9;
        if (_reset) reset();
        return ret;
    }
};