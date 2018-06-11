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

#include <SDL2/SDL.h>

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

    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        std::cerr << "Fail to init SDL: " << SDL_GetError() << "\n";
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("GG", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 320, 288, 0);
    if (window == NULL) {
        std::cerr << "Fail to create window: " << SDL_GetError() << "\n";
        return -1;
    }

    CPU cpu;
    MMU mmu;

    ClockSyncer syncer;

    cpu.read8  = [&](u16 addr) -> u8 { return mmu.read8(addr); };
    cpu.write8 = [&](u16 addr, u8 val) { mmu.write8(addr, val); };


    u64 ticks = 0;
    bool running = true;
    while (running) {
        u8 elapsed = cpu.cycle();
        ticks += elapsed;

        mmu.step(elapsed);

        if (syncer.step(elapsed)) {
            // frame synced
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                    case SDL_QUIT: running = false; break;
                }
            }
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

