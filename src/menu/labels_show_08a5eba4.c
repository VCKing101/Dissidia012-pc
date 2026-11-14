/* Hides the five state labels, then shows the one for the current state (state 0 has two variants). */
typedef struct StateLabels {
    char pad_000[0x68];
    void *field_68;
    void *field_6C;
    void *field_70;
    void *field_74;
    void *field_78;
    char pad_07C[0xFC - 0x7C];
    int field_FC;
    char pad_100[0x128 - 0x100];
    int field_128;
} StateLabels;

extern void func_088D9CFC(void *label);
extern void func_088D9CD8(void *label);

void func_08A5EBA4(StateLabels *self)
{
    func_088D9CFC(self->field_68);
    func_088D9CFC(self->field_6C);
    func_088D9CFC(self->field_70);
    func_088D9CFC(self->field_74);
    func_088D9CFC(self->field_78);
    switch (self->field_FC) {
    case 0:
        if (self->field_128 == 0) {
            func_088D9CD8(self->field_68);
        } else {
            func_088D9CD8(self->field_6C);
        }
        break;
    case 1:
        func_088D9CD8(self->field_70);
        break;
    case 2:
        func_088D9CD8(self->field_74);
        break;
    case 3:
        func_088D9CD8(self->field_78);
        break;
    }
}
