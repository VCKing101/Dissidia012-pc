/* Waits for the screen's layer to finish closing (state 12), then switches the screen; state 13. */
typedef struct LayerSlot {
    void *field_00;
    void *field_04;
    int field_08;
    int field_0C;
} LayerSlot;

typedef struct LayerOwner {
    char pad_000[0x15C];
    unsigned char field_15C;
} LayerOwner;

typedef struct LayerScreen {
    char pad_000[0x13C];
    LayerOwner *field_13C;
    char pad_140[0xC];
    LayerSlot field_14C;
    char pad_15C[0x24];
    char field_180[4];
} LayerScreen;

extern int func_089EE4F4(void *layer);
extern void func_089EE438(void *layer);
extern int func_088D4C5C(void);
extern void func_089E9348(LayerScreen *self, int screen, void (*next)(void));
extern void func_089E9420(void);
extern void func_088D54DC(void *fade);
extern void func_089ECB40(LayerOwner *owner);

void func_089E8A88(LayerScreen *self)
{
    LayerSlot *slot = &self->field_14C;
    if (func_089EE4F4(slot->field_00) == 1) {
        func_089EE438(slot->field_00);
        slot->field_08 = slot->field_0C;
        if (slot->field_0C == 12) {
            func_089E9348(self, func_088D4C5C(), func_089E9420);
            func_088D54DC(self->field_180);
            self->field_13C->field_15C = 0;
            func_089ECB40(self->field_13C);
            slot->field_0C = 13;
        }
    }
}
