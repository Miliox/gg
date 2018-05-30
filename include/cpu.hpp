/*
 * cpu.h
 * Copyright (C) 2018 Emiliano Firmino <emiliano.firmino@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef CPU_HPP
#define CPU_HPP

#include "common.h"

#include <functional>
#include <vector>

class Registers {
public:
    struct {
        union {
            struct {
                u8 f; // flag
                u8 a; // accumulator
            };
            u16 af;
        };
    };

    struct {
        union {
            struct {
                u8 c;
                u8 b;
            };
            u16 bc;
        };
    };

    struct {
        union {
            struct {
                u8 e;
                u8 d;
            };
            u16 de;
        };
    };

    struct {
        union {
            struct {
                u8 l;
                u8 h;
            };
            u16 hl;
        };
    };

    u16 pc;  // program counter
    u16 sp;  // stack pointer

    u8 ie;  // interrupt enable

    Registers() : af(0), bc(0), de(0), hl(0), pc(0), sp(0), ie(0) {}
};

class CPU {
public:
    Registers r;

    // using a table of lambdas to not have to define 512 functions
    std::vector<std::function<u8()>> isa;

    CPU();
private:
    u8  read8();
    u16 read16();

    u8  read8(u16 addr);
    u16 read16(u16 addr);

    void write8(u16 addr, u8 val);
    void write16(u16 addr, u16 val);

    u8 mread8(u8 addr);
    u16 mread16(u8 addr);

    void mwrite8(u8 addr, u8 val);
    void mwrite16(u8 addr, u16 val);

    void call(u16 addr);
    void rst( u16 addr);

    void push(u16& reg);
    void pop( u16& reg);
};

#endif /* !CPU_HPP */
