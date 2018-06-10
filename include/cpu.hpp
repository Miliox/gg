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
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>


class Regs {
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

    Regs() : af(0), bc(0), de(0), hl(0), pc(0), sp(0), ie(0) {}

    std::string str() {
        std::stringstream ss;

        ss << std::hex;
        ss << "af:" << std::setw(4) << std::setfill('0') << af << " ";
        ss << "bc:" << std::setw(4) << std::setfill('0') << bc << " ";
        ss << "de:" << std::setw(4) << std::setfill('0') << de << " ";
        ss << "hl:" << std::setw(4) << std::setfill('0') << hl << " ";
        ss << "sp:" << std::setw(4) << std::setfill('0') << sp << " ";
        ss << "pc:" << std::setw(4) << std::setfill('0') << pc << " ";
        ss << "ie:" << static_cast<uint16_t>(ie) << " ";

        ss << ((f & 0x80) ? "z" : "-");
        ss << ((f & 0x40) ? "n" : "-");
        ss << ((f & 0x20) ? "h" : "-");
        ss << ((f & 0x10) ? "c" : "-");;

        return ss.str();
    }
};

class CPU {
public:
    CPU();

    Regs dump();
    void restore(const Regs& regs);

    // weak reference for mmu read/write
    std::function<u8(u16)>       read8;
    std::function<void(u16,u8&)> write8;

private:
    Regs r;

    // using a table of lambdas to not have to define 512 functions
    std::vector<std::function<u8()>> isa;

    u8   next8();
    u16  next16();
    u16  read16( u16 addr);
    void write16(u16 addr, u16 val);

    // zero page read/write
    u8   zread8(  u8 addr);
    u16  zread16( u8 addr);
    void zwrite8( u8 addr, u8 val);
    void zwrite16(u8 addr, u16 val);

    void call(u16 addr);
    void rst( u16 addr);
    void ret();

    void push(u16& reg);
    void pop( u16& reg);
};

#endif /* !CPU_HPP */
