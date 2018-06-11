/*
 * clock_syncer.cpp
 * Copyright (C) 2018 Emiliano Firmino <emiliano.firmino@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "clock_syncer.hpp"

#include <iostream>
#include <thread>

using namespace std::chrono;

static const u64 kTicksPerSec  = 4'194'304;
static const u64 kFramesPerSec = 60;

static const u64 kTicksPerFrame = kTicksPerSec / kFramesPerSec;

static const auto kNanosPerFrame = nanoseconds(1000'000'000 / kFramesPerSec);

ClockSyncer::ClockSyncer() : frames(0), ticks(0) {
    lastTs  = high_resolution_clock::now();
    beginTs = lastTs;

    oversleep = nanoseconds(0);
}

bool ClockSyncer::step(u8 ticks) {
    this->ticks += ticks;

    // Suspend execution to match real gameboy clock speed
    if (this->ticks >= kTicksPerFrame) {
        this->ticks -= kTicksPerFrame;

        frames += 1;

        // FOR DEBUG: print timestamp of sync frame
        //if (frames % 60 == 0) {
        //    auto now = high_resolution_clock::now();
        //    std::cout << duration_cast<std::chrono::milliseconds>(now - beginTs).count() << "\n";
        //}

        auto now  = high_resolution_clock::now();
        auto duty = duration_cast<nanoseconds>(now - lastTs);

        if ((duty + oversleep) < kNanosPerFrame) {
            auto delay = kNanosPerFrame - duty - oversleep;
            std::this_thread::sleep_for(nanoseconds(delay));

            // store oversleep to compesate for it on frame sync
            oversleep = duration_cast<nanoseconds>(
                    high_resolution_clock::now() - now) - delay;
        } else {
            oversleep = nanoseconds(0);
        }

        lastTs = high_resolution_clock::now();
        return true;
    }

    return false;
}

