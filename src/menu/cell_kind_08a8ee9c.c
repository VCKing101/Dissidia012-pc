/* Kind byte of the current page's selected cell (page 0 uses kinds 0-8, others from 9).
   Retail keeps both temporaries in the frame; one-element arrays reproduce that. */
typedef struct CellKind {
    signed char field_00;
    char pad_01[0x13];
} CellKind;

typedef struct CellPages {
    union {
        struct {
            char pad_000[0x2B4];
            int field_2B4;
            char pad_2B8[0x4];
            int field_2BC[(0x308 - 0x2BC) / 4];
            int field_308[8];
        } pages;
        struct {
            char pad_000[0x628];
            int field_628[4][0x2CC / 4];
        } cells;
        struct {
            char pad_000[0x92C];
            CellKind field_92C[32];
        } kinds;
    } u;
} CellPages;

typedef struct CellOwner {
    char pad_00[0x8];
    CellPages *field_08;
} CellOwner;

int func_08A8EE9C(CellOwner *self)
{
    int cell[1];
    int kind[1];
    CellPages *pages = self->field_08;
    int page = pages->u.pages.field_308[pages->u.pages.field_2B4];
    int first;
    cell[0] = pages->u.cells.field_628[page][pages->u.pages.field_2BC[page]];
    first = page == 0 ? 0 : 9;
    kind[0] = pages->u.kinds.field_92C[first + cell[0]].field_00;
    return kind[0];
}
