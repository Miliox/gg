/*
 * cpu.cpp
 * Copyright (C) 2018 Emiliano Firmino <emiliano.firmino@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "cpu.hpp"
#include "alu.hpp"

u8 CPU::read8() {
    // TODO: MMU
    r.pc += 1;
    return 0xff;
}

u16 CPU::read16() {
    // TODO: MMU
    r.pc += 2;
    return 0xffff;
}

u8 CPU::read8(u16 addr) {
    // TODO: MMU
    return 0xff;
}

u16 CPU::read16(u16 addr) {
    // TODO: MMU
    return 0xffff;
}

void CPU::write8(u16 addr, u8 val) {
    // TODO: MMU
}

void CPU::write16(u16 addr, u16 val) {
    // TODO: MMU
}

u8 CPU::mread8(u8 addr) {
    return read8(0xff00 + addr);
}

u16 CPU::mread16(u8 addr) {
    return read16(0xff00 + addr);
}

void CPU::mwrite8(u8 addr, u8 val) {
    write8(0xff00 + addr, val);
}

void CPU::mwrite16(u8 addr, u16 val) {
    write16(0xff00 + addr, val);
}

CPU::CPU() : isa(512, [&]() { /*NOP*/ return 4; }) {
    // NOP
    isa.at(0x00) = [&]() {
        return 4;
    };

    // LD BC,d16
    isa.at(0x01) = [&]() {
        r.bc = read16();
        return 12;
    };

    // LD (BC),A
    isa.at(0x02) = [&]() {
        write8(r.bc, r.a);
        return 8;
    };

    // INC BC
    isa.at(0x03) = [&]() {
        alu::inc16(r.f, r.bc);
        return 8;
    };

    // INC B
    isa.at(0x04) = [&]() {
        alu::inc8(r.f, r.b);
        return 4;
    };

    // DEC B
    isa.at(0x05) = [&]() {
        alu::dec8(r.f, r.b);
        return 4;
    };

    // LD B,d8
    isa.at(0x06) = [&]() {
        r.b = read8();
        return 8;
    };

    // RLCA
    isa.at(0x07) = [&]() {
        alu::rlc(r.f, r.a);
        return 4;
    };

    // LD (a16),SP
    isa.at(0x08) = [&]() {
        u16 addr = read16();
        write16(addr, r.sp);
        return 20;
    };

    // ADD HL,BC
    isa.at(0x09) = [&]() {
        alu::add16(r.f, r.hl, r.bc);
        return 8;
    };

    // LD A,(BC)
    isa.at(0x0a) = [&]() {
        r.a = read16(r.bc);
        return 8;
    };

    // DEC BC
    isa.at(0x0b) = [&]() {
        alu::dec16(r.f, r.bc);
        return 8;
    };

    // INC C
    isa.at(0x0c) = [&]() {
        alu::inc8(r.f, r.c);
        return 4;
    };

    // DEC C
    isa.at(0x0d) = [&]() {
        alu::dec8(r.f, r.c);
        return 4;
    };

    // LD C,d8
    isa.at(0x0e) = [&]() {
        r.c = read8();
        return 8;
    };

    // RRCA
    isa.at(0x0f) = [&]() {
        alu::rrc(r.f, r.a);
        return 4;
    };
}
