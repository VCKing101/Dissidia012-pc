typedef struct State0888B6DC {
    unsigned char unknown_00[0x18];
    unsigned char active;
} State0888B6DC;

extern State0888B6DC *D_089D6A00;
void func_088876B0(State0888B6DC *state);

void func_0888B6DC(void) {
    if (D_089D6A00->active != 0) {
        func_088876B0(D_089D6A00);
    }
}
