typedef struct Agent4Pair {
    int field_00;
    int field_04;
} Agent4Pair;

typedef struct Agent4Valued {
    unsigned char unknown_0000[0x6C];
    int value;
} Agent4Valued;

extern Agent4Pair D_089D2800;

int func_0880844C(Agent4Valued *self) {
    return self == 0 ? D_089D2800.field_00 : self->value;
}
