/* Enters state 6 once the waited-for task is done. */
struct State08B36868 {
    int field_0;
    char pad_4[0x10 - 4];
    int field_10;
};

struct Object08B36868 {
    char pad_00[0xC4];
    struct State08B36868 *field_C4;
};

extern void *func_08B45EF8(int id);
extern int func_08921B20(void *task);

void func_08B36868(struct Object08B36868 *self)
{
    if (func_08921B20(func_08B45EF8(self->field_C4->field_10)) == 0) {
        return;
    }
    self->field_C4->field_0 = 6;
}
