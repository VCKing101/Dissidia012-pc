extern "C" {
typedef struct KindUnit {
    char pad_00[0xC];
    short kind;
} KindUnit;

typedef struct KindTable {
    unsigned char active;
    char pad_01[0x3];
    KindUnit *unit;
} KindTable;

extern int D_089E70E8[3];
extern int D_089E70F4[3][5];

/* Table value for the unit kind and slot: -1 inactive, -2 bad slot, -3 kind not listed. */
int func_089E7038(KindTable *self, unsigned int slot)
{
    int result;
    if (self->active == 0) {
        return -1;
    }
    if (slot >= 5) {
        return -2;
    }
    short kind = self->unit->kind;
    result = -1;
    for (int i = 0; i < 3; i++) {
        if (kind == D_089E70E8[i]) {
            result = D_089E70F4[i][slot];
            break;
        }
    }
    if (result < 0) {
        return -3;
    }
    return result;
}
}
