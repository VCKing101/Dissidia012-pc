#include "battle/runtime_helpers.h"

extern void func_089E132C(void *);
extern int func_089E0DC0(unsigned char *);
extern void func_089E2BB4(void *);
extern void func_089E2418(void *);
extern void func_089E3BF0(void *);
extern int func_089E752C(void *);
extern int func_089E6FBC(void *);
extern int func_089E91D0(void *);

int func_089E0904(unsigned char *object)
{
    if (!func_089E0B9C(object)) return -1;
    {
        int i = 0;
        unsigned char *entry = object + 0x18;
        do {
            func_089E132C(entry);
            ++i;
            entry += 0x1C;
        } while (i < 4);
    }
    func_089E0DC0(object + 0x88);
    return 0;
}

int func_089E1FD0(unsigned char *object)
{
    if (!object[0]) return -1;
    {
        int i = 0;
        unsigned char *entry_a = object + 0xC;
        unsigned char *entry_b = object + 0x44;
        do {
            func_089E2BB4(entry_a);
            func_089E2418(entry_b);
            ++i;
            entry_a += 0x1C;
            entry_b += 0x68;
        } while (i < 2);
    }
    return 0;
}

int func_089E3618(unsigned char *object)
{
    if (!object[0]) return -1;
    {
        int i = 0;
        unsigned char *entry = object + 8;
        do {
            func_089E3BF0(entry);
            ++i;
            entry += 0xD8;
        } while (i < 2);
    }
    return 0;
}

int func_089E7318(unsigned char *object)
{
    if (!object[0]) return -1;
    {
        int i = 0;
        unsigned char *entry = object + 4;
        do {
            if (func_089E752C(entry)) return -2;
            ++i;
            entry += 0x20;
        } while (i < 2);
    }
    return 0;
}

int func_089E7D60(unsigned char *object)
{
    if (!object[0]) return -1;
    if (func_089DE360(object + 4)) return -2;
    func_089E6FBC(object + 0x858);
    {
        int i;
        unsigned char *entry_a = object + 0x650;
        i = 0;
        unsigned char *entry_b = object + 0x808;
        do {
            if (func_089E5598(entry_a)) return -3;
            if (func_089E91D0(entry_b)) return -4;
            ++i;
            entry_a += 0xDC;
            entry_b += 0x28;
        } while (i < 2);
    }
    return 0;
}

int func_089E7F2C(unsigned char *object)
{
    if (!object[0]) return -1;
    {
        int i = 0;
        unsigned char *flags = object + 0x644;
        unsigned char *entry = object + 0x650;
        do {
            if (!flags[i]) func_089E5660(entry);
            ++i;
            entry += 0xDC;
        } while (i < 2);
    }
    return 0;
}
