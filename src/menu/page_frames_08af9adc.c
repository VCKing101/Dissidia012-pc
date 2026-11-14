/* Redraws the page layout and, with a value set, refreshes the listed icons. */
typedef struct FramesState {
    char pad_0000[0x41C];
    char field_41C[0x48C];
    void *field_8A8[(0x2C04 - 0x8A8) / 4];
    int field_2C04;
} FramesState;

typedef struct FramesOwner {
    char pad_000[0x140];
    FramesState *field_140;
} FramesOwner;

extern char D_08B0057E[];
extern unsigned short D_08B01274[];
extern char D_08B01270[];
extern void func_088D44EC(void *layout, void *icons, void *script);
extern void func_0887B828(void *icon, void *frames);

void func_08AF9ADC(FramesOwner *self)
{
    func_088D44EC(self->field_140->field_41C, self->field_140->field_8A8, D_08B0057E);
    if (self->field_140->field_2C04 != 0) {
        unsigned short *id = D_08B01274;
        for (;;) {
            if (*id == 0xFFFF) {
                break;
            }
            func_0887B828(self->field_140->field_8A8[*id], D_08B01270);
            id++;
        }
    }
}
