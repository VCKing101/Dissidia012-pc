/* Sums the text values of the current group's 64 entries. */
typedef struct TotalGroup {
    char data[0xC6];
} TotalGroup;

typedef struct TotalState {
    char pad_0000[0xC80];
    TotalGroup field_C80[4];
    char pad_0F98[0x13E3 - 0xF98];
    signed char field_13E3;
} TotalState;

typedef struct TotalOwner {
    char pad_000[0x140];
    TotalState *field_140;
} TotalOwner;

extern int func_088B7AE8(TotalGroup *group, int index);
extern int func_08AF3AD8(TotalOwner *self, int id);

int func_08AF47E0(TotalOwner *self)
{
    int total = 0;
    TotalGroup *group = &self->field_140->field_C80[self->field_140->field_13E3];
    int i;
    for (i = 0; i < 64; i++) {
        int id = func_088B7AE8(group, i);
        if (id != -1) {
            total += func_08AF3AD8(self, id);
        }
    }
    return total;
}
