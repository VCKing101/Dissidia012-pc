/* Redraws the page layout (alternate script when set); otherwise also pulses the two cursors. */
typedef struct DrawPage2 {
    char pad_000[0x41C];
    char field_41C[0x48C];
    char field_8A8[0x98];
    void *field_940;
    char pad_944[0x4];
    void *field_948;
} DrawPage2;

typedef struct DrawOwner2 {
    char pad_000[0x140];
    DrawPage2 *field_140;
} DrawOwner2;

extern char D_08B00646[];
extern char D_08B005D6[];
extern void func_08AF9ADC(DrawOwner2 *self);
extern int func_08AF96B0(void);
extern int func_08AF9FD4(DrawOwner2 *self);
extern void func_088D44EC(void *layout, void *icons, void *script);
extern void func_0887B848(void *effect, int alpha, int a, int b);

void func_08AF1BDC(DrawOwner2 *self)
{
    void *layout;
    void *icons;
    int alternate;
    func_08AF9ADC(self);
    {
        alternate = func_08AF96B0();
        layout = self->field_140->field_41C;
        icons = self->field_140->field_8A8;
        if (alternate != 0) {
            func_088D44EC(layout, icons, D_08B00646);
        } else {
            func_088D44EC(layout, icons, D_08B005D6);
            if (func_08AF9FD4(self) != 0) {
                func_0887B848(self->field_140->field_940, 0xFF, 0x20, 0x20);
                func_0887B848(self->field_140->field_948, 0xFF, 0x20, 0x20);
            }
        }
    }
}
