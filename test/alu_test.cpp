/*
 * alu_test.cpp
 * Copyright (C) 2018 Emiliano Firmino <emiliano.firmino@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "alu.hpp"

#include "catch.hpp"

static const char* tag = "[ALU]";

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

TEST_CASE("ADD8 Zero Flag", tag) {
    u8 a = 0;
    u8 b = 0;
    u8 f = 0;
    alu::add8(f, a, b);

    REQUIRE(a == 0);
    REQUIRE(f == (alu::kFZ));
}

TEST_CASE("ADD8 Half Flag", tag) {
    u8 a = 0xf;
    u8 b = 0xf;
    u8 f = 0;
    alu::add8(f, a, b);

    REQUIRE(a == 0x1e);
    REQUIRE(f == (alu::kFH));
}

TEST_CASE("ADD8 Carry Flag", tag) {
    u8 a = 0xf0;
    u8 b = 0xf0;
    u8 f = 0;
    alu::add8(f, a, b);

    REQUIRE(a == 0xe0);
    REQUIRE(f == (alu::kFC));
}

TEST_CASE("ADC8 No Flags", tag) {
    u8 a = 0;
    u8 b = 0;
    u8 f = alu::kFC;
    alu::adc8(f, a, b);

    REQUIRE(a == 1);
    REQUIRE(f == 0);
}

TEST_CASE("ADC8 Flags", tag) {
    u8 a = 0xff;
    u8 b = 0x00;
    u8 f = alu::kFC;
    alu::adc8(f, a, b);

    REQUIRE(a == 0);
    REQUIRE(f == (alu::kFZ | alu::kFH | alu::kFC));

}

TEST_CASE("ADC8 No Carry", tag) {
    u8 a = 0;
    u8 b = 0;
    u8 f = 0;
    alu::adc8(f, a, b);

    REQUIRE(a == 0);
    REQUIRE(f == (alu::kFZ));
}

TEST_CASE("SUB8 Zero Flag", tag) {
    u8 a = 1;
    u8 b = 1;
    u8 f = 0;
    alu::sub8(f, a, b);

    REQUIRE(a == 0);
    REQUIRE(f == (alu::kFZ | alu::kFN));
}

TEST_CASE("SUB8 Underflow", tag) {
    u8 a = 0xfe;
    u8 b = 0xff;
    u8 f = 0;
    alu::sub8(f, a, b);

    REQUIRE(a == 0xff);
    REQUIRE(f == (alu::kFN | alu::kFH | alu::kFC));
}

TEST_CASE("SBC8 Zero Flag", tag) {
    u8 a = 2;
    u8 b = 1;
    u8 f = alu::kFC;
    alu::sbc8(f, a, b);

    REQUIRE(a == 0);
    REQUIRE(f == (alu::kFZ | alu::kFN));
}

TEST_CASE("SBC8 Underflow", tag) {
    u8 a = 0xfe;
    u8 b = 0xfe;
    u8 f = alu::kFC;
    alu::sbc8(f, a, b);

    REQUIRE(a == 0xff);
    REQUIRE(f == (alu::kFN | alu::kFH | alu::kFC));
}

TEST_CASE("INC8 No Flags", tag) {
    u8 a = 0;
    u8 f = 0;
    alu::inc8(f, a);

    REQUIRE(a == 1);
    REQUIRE(f == 0);
}

TEST_CASE("INC8 Half Carry", tag) {
    u8 a = 0xf;
    u8 f = 0;
    alu::inc8(f, a);

    REQUIRE(a == 0x10);
    REQUIRE(f == alu::kFH);
}

TEST_CASE("INC8 Overflow", tag) {
    u8 a = 0xff;
    u8 f = 0;
    alu::inc8(f, a);

    REQUIRE(a == 0);
    REQUIRE(f == (alu::kFZ | alu::kFH));
}

TEST_CASE("DEC8 No Flags", tag) {
    u8 a = 2;
    u8 f = 0;
    alu::dec8(f, a);

    REQUIRE(a == 1);
    REQUIRE(f == (alu::kFN));
}

TEST_CASE("DEC8 Zero Flag", tag) {
    u8 a = 1;
    u8 f = 0;
    alu::dec8(f, a);

    REQUIRE(a == 0);
    REQUIRE(f == (alu::kFZ | alu::kFN));
}

TEST_CASE("DEC8 Underflow") {
    u8 a = 0;
    u8 f = 0;
    alu::dec8(f, a);

    REQUIRE(a == 0xff);
    REQUIRE(f == alu::kFN);
}

TEST_CASE("ADD16", tag) {
    u16 a = 0x0080;
    u16 b = 0x0040;
    u8 f = 0;
    alu::add16(f, a, b);

    REQUIRE(a == 0x00c0);
    REQUIRE(f == 0);
}

TEST_CASE("ADD16 Overflow", tag) {
    u16 a = 0xffff;
    u16 b = 0x0001;
    u8  f = 0;
    alu::add16(f, a, b);

    REQUIRE(a == 0);
    REQUIRE(f == (alu::kFH | alu::kFC));
}

TEST_CASE("SUB16", tag) {
    u16 a = 0x0ff0;
    u16 b = 0x0f00;
    u8 f = 0;
    alu::sub16(f, a, b);

    REQUIRE(a == 0x00f0);
    REQUIRE(f == (alu::kFN));
}

TEST_CASE("INC16 No Flags", tag) {
    u16 a = 0;
    u8  f = 0;
    alu::inc16(f, a);

    REQUIRE(a == 1);
    REQUIRE(f == 0);
}

TEST_CASE("INC16 Half Carry", tag) {
    u16 a = 0x00ff;
    u8  f = 0;
    alu::inc16(f, a);

    REQUIRE(a == 0x0100);
    REQUIRE(f == 0);
}

TEST_CASE("INC16 Overflow", tag) {
    u16 a = 0xffff;
    u8  f = 0;
    alu::inc16(f, a);

    REQUIRE(a == 0);
    REQUIRE(f == 0);
}

TEST_CASE("DEC16 Simple", tag) {
    u16 a = 1;
    u8  f = 0;
    alu::dec16(f, a);

    REQUIRE(a == 0);
    REQUIRE(f == 0);
}

TEST_CASE("DEC16 Underflow", tag) {
    u16 a = 0;
    u8  f = 0;
    alu::dec16(f, a);

    REQUIRE(a == 0xffff);
    REQUIRE(f == 0);
}

TEST_CASE("DEC16 Half", tag) {
    u16 a = 0x0100;
    u8  f = 0;
    alu::dec16(f, a);

    REQUIRE(a == 0xff);
    REQUIRE(f == 0);
}

TEST_CASE("AND Zero", tag) {
    u8 a = 0b1010'1010;
    u8 b = 0b0101'0101;
    u8 f = 0;
    alu::land(f, a, b);

    REQUIRE(a == 0);
    REQUIRE(f == (alu::kFZ | alu::kFH));
}

TEST_CASE("AND Some", tag) {
    u8 a = 0b1100'0011;
    u8 b = 0b1001'1001;
    u8 f = 0;
    alu::land(f, a, b);

    REQUIRE(a == 0b1000'0001);
    REQUIRE(f == alu::kFH);
}

TEST_CASE("AND All", tag) {
    u8 a = 0b1111'1111;
    u8 b = 0b1111'1111;
    u8 f = 0;
    alu::land(f, a, b);

    REQUIRE(a == 0b1111'1111);
    REQUIRE(f == alu::kFH);
}

TEST_CASE("XOR Zero", tag) {
    u8 a = 0b0000'1111;
    u8 b = 0b0000'1111;
    u8 f = 0;
    alu::lxor(f, a, b);

    REQUIRE(a == 0);
    REQUIRE(f == alu::kFZ);
}

TEST_CASE("XOR Some", tag) {
    u8 a = 0b1001'1100;
    u8 b = 0b0101'0101;
    u8 f = 0;
    alu::lxor(f, a, b);

    REQUIRE(a == 0b1100'1001);
    REQUIRE(f == 0);
}

TEST_CASE("XOR All", tag) {
    u8 a = 0b1010'1010;
    u8 b = 0b0101'0101;
    u8 f = 0;
    alu::lxor(f, a, b);

    REQUIRE(a == 0b1111'1111);
    REQUIRE(f == 0);
}

TEST_CASE("CP Equal", tag) {
    u8 a = 1;
    u8 b = 1;
    u8 f = 0;
    alu::lcp(f, a, b);

    REQUIRE(a == 1);
    REQUIRE(b == 1);
    REQUIRE(f == (alu::kFN | alu::kFZ));
}

TEST_CASE("CP Less", tag) {
    u8 a = 10;
    u8 b = 20;
    u8 f = 0;
    alu::lcp(f, a, b);

    REQUIRE(f == (alu::kFN | alu::kFC));
}

TEST_CASE("CP Greater", tag) {
    u8 a = 20;
    u8 b = 10;
    u8 f = 0;
    alu::lcp(f, a, b);

    REQUIRE(f == (alu::kFN | alu::kFH));
}

TEST_CASE("RL No Carry", tag) {
    u8 a = 0b0100'1110;
    u8 f = 0;
    alu::rl(f, a);

    REQUIRE(a == 0b1001'1100);
    REQUIRE(f == 0);
}

TEST_CASE("RL To Carry", tag) {
    u8 a = 0b1100'1110;
    u8 f = 0;
    alu::rl(f, a);

    REQUIRE(a == 0b1001'1100);
    REQUIRE(f == (alu::kFC));
}

TEST_CASE("RL With Carry", tag) {
    u8 a = 0b0100'1110;
    u8 f = alu::kFC;
    alu::rl(f, a);

    REQUIRE(a == 0b1001'1101);
    REQUIRE(f == 0);
}

TEST_CASE("RLC Set Carry", tag) {
    u8 a = 0b1000'1110;
    u8 f = 0;
    alu::rlc(f, a);

    REQUIRE(a == 0b0001'1101);
    REQUIRE(f == alu::kFC);
}

TEST_CASE("RLC No Carry", tag) {
    u8 a = 0b0100'1110;
    u8 f = 0;
    alu::rlc(f, a);

    REQUIRE(a == 0b1001'1100);
    REQUIRE(f == 0);

}

TEST_CASE("RR No Carry", tag) {
    u8 a = 0b0100'1110;
    u8 f = 0;
    alu::rr(f, a);

    REQUIRE(a == 0b0010'0111);
    REQUIRE(f == 0);
}

TEST_CASE("RR With Carry", tag) {
    u8 a = 0b1100'1110;
    u8 f = alu::kFC;
    alu::rr(f, a);

    REQUIRE(a == 0b1110'0111);
    REQUIRE(f == 0);
}

TEST_CASE("RR To Carry", tag) {
    u8 a = 0b1100'1111;
    u8 f = 0;
    alu::rr(f, a);

    REQUIRE(a == 0b0110'0111);
    REQUIRE(f == alu::kFC);
}

TEST_CASE("RRC Set Carry", tag) {
    u8 a = 0b1000'1111;
    u8 f = 0;
    alu::rrc(f, a);

    REQUIRE(a == 0b1100'0111);
    REQUIRE(f == alu::kFC);
}

TEST_CASE("RRC No Carry", tag) {
    u8 a = 0b0100'1110;
    u8 f = 0;
    alu::rrc(f, a);

    REQUIRE(a == 0b0010'0111);
    REQUIRE(f == 0);
}

TEST_CASE("SLA Set Carry", tag) {
    u8 a = 0b1000'0001;
    u8 f = 0;
    alu::sla(f, a);

    REQUIRE(a == 0b0000'0010);
    REQUIRE(f == alu::kFC);
}

TEST_CASE("SLA No Carry", tag) {
    u8 a = 0b0000'0001;
    u8 f = alu::kFC;
    alu::sla(f, a);

    REQUIRE(a == 0b0000'0010);
    REQUIRE(f == 0);
}

TEST_CASE("SRA Set Carry", tag) {
    u8 a = 0b1000'0001;
    u8 f = 0;
    alu::sra(f, a);

    REQUIRE(a == 0b1100'0000);
    REQUIRE(f == alu::kFC);
}

TEST_CASE("SRA No Carry", tag) {
    u8 a = 0b0000'1000;
    u8 f = alu::kFC;
    alu::sra(f, a);

    REQUIRE(a == 0b0000'0100);
    REQUIRE(f == 0);
}

TEST_CASE("SRL No Carry", tag) {
    u8 a = 0b0000'1000;
    u8 f = alu::kFC;
    alu::srl(f, a);

    REQUIRE(a == 0b0000'0100);
    REQUIRE(f == 0);

}

TEST_CASE("SRL Set Carry", tag) {
    u8 a = 0b0000'0001;
    u8 f = 0;
    alu::srl(f, a);

    REQUIRE(a == 0b0000'0000);
    REQUIRE(f == (alu::kFC | alu::kFZ));

}
