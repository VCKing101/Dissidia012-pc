extern "C" {
/* Index of the slot holding the current resource, or -1. */
typedef struct Res0886B1DC {
    int field_00;
    int field_04;
    int id;
} Res0886B1DC;

typedef struct Slot0886B1DC {
    int field_00;
    int field_04;
    int field_08;
    int id;
} Slot0886B1DC;

typedef struct Owner0886B1DC {
    Slot0886B1DC slots[8];
    int field_80;
    Res0886B1DC *res;
    int field_88;
    signed char count;
} Owner0886B1DC;

int func_0886B1DC(Owner0886B1DC *o)
{
    for (int i = 0; i < o->count; i++) {
        if (o->slots[i].id == o->res->id) {
            return i;
        }
    }
    return -1;
}
}
