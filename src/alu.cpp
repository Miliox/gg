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

u64 alu::ld8(u8& dst, u8 src) {
    dst = src;
    return 4;
}

u64 alu::ld16(u16& dst, u16 src) {
    dst = src;
    return 8;
}

u64 alu::add8(u8& flags, u8& acc, u8 arg) {
    u16 n = acc + arg;

    bool z = (n & 0xff) == 0;
    bool h = ((acc & 0xf) + (arg & 0xf)) & 0x10;
    bool c = (n > 0xff);

    flags = cond_bitset(z, flags, alu::kFZ);
    flags = cond_bitset(0, flags, alu::kFN);
    flags = cond_bitset(h, flags, alu::kFH);
    flags = cond_bitset(c, flags, alu::kFZ);

    acc = (u8) n;
    return 4;
}

u64 alu::adc8(u8& flags, u8& acc, u8 arg) {
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
    return 4;
}

u64 alu::sub8(u8& flags, u8& acc, u8 arg) {
    u8 n = acc - arg;

    bool z = n == 0;
    bool h = (acc & 0xf) < (arg & 0xf);
    bool c = acc < arg;

    flags = cond_bitset(z, flags, alu::kFZ);
    flags = cond_bitset(1, flags, alu::kFN);
    flags = cond_bitset(h, flags, alu::kFH);
    flags = cond_bitset(c, flags, alu::kFC);

    acc = n;
    return 4;
}

u64 alu::sbc8(u8& flags, u8& acc, u8 arg) {
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
    return 4;
}

u64 alu::inc8(u8& flags, u8& acc) {
    u8 n = acc + 1;

    bool z = n == 0;
    bool h = acc == 0xf;

    flags = cond_bitset(z, flags, alu::kFZ);
    flags = cond_bitset(0, flags, alu::kFN);
    flags = cond_bitset(h, flags, alu::kFH);

    acc = n;
    return 4;
}

u64 alu::dec8(u8& flags, u8& acc) {
    u8 n = acc - 1;

    bool z = n == 0;
    bool h = acc & 0x10;

    flags = cond_bitset(z, flags, alu::kFZ);
    flags = cond_bitset(1, flags, alu::kFN);
    flags = cond_bitset(h, flags, alu::kFH);

    acc = n;
    return 4;
}

u64 alu::add16(u8& flags, u16& acc, u16 arg) {
    u32 n = acc + arg;

    bool h = (acc & 0xff) + (arg & 0xff) & 0x100;
    bool c = n > 0xffff;

    flags = cond_bitset(0, flags, alu::kFN);
    flags = cond_bitset(h, flags, alu::kFH);
    flags = cond_bitset(c, flags, alu::kFC);

    acc = n & 0xffff;
    return 8;
}

u64 alu::sub16(u8& flags, u16& acc, u16 arg) {
    u16 n = acc - arg;

    bool z = (n == 0);
    bool h = (acc & 0xff) < (arg & 0xff);
    bool c = (acc < arg);

    flags = cond_bitset(z, flags, alu::kFZ);
    flags = cond_bitset(1, flags, alu::kFN);
    flags = cond_bitset(h, flags, alu::kFH);
    flags = cond_bitset(c, flags, alu::kFC);

    acc = n;
    return 8;
}

u64 alu::inc16(u8& flags, u16& acc) {
    acc += 1;
    return 8;
}

u64 alu::dec16(u8& flags, u16& acc) {
    acc -= 1;
    return 8;
}

u64 alu::land(u8& flags, u8& acc, u8 arg) {
    u8 n = acc & arg;

    bool z = n == 0;

    flags = cond_bitset(z, flags, alu::kFZ);
    flags = cond_bitset(0, flags, alu::kFN);
    flags = cond_bitset(1, flags, alu::kFH);
    flags = cond_bitset(0, flags, alu::kFC);

    acc = n;
    return 4;
}

u64 alu::lxor(u8& flags, u8& acc, u8 arg) {
    u8 n = acc ^ arg;

    bool z = n == 0;

    flags = cond_bitset(z, flags, alu::kFZ);
    flags = cond_bitset(0, flags, alu::kFN);
    flags = cond_bitset(0, flags, alu::kFH);
    flags = cond_bitset(0, flags, alu::kFC);

    acc = n;
    return 4;
}

u64 alu::lor(u8& flags, u8& acc, u8 arg) {
    u8 n = acc | arg;

    bool z = n == 0;

    flags = cond_bitset(z, flags, alu::kFZ);
    flags = cond_bitset(0, flags, alu::kFN);
    flags = cond_bitset(0, flags, alu::kFH);
    flags = cond_bitset(0, flags, alu::kFC);

    return 4;
}

u64 alu::lcp(u8& flags, u8& acc, u8 arg) {
    u8 n = acc - arg;

    bool z = n == 0;
    bool h = (acc & 0xf) < (arg & 0xf);
    bool c = acc < arg;

    flags = cond_bitset(z, flags, alu::kFZ);
    flags = cond_bitset(1, flags, alu::kFN);
    flags = cond_bitset(h, flags, alu::kFH);
    flags = cond_bitset(c, flags, alu::kFC);

    return 4;
}

u64 alu::bit(u8& flags, u8& acc, u8 arg) {
    bool z = acc & (1 << arg);

    flags = cond_bitset(z, flags, alu::kFZ);
    flags = cond_bitset(0, flags, alu::kFN);
    flags = cond_bitset(1, flags, alu::kFH);

    return 8;
}

u64 alu::set(u8& flags, u8& acc, u8 arg) {
    u8 n = acc | (1 << arg);

    acc = n;
    return 8;
}

u64 alu::res(u8& flags, u8& acc, u8& arg) {
    u8 n = acc & ~(1 << arg);

    acc = n;
    return 8;
}

u64 alu::cpl(u8& flags, u8& acc) {
    u8 n = ~acc;

    flags = cond_bitset(1, flags, alu::kFN);
    flags = cond_bitset(1, flags, alu::kFH);

    acc = n;
    return 4;
}

u64 alu::rl(u8& flags, u8& acc) {
    bool c = acc & 0x80;

    u8 k = (flags & alu::kFC) ? 1 : 0;
    u8 n = (acc << 1) | k;

    bool z = n == 0;

    flags = cond_bitset(z, flags, alu::kFZ);
    flags = cond_bitset(0, flags, alu::kFN);
    flags = cond_bitset(0, flags, alu::kFH);
    flags = cond_bitset(c, flags, alu::kFC);

    acc = n;
    return 4;
}

u64 alu::rr(u8& flags, u8& acc) {
    bool c = acc & 0x01;

    u8 k = (flags & alu::kFC) ? 0x80 : 0;
    u8 n = (acc >> 1) | k;

    bool z = n == 0;

    flags = cond_bitset(z, flags, alu::kFZ);
    flags = cond_bitset(0, flags, alu::kFN);
    flags = cond_bitset(0, flags, alu::kFH);
    flags = cond_bitset(c, flags, alu::kFC);

    acc = n;
    return 4;
}

u64 alu::rlc(u8& flags, u8& acc) {
    bool c = acc & 0x80;

    un8 k = c ? 1 : 0;
    un8 n = (acc << 1) | k;

    bool z = n == 0;

    flags = 0;
    flags = cond_bitset(z, flags, alu::kFZ);
    flags = cond_bitset(0, flags, alu::kFN);
    flags = cond_bitset(0, flags, alu::kFH);
    flags = cond_bitset(c, flags, alu::kFC);

    acc = n;
    return 4;
}

u64 alu::rrc(u8& flags, u8& acc) {
    bool c = acc & 0x01;

    u8 k = c ? 0x80 : 0;
    u8 n = (acc >> 1) | k;

    bool z = n == 0;

    flags = cond_bitset(z, flags, alu::kFZ);
    flags = cond_bitset(0, flags, alu::kFN);
    flags = cond_bitset(0, flags, alu::kFH);
    flags = cond_bitset(c, flags, alu::kFC);


    acc = n;
    return 4;
}

u64 alu::sla(u8& flags, u8& acc) {
    u8 n = acc << 1;

    bool z = n == 0;
    bool c = acc & 0x80;

    flags = cond_bitset(z, flags, alu::kFZ);
    flags = cond_bitset(0, flags, alu::kFN);
    flags = cond_bitset(0, flags, alu::kFH);
    flags = cond_bitset(c, flags, alu::kFC);

    acc = n;
    return 4;
}

u64 alu::sra(u8& flags, u8& acc) {
    u8 n = (acc >> 1) | (acc & 0x80);

    bool z = n == 0;

    flags = cond_bitset(z, flags, alu::kFZ);
    flags = cond_bitset(0, flags, alu::kFN);
    flags = cond_bitset(0, flags, alu::kFH);
    flags = cond_bitset(0, flags, alu::kFC);

    acc = n;
    return 4;
}

u64 alu::srl(u8& flags, u8& acc) {
    u8 n = acc >> 1;

    bool z = n == 0;
    bool c = acc & 1;

    flags = cond_bitset(z, flags, alu::kFZ);
    flags = cond_bitset(0, flags, alu::kFN);
    flags = cond_bitset(0, flags, alu::kFH);
    flags = cond_bitset(c, flags, alu::kFC);

    acc = n;
    return 4;
}
