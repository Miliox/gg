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

void CPU::call(u16 addr) {

}

void CPU::rst(u16 addr) {

}

void CPU::ret() {

}

void CPU::push(u16& reg) {

}

void CPU::pop(u16& reg) {

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


    // STOP 0
    isa.at(0x10) = [&]() {
        return 4;
    };

    // LD DE,d16
    isa.at(0x11) = [&]() {
        r.de = read16();
        return 12;
    };

    // LD (DE),A
    isa.at(0x12) = [&]() {
        write8(r.de, r.a);
        return 8;
    };

    // INC DE
    isa.at(0x13) = [&]() {
        alu::inc16(r.f, r.de);
        return 8;
    };

    // INC D
    isa.at(0x14) = [&]() {
        alu::inc8(r.f, r.d);
        return 4;
    };

    // DEC D
    isa.at(0x15) = [&]() {
        alu::dec8(r.f, r.d);
        return 4;
    };

    // LD D,d8
    isa.at(0x16) = [&]() {
        r.d = read8();
        return 8;
    };

    // RLA
    isa.at(0x17) = [&]() {
        alu::rl(r.f, r.a);
        return 4;
    };

    // JR r8
    isa.at(0x18) = [&](){
        r.pc += s8(read8());
        return 12;
    };

    // ADD HL,DE
    isa.at(0x19) = [&]() {
        alu::add16(r.f, r.hl, r.de);
        return 8;
    };

    // LD A,(DE)
    isa.at(0x1a) = [&]() {
        r.a = read8(r.de);
        return 8;
    };

    // DEC DE
    isa.at(0x1b) = [&]() {
        alu::dec16(r.f, r.de);
        return 8;
    };

    // INC E
    isa.at(0x1c) = [&]() {
        alu::inc8(r.f, r.e);
        return 4;
    };

    // DEC E
    isa.at(0x1d) = [&]() {
        alu::dec8(r.f, r.e);
        return 4;
    };

    // LD E,d8
    isa.at(0x1e) = [&]() {
        r.e = read8();
        return 8;
    };

    // RRA
    isa.at(0x1f) = [&]() {
        alu::rr(r.f, r.a);
        return 4;
    };

    // JR NZ,r8
    isa.at(0x20) = [&]() {
        s8 offset = s8(read8());
        if ((r.f & alu::kFZ) == 0) {
            r.pc += offset;
            return 12;
        }
        return 8;
    };

    // LD HL,d16
    isa.at(0x21) = [&]() {
        r.hl = read16();
        return 12;
    };

    // LD (HL+),A
    isa.at(0x22) = [&]() {
        write8(r.hl++, r.a);
        return 8;
    };

    // INC HL
    isa.at(0x23) = [&]() {
        alu::inc16(r.f, r.hl);
        return 8;
    };

    // INC H
    isa.at(0x24) = [&]() {
        alu::inc8(r.f, r.h);
        return 4;
    };

    // DEC H
    isa.at(0x25) = [&]() {
        alu::dec8(r.f, r.h);
        return 4;
    };

    // LD H,d8
    isa.at(0x26) = [&]() {
        r.h = read8();
        return 8;
    };

    // DAA
    isa.at(0x27) = [&]() {
        alu::daa(r.f, r.a);
        return 4;
    };

    // JR Z,r8
    isa.at(0x28) = [&]() {
        s8 offset = s8(read8());
        if ((r.f & alu::kFZ) != 0) {
            r.pc += offset;
            return 12;
        }
        return 8;
    };

    // ADD HL,HL
    isa.at(0x29) = [&]() {
        alu::add16(r.f, r.hl, r.hl);
        return 8;
    };

    // LD A,(HL+)
    isa.at(0x2a) = [&]() {
        r.a = read8(r.hl++);
        return 8;
    };

    // DEC HL
    isa.at(0x2b) = [&]() {
        alu::dec16(r.f, r.hl);
        return 8;
    };

    // INC L
    isa.at(0x2c) = [&]() {
        alu::inc8(r.f, r.l);
        return 4;
    };

    // DEC L
    isa.at(0x2d) = [&]() {
        alu::dec8(r.f, r.l);
        return 4;
    };

    // LD L,d8
    isa.at(0x2e) = [&]() {
        r.l = read8();
        return 8;
    };

    // CPL
    isa.at(0x2f) = [&]() {
        alu::cpl(r.f, r.a);
        return 4;
    };

    // JR NC,r8
    isa.at(0x30) = [&]() {
        s8 offset = s8(read8());
        if ((r.f & alu::kFC) == 0) {
            r.pc += offset;
            return 12;
        }
        return 8;
    };

    // LD SP,d16
    isa.at(0x31) = [&]() {
        r.sp = read16();
        return 12;
    };

    // LD (HL-),A
    isa.at(0x32) = [&]() {
        write8(r.hl--, r.a);
        return 8;
    };

    // INC SP
    isa.at(0x33) = [&]() {
        alu::inc16(r.f, r.sp);
        return 8;
    };

    // INC (HL)
    isa.at(0x34) = [&]() {
        u8 v = read8(r.hl);
        alu::inc8(r.f, v);
        write8(r.hl, v);
        return 12;
    };

    // DEC (HL)
    isa.at(0x35) = [&]() {
        u8 v = read8(r.hl);
        alu::dec8(r.f, v);
        write8(r.hl, v);
        return 12;
    };

    // LD (HL),d8
    isa.at(0x36) = [&]() {
        u8 v = read8();
        write8(r.hl, v);
        return 12;
    };

    // SCF
    isa.at(0x37) = [&]() {
        alu::scf(r.f);
        return 4;
    };

    // JR C,r8
    isa.at(0x38) = [&]() {
        s8 offset = s8(read8());
        if ((r.f & alu::kFC) != 0) {
            r.pc += offset;
            return 12;
        }
        return 8;
    };

    // ADD HL,SP
    isa.at(0x39) = [&]() {
        alu::add16(r.f, r.hl, r.sp);
        return 8;
    };

    // LD A,(HL-)
    isa.at(0x3a) = [&]() {
        r.a = read8(r.hl--);
        return 8;
    };

    // DEC SP
    isa.at(0x3b) = [&]() {
        alu::dec16(r.f, r.sp);
        return 8;
    };

    // INC A
    isa.at(0x3c) = [&]() {
        alu::inc8(r.f, r.a);
        return 4;
    };

    // DEC A
    isa.at(0x3d) = [&]() {
        alu::dec8(r.f, r.a);
        return 4;
    };

    // LD A,d8
    isa.at(0x3e) = [&]() {
        r.a = read8();
        return 8;
    };

    // CCF
    isa.at(0x3f) = [&]() {
        alu::ccf(r.f);
        return 4;
    };

    // LD B,B
    isa.at(0x40) = [&]() {
        r.b = r.b;
        return 4;
    };

    // LD B,C
    isa.at(0x41) = [&]() {
        r.b = r.c;
        return 4;
    };

    // LD B,D
    isa.at(0x42) = [&]() {
        r.b = r.d;
        return 4;
    };

    // LD B,E
    isa.at(0x43) = [&]() {
        r.b = r.e;
        return 4;
    };

    // LD B,H
    isa.at(0x44) = [&]() {
        r.b = r.h;
        return 4;
    };

    // LD B,L
    isa.at(0x45) = [&]() {
        r.b = r.l;
        return 4;
    };

    // LD B,(HL)
    isa.at(0x46) = [&]() {
        r.b = read8(r.hl);
        return 8;
    };

    // LD B,A
    isa.at(0x47) = [&]() {
        r.b = r.a;
        return 4;
    };

    // LD C,B
    isa.at(0x48) = [&]() {
        r.c = r.b;
        return 4;
    };

    // LD C,C
    isa.at(0x49) = [&]() {
        r.c = r.c;
        return 4;
    };

    // LD C,D
    isa.at(0x4a) = [&]() {
        r.c = r.d;
        return 4;
    };

    // LD C,E
    isa.at(0x4b) = [&]() {
        r.c = r.e;
        return 4;
    };

    // LD C,H
    isa.at(0x4c) = [&]() {
        r.c = r.h;
        return 4;
    };

    // LD C,L
    isa.at(0x4d) = [&]() {
        r.c = r.l;
        return 4;
    };

    // LD C,(HL)
    isa.at(0x4e) = [&]() {
        r.c = read8(r.hl);
        return 8;
    };

    // LD C,A
    isa.at(0x4f) = [&]() {
        r.c = r.a;
        return 4;
    };


    // LD D,B
    isa.at(0x50) = [&]() {
        r.d = r.b;
        return 4;
    };

    // LD D,C
    isa.at(0x51) = [&]() {
        r.d = r.c;
        return 4;
    };

    // LD D,D
    isa.at(0x52) = [&]() {
        r.d = r.d;
        return 4;
    };

    // LD D,E
    isa.at(0x53) = [&]() {
        r.d = r.e;
        return 4;
    };

    // LD D,H
    isa.at(0x54) = [&]() {
        r.d = r.h;
        return 4;
    };

    // LD D,L
    isa.at(0x55) = [&]() {
        r.d = r.l;
        return 4;
    };

    // LD D,(HL)
    isa.at(0x56) = [&]() {
        r.d = read8(r.hl);
        return 8;
    };

    // LD D,A
    isa.at(0x57) = [&]() {
        r.d = r.a;
        return 4;
    };

    // LD E,B
    isa.at(0x58) = [&]() {
        r.e = r.b;
        return 4;
    };

    // LD E,C
    isa.at(0x59) = [&]() {
        r.e = r.c;
        return 4;
    };

    // LD E,D
    isa.at(0x5a) = [&]() {
        r.e = r.d;
        return 4;
    };

    // LD E,E
    isa.at(0x5b) = [&]() {
        r.e = r.e;
        return 4;
    };

    // LD E,H
    isa.at(0x5c) = [&]() {
        r.e = r.h;
        return 4;
    };

    // LD E,L
    isa.at(0x5d) = [&]() {
        r.e = r.l;
        return 4;
    };

    // LD E,(HL)
    isa.at(0x5e) = [&]() {
        r.e = read8(r.hl);
        return 8;
    };

    // LD E,A
    isa.at(0x5f) = [&]() {
        r.e = r.a;
        return 4;
    };

    // LD H,B
    isa.at(0x60) = [&]() {
        r.h = r.b;
        return 4;
    };

    // LD H,C
    isa.at(0x61) = [&]() {
        r.h = r.c;
        return 4;
    };

    // LD H,D
    isa.at(0x62) = [&]() {
        r.h = r.d;
        return 4;
    };

    // LD H,E
    isa.at(0x63) = [&]() {
        r.h = r.e;
        return 4;
    };

    // LD H,H
    isa.at(0x64) = [&]() {
        r.h = r.h;
        return 4;
    };

    // LD H,L
    isa.at(0x65) = [&]() {
        r.h = r.l;
        return 4;
    };

    // LD H,(HL)
    isa.at(0x66) = [&]() {
        r.h = read8(r.hl);
        return 8;
    };

    // LD H,A
    isa.at(0x67) = [&]() {
        r.h = r.a;
        return 4;
    };


    // LD L,B
    isa.at(0x68) = [&]() {
        r.l = r.b;
        return 4;
    };

    // LD L,C
    isa.at(0x69) = [&]() {
        r.l = r.c;
        return 4;
    };

    // LD L,D
    isa.at(0x6a) = [&]() {
        r.l = r.d;
        return 4;
    };

    // LD L,E
    isa.at(0x6b) = [&]() {
        r.l = r.e;
        return 4;
    };

    // LD L,H
    isa.at(0x6c) = [&]() {
        r.l = r.h;
        return 4;
    };

    // LD L,L
    isa.at(0x6d) = [&]() {
        r.l = r.l;
        return 4;
    };

    // LD L,(HL)
    isa.at(0x6e) = [&]() {
        r.l = read8(r.hl);
        return 8;
    };

    // LD L,A
    isa.at(0x6f) = [&]() {
        r.l = r.a;
        return 4;
    };


    // LD (HL),B
    isa.at(0x70) = [&]() {
        write8(r.hl, r.b);
        return 8;
    };

    // LD (HL),C
    isa.at(0x71) = [&]() {
        write8(r.hl, r.c);
        return 8;
    };

    // LD (HL),D
    isa.at(0x72) = [&]() {
        write8(r.hl, r.d);
        return 8;
    };

    // LD (HL),E
    isa.at(0x73) = [&]() {
        write8(r.hl, r.e);
        return 8;
    };

    // LD (HL),H
    isa.at(0x74) = [&]() {
        write8(r.hl, r.h);
        return 8;
    };

    // LD (HL),L
    isa.at(0x75) = [&]() {
        write8(r.hl, r.l);
        return 8;
    };

    // HALT
    isa.at(0x76) = [&]() {
        return 4;
    };

    // LD (HL),A
    isa.at(0x77) = [&]() {
        write8(r.hl, r.a);
        return 8;
    };

    // LD A,B
    isa.at(0x78) = [&]() {
        r.a = r.b;
        return 4;
    };

    // LD A,C
    isa.at(0x79) = [&]() {
        r.a = r.c;
        return 4;
    };

    // LD A,D
    isa.at(0x7a) = [&]() {
        r.a = r.d;
        return 4;
    };

    // LD A,E
    isa.at(0x7b) = [&]() {
        r.a = r.e;
        return 4;
    };

    // LD A,H
    isa.at(0x7c) = [&]() {
        r.a = r.h;
        return 4;
    };

    // LD A,L
    isa.at(0x7d) = [&]() {
        r.a = r.l;
        return 4;
    };

    // LD A,(HL)
    isa.at(0x7e) = [&]() {
        r.a = read8(r.hl);
        return 8;
    };

    // LD A,A
    isa.at(0x7f) = [&]() {
        r.a = r.a;
        return 4;
    };

    // ADD A,B
    isa.at(0x80) = [&]() {
        alu::add8(r.f, r.a, r.b);
        return 4;
    };

    // ADD A,C
    isa.at(0x81) = [&]() {
        alu::add8(r.f, r.a, r.c);
        return 4;
    };

    // ADD A,D
    isa.at(0x82) = [&]() {
        alu::add8(r.f, r.a, r.d);
        return 4;
    };

    // ADD A,E
    isa.at(0x83) = [&]() {
        alu::add8(r.f, r.a, r.e);
        return 4;
    };

    // ADD A,H
    isa.at(0x84) = [&]() {
        alu::add8(r.f, r.a, r.h);
        return 4;
    };

    // ADD A,L
    isa.at(0x85) = [&]() {
        alu::add8(r.f, r.a, r.l);
        return 4;
    };

    // ADD A,(HL)
    isa.at(0x86) = [&]() {
        alu::add8(r.f, r.a, read8(r.hl));
        return 8;
    };

    // ADD A,A
    isa.at(0x87) = [&]() {
        alu::add8(r.f, r.a, r.a);
        return 4;
    };

    // ADC A,B
    isa.at(0x88) = [&]() {
        alu::adc8(r.f, r.a, r.b);
        return 4;
    };

    // ADC A,C
    isa.at(0x89) = [&]() {
        alu::adc8(r.f, r.a, r.c);
        return 4;
    };

    // ADC A,D
    isa.at(0x8a) = [&]() {
        alu::adc8(r.f, r.a, r.d);
        return 4;
    };

    // ADC A,E
    isa.at(0x8b) = [&]() {
        alu::adc8(r.f, r.a, r.e);
        return 4;
    };

    // ADC A,H
    isa.at(0x8c) = [&]() {
        alu::adc8(r.f, r.a, r.h);
        return 4;
    };

    // ADC A,L
    isa.at(0x8d) = [&]() {
        alu::adc8(r.f, r.a, r.l);
        return 4;
    };

    // ADC A,(HL)
    isa.at(0x8e) = [&]() {
        alu::adc8(r.f, r.a, read8(r.hl));
        return 8;
    };

    // ADC A,A
    isa.at(0x8f) = [&]() {
        alu::adc8(r.f, r.a, r.a);
        return 4;
    };


    // SUB B
    isa.at(0x90) = [&]() {
        alu::sub8(r.f, r.a, r.b);
        return 4;
    };

    // SUB C
    isa.at(0x91) = [&]() {
        alu::sub8(r.f, r.a, r.c);
        return 4;
    };

    // SUB D
    isa.at(0x92) = [&]() {
        alu::sub8(r.f, r.a, r.d);
        return 4;
    };

    // SUB E
    isa.at(0x93) = [&]() {
        alu::sub8(r.f, r.a, r.e);
        return 4;
    };

    // SUB H
    isa.at(0x94) = [&]() {
        alu::sub8(r.f, r.a, r.h);
        return 4;
    };

    // SUB L
    isa.at(0x95) = [&]() {
        alu::sub8(r.f, r.a, r.l);
        return 4;
    };

    // SUB (HL)
    isa.at(0x96) = [&]() {
        alu::sub8(r.f, r.a, read8(r.hl));
        return 4;
    };

    // SUB A
    isa.at(0x97) = [&]() {
        alu::sub8(r.f, r.a, r.a);
        return 4;
    };

    // SBC A,B
    isa.at(0x98) = [&]() {
        alu::sbc8(r.f, r.a, r.b);
        return 4;
    };

    // SBC A,C
    isa.at(0x99) = [&]() {
        alu::sbc8(r.f, r.a, r.c);
        return 4;
    };

    // SBC A,D
    isa.at(0x9a) = [&]() {
        alu::sbc8(r.f, r.a, r.d);
        return 4;
    };

    // SBC A,E
    isa.at(0x9b) = [&]() {
        alu::sbc8(r.f, r.a, r.e);
        return 4;
    };

    // SBC A,H
    isa.at(0x9c) = [&]() {
        alu::sbc8(r.f, r.a, r.h);
        return 4;
    };

    // SBC A,L
    isa.at(0x9d) = [&]() {
        alu::sbc8(r.f, r.a, r.l);
        return 4;
    };

    // SBC A,(HL)
    isa.at(0x9e) = [&]() {
        alu::sbc8(r.f, r.a, read8(r.hl));
        return 8;
    };

    // SBC A,A
    isa.at(0x9f) = [&]() {
        alu::sbc8(r.f, r.a, r.a);
        return 4;
    };


    // AND B
    isa.at(0xa0) = [&]() {
        alu::land(r.f, r.a, r.b);
        return 4;
    };

    // AND C
    isa.at(0xa1) = [&]() {
        alu::land(r.f, r.a, r.c);
        return 4;
    };

    // AND D
    isa.at(0xa2) = [&]() {
        alu::land(r.f, r.a, r.d);
        return 4;
    };

    // AND E
    isa.at(0xa3) = [&]() {
        alu::land(r.f, r.a, r.e);
        return 4;
    };

    // AND H
    isa.at(0xa4) = [&]() {
        alu::land(r.f, r.a, r.h);
        return 4;
    };

    // AND L
    isa.at(0xa5) = [&]() {
        alu::land(r.f, r.a, r.l);
        return 4;
    };

    // AND (HL)
    isa.at(0xa6) = [&]() {
        alu::land(r.f, r.a, read8(r.hl));
        return 8;
    };

    // AND A
    isa.at(0xa7) = [&]() {
        alu::land(r.f, r.a, r.a);
        return 4;
    };

    // XOR B
    isa.at(0xa8) = [&]() {
        alu::lxor(r.f, r.a, r.b);
        return 4;
    };

    // XOR C
    isa.at(0xa9) = [&]() {
        alu::lxor(r.f, r.a, r.c);
        return 4;
    };

    // XOR D
    isa.at(0xaa) = [&]() {
        alu::lxor(r.f, r.a, r.d);
        return 4;
    };

    // XOR E
    isa.at(0xab) = [&]() {
        alu::lxor(r.f, r.a, r.e);
        return 4;
    };

    // XOR H
    isa.at(0xac) = [&]() {
        alu::lxor(r.f, r.a, r.h);
        return 4;
    };

    // XOR L
    isa.at(0xad) = [&]() {
        alu::lxor(r.f, r.a, r.l);
        return 4;
    };

    // XOR (HL)
    isa.at(0xae) = [&]() {
        alu::lxor(r.f, r.a, read8(r.hl));
        return 8;
    };

    // XOR A
    isa.at(0xaf) = [&]() {
        alu::lxor(r.f, r.a, r.a);
        return 4;
    };


    // OR B
    isa.at(0xb0) = [&]() {
        alu::lor(r.f, r.a, r.b);
        return 4;
    };

    // OR C
    isa.at(0xb1) = [&]() {
        alu::lor(r.f, r.a, r.c);
        return 4;
    };

    // OR D
    isa.at(0xb2) = [&]() {
        alu::lor(r.f, r.a, r.d);
        return 4;
    };

    // OR E
    isa.at(0xb3) = [&]() {
        alu::lor(r.f, r.a, r.e);
        return 4;
    };

    // OR H
    isa.at(0xb4) = [&]() {
        alu::lor(r.f, r.a, r.h);
        return 4;
    };

    // OR L
    isa.at(0xb5) = [&]() {
        alu::lor(r.f, r.a, r.l);
        return 4;
    };

    // OR (HL)
    isa.at(0xb6) = [&]() {
        alu::lor(r.f, r.a, read8(r.hl));
        return 8;
    };

    // OR A
    isa.at(0xb7) = [&]() {
        alu::lor(r.f, r.a, r.a);
        return 4;
    };

    // CP B
    isa.at(0xb8) = [&]() {
        alu::lcp(r.f, r.a, r.b);
        return 4;
    };

    // CP C
    isa.at(0xb9) = [&]() {
        alu::lcp(r.f, r.a, r.c);
        return 4;
    };

    // CP D
    isa.at(0xba) = [&]() {
        alu::lcp(r.f, r.a, r.d);
        return 4;
    };

    // CP E
    isa.at(0xbb) = [&]() {
        alu::lcp(r.f, r.a, r.e);
        return 4;
    };

    // CP H
    isa.at(0xbc) = [&]() {
        alu::lcp(r.f, r.a, r.h);
        return 4;
    };

    // CP L
    isa.at(0xbd) = [&]() {
        alu::lcp(r.f, r.a, r.l);
        return 4;
    };

    // CP (HL)
    isa.at(0xbe) = [&]() {
        alu::lcp(r.f, r.a, read8(r.hl));
        return 8;
    };

    // CP A
    isa.at(0xbf) = [&]() {
        alu::lcp(r.f, r.a, r.a);
        return 4;
    };

    // RET NZ
    isa.at(0xc0) = [&]() {
        if ((r.f & alu::kFZ) == 0) {
            ret();
            return 20;
        }
        return 8;
    };

    // POP BC
    isa.at(0xc1) = [&]() {
        pop(r.bc);
        return 12;
    };

    // JP NZ,a16
    isa.at(0xc2) = [&]() {
        u16 addr = read16();
        if ((r.f & alu::kFZ) == 0) {
            r.pc = addr;
            return 16;
        }
        return 12;
    };

    // JP a16
    isa.at(0xc3) = [&]() {
        r.pc = read16();
        return 12;
    };

    // CALL NZ,a16
    isa.at(0xc4) = [&]() {
        u16 addr = read16();
        if ((r.f & alu::kFZ) == 0) {
            call(addr);
            return 24;
        }
        return 12;
    };

    // PUSH BC
    isa.at(0xc5) = [&]() {
        push(r.bc);
        return 16;
    };

    // ADD A,d8
    isa.at(0xc6) = [&]() {
        alu::add8(r.f, r.a, read8());
        return 8;
    };

    // RST 00H
    isa.at(0xc7) = [&]() {
        rst(0x00);
        return 16;
    };

    // RET Z
    isa.at(0xc8) = [&]() {
        if ((r.f & alu::kFZ) != 0) {
            ret();
            return 20;
        }
        return 8;
    };

    // RET
    isa.at(0xc9) = [&]() {
        ret();
        return 16;
    };

    // JP Z,a16
    isa.at(0xca) = [&]() {
        u16 addr = read16();
        if ((r.f & alu::kFZ) != 0) {
            r.pc = addr;
            return 16;
        }
        return 12;
    };

    // PREFIX CB
    isa.at(0xcb) = [&]() {
        u8 opcode = read8();
        return 4 + isa.at(0x100 | opcode)();
    };

    // CALL Z,a16
    isa.at(0xcc) = [&]() {
        u16 addr = read16();
        if ((r.f & alu::kFZ) != 0) {
            call(addr);
            return 12;
        }
        return 8;
    };

    // CALL a16
    isa.at(0xcd) = [&]() {
        call(read16());
        return 8;
    };

    // ADC A,d8
    isa.at(0xce) = [&]() {
        alu::adc8(r.f, r.a, read8());
        return 8;
    };

    // RST 08H
    isa.at(0xcf) = [&]() {
        rst(0x08);
        return 16;
    };


    // RET NC
    isa.at(0xd0) = [&]() {
        if ((r.f & alu::kFC) == 0) {
            ret();
            return 20;
        }
        return 8;
    };

    // POP DE
    isa.at(0xd1) = [&]() {
        pop(r.de);
        return 12;
    };

    // JP NC,a16
    isa.at(0xd2) = [&]() {
        u16 addr = read16();
        if ((r.f & alu::kFC) == 0) {
            r.pc = addr;
            return 16;
        }
        return 12;
    };

    // NOP
    isa.at(0xd3) = [&]() {
        return 4;
    };

    // CALL NC,a16
    isa.at(0xd4) = [&]() {
        u16 addr = read16();
        if ((r.f & alu::kFC) == 0) {
            call(addr);
            return 24;
        }
        return 12;
    };

    // PUSH DE
    isa.at(0xd5) = [&]() {
        push(r.de);
        return 16;
    };

    // SUB d8
    isa.at(0xd6) = [&]() {
        alu::sub8(r.f, r.a, read8());
        return 8;
    };

    // RST 10H
    isa.at(0xd7) = [&]() {
        rst(0x10);
        return 16;
    };

    // RET C
    isa.at(0xd8) = [&]() {
        if ((r.f & alu::kFC) != 0) {
            ret();
            return 20;
        }
        return 8;
    };

    // RETI
    isa.at(0xd9) = [&]() {
        ret();
        r.ie = 1;
        return 16;
    };

    // JP C,a16
    isa.at(0xda) = [&]() {
        u16 addr = read16();
        if ((r.f & alu::kFC) != 0) {
            r.pc = addr;
            return 16;
        }
        return 12;
    };

    // NOP
    isa.at(0xdb) = [&]() {
        return 4;
    };

    // CALL C,a16
    isa.at(0xdc) = [&]() {
        u16 addr = read16();
        if ((r.f & alu::kFC) != 0) {
            call(addr);
            return 24;
        }
        return 12;
    };

    // NOP
    isa.at(0xdd) = [&]() {
        return 4;
    };

    // SBC A,d8
    isa.at(0xde) = [&]() {
        alu::sbc8(r.f, r.a, read8());
        return 8;
    };

    // RST 18H
    isa.at(0xdf) = [&]() {
        rst(0x18);
        return 16;
    };

    // LDH (a8),A
    isa.at(0xe0) = [&]() {
        mwrite8(read8(), r.a);
        return 12;
    };

    // POP HL
    isa.at(0xe1) = [&]() {
        pop(r.hl);
        return 12;
    };

    // LD (C),A
    isa.at(0xe2) = [&]() {
        mwrite8(r.c, r.a);
        return 8;
    };

    // NOP
    isa.at(0xe3) = [&]() {
        return 4;
    };

    // NOP
    isa.at(0xe4) = [&]() {
        return 4;
    };

    // PUSH HL
    isa.at(0xe5) = [&]() {
        push(r.hl);
        return 16;
    };

    // AND d8
    isa.at(0xe6) = [&]() {
        alu::land(r.f, r.a, read8());
        return 8;
    };

    // RST 20H
    isa.at(0xe7) = [&]() {
        rst(0x20);
        return 16;
    };

    // ADD SP,r8
    isa.at(0xe8) = [&]() {
        r.pc += s8(read8());
        return 16;
    };

    // JP (HL)
    isa.at(0xe9) = [&]() {
        r.pc = r.hl;
        return 4;
    };

    // LD (a16),A
    isa.at(0xea) = [&]() {
        write8(read16(), r.a);
        return 16;
    };

    // NOP
    isa.at(0xeb) = [&]() {
        return 4;
    };

    // NOP
    isa.at(0xec) = [&]() {
        return 4;
    };

    // NOP
    isa.at(0xed) = [&]() {
        return 4;
    };

    // XOR d8
    isa.at(0xee) = [&]() {
        alu::lxor(r.f, r.a, read8());
        return 8;
    };

    // RST 28H
    isa.at(0xef) = [&]() {
        rst(0x28);
        return 16;
    };

    // LDH A,(a8)
    isa.at(0xf0) = [&]() {
        r.a = mread8(read8());
        return 12;
    };

    // POP AF
    isa.at(0xf1) = [&]() {
        pop(r.af);
        return 12;
    };

    // LD A,(C)
    isa.at(0xf2) = [&]() {
        r.a = mread8(r.c);
        return 8;
    };

    // DI
    isa.at(0xf3) = [&]() {
        r.ie = 0;
        return 4;
    };

    // NOP
    isa.at(0xf4) = [&]() {
        return 4;
    };

    // PUSH AF
    isa.at(0xf5) = [&]() {
        push(r.af);
        return 16;
    };

    // OR d8
    isa.at(0xf6) = [&]() {
        alu::lor(r.f, r.a, read8());
        return 8;
    };

    // RST 30H
    isa.at(0xf7) = [&]() {
        rst(0x30);
        return 16;
    };

    // LD HL,SP+r8
    isa.at(0xf8) = [&]() {
        r.hl = r.pc + s8(read8());
        return 12;
    };

    // LD SP,HL
    isa.at(0xf9) = [&]() {
        r.sp = r.hl;
        return 8;
    };

    // LD A,(a16)
    isa.at(0xfa) = [&]() {
        r.a = read8(read16());
        return 16;
    };

    // EI
    isa.at(0xfb) = [&]() {
        r.ie = 1;
        return 4;
    };

    // NOP
    isa.at(0xfc) = [&]() {
        return 4;
    };

    // NOP
    isa.at(0xfd) = [&]() {
        return 4;
    };

    // CP d8
    isa.at(0xfe) = [&]() {
        alu::lcp(r.f, r.a, read8());
        return 8;
    };

    // RST 38H
    isa.at(0xff) = [&]() {
        rst(0x38);
        return 16;
    };

    // CB PREFIX

    // RLC B
    isa.at(0x100) = [&]() {
        alu::rlc(r.f, r.b);
        return 8;
    };

    // RLC C
    isa.at(0x101) = [&]() {
        alu::rlc(r.f, r.c);
        return 8;
    };

    // RLC D
    isa.at(0x102) = [&]() {
        alu::rlc(r.f, r.d);
        return 8;
    };

    // RLC E
    isa.at(0x103) = [&]() {
        alu::rlc(r.f, r.e);
        return 8;
    };

    // RLC H
    isa.at(0x104) = [&]() {
        alu::rlc(r.f, r.h);
        return 8;
    };

    // RLC L
    isa.at(0x105) = [&]() {
        alu::rlc(r.f, r.l);
        return 8;
    };

    // RLC (HL)
    isa.at(0x106) = [&]() {
        u8 v = read8(r.hl);
        alu::rlc(r.f, v);
        write8(r.hl, v);
        return 16;
    };

    // RLC A
    isa.at(0x107) = [&]() {
        alu::rlc(r.f, r.a);
        return 8;
    };

    // RRC B
    isa.at(0x108) = [&]() {
        alu::rrc(r.f, r.b);
        return 8;
    };

    // RRC C
    isa.at(0x109) = [&]() {
        alu::rrc(r.f, r.c);
        return 8;
    };

    // RRC D
    isa.at(0x10a) = [&]() {
        alu::rrc(r.f, r.d);
        return 8;
    };

    // RRC E
    isa.at(0x10b) = [&]() {
        alu::rrc(r.f, r.e);
        return 8;
    };

    // RRC H
    isa.at(0x10c) = [&]() {
        alu::rrc(r.f, r.h);
        return 8;
    };

    // RRC L
    isa.at(0x10d) = [&]() {
        alu::rrc(r.f, r.l);
        return 8;
    };

    // RRC (HL)
    isa.at(0x10e) = [&]() {
        u8 v = read8(r.hl);
        alu::rrc(r.f, v);
        write8(r.hl, v);
        return 16;
    };

    // RRC A
    isa.at(0x10f) = [&]() {
        alu::rrc(r.f, r.a);
        return 8;
    };

    // RL B
    isa.at(0x110) = [&]() {
        alu::rl(r.f, r.b);
        return 8;
    };

    // RL C
    isa.at(0x111) = [&]() {
        alu::rl(r.f, r.c);
        return 8;
    };

    // RL D
    isa.at(0x112) = [&]() {
        alu::rl(r.f, r.d);
        return 8;
    };

    // RL E
    isa.at(0x113) = [&]() {
        alu::rl(r.f, r.e);
        return 8;
    };

    // RL H
    isa.at(0x114) = [&]() {
        alu::rl(r.f, r.h);
        return 8;
    };

    // RL L
    isa.at(0x115) = [&]() {
        alu::rl(r.f, r.l);
        return 8;
    };

    // RL (HL)
    isa.at(0x116) = [&]() {
        u8 v = read8(r.hl);
        alu::rl(r.f, v);
        write8(r.hl, v);
        return 8;
    };

    // RL A
    isa.at(0x117) = [&]() {
        alu::rl(r.f, r.a);
        return 8;
    };


    // RR B
    isa.at(0x118) = [&]() {
        alu::rr(r.f, r.b);
        return 8;
    };

    // RR C
    isa.at(0x119) = [&]() {
        alu::rr(r.f, r.c);
        return 8;
    };

    // RR D
    isa.at(0x11a) = [&]() {
        alu::rr(r.f, r.d);
        return 8;
    };

    // RR E
    isa.at(0x11b) = [&]() {
        alu::rr(r.f, r.e);
        return 8;
    };

    // RR H
    isa.at(0x11c) = [&]() {
        alu::rr(r.f, r.h);
        return 8;
    };

    // RR L
    isa.at(0x11d) = [&]() {
        alu::rr(r.f, r.l);
        return 8;
    };

    // RR (HL)
    isa.at(0x11e) = [&]() {
        u8 v = read8(r.hl);
        alu::rr(r.f, v);
        write8(r.hl, v);
        return 16;
    };

    // RR A
    isa.at(0x11f) = [&]() {
        alu::rr(r.f, r.a);
        return 8;
    };

    // SLA B
    isa.at(0x120) = [&]() {
        alu::sla(r.f, r.b);
        return 8;
    };

    // SLA C
    isa.at(0x121) = [&]() {
        alu::sla(r.f, r.c);
        return 8;
    };

    // SLA D
    isa.at(0x122) = [&]() {
        alu::sla(r.f, r.d);
        return 8;
    };

    // SLA E
    isa.at(0x123) = [&]() {
        alu::sla(r.f, r.e);
        return 8;
    };

    // SLA H
    isa.at(0x124) = [&]() {
        alu::sla(r.f, r.h);
        return 8;
    };

    // SLA L
    isa.at(0x125) = [&]() {
        alu::sla(r.f, r.l);
        return 8;
    };

    // SLA (HL)
    isa.at(0x126) = [&]() {
        u8 v = read8(r.hl);
        alu::sla(r.f, v);
        write8(r.hl, v);
        return 16;
    };

    // SLA A
    isa.at(0x127) = [&]() {
        alu::sla(r.f, r.a);
        return 8;
    };


    // SRA B
    isa.at(0x128) = [&]() {
        alu::sra(r.f, r.b);
        return 8;
    };

    // SRA C
    isa.at(0x129) = [&]() {
        alu::sra(r.f, r.c);
        return 8;
    };

    // SRA D
    isa.at(0x12a) = [&]() {
        alu::sra(r.f, r.d);
        return 8;
    };

    // SRA E
    isa.at(0x12b) = [&]() {
        alu::sra(r.f, r.e);
        return 8;
    };

    // SRA H
    isa.at(0x12c) = [&]() {
        alu::sra(r.f, r.h);
        return 8;
    };

    // SRA L
    isa.at(0x12d) = [&]() {
        alu::sra(r.f, r.l);
        return 8;
    };

    // SRA (HL)
    isa.at(0x12e) = [&]() {
        u8 v = read8(r.hl);
        alu::sra(r.f, v);
        write8(r.hl, v);
        return 16;
    };

    // SRA A
    isa.at(0x12f) = [&]() {
        alu::sra(r.f, r.b);
        return 8;
    };

    // SWAP B
    isa.at(0x130) = [&]() {
        alu::swap(r.f, r.b);
        return 8;
    };

    // SWAP C
    isa.at(0x131) = [&]() {
        alu::swap(r.f, r.c);
        return 8;
    };

    // SWAP D
    isa.at(0x132) = [&]() {
        alu::swap(r.f, r.d);
        return 8;
    };

    // SWAP E
    isa.at(0x133) = [&]() {
        alu::swap(r.f, r.e);
        return 8;
    };

    // SWAP H
    isa.at(0x134) = [&]() {
        alu::swap(r.f, r.h);
        return 8;
    };

    // SWAP L
    isa.at(0x135) = [&]() {
        alu::swap(r.f, r.l);
        return 8;
    };

    // SWAP (HL)
    isa.at(0x136) = [&]() {
        u8 v = read8(r.hl);
        alu::swap(r.f, v);
        write8(r.hl, v);
        return 16;
    };

    // SWAP A
    isa.at(0x137) = [&]() {
        alu::swap(r.f, r.a);
        return 8;
    };


    // SRL B
    isa.at(0x138) = [&]() {
        alu::srl(r.f, r.b);
        return 8;
    };

    // SRL C
    isa.at(0x139) = [&]() {
        alu::srl(r.f, r.c);
        return 8;
    };

    // SRL D
    isa.at(0x13a) = [&]() {
        alu::srl(r.f, r.d);
        return 8;
    };

    // SRL E
    isa.at(0x13b) = [&]() {
        alu::srl(r.f, r.e);
        return 8;
    };

    // SRL H
    isa.at(0x13c) = [&]() {
        alu::srl(r.f, r.h);
        return 8;
    };

    // SRL L
    isa.at(0x13d) = [&]() {
        alu::srl(r.f, r.l);
        return 8;
    };

    // SRL (HL)
    isa.at(0x13e) = [&]() {
        u8 v = read8(r.hl);
        alu::srl(r.f, v);
        write8(r.hl, v);
        return 16;
    };

    // SRL A
    isa.at(0x13f) = [&]() {
        alu::srl(r.f, r.a);
        return 8;
    };

    {
        u16 opcode = 0x140;

        // RES [0x140~0x17F]
        assert(opcode == 0x140);

        for (u8 i = 0; i < 8; i ++) {
            isa.at(opcode++) = [this,i]() {
                alu::bit(r.f, r.b, i);
                return 8;
            };

            isa.at(opcode++) = [this,i]() {
                alu::bit(r.f, r.c, i);
                return 8;
            };

            isa.at(opcode++) = [this,i]() {
                alu::bit(r.f, r.d, i);
                return 8;
            };

            isa.at(opcode++) = [this,i]() {
                alu::bit(r.f, r.e, i);
                return 8;
            };

            isa.at(opcode++) = [this,i]() {
                alu::bit(r.f, r.h, i);
                return 8;
            };

            isa.at(opcode++) = [this,i]() {
                alu::bit(r.f, r.l, i);
                return 8;
            };

            isa.at(opcode++) = [this,i]() {
                u8 v = read8(r.hl);
                alu::bit(r.f, v, i);
                write8(r.hl, v);
                return 16;
            };

            isa.at(opcode++) = [this,i]() {
                alu::bit(r.f, r.a, i);
                return 8;
            };
        }

        // RES [0x180~0x1BF]
        assert(opcode == 0x180);

        for (u8 i = 0; i < 8; i ++) {
            isa.at(opcode++) = [this,i]() {
                alu::res(r.f, r.b, i);
                return 8;
            };

            isa.at(opcode++) = [this,i]() {
                alu::res(r.f, r.c, i);
                return 8;
            };

            isa.at(opcode++) = [this,i]() {
                alu::res(r.f, r.d, i);
                return 8;
            };

            isa.at(opcode++) = [this,i]() {
                alu::res(r.f, r.e, i);
                return 8;
            };

            isa.at(opcode++) = [this,i]() {
                alu::res(r.f, r.h, i);
                return 8;
            };

            isa.at(opcode++) = [this,i]() {
                alu::res(r.f, r.l, i);
                return 8;
            };

            isa.at(opcode++) = [this,i]() {
                u8 v = read8(r.hl);
                alu::res(r.f, v, i);
                write8(r.hl, v);
                return 16;
            };

            isa.at(opcode++) = [this,i]() {
                alu::res(r.f, r.a, i);
                return 8;
            };
        }

        // SET [0x1C0~0x1FF]
        assert(opcode == 0x1C0);

        for (u8 i = 0; i < 8; i ++) {
            isa.at(opcode++) = [this,i]() {
                alu::set(r.f, r.b, i);
                return 8;
            };

            isa.at(opcode++) = [this,i]() {
                alu::set(r.f, r.c, i);
                return 8;
            };

            isa.at(opcode++) = [this,i]() {
                alu::set(r.f, r.d, i);
                return 8;
            };

            isa.at(opcode++) = [this,i]() {
                alu::set(r.f, r.e, i);
                return 8;
            };

            isa.at(opcode++) = [this,i]() {
                alu::set(r.f, r.h, i);
                return 8;
            };

            isa.at(opcode++) = [this,i]() {
                alu::set(r.f, r.l, i);
                return 8;
            };

            isa.at(opcode++) = [this,i]() {
                u8 v = read8(r.hl);
                alu::set(r.f, v, i);
                write8(r.hl, v);
                return 16;
            };

            isa.at(opcode++) = [this,i]() {
                alu::set(r.f, r.a, i);
                return 8;
            };
        }

        assert(opcode == 0x200);
    }

}
