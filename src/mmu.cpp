/*
 * mmu.cpp
 * Copyright (C) 2018 Emiliano Firmino <emiliano.firmino@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "mmu.hpp"

#include <utility>


class Range {
public:
    const u16 begin;
    const u16 end;

    Range (u16 begin, u16 end) : begin(begin), end(end) {}

    bool in(u16 addr) {
        return addr >= begin && addr <= end;
    }

};

// Memory Layout (Simplified for convenience)
const Range cartridgeRom  = { 0x0000u, 0x7FFFu }; // includes rest/int, cart header, cart banks
const Range videoRam      = { 0x8000u, 0x9FFFu };
const Range cartridgeRam  = { 0xA000u, 0xBFFFu };
const Range internalRam   = { 0xC000u, 0xDFFFu };
const Range echoRam       = { 0xE000u, 0xFDFFu };
const Range objAttrMem    = { 0xFE00u, 0xFEFFu }; // includes unusable memory
const Range hardwareIoReg = { 0xFF00u, 0xFF7Fu };
const Range zeroPageRam   = { 0xFF80u, 0xFFFFu }; // includes interrupt enable flag

MMU::MMU() :
    crom(0x0000u, 0x7FFFu), // includes rest/int, cart header, cart banks
    vram(0x8000u, 0x9FFFu),
    iram(0xC000u, 0xDFFFu),
    oram(0xFE00u, 0xFEFFu),
    hwio(0xFF00u, 0xFF7Fu),
    zram(0xFF80u, 0xFFFFu) {

}

u8 MMU::read8(u16 addr) {
    if (crom.contains(addr)) {
        return crom.read8(addr);
    }
    else if (vram.contains(addr)) {
        return vram.read8(addr);
    }
    else if (iram.contains(addr)) {
        return iram.read8(addr);
    }
    else if (oram.contains(addr)) {
        return oram.read8(addr);
    }
    else if (hwio.contains(addr)) {
        return hwio.read8(addr);
    }
    else if (zram.contains(addr)) {
        return zram.read8(addr);
    }

    return 0xffu;
}


void MMU::write8(u16 addr, u8 val) {
    if (crom.contains(addr)) {
        crom.write8(addr, val);
    }
    else if (vram.contains(addr)) {
        vram.write8(addr, val);
    }
    else if (iram.contains(addr)) {
        iram.write8(addr, val);
    }
    else if (oram.contains(addr)) {
        oram.write8(addr, val);
    }
    else if (hwio.contains(addr)) {
        hwio.write8(addr, val);
    }
    else if (zram.contains(addr)) {
        zram.write8(addr, val);
    }
}

void MMU::step(u8 ticks) {

}
