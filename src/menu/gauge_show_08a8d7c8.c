/* Shows the gauge for value 0-100 (the full mark only at 100) or hides it for -1, then updates it. */
typedef struct GaugeParts {
    char pad_000[0x198];
    void *field_198;
    char pad_19C[0xC];
    void *field_1A8;
} GaugeParts;

typedef struct GaugeOwner {
    char pad_00[0x8];
    GaugeParts *field_08;
} GaugeOwner;

extern void func_088D9CFC(void *label);
extern void func_088D9CD8(void *label);
extern void func_08A8D70C(GaugeOwner *self, int gauge, int value, int shown);

void func_08A8D7C8(GaugeOwner *self, int value)
{
    int shown;
    if (value >= 101) {
        return;
    }
    if (value == -1) {
        func_088D9CFC(self->field_08->field_198);
        func_088D9CFC(self->field_08->field_1A8);
        shown = 0;
    } else {
        func_088D9CD8(self->field_08->field_198);
        if (value == 100) {
            func_088D9CD8(self->field_08->field_1A8);
        }
        shown = 1;
    }
    func_08A8D70C(self, 1, value, shown);
}
