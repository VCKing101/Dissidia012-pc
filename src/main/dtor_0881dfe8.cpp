extern "C" void func_0881E200(int s);
extern "C" void func_08992F04(int s);
extern "C" char D_089C7548[];
extern "C" char D_089C7558[];
extern "C" char D_089C7568[];
extern "C" char D_089C7578[];
extern "C" char D_089C7588[];
extern "C" char D_089C7598[];
extern "C" char D_089C75A8[];
extern "C" char D_089C75B8[];
extern "C" char D_089C75C8[];
extern "C" char D_089C75D8[];
extern "C" char D_089C75E8[];
extern "C" char D_089C75F8[];
extern "C" char D_089C7608[];
extern "C" char D_089C7618[];

struct Dtor102 { static void run(int self, unsigned flags) {
    if (self != 0) {
        *(char **)(self + 0xFC) = D_089C7548;
        func_0881E200(self);
        if (self + 0xF0 != 0) {
            *(char **)(self + 0xF8) = D_089C7618;
        }
        if (self + 0xE4 != 0) {
            *(char **)(self + 0xEC) = D_089C7618;
        }
        if (self + 0xD8 != 0) {
            *(char **)(self + 0xE0) = D_089C7608;
        }
        if (self + 0xCC != 0) {
            *(char **)(self + 0xD4) = D_089C75F8;
        }
        if (self + 0xC0 != 0) {
            *(char **)(self + 0xC8) = D_089C75F8;
        }
        if (self + 0xB4 != 0) {
            *(char **)(self + 0xBC) = D_089C75E8;
        }
        if (self + 0xA8 != 0) {
            *(char **)(self + 0xB0) = D_089C75D8;
        }
        if (self + 0x94 != 0) {
            *(char **)(self + 0xA4) = D_089C75C8;
        }
        if (self + 0x88 != 0) {
            *(char **)(self + 0x90) = D_089C75B8;
        }
        if (self + 0x7C != 0) {
            *(char **)(self + 0x84) = D_089C75A8;
        }
        if (self + 0x70 != 0) {
            *(char **)(self + 0x78) = D_089C75A8;
        }
        if (self + 0x64 != 0) {
            *(char **)(self + 0x6C) = D_089C75A8;
        }
        if (self + 0x58 != 0) {
            *(char **)(self + 0x60) = D_089C75A8;
        }
        if (self + 0x4C != 0) {
            *(char **)(self + 0x54) = D_089C7598;
        }
        if (self + 0x40 != 0) {
            *(char **)(self + 0x48) = D_089C7588;
        }
        if (self + 0x2C != 0) {
            *(char **)(self + 0x3C) = D_089C7578;
        }
        if (self + 0x18 != 0) {
            *(char **)(self + 0x28) = D_089C7568;
        }
        if (self + 0xC != 0) {
            *(char **)(self + 0x14) = D_089C7558;
        }
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
} };

extern "C" void func_0881DFE8(int self, unsigned flags) { Dtor102::run(self, flags); }
