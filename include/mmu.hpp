/*
 * mmu.hpp
 * Copyright (C) 2018 Emiliano Firmino <emiliano.firmino@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef MMU_H
#define MMU_H

#include "common.h"

#include <cassert>

class MemoryRegion {
public:
    const u16 beg;
    const u16 end;

    bool readable;
    bool writable;

    buffer mem;

    MemoryRegion(u16 begin, u16 end)
        : beg(begin), end(end),
          readable(true),
          writable(true),
          mem(end - begin + 1) {
        assert(begin <= end);
        mem.assign(mem.size(), 0xff);
    }

    bool contains(u16 addr) {
        return addr >= beg && addr <= end;
    }

    u8 read8(u16 addr) {
        if (!readable) {
            return 0xff;
        }

        s32 offset = addr - beg;
        if (offset >= 0 && offset < mem.size()) {
            return mem.at(offset);
        }
        return 0xffu;
    }

    void write8(u16 addr, u8 val) {
        if (!writable) {
            return;
        }

        s32 offset = addr - beg;
        if (offset >= 0 && offset < mem.size()) {
            mem.at(offset) = val;
        }
    }
};

class MMU {
public:

    MMU();

    u8   read8( u16 addr);
    void write8(u16 addr, u8 val);
    void step(u8 ticks);

    void read( u16 src, buffer& dst);
    void write(u16 dst, buffer& src);

    MemoryRegion crom; // cartridge rom
    MemoryRegion vram; // video ram
    MemoryRegion iram; // internal ram
    MemoryRegion oram; // objects ram
    MemoryRegion hwio; // hardware io
    MemoryRegion zram; // zero page ram
};

#endif /* !MMU_H */
