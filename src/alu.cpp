/*
 * alu.cpp
 * Copyright (C) 2018 Emiliano Firmino <emiliano.firmino@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "alu.hpp"

const u8 alu::kFZ = 0b1000'0000;
const u8 alu::kFN = 0b0100'0000;
const u8 alu::kFH = 0b0010'0000;
const u8 alu::kFC = 0b0001'0000;

static inline u8 cond_bitset(bool clause, u8 bits, u8 mask) {
    if (clause) {
        return (bits | mask);
    } else {
        return (bits & ~mask);
    }
}

void alu::ld8(u8& dst, u8 src) {
    dst = src;
}

void alu::ld16(u16& dst, u16 src) {
    dst = src;
}

void alu::add8(u8& flags, u8& acc, u8 arg) {
    u16 n = acc + arg;

    bool z = (n & 0xff) == 0;
    bool h = ((acc & 0xf) + (arg & 0xf)) & 0x10;
    bool c = (n > 0xff);

    flags = cond_bitset(z, flags, alu::kFZ);
    flags = cond_bitset(0, flags, alu::kFN);
    flags = cond_bitset(h, flags, alu::kFH);
    flags = cond_bitset(c, flags, alu::kFZ);

    acc = (u8) n;
}

void alu::adc8(u8& flags, u8& acc, u8 arg) {
    u8  k = (flags & alu::kFC) ? 1 : 0;
    u16 n = acc + arg + k;

    bool z = (n & 0xff) == 0;
    bool h = ((acc & 0xf) + (arg & 0xf) + k) & 0x10;
    bool c = (n > 0xff);

    flags = cond_bitset(z, flags, alu::kFZ);
    flags = cond_bitset(0, flags, alu::kFN);
    flags = cond_bitset(h, flags, alu::kFH);
    flags = cond_bitset(c, flags, alu::kFZ);

    acc = (u8) n;
}

void alu::sub8(u8& flags, u8& acc, u8 arg) {
    u8 n = acc - arg;

    bool z = n == 0;
    bool h = (acc & 0xf) < (arg & 0xf);
    bool c = acc < arg;

    flags = cond_bitset(z, flags, alu::kFZ);
    flags = cond_bitset(1, flags, alu::kFN);
    flags = cond_bitset(h, flags, alu::kFH);
    flags = cond_bitset(c, flags, alu::kFC);

    acc = n;
}

void alu::sbc8(u8& flags, u8& acc, u8 arg) {
    u8 k = (flags & alu::kFC) ? 1 :0;
    u8 n = acc - arg - k;

    bool z = n == 0;
    bool h = (acc & 0xf) < ((arg & 0xf) + k);
    bool c = acc < (arg + k);

    flags = cond_bitset(z, flags, alu::kFZ);
    flags = cond_bitset(1, flags, alu::kFN);
    flags = cond_bitset(h, flags, kFH);
    flags = cond_bitset(c, flags, kFC);

    acc = n;
}

void alu::inc8(u8& flags, u8& acc) {
    u8 n = acc + 1;

    bool z = n == 0;
    bool h = acc == 0xf;

    flags = cond_bitset(z, flags, alu::kFZ);
    flags = cond_bitset(0, flags, alu::kFN);
    flags = cond_bitset(h, flags, alu::kFH);

    acc = n;
}

void alu::dec8(u8& flags, u8& acc) {
    u8 n = acc - 1;

    bool z = n == 0;
    bool h = acc & 0x10;

    flags = cond_bitset(z, flags, alu::kFZ);
    flags = cond_bitset(1, flags, alu::kFN);
    flags = cond_bitset(h, flags, alu::kFH);

    acc = n;
}

void alu::add16(u8& flags, u16& acc, u16 arg) {
    u32 n = acc + arg;

    bool h = (acc & 0xff) + (arg & 0xff) & 0x100;
    bool c = n > 0xffff;

    flags = cond_bitset(0, flags, alu::kFN);
    flags = cond_bitset(h, flags, alu::kFH);
    flags = cond_bitset(c, flags, alu::kFC);

    acc = n & 0xffff;
}

void alu::sub16(u8& flags, u16& acc, u16 arg) {
    u16 n = acc - arg;

    bool z = (n == 0);
    bool h = (acc & 0xff) < (arg & 0xff);
    bool c = (acc < arg);

    flags = cond_bitset(z, flags, alu::kFZ);
    flags = cond_bitset(1, flags, alu::kFN);
    flags = cond_bitset(h, flags, alu::kFH);
    flags = cond_bitset(c, flags, alu::kFC);

    acc = n;
}

void alu::inc16(u8& flags, u16& acc) {
    acc += 1;
}

void alu::dec16(u8& flags, u16& acc) {
    acc -= 1;
}

void alu::land(u8& flags, u8& acc, u8 arg) {
    u8 n = acc & arg;

    bool z = n == 0;

    flags = cond_bitset(z, flags, alu::kFZ);
    flags = cond_bitset(0, flags, alu::kFN);
    flags = cond_bitset(1, flags, alu::kFH);
    flags = cond_bitset(0, flags, alu::kFC);

    acc = n;
}

void alu::lxor(u8& flags, u8& acc, u8 arg) {
    u8 n = acc ^ arg;

    bool z = n == 0;

    flags = cond_bitset(z, flags, alu::kFZ);
    flags = cond_bitset(0, flags, alu::kFN);
    flags = cond_bitset(0, flags, alu::kFH);
    flags = cond_bitset(0, flags, alu::kFC);

    acc = n;
}

void alu::lor(u8& flags, u8& acc, u8 arg) {
    u8 n = acc | arg;

    bool z = n == 0;

    flags = cond_bitset(z, flags, alu::kFZ);
    flags = cond_bitset(0, flags, alu::kFN);
    flags = cond_bitset(0, flags, alu::kFH);
    flags = cond_bitset(0, flags, alu::kFC);

}

void alu::lcp(u8& flags, u8& acc, u8 arg) {
    u8 n = acc - arg;

    bool z = n == 0;
    bool h = (acc & 0xf) < (arg & 0xf);
    bool c = acc < arg;

    flags = cond_bitset(z, flags, alu::kFZ);
    flags = cond_bitset(1, flags, alu::kFN);
    flags = cond_bitset(h, flags, alu::kFH);
    flags = cond_bitset(c, flags, alu::kFC);

}

void alu::bit(u8& flags, u8& acc, u8 arg) {
    bool z = acc & (1 << arg);

    flags = cond_bitset(z, flags, alu::kFZ);
    flags = cond_bitset(0, flags, alu::kFN);
    flags = cond_bitset(1, flags, alu::kFH);

}

void alu::set(u8& flags, u8& acc, u8 arg) {
    u8 n = acc | (1 << arg);

    acc = n;
}

void alu::res(u8& flags, u8& acc, u8 arg) {
    u8 n = acc & ~(1 << arg);

    acc = n;
}

void alu::cpl(u8& flags, u8& acc) {
    u8 n = ~acc;

    flags = cond_bitset(1, flags, alu::kFN);
    flags = cond_bitset(1, flags, alu::kFH);

    acc = n;
}

void alu::rl(u8& flags, u8& acc) {
    bool c = acc & 0x80;

    u8 k = (flags & alu::kFC) ? 1 : 0;
    u8 n = (acc << 1) | k;

    bool z = n == 0;

    flags = cond_bitset(z, flags, alu::kFZ);
    flags = cond_bitset(0, flags, alu::kFN);
    flags = cond_bitset(0, flags, alu::kFH);
    flags = cond_bitset(c, flags, alu::kFC);

    acc = n;
}

void alu::rr(u8& flags, u8& acc) {
    bool c = acc & 0x01;

    u8 k = (flags & alu::kFC) ? 0x80 : 0;
    u8 n = (acc >> 1) | k;

    bool z = n == 0;

    flags = cond_bitset(z, flags, alu::kFZ);
    flags = cond_bitset(0, flags, alu::kFN);
    flags = cond_bitset(0, flags, alu::kFH);
    flags = cond_bitset(c, flags, alu::kFC);

    acc = n;
}

void alu::rlc(u8& flags, u8& acc) {
    bool c = acc & 0x80;

    u8 k = c ? 1 : 0;
    u8 n = (acc << 1) | k;

    bool z = n == 0;

    flags = 0;
    flags = cond_bitset(z, flags, alu::kFZ);
    flags = cond_bitset(0, flags, alu::kFN);
    flags = cond_bitset(0, flags, alu::kFH);
    flags = cond_bitset(c, flags, alu::kFC);

    acc = n;
}

void alu::rrc(u8& flags, u8& acc) {
    bool c = acc & 0x01;

    u8 k = c ? 0x80 : 0;
    u8 n = (acc >> 1) | k;

    bool z = n == 0;

    flags = cond_bitset(z, flags, alu::kFZ);
    flags = cond_bitset(0, flags, alu::kFN);
    flags = cond_bitset(0, flags, alu::kFH);
    flags = cond_bitset(c, flags, alu::kFC);


    acc = n;
}

void alu::sla(u8& flags, u8& acc) {
    u8 n = acc << 1;

    bool z = n == 0;
    bool c = acc & 0x80;

    flags = cond_bitset(z, flags, alu::kFZ);
    flags = cond_bitset(0, flags, alu::kFN);
    flags = cond_bitset(0, flags, alu::kFH);
    flags = cond_bitset(c, flags, alu::kFC);

    acc = n;
}

void alu::sra(u8& flags, u8& acc) {
    u8 n = (acc >> 1) | (acc & 0x80);

    bool z = n == 0;

    flags = cond_bitset(z, flags, alu::kFZ);
    flags = cond_bitset(0, flags, alu::kFN);
    flags = cond_bitset(0, flags, alu::kFH);
    flags = cond_bitset(0, flags, alu::kFC);

    acc = n;
}

void alu::srl(u8& flags, u8& acc) {
    u8 n = acc >> 1;

    bool z = n == 0;
    bool c = acc & 1;

    flags = cond_bitset(z, flags, alu::kFZ);
    flags = cond_bitset(0, flags, alu::kFN);
    flags = cond_bitset(0, flags, alu::kFH);
    flags = cond_bitset(c, flags, alu::kFC);

    acc = n;
}
