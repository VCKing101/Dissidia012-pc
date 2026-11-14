#include "battle/agent11_types.h"

typedef struct SideEntryA {
    char data[0x28];
} SideEntryA;

typedef struct SideEntryB {
    char data[0x24];
} SideEntryB;

typedef struct SideEntryC {
    char data[0x24];
} SideEntryC;

typedef struct SideEntryD {
    char data[0x118];
} SideEntryD;

class SideTask : public BattleTaskBase {
public:
    int field_04;
    char pad_08[0xC];
    SideEntryA a[2];
    SideEntryB b[2];
    SideEntryC c[2];
    SideEntryD d[2];
};

extern "C" {

extern void func_08A175E8(SideEntryD *entry);
extern void func_08A01E88(SideEntryC *entry);
extern void func_08A18E18(SideEntryB *entry);
extern void func_08A01844(SideEntryA *entry);

void func_08A19434(SideTask *self)
{
    for (int i = 0; i < 2; i++) {
        func_08A175E8(&self->d[i]);
        func_08A01E88(&self->c[i]);
        func_08A18E18(&self->b[i]);
        func_08A01844(&self->a[i]);
    }
    self->slot_010();
}

}
