typedef struct Timer08872540 {
    unsigned long long stamp;
    unsigned char unknown_08[0x3C];
    unsigned int running : 1;
} Timer08872540;

extern unsigned long long D_089C9A68;
void func_08871B40(Timer08872540 *self, unsigned long long period);
unsigned long long func_088726B0(Timer08872540 *self);

void func_08872540(Timer08872540 *self) {
    self->running = 0;
    func_08871B40(self, D_089C9A68);
    self->stamp = func_088726B0(self);
}
