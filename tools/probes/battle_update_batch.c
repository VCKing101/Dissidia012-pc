#include "battle/runtime_helpers.h"

typedef struct BattleFlagSlots {
    unsigned char field_00;
    unsigned char padding_01[0x1B7];
    unsigned char field_1B8[3];
} BattleFlagSlots;
extern void func_089DEDB0(void *);
extern void func_089DEA48(void *);
extern void func_089DEB54(void *);
extern int func_0887AFB0(void *);
extern void func_089E132C(void *);
extern void func_089E381C(void *);
extern void func_089E3CDC(void *);
extern void func_089E4674(void *);
extern void func_089E4060(void *);
extern void func_089E4368(void *);
extern int func_089E56EC(void *);
extern int func_089E57C4(void *);
extern int func_089E5988(void *);
extern void func_089E1EC0(void *, void *);
extern void func_089E9F74(void *, void *, void *);
extern void func_089E6CE4(void *);
extern void func_089E2BB4(void *);
extern void func_089E2418(void *);
extern void func_089E3BF0(void *);
extern int func_089E752C(void *);
extern int func_089EA11C(void *);
extern void func_089EA448(void *);
extern void func_089EA450(void *);

unsigned char func_089E0B9C(unsigned char *object);
int func_089E0DC0(unsigned char *object);
int func_089E1FD0(unsigned char *object);
int func_089EA064(unsigned char *object);

int func_089DE360(unsigned char *object)
{
    if (!object[0]) return -1;
    func_089DEDB0(object);
    func_089DEA48(object);
    func_089DEB54(object);
    return 0;
}

int func_089E0904(unsigned char *object)
{
    int i;
    unsigned char *entry;
    if (func_089E0B9C(object)) {
        i = 0;
        entry = object + 0x18;
        do {
            func_089E132C(entry);
            ++i;
            entry += 0x1C;
        } while (i < 4);
        func_089E0DC0(object + 0x88);
        return 0;
    }
    return -1;
}

unsigned char func_089E0B9C(unsigned char *object)
{
    unsigned int result = 0;
    if (object[0] == 1 && func_0887AFB0(object + 8) == 1) result = 1;
    return (unsigned char)result;
}

int func_089E0DC0(unsigned char *object)
{
    if (!object[0]) return -1;
    return 0;
}

int func_089E1FD0(unsigned char *object)
{
    int i;
    unsigned char *entry_a;
    unsigned char *entry_b;
    if (object[0]) {
        i = 0;
        entry_a = object + 0xC;
        entry_b = object + 0x44;
        do {
            func_089E2BB4(entry_a);
            func_089E2418(entry_b);
            ++i;
            entry_a += 0x1C;
            entry_b += 0x68;
        } while (i < 2);
        return 0;
    }
    return -1;
}

int func_089E3618(unsigned char *object)
{
    int i;
    unsigned char *entry;
    if (object[0]) {
        i = 0;
        entry = object + 8;
        do {
            func_089E3BF0(entry);
            ++i;
            entry += 0xD8;
        } while (i < 2);
        return 0;
    }
    return -1;
}

int func_089E3768(unsigned char *object, unsigned int index)
{
    if (!object[0]) return -1;
    if (index >= 3) return -2;
    ((BattleFlagSlots *)object)->field_1B8[index] = 0;
    func_089E381C(object);
    return 0;
}

int func_089E37C0(unsigned char *object, unsigned int index)
{
    if (!object[0]) return -1;
    if (index >= 3) return -2;
    ((BattleFlagSlots *)object)->field_1B8[index] = 1;
    func_089E381C(object);
    return 0;
}

int func_089E3B0C(unsigned char *object)
{
    if (!object[0]) return -1;
    func_089E3CDC(object);
    func_089E4674(object + 8);
    func_089E4674(object + 0x3C);
    func_089E4060(object + 0x70);
    return 0;
}

int func_089E3C3C(unsigned char *object)
{
    if (!object[0]) return -1;
    func_089E4368(object + 0x70);
    return 0;
}

int func_089E5598(unsigned char *object)
{
    if (!object[0]) return -1;
    if (func_089E56EC(object)) return -2;
    if (func_089E57C4(object)) return -3;
    if (func_089E5988(object)) return -4;
    return 0;
}

int func_089E69F8(unsigned char *object, void *field_a, void *field_b)
{
    func_089E69C0(object);
    if (!field_a || !field_b) return -1;
    func_089E1EC0(object + 8, field_b);
    func_089E9F74(object + 0x124, field_a, field_b);
    object[0] = 1;
    return 0;
}

int func_089E6A74(unsigned char *object)
{
    if (!object[0]) return -1;
    func_089E6CE4(object);
    func_089E1FD0(object + 8);
    func_089EA064(object + 0x124);
    return 0;
}

int func_089E7318(unsigned char *object)
{
    int i;
    unsigned char *entry;
    if (object[0]) {
        i = 0;
        entry = object + 4;
        do {
            if (func_089E752C(entry)) return -2;
            ++i;
            entry += 0x20;
        } while (i < 2);
        return 0;
    }
    return -1;
}

int func_089EA064(unsigned char *object)
{
    if (!func_089EA11C(object)) return -1;
    func_089EA448(object);
    func_089EA450(object);
    return 0;
}
