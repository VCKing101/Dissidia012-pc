#include "main/agent2_types.h"

typedef void *(*Agent2Alloc)(unsigned int);
typedef void (*Agent2Free)(void *);

extern const char D_089C93E0[];
extern const char D_089C93F4[];
extern int D_09B26AB0;

extern void *func_0888204C(int);
extern void *func_0885C40C(void *, int, const char *, int);
extern int func_089C5A60(void);
extern int func_089C5A98(void);
extern void func_0899B6B0(Agent2Alloc, Agent2Free);
extern void func_089BA5BC(Agent2Alloc, Agent2Free, int, int);
extern void func_0899B6F4(Agent2Alloc, Agent2Free);
extern void func_089BA5F8(Agent2Alloc, Agent2Free, int, int);
extern void func_08940360(Agent2Alloc, Agent2Free);
extern void func_089AB464(Agent2Alloc, Agent2Free, int, int);
extern void func_089403A0(Agent2Alloc, Agent2Free);
extern void func_089AB4A0(Agent2Alloc, Agent2Free, int, int);
extern void func_0899B728(Agent2Alloc, Agent2Free);
extern void func_089BA634(Agent2Alloc, Agent2Free, int, int);
extern void func_089403D8(Agent2Alloc, Agent2Free);
extern void func_089AB4DC(Agent2Alloc, Agent2Free, int, int);
extern void *func_088664C4(unsigned int);
extern void func_088664FC(void *);
extern void *func_08866528(unsigned int);
extern void func_08866560(void *);

void func_088667DC(Agent2GlobalState *self, int base, int extra) {
    int size = base + extra;
    void *memory = func_0888204C(size);
    self->field_0000[2] = (int)memory;
    self->field_0710 = func_0885C40C(memory, size, D_089C93E0, 0x40);
    func_0899B6B0(func_088664C4, func_088664FC);
    func_089BA5BC(func_088664C4, func_088664FC, 0x10, 0);
    func_0899B6F4(func_088664C4, func_088664FC);
    func_089BA5F8(func_088664C4, func_088664FC, 0x10, 0);
    func_08940360(func_088664C4, func_088664FC);
    func_089AB464(func_088664C4, func_088664FC, 0x10, 0);
    func_089403A0(func_088664C4, func_088664FC);
    func_089AB4A0(func_088664C4, func_088664FC, 0x10, 0);
}

void func_088668C0(Agent2GlobalState *self) {
    int size = func_089C5A60();
    int start = func_089C5A98() + D_09B26AB0;
    self->field_0714 = func_0885C40C((void *)start, size - D_09B26AB0,
                                     D_089C93F4, 0x40);
    func_0899B728(func_08866528, func_08866560);
    func_089BA634(func_08866528, func_08866560, 0x10, 0);
    func_089403D8(func_08866528, func_08866560);
    func_089AB4DC(func_08866528, func_08866560, 0x10, 0);
}
