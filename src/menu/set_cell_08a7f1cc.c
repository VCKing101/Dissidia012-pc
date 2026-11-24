/* Writes a halfword into one of four fields of a table cell (cell chosen by position, or by an explicit index). */
typedef struct Cell { short a, b, c, d; } Cell;
typedef struct Tb { char pad0[0x608]; int x; int y; char pad1[0x678 - 0x610]; Cell cells[1]; } Tb;
typedef struct Hd { char pad0[0x28]; Tb *t; } Hd;
void func_08A7F1CC(Hd *self, int field, int value, int index) {
    int pad0[1];
    int cell[1];
    Tb *t = self->t;
    cell[0] = t->y * 10 + t->x;
    if (index != -1) cell[0] = index;
    if (field == 0) {
        t->cells[cell[0]].a = value;
    } else if (field == 1) {
        t->cells[cell[0]].b = value;
    } else if (field == 2) {
        t->cells[cell[0]].c = value;
    } else if (field == 7) {
        t->cells[cell[0]].d = value;
    }
}
