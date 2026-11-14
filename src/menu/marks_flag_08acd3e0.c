typedef struct FlagMark {
    char pad_00[0x10];
    unsigned int field_10;
} FlagMark;

typedef struct FlagState {
    char pad_000[0x880];
    FlagMark *field_880[1];
} FlagState;

typedef struct FlagOwner {
    char pad_0000[0xDB30];
    FlagState *field_DB30;
} FlagOwner;

extern unsigned short D_08ADA77A[];

void func_08ACD3E0(FlagOwner *self)
{
    unsigned short *id = D_08ADA77A;
    for (;;) {
        FlagMark *mark;
        if (*id == 0xFFFF) {
            return;
        }
        mark = self->field_DB30->field_880[*id];
        if (mark != 0) {
            unsigned int flags = mark->field_10 | 0x800;
            mark->field_10 = flags;
        }
        id++;
    }
}
