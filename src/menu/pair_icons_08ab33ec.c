/* Sets the two icons of the pair: ids 1/2 of the table when off, 0/3 when on. */
struct PairParts08AB33EC {
    char pad_00[0x10];
    void *field_10;
    void *field_14;
};

struct PairOwner08AB33EC {
    struct PairParts08AB33EC *field_00;
};

extern unsigned short D_08AB40AC[4];
extern void func_088D9F5C(void *part, int id, int show);

void func_08AB33EC(struct PairOwner08AB33EC *self, int on)
{
    if (on == 0) {
        func_088D9F5C(self->field_00->field_10, D_08AB40AC[1], 1);
        func_088D9F5C(self->field_00->field_14, D_08AB40AC[2], 1);
    } else {
        func_088D9F5C(self->field_00->field_10, D_08AB40AC[0], 1);
        func_088D9F5C(self->field_00->field_14, D_08AB40AC[3], 1);
    }
}
