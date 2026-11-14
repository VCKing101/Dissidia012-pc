/* Sets the help lines for the current page (page kind 2 uses six lines). */
typedef struct HelpState {
    char pad_000[0x2B4];
    int field_2B4;
    char pad_2B8[0x308 - 0x2B8];
    int field_308[8];
} HelpState;

typedef struct HelpOwner2 {
    char pad_00[0x8];
    HelpState *field_08;
} HelpOwner2;

extern void func_08A8D480(HelpOwner2 *self, int line, int text);
extern void func_08A8DC60(HelpOwner2 *self, int kind, int text);

void func_08A8C644(HelpOwner2 *self)
{
    if (self->field_08->field_308[self->field_08->field_2B4] != 2) {
        func_08A8D480(self, 0x13, -1);
        func_08A8D480(self, 0x10, 0x2D);
        func_08A8DC60(self, 10, 0x41);
    } else {
        func_08A8D480(self, 4, 0x2B);
        func_08A8D480(self, 5, -1);
        func_08A8D480(self, 6, -1);
        func_08A8D480(self, 7, -1);
        func_08A8D480(self, 8, -1);
        func_08A8D480(self, 9, -1);
        func_08A8DC60(self, 0, 0x41);
    }
}
