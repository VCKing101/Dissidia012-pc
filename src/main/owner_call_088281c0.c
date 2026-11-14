typedef struct Agent4Unit281C0 {
    unsigned char unknown_0000[0x370];
    float field_0370;
    unsigned char unknown_0374[0x24];
    int handle;
    unsigned char unknown_039C[0x14];
    float field_03B0;
    float field_03B4;
    unsigned char unknown_03B8[0x198];
    struct Agent4Unit281C0 *partner;
} Agent4Unit281C0;

void func_0880EA20(int handle, int arg, Agent4Unit281C0 *self);

void func_088281C0(Agent4Unit281C0 *self, int arg) {
    func_0880EA20(self->handle, arg, self);
}
