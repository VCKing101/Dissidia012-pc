/* Reports a versus side's slot id and whether its unit is of kind 2, or -1s
   when the side is empty. */
typedef struct VersusSide {
    signed char id;
    char pad_001[0xF3C - 1];
    short unit;
    char pad_F3E[0x1060 - 0xF3E];
} VersusSide;

typedef struct VersusState {
    VersusSide sides[2];
} VersusState;

typedef struct UnitInfo {
    char pad_00[3];
    unsigned char kind;
} UnitInfo;

extern VersusState D_09AFB8A0;

extern UnitInfo *func_088BBD8C(int id);

void func_08889C38(void *self, int side, int *out)
{
    int none = -1;
    VersusSide *s;
    signed char id;
    int special;

    out[0] = none;
    out[1] = none;
    s = (VersusSide *)((char *)&D_09AFB8A0 + side * 0x1060);
    id = s->id;
    special = 0;
    if (s->unit != none && func_088BBD8C(s->unit)->kind == 2) {
        special = 1;
    }
    if (id != none) {
        out[0] = id;
        out[1] = special;
    }
}
