/*
 * apu.h
 * Copyright (C) 2018 Emiliano Firmino <emiliano.firmino@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef APU_H
#define APU_H

#include "common.h"

namespace alu {

// flags
extern const u8 kFZ; // Flag Zero
extern const u8 kFN; // Flag Negative
extern const u8 kFH; // Flag Half Carry
extern const u8 kFC; // Flag Carry

// move operations

u64 ld8(u8& dst, u8 src);
u64 ld16(u16& dst, u16 src);

// arithmetic operations

u64 add8(u8& flags, u8& acc, u8 arg);
u64 adc8(u8& flags, u8& acc, u8 arg);
u64 sub8(u8& flags, u8& acc, u8 arg);
u64 sbc8(u8& flags, u8& acc, u8 arg);
u64 inc8(u8& flags, u8& acc);
u64 dec8(u8& flags, u8& acc);

u64 add16(u8& flags, u16& acc, u16 arg);
u64 sub16(u8& flags, u16& acc, u16 arg);
u64 inc16(u8& flags, u16& acc);
u64 dec16(u8& flags, u16& acc);

// logical operation

u64 land(u8& flags, u8& acc, u8 arg);
u64 lxor(u8& flags, u8& acc, u8 arg);
u64 lor( u8& flags, u8& acc, u8 arg);
u64 lcp( u8& flags, u8& acc, u8 arg);

// bit manipulation

u64 bit(u8& flags, u8& acc, u8 arg);
u64 set(u8& flags, u8& acc, u8 arg);
u64 res(u8& flags, u8& acc, u8 arg);
u64 cpl(u8& flags, u8& acc);

// bit rotation and shifts

u64 rla( u8& flags, u8& acc);
u64 rra( u8& flags, u8& acc);
u64 rlca(u8& flags, u8& acc);
u64 rrca(u8& flags, u8& acc);

u64 rr(  u8& flags, u8& acc, u8 arg);
u64 rlc( u8& flags, u8& acc, u8 arg);
u64 rrc( u8& flags, u8& acc, u8 arg);

u64 sra(u8& flags, u8& acc);
u64 srl(u8& flags, u8& acc);

// misc

u64 daa(u8& flags, u8& acc);
u64 ccf(u8& flags, u8& acc);
u64 scf(u8& flags, u8& acc);
u64 swap(u8& flags, u8& acc);

}

#endif /* !APU_H */
