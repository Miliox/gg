/*
 * clock_syncer.cpp
 * Copyright (C) 2018 Emiliano Firmino <emiliano.firmino@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "clock_syncer.hpp"

#include <iostream>
#include <thread>

static const u64 kTicksPerSec  = 4'194'304;
static const u64 kFramesPerSec = 60;

static const u64 kTicksPerFrame = kTicksPerSec / kFramesPerSec;

static const auto kNanosPerFrame = std::chrono::nanoseconds(1000'000'000 / kFramesPerSec);

ClockSyncer::ClockSyncer() : frames(0), ticks(0) {
    lastTs  = std::chrono::high_resolution_clock::now();
    beginTs = lastTs;

    oversleep = std::chrono::nanoseconds(0);
}

void ClockSyncer::step(u8 ticks) {
    this->ticks += ticks;

    // Suspend execution to match real gameboy clock speed
    if (this->ticks >= kTicksPerFrame) {
        this->ticks -= kTicksPerFrame;

        frames += 1;

        // FOR DEBUG: print timestamp of sync frame
        //if (frames % 60 == 0) {
        //    auto now = std::chrono::high_resolution_clock::now();
        //    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(now - beginTs).count() << "\n";
        //}

        auto now  = std::chrono::high_resolution_clock::now();
        auto duty = std::chrono::duration_cast<std::chrono::nanoseconds>(now - lastTs);

        if ((duty + oversleep) < kNanosPerFrame) {
            auto delay = kNanosPerFrame - duty - oversleep;
            std::this_thread::sleep_for(std::chrono::nanoseconds(delay));

            // store oversleep to compesate for it on frame sync
            oversleep = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::high_resolution_clock::now() - now) - delay;
        }

        lastTs = std::chrono::high_resolution_clock::now();
    }
}

