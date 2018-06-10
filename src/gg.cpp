/*
 * gg.cpp
 * Copyright (C) 2018 Emiliano Firmino <emiliano.firmino@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "gg.h"

#include "cpu.hpp"
#include "mmu.hpp"
#include "clock_syncer.hpp"

#include <string>


#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

int main(int argc, char** argv) {
    bool runTests = false;

    for (int i = 0; i < argc; i++) {
        if (std::string(argv[i]) == "--run-tests") {
            runTests = true;
        }
    }

    if (runTests) {
        Catch::Session().run(1, argv);
    }

    CPU cpu;
    MMU mmu;

    ClockSyncer syncer;

    cpu.read8  = [&](u16 addr) -> u8 { return mmu.read8(addr); };
    cpu.write8 = [&](u16 addr, u8 val) { mmu.write8(addr, val); };

    u64 ticks = 0;
    for (;;) {
        u8 elapsed = cpu.cycle();
        ticks += elapsed;

        mmu.step(elapsed);
        syncer.step(elapsed);
    }

    return 0;
}

