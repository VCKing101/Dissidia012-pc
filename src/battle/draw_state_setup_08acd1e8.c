typedef struct DrawPass {
    int pad_00;
    void *context;
} DrawPass;

extern void func_089C6080(void *context, int mode);
extern void func_089A0384(int state);
extern void func_089A0E60(int a, int b);
extern void func_089A0E20(int a, int b);
extern void func_089A0E8C(int mode, float value);
extern void func_089A0DB8(int a, int b, int c, int d);
extern void func_089A0FC8(void);
extern void func_089A0F04(int x, int width, int stride, int height, void *context);

/* Set the render state for the pass and bind its 512x480 target. */
void func_08ACD1E8(DrawPass *self)
{
    func_089C6080(self->context, 4);
    func_089A0384(9);
    func_089A0E60(1, 1);
    func_089A0E20(0, 0);
    func_089A0E8C(0, 0.0f);
    func_089A0DB8(1, 0, 0, 0);
    func_089A0FC8();
    func_089A0F04(0, 0x200, 0x200, 0x1E0, self->context);
}
