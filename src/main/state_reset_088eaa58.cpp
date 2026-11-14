extern "C" {
typedef struct Pair { int a; int b; } Pair;
typedef struct Slot { int value; unsigned char on; unsigned char held; char pad[2]; } Slot;
typedef struct State {
    Pair pairs[2];
    int field_10; int field_14; int field_18; int field_1C; int field_20; int field_24; int field_28;
    Slot slots[8];
    int field_6C; int field_70; int field_74; int field_78; unsigned char field_7C;
} State;

void func_088EAA58(State *s, unsigned char keep)
{
    s->field_20 = 0;
    for (unsigned int i = 0; i < 2; i++) {
        if (!keep) {
            s->pairs[i].a = 0;
        }
        s->pairs[i].b = 0;
    }
    s->field_10 = 0;
    if (!keep) {
        s->field_14 = 0;
        s->field_18 = 0;
        s->field_24 = 0;
        s->field_28 = -1;
        s->field_6C = 0;
        s->field_70 = 0;
    }
    s->field_1C = 0;
    for (unsigned int i = 0; i < 8; i++) {
        if (!keep) {
            s->slots[i].value = 0;
        }
        s->slots[i].on = 1;
        s->slots[i].held = 0;
    }
    s->field_74 = 0;
    s->field_78 = 0;
    s->field_7C = 0;
}
}
