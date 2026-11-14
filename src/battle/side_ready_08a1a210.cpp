typedef struct SideEntryA {
    char data[0x28];
} SideEntryA;

typedef struct SideEntryC {
    char data[0x24];
} SideEntryC;

typedef struct SideEntryD {
    char data[0x118];
} SideEntryD;

typedef struct SideReady {
    char pad_00[0x8];
    int state;
    char pad_0C[0x8];
    SideEntryA a[2];
    char pad_64[0x48];
    SideEntryC c[2];
    SideEntryD d[2];
} SideReady;

extern "C" {

extern int func_08A179D8(SideEntryD *entry);
extern void func_08A01C74(SideEntryA *entry, int mode);

int func_08A1A210(SideReady *self)
{
    int all = 1;
    for (int i = 0; i < 2; i++) {
        if (func_08A179D8(&self->d[i]) == 0) {
            all = 0;
            break;
        }
    }
    if (all == 1) {
        for (int i = 0; i < 2; i++) {
            func_08A01C74(&self->a[i], 3);
        }
        self->state = 6;
    }
    return 0;
}

}
