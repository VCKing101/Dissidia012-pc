#include "battle/runtime_helpers.h"

/* Partial pointer-field views; ownership beyond the observed cleanup calls
 * is unknown. Member tables are kept as preserved data symbols. */
typedef struct BattleResourcePrefix { unsigned char padding[4]; void *field_04; } BattleResourcePrefix;
typedef struct BattleResource64C { unsigned char padding[0x64C]; void *field_64C; } BattleResource64C;
extern unsigned char D_089E0CA8[], D_089E11F0[], D_089E38C0[], D_089E3920[];
extern unsigned char D_089E6E50[], D_089E7390[], D_089E73D8[], D_089E8228[];
extern unsigned char D_089E5270[], D_089E8FA0[], D_089E8F58[], D_089ED850[], D_089E70D8[];
extern void *D_089D7830;
extern void func_08992F04(void *);
extern void func_0899372C(void *, unsigned int, unsigned int, void *, int, int);
extern void func_0887A644(void *, int);
extern void func_089E0C6C(void *);
extern void func_089E0CF0(void *, int);
extern void func_089E9E68(void *, int);
extern void func_089E1D68(void *, int);
extern void func_089DE0A0(void *, int);
extern void func_089E32D0(void *);
extern void func_089E1268(void *);
extern void func_089E0D54(void *);
extern void func_089E39CC(void *);
extern void func_089E7450(void *);
extern void func_089E9024(void *);
extern void func_089E5314(void *);
extern void func_089DE1D8(void *);
extern void func_089E7AD0(void *);
extern void func_089ECFD0(void *);
extern void func_088D0A1C(void *, void *);
extern void func_088DA060(void *);

void func_089E07AC(unsigned char *object);
void func_089E3300(unsigned char *object);
void func_089E7200(unsigned char *object);
void func_089E7AFC(unsigned char *object);
void func_089ED01C(unsigned char *object);
void func_089E6EFC(unsigned char *object);
void func_089E6E98(unsigned char *object, int flags);
void func_089E6EF0(unsigned char *object);

void func_089E0708(unsigned char *object, int flags)
{
    if (object) {
        ((BattleTableAt94 *)object)->member_table = D_089E0CA8;
        func_089E07AC(object);
        func_089E0C6C(object);
        func_089E0CF0(object + 0x88, 2);
        func_0899372C(object + 0x18, 4, 0x1C, D_089E11F0, 0, 0);
        func_0887A644(object + 8, 2);
        if (flags & 1) func_08992F04(object);
    }
}

void func_089E3258(unsigned char *object, int flags)
{
    if (object) {
        ((BattleTableAt1C0 *)object)->member_table = D_089E38C0;
        func_089E3300(object);
        func_0899372C(object + 8, 2, 0xD8, D_089E3920, 0, 0);
        if (flags & 1) func_08992F04(object);
    }
}

void func_089E6928(unsigned char *object, int flags)
{
    if (object) {
        ((BattleTableAt11DC *)object)->member_table = D_089E6E50;
        func_089E69C0(object);
        func_089E9E68(object + 0x124, 2);
        func_089E1D68(object + 8, 2);
        if (flags & 1) func_08992F04(object);
    }
}

void func_089E7180(unsigned char *object, int flags)
{
    if (object) {
        ((BattleTableAt44 *)object)->member_table = D_089E7390;
        func_089E7200(object);
        func_0899372C(object + 4, 2, 0x20, D_089E73D8, 0, 0);
        if (flags & 1) func_08992F04(object);
    }
}

void func_089E7A20(unsigned char *object, int flags)
{
    if (object) {
        ((BattleTableAt864 *)object)->member_table = D_089E8228;
        func_089E7AFC(object);
        func_089E6E98(object + 0x858, 2);
        func_0899372C(object + 0x808, 2, 0x28, D_089E8FA0, 0, 0);
        func_0899372C(object + 0x650, 2, 0xDC, D_089E5270, 0, 0);
        func_089DE0A0(object + 4, 2);
        if (flags & 1) func_08992F04(object);
    }
}

void func_089E8EA8(unsigned char *object, int flags)
{
    if (object) {
        ((BattleTableAt4 *)object)->member_table = D_089E8F58;
        if (flags & 1) func_08992F04(object);
    }
}

void func_089ECF78(unsigned char *object, int flags)
{
    if (object) {
        ((BattleTableAt20 *)object)->member_table = D_089ED850;
        func_089ED01C(object);
        if (flags & 1) func_08992F04(object);
    }
}

void func_089E6E98(unsigned char *object, int flags)
{
    if (object) {
        ((BattleTableAt8 *)object)->member_table = D_089E70D8;
        func_089E6EFC(object);
        if (flags & 1) func_08992F04(object);
    }
}

void func_089E07AC(unsigned char *object)
{
    int i;
    unsigned char *entry;
    void *resource;
    func_089E0D54(object + 0x88);
    i = 0;
    entry = object + 0x18;
    do { func_089E1268(entry); ++i; entry += 0x1C; } while (i < 4);
    resource = ((BattleResourcePrefix *)object)->field_04;
    if (resource) func_088D0A1C(D_089D7830, resource);
    func_089E07A0(object);
}

void func_089E3300(unsigned char *object)
{
    int i;
    unsigned char *entry;
    i = 0;
    entry = object + 8;
    do { func_089E39CC(entry); ++i; entry += 0xD8; } while (i < 2);
    func_089E32D0(object);
}

void func_089E7200(unsigned char *object)
{
    int i;
    unsigned char *entry;
    i = 0;
    entry = object + 4;
    do { func_089E7450(entry); ++i; entry += 0x20; } while (i < 2);
    func_089E71F8(object);
}

void func_089E7AFC(unsigned char *object)
{
    int i;
    unsigned char *entry_a;
    unsigned char *entry_b;
    void *resource;
    i = 0;
    entry_a = object + 0x808;
    entry_b = object + 0x650;
    do {
        func_089E9024(entry_a);
        func_089E5314(entry_b);
        ++i; entry_a += 0x28; entry_b += 0xDC;
    } while (i < 2);
    func_089E6EFC(object + 0x858);
    resource = ((BattleResource64C *)object)->field_64C;
    if (resource) func_088D0A1C(D_089D7830, resource);
    func_089DE1D8(object + 4);
    func_089E7AD0(object);
}

void func_089ED01C(unsigned char *object)
{
    void *resource = ((BattleResourcePrefix *)object)->field_04;
    if (resource) func_088D0A1C(D_089D7830, resource);
    func_089ECFD0(object);
}

void func_089E0D48(unsigned char *object)
{
    ((BattleResetPrefix *)object)->field_00 = 0;
    ((BattleResetPrefix *)object)->field_04 = 0;
}

void func_089E6EF0(unsigned char *object)
{
    ((BattleResetPrefix *)object)->field_00 = 0;
    ((BattleResetPrefix *)object)->field_04 = 0;
}

void func_089E6EFC(unsigned char *object)
{
    void *resource = ((BattleResourcePrefix *)object)->field_04;
    if (resource) {
        func_088DA060(resource);
        ((BattleResourcePrefix *)object)->field_04 = 0;
    }
    func_089E6EF0(object);
}
