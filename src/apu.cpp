/*
 * apu.cpp
 * Copyright (C) 2018 Emiliano Firmino <emiliano.firmino@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "apu.h"

const u8 apu::kFZ = 0b1000'0000;
const u8 apu::kFN = 0b0100'0000;
const u8 apu::kFH = 0b0010'0000;
const u8 apu::kFC = 0b0001'0000;

static inline u8 cond_bitset(bool clause, u8 bits, u8 mask) {
    if (clause) {
        return (bits | mask);
    } else {
        return (bits & ~mask);
    }
}

u64 apu::ld8(u8& dst, u8 src) {
    dst = src;
    return 4;
}

u64 apu::ld16(u16& dst, u16 src) {
    dst = src;
    return 8;
}

u64 apu::add8(u8& flags, u8& acc, u8 arg) {
    u16 n = acc + arg;

    bool z = (n & 0xff) == 0;
    bool h = ((acc & 0xf) + (arg & 0xf)) & 0x10;
    bool c = (n > 0xff);

    flags = cond_bitset(z, flags, apu::kFZ);
    flags = cond_bitset(0, flags, apu::kFN);
    flags = cond_bitset(h, flags, apu::kFH);
    flags = cond_bitset(c, flags, apu::kFZ);

    acc = (u8) n;
    return 4;
}

u64 apu::adc8(u8& flags, u8& acc, u8 arg) {
    u8  k = (flags & apu::kFC) ? 1 : 0;
    u16 n = acc + arg + k;

    bool z = (n & 0xff) == 0;
    bool h = ((acc & 0xf) + (arg & 0xf) + k) & 0x10;
    bool c = (n > 0xff);

    flags = cond_bitset(z, flags, apu::kFZ);
    flags = cond_bitset(0, flags, apu::kFN);
    flags = cond_bitset(h, flags, apu::kFH);
    flags = cond_bitset(c, flags, apu::kFZ);

    acc = (u8) n;
    return 4;
}

u64 apu::sub8(u8& flags, u8& acc, u8 arg) {
    u8 n = acc - arg;

    bool z = n == 0;
    bool h = (acc & 0xf) < (arg & 0xf);
    bool c = acc < arg;

    flags = cond_bitset(z, flags, apu::kFZ);
    flags = cond_bitset(1, flags, apu::kFN);
    flags = cond_bitset(h, flags, apu::kFH);
    flags = cond_bitset(c, flags, apu::kFC);

    acc = n;
    return 4;
}

u64 apu::sbc8(u8& flags, u8& acc, u8 arg) {
    u8 k = (flags & apu::kFC) ? 1 :0;
    u8 n = acc - arg - k;

    bool z = n == 0;
    bool h = (acc & 0xf) < ((arg & 0xf) + k);
    bool c = acc < (arg + k);

    flags = cond_bitset(z, flags, apu::kFZ);
    flags = cond_bitset(1, flags, apu::kFN);
    flags = cond_bitset(h, flags, kFH);
    flags = cond_bitset(c, flags, kFC);

    acc = n;
    return 4;
}

u64 apu::inc8(u8& flags, u8& acc) {
    u8 n = acc + 1;

    bool z = n == 0;
    bool h = acc == 0xf;

    flags = cond_bitset(z, flags, apu::kFZ);
    flags = cond_bitset(0, flags, apu::kFN);
    flags = cond_bitset(h, flags, apu::kFH);

    acc = n;
    return 4;
}

u64 apu::dec8(u8& flags, u8& acc) {
    u8 n = acc - 1;

    bool z = n == 0;
    bool h = acc & 0x10;

    flags = cond_bitset(z, flags, apu::kFZ);
    flags = cond_bitset(1, flags, apu::kFN);
    flags = cond_bitset(h, flags, apu::kFH);

    acc = n;
    return 4;
}

u64 apu::add16(u8& flags, u16& acc, u16 arg) {
    u32 n = acc + arg;

    bool h = (acc & 0xff) + (arg & 0xff) & 0x100;
    bool c = n > 0xffff;

    flags = cond_bitset(0, flags, apu::kFN);
    flags = cond_bitset(h, flags, apu::kFH);
    flags = cond_bitset(c, flags, apu::kFC);

    acc = n & 0xffff;
    return 8;
}

u64 apu::sub16(u8& flags, u16& acc, u16 arg) {
    u16 n = acc - arg;

    bool z = (n == 0);
    bool h = (acc & 0xff) < (arg & 0xff);
    bool c = (acc < arg);

    flags = cond_bitset(z, flags, apu::kFZ);
    flags = cond_bitset(1, flags, apu::kFN);
    flags = cond_bitset(h, flags, apu::kFH);
    flags = cond_bitset(c, flags, apu::kFC);

    acc = n;
    return 8;
}

u64 apu::inc16(u8& flags, u16& acc, u16 arg) {
    acc += 1;
    return 8;
}

u64 apu::dec16(u8& flags, u16& acc, u16 arg) {
    acc -= 1;
    return 8;
}
