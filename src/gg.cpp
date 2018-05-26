/*
 * gg.cpp
 * Copyright (C) 2018 Emiliano Firmino <emiliano.firmino@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "gg.h"

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

    return 0;
}

