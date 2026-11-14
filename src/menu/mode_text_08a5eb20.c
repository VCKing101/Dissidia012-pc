/* Shows the help text for the current mode (0x82-0x86, or none). */
typedef struct ModeTextPanel {
    char pad_000[0xA0];
    char field_A0[0x5C];
    int field_FC;
    char pad_100[0x128 - 0x100];
    int field_128;
} ModeTextPanel;

extern void func_08A603C0(void *help, int text);

void func_08A5EB20(ModeTextPanel *self)
{
    int text = -1;
    switch (self->field_FC) {
    case 0:
        text = self->field_128 == 0 ? 0x82 : 0x83;
        break;
    case 1:
        text = 0x84;
        break;
    case 2:
        text = 0x85;
        break;
    case 3:
        text = 0x86;
        break;
    }
    func_08A603C0(self->field_A0, text);
}
