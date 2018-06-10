/*
 * clock_syncer.cpp
 * Copyright (C) 2018 Emiliano Firmino <emiliano.firmino@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "clock_syncer.hpp"

#include <chrono>
#include <thread>

static const u64 kTicksPerSec  = 4'194'304;
static const u64 kFramesPerSec = 60;

static const u64 kTicksPerFrame = kTicksPerSec / kFramesPerSec;

static const auto kNanosPerFrame = std::chrono::nanoseconds(1000'000'000 / kFramesPerSec);

ClockSyncer::ClockSyncer() : ticks(0) {
    lastTs = std::chrono::high_resolution_clock::now();
}

void ClockSyncer::step(u8 ticks) {
    this->ticks += ticks;

    if (this->ticks >= kTicksPerFrame) {
        this->ticks -= kTicksPerFrame;

        auto now  = std::chrono::high_resolution_clock::now();
        auto duty = std::chrono::duration_cast<std::chrono::nanoseconds>(now - lastTs);

        if (duty < kNanosPerFrame) {
            auto delay = kNanosPerFrame - duty;
            std::this_thread::sleep_for(std::chrono::nanoseconds(delay));
        }

        lastTs = std::chrono::high_resolution_clock::now();
    }
}

