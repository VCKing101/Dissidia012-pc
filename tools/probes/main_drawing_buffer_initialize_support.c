typedef struct MainDrawingBufferPrefix {
    unsigned char unknown_0000[4];
    unsigned int field_0004;
    void *field_0008;
    unsigned int field_000C;
} MainDrawingBufferPrefix;

extern void *func_0885C6CC(void *, unsigned int, int, unsigned int,
                          unsigned char, unsigned char);
extern void *D_089D6C88;
extern void func_088A18B0(void *, int, int);
extern unsigned int D_09B69878[2];

void func_088D52AC(void *object, void *allocator, unsigned int size) {
    MainDrawingBufferPrefix *self = object;
    self->field_0004 = 0;
    self->field_0008 = func_0885C6CC(allocator,size,0x80,0,1,0);
    self->field_000C = 0;
    func_088A18B0(D_089D6C88,100,0);
    D_09B69878[0] = 0;
    D_09B69878[1] = 0;
}
