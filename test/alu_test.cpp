/*
 * alu_test.cpp
 * Copyright (C) 2018 Emiliano Firmino <emiliano.firmino@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "alu.hpp"

#include "catch.hpp"

static const char* tag = "[APU]";

TEST_CASE("LD8", tag) {
    u8 a = 0;
    u8 b = 0xff;
    alu::ld8(a, b);

    REQUIRE(a == 0xff);
}

TEST_CASE("LD16", tag) {
    u16 a = 0;
    u16 b = 0xffff;
    alu::ld16(a, b);

    REQUIRE(a == 0xffff);
}

TEST_CASE("ADD8 No Flags", tag) {
    u8 a = 0;
    u8 b = 1;
    u8 f = 0;
    alu::add8(f, a, b);

    REQUIRE(a == 1);
    REQUIRE(f == 0);
}
