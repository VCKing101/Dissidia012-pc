/* Moves the two-column list cursor onto entry `id` (unless -1), then
   redraws. */
struct Cursor08B3025C {
    unsigned int flags;
};

struct State08B3025C {
    char pad_000[0x934];
    struct Cursor08B3025C field_934;
    char pad_938[0x960 - 0x938];
    struct Cursor08B3025C field_960;
    char pad_964[0x98C - 0x964];
    int field_98C;
};

struct Owner08B3025C {
    char pad_000[0x9D8];
    struct State08B3025C *field_9D8;
};

extern int func_08B30218(struct Owner08B3025C *self, int id);
extern void func_088D46E4(struct Cursor08B3025C *cursor, int pos, int a, int last, int b, int c);
extern void func_08B303B4(struct Owner08B3025C *self);
extern void func_08B30334(struct Owner08B3025C *self);

void func_08B3025C(struct Owner08B3025C *self, int id)
{
    if (id != -1) {
        int index = func_08B30218(self, id);
        int row = index / 2;
        int column = index % 2;
        struct Cursor08B3025C *cursor;
        func_088D46E4(&self->field_9D8->field_934, row, 0, self->field_9D8->field_98C / 2 - 1, 0, -1);
        func_088D46E4(&self->field_9D8->field_960, column, 0, 1, 0, -1);
        cursor = &self->field_9D8->field_960;
        cursor->flags |= 1;
    }
    func_08B303B4(self);
    func_08B30334(self);
}
