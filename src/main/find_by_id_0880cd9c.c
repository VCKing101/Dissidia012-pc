/* Returns the element whose descriptor id matches, or 0. */
typedef struct Desc0880CD9C {
    short id;
} Desc0880CD9C;

typedef struct Elem0880CD9C {
    char pad_000[0x158];
    Desc0880CD9C *desc;
    char pad_15C[0x14];
} Elem0880CD9C;

typedef struct Owner0880CD9C {
    char pad_00[0x20];
    int count;
    char pad_24[0x10];
    Elem0880CD9C *elems;
} Owner0880CD9C;

Elem0880CD9C *func_0880CD9C(Owner0880CD9C *self, int id)
{
    int count = self->count;
    int i;
    for (i = 0; i < count; i++) {
        if (self->elems[i].desc->id == id) {
            return &self->elems[i];
        }
    }
    return 0;
}
