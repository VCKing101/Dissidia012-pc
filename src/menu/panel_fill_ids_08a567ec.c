/* Writes the current entry's id and two record bytes (or -1) into the panel's three result bytes. */
typedef struct Panel {
    char g0[0x104];
    int index;
    int ids[1];
} Panel;
typedef struct Rec {
    char g0;
    signed char b1;
    signed char b2;
} Rec;
extern Rec *func_08927D2C(int mgr, int id);
extern int D_089D8530;
void func_08A567EC(Panel *self) {
    int id = *(int *)((char *)self + self->index * 4 + 0x138);
    Rec *r = func_08927D2C(D_089D8530, id);
    if (r != 0) {
        *((char *)self + 0x238) = id;
        *((char *)self + 0x239) = r->b1;
        *((char *)self + 0x23A) = r->b2;
    } else {
        *((char *)self + 0x238) = id;
        *((char *)self + 0x239) = -1;
        *((char *)self + 0x23A) = -1;
    }
}
