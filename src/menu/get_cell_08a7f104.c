/* Reads one of four halfword fields of a table cell (cell chosen by position, or by an explicit index). */
typedef struct Cell { unsigned short a; short b, c, d; } Cell;
typedef struct Tb { char pad0[0x608]; int x; int y; char pad1[0x678 - 0x610]; Cell cells[1]; } Tb;
typedef struct Hd { char pad0[0x28]; Tb *t; } Hd;
int func_08A7F104(Hd *self, int field, int index) {
    int result[1];
    int cell[1];
    Tb *t = self->t;
    cell[0] = t->y * 10 + t->x;
    if (index != -1) cell[0] = index;
    if (field == 0) {
        result[0] = t->cells[cell[0]].a;
    } else if (field == 1) {
        result[0] = t->cells[cell[0]].b;
    } else if (field == 2) {
        result[0] = t->cells[cell[0]].c;
    } else if (field == 7) {
        result[0] = t->cells[cell[0]].d;
    }
    return result[0];
}
