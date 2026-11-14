typedef struct SideEntryC {
    char data[0x24];
} SideEntryC;

typedef struct SideEntryD {
    char data[0x118];
} SideEntryD;

typedef struct SideReady {
    char pad_00[0x8];
    int state;
    char pad_0C[0xA0];
    SideEntryC c[2];
    SideEntryD d[2];
} SideReady;

extern "C" {

extern int func_08A020A4(SideEntryC *entry);
extern void func_08A17994(SideEntryD *entry);

int func_08A1A16C(SideReady *self)
{
    int all = 1;
    for (int i = 0; i < 2; i++) {
        if (func_08A020A4(&self->c[i]) == 0) {
            all = 0;
            break;
        }
    }
    if (all == 1) {
        for (int i = 0; i < 2; i++) {
            func_08A17994(&self->d[i]);
        }
        self->state = 5;
    }
    return 0;
}

}
