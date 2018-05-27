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

    u64 t = alu::ld8(a, b);

    REQUIRE(a == 0xff);
    REQUIRE(t == 4);
}

TEST_CASE("LD16", tag) {
    u16 a = 0;
    u16 b = 0xffff;

    u64 t = alu::ld16(a, b);

    REQUIRE(a == 0xffff);
    REQUIRE(t == 8);
}

TEST_CASE("ADD8 No Flags", tag) {
    u8 a = 0;
    u8 b = 1;
    u8 f = 0;

    u64 t = alu::add8(f, a, b);

    REQUIRE(a == 1);
    REQUIRE(f == 0);
    REQUIRE(t == 4);
}
