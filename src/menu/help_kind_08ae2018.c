/* Shows the help lines for `index`, or stops both help effects for -1. */
typedef struct HelpKindState {
    char pad_000[0x58C];
    void *field_58C;
    void *field_590;
} HelpKindState;

typedef struct HelpKindPanel {
    char pad_000[0x850];
    HelpKindState *field_850;
} HelpKindPanel;

extern short D_08AE45F2[];
extern void func_08AE1044(HelpKindPanel *self, int line);
extern void func_08AE0C94(HelpKindPanel *self, int line, int text, int arg);
extern void func_0887B944(void *effect);

void func_08AE2018(HelpKindPanel *self, int index)
{
    if (index != -1) {
        short *text;
        func_08AE1044(self, 0xF);
        text = &D_08AE45F2[index];
        func_08AE0C94(self, 0x10, *text, -1);
        func_08AE0C94(self, 0xF, *text, -1);
    } else {
        func_0887B944(self->field_850->field_58C);
        func_0887B944(self->field_850->field_590);
    }
}
