/* Resets the score board (two five-point counters, cleared rows) and redraws it. */
typedef struct ResetScoreRow {
    short field_00;
    short field_02;
} ResetScoreRow;

typedef struct ResetScoreState {
    char pad_000[0x644];
    int field_644;
    int field_648;
    int field_64C;
    int field_650;
    int field_654;
    int field_658;
    int field_65C;
    ResetScoreRow field_660[5];
} ResetScoreState;

typedef struct ResetScoreOwner {
    char pad_000[0x67C];
    ResetScoreState *field_67C;
} ResetScoreOwner;

extern void func_08B03850(ResetScoreOwner *self);
extern void func_08B023F8(ResetScoreOwner *self);

void func_08B021CC(ResetScoreOwner *self)
{
    int i;
    self->field_67C->field_648 = 0;
    self->field_67C->field_644 = 0;
    self->field_67C->field_64C = 5;
    self->field_67C->field_650 = 0;
    self->field_67C->field_654 = 0;
    self->field_67C->field_658 = 0;
    self->field_67C->field_65C = 5;
    for (i = 0; i < 5; i++) {
        self->field_67C->field_660[i].field_00 = 0;
        self->field_67C->field_660[i].field_02 = 0;
    }
    func_08B03850(self);
    func_08B023F8(self);
}
