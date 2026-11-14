typedef struct Effect0887D68C {
    unsigned char flags;
    unsigned char unknown_01;
    unsigned char timer;
    unsigned char unknown_03[0x1E5];
    unsigned char field_1E8[4];
} Effect0887D68C;

void func_0887E7D0(void *p);

void func_0887D68C(void *self, Effect0887D68C *effect) {
    if (effect->timer != 0) {
        effect->timer--;
        if (effect->timer == 0) {
            func_0887E7D0(effect->field_1E8);
            effect->flags &= ~1;
        }
    }
}
