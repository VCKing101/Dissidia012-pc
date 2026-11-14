typedef struct Owner08896558 {
    unsigned char unknown_00[0xC];
    int start;
    int end;
    unsigned char anim[0x24];
    int state;
} Owner08896558;

void func_0889F830(void *anim, int a);
void func_0889F704(void *anim);
void func_0889F6A0(void *anim, int mode, int start, int end);
void func_088967B4(void);
void func_0889F750(float rate, void *anim, int a, void (*done)(void), void *ctx);

void func_08896558(Owner08896558 *self) {
    void *anim = self->anim;
    if (self->state == 1 || self->state == 2) {
        func_0889F830(anim, 0);
        func_0889F704(anim);
    }
    func_0889F6A0(anim, 3, self->start, self->end);
    func_0889F750(1.0f, anim, 1, func_088967B4, self);
    self->state = 3;
}
