#include "main/agent2_types.h"

/* Heap object created by 0x0885C40C; its SNC vtable pointer sits at +0x24. */
class Agent2Heap {
public:
    unsigned char unknown_0000[0x24];
    virtual void slot_008();
    virtual void slot_010();
    virtual void slot_018();
};

typedef void *(*Agent2Alloc)(unsigned int);
typedef void (*Agent2Free)(void *);

extern "C" {
void func_0885C488(void *);
void func_0888208C(int);
void func_0899B6B0(Agent2Alloc, Agent2Free);
void func_089BA5BC(Agent2Alloc, Agent2Free, int, int);
void func_0899B6F4(Agent2Alloc, Agent2Free);
void func_089BA5F8(Agent2Alloc, Agent2Free, int, int);
void func_08940360(Agent2Alloc, Agent2Free);
void func_089AB464(Agent2Alloc, Agent2Free, int, int);
void func_089403A0(Agent2Alloc, Agent2Free);
void func_089AB4A0(Agent2Alloc, Agent2Free, int, int);
void func_0899B728(Agent2Alloc, Agent2Free);
void func_089BA634(Agent2Alloc, Agent2Free, int, int);
void func_089403D8(Agent2Alloc, Agent2Free);
void func_089AB4DC(Agent2Alloc, Agent2Free, int, int);

void func_08866968(Agent2GlobalState *self) {
    ((Agent2Heap *)self->field_0710)->slot_018();
    func_0885C488(self->field_0710);
    self->field_0710 = 0;
    func_0888208C(self->field_0000[2]);
    self->field_0000[2] = 0;
    func_0899B6B0(0, 0);
    func_089BA5BC(0, 0, 0x10, 0);
    func_0899B6F4(0, 0);
    func_089BA5F8(0, 0, 0x10, 0);
    func_08940360(0, 0);
    func_089AB464(0, 0, 0x10, 0);
    func_089403A0(0, 0);
    func_089AB4A0(0, 0, 0x10, 0);
}

void func_08866A3C(Agent2GlobalState *self) {
    func_0885C488(self->field_0714);
    self->field_0714 = 0;
    func_0899B728(0, 0);
    func_089BA634(0, 0, 0x10, 0);
    func_089403D8(0, 0);
    func_089AB4DC(0, 0, 0x10, 0);
}
}
