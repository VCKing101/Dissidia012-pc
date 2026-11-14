/* Returns cell 5 of board row `row`, or null while func_08AB6DAC reports the menu busy.
   Retail keeps `self` in the frame (0($sp)) across the guard call (Kept holder). */
template <class T> struct Kept {
    T value;
    Kept(T v) : value(v) {}
};

struct GuardCell {
    signed char data[6];
};

/* Board rows as seen from their cell arrays (+6 into each 0x3A-byte row). */
struct GuardRow {
    GuardCell field_00[4];
    GuardCell field_18;
    GuardCell field_1E;
    char pad_24[0x3A - 0x24];
};

struct GuardBlock {
    char pad_00[0x6];
    GuardRow field_06[8];
};

struct GuardGlobal {
    char pad_000[0x148];
    GuardBlock field_148;
};

struct GuardMenu;

extern "C" {

extern GuardGlobal D_09B6DF08;
int func_08AB6DAC(GuardMenu *menu);
void *func_08AB6D58(GuardMenu *menu);

GuardCell *func_08AB4884(GuardMenu *self, int row)
{
    Kept<GuardMenu *> saved(self);
    int which = row;
    int busy = func_08AB6DAC(self);
    self = saved.value;
    if (busy == 1) {
        return 0;
    }
    func_08AB6D58(self);
    GuardBlock *block = &D_09B6DF08.field_148;
    GuardRow *rows = block->field_06;
    return &rows[which].field_1E;
}

}
