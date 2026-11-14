extern int func_089E8EF0(int);
typedef struct ModeB6AC { char pad_000[0x838]; int field_838; } ModeB6AC;
int func_089EB6AC(ModeB6AC *self)
{
    int r = func_089E8EF0(self->field_838);
    return r == 0 ? 3 : 2;
}
