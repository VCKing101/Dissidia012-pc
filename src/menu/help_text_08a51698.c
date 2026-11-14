/* Shows help text 0 or 10 for the panel's current mode. */
typedef struct HelpData {
    char pad_0000[0x6B1C];
    void *field_6B1C;
    char pad_6B20[0x7998 - 0x6B20];
    void *field_7998;
} HelpData;

typedef struct HelpOwner {
    char pad_00[0x58];
    HelpData *field_58;
} HelpOwner;

extern int func_08A6B760(void *panel);
extern void func_08A697B0(void *help, int text, int arg);

void func_08A51698(HelpOwner *self)
{
    switch (func_08A6B760(self->field_58->field_7998)) {
    case 0:
        func_08A697B0(self->field_58->field_6B1C, 0, -1);
        break;
    case 1:
        func_08A697B0(self->field_58->field_6B1C, 10, -1);
        break;
    }
}
