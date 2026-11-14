/* new (heap) for the 0x9B74-byte screen: vtable, three member constructors and the 16-entry table. */
struct ScreenObject {
    void *field_000;
    char field_004[0x430 - 0x4];
    char field_430[0x10];
    char field_440[0x45C - 0x440];
    char field_45C[0x9B74 - 0x45C];
};

extern "C" {
void *func_0885C6CC(void *heap, unsigned size, int align, int offset, int kind, int flags);
void func_08A75490(char *member);
void func_08A74260(char *member);
void func_08A72D18(char *member);
void func_08993560(char *array, int count, int size, void (*construct)(void));
void func_08A7A0F8(void);
extern char D_08A7C9D8[];

ScreenObject *func_08A7A3A8(void *heap)
{
    int one = 1;
    ScreenObject *none = 0;
    ScreenObject *result[1];
    result[0] = none;
    ScreenObject *self = (ScreenObject *)func_0885C6CC(heap, 0x9B74, 0, 0, one, 0);
    ScreenObject *made = result[0];
    if (self != 0) {
        self->field_000 = D_08A7C9D8;
        func_08A75490(self->field_004);
        func_08A74260(self->field_430);
        func_08A72D18(self->field_440);
        func_08993560(self->field_45C, 0x10, 0x970, func_08A7A0F8);
        made = self;
    }
    return made;
}
}
