typedef struct Step08818CB8 {
    unsigned char unknown_00[0x10];
    int state;
    void *child;
} Step08818CB8;

float func_08819294(Step08818CB8 *self);
void func_088198F4(Step08818CB8 *self, float t);
void func_088151A0(void *child);

int func_08818CB8(Step08818CB8 *self) {
    if (self->state == 2) {
        func_088198F4(self, func_08819294(self));
    }
    if (self->child != 0) {
        func_088151A0(self->child);
    }
    return 0;
}
