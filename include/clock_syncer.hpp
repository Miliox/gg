/*
 * clock_syncer.h
 * Copyright (C) 2018 Emiliano Firmino <emiliano.firmino@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef CLOCK_SYNCER_H
#define CLOCK_SYNCER_H

#include "common.h"

#include <chrono>
#include <functional>

class ClockSyncer {
public:
    ClockSyncer();

    void step(u8 ticks);

private:
    u64 frames;
    u64 ticks;

    std::chrono::high_resolution_clock::time_point beginTs;
    std::chrono::high_resolution_clock::time_point lastTs;
    std::chrono::nanoseconds oversleep;
};

#endif /* !CLOCK_SYNCER_H */
