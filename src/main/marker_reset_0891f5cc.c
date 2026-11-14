/* Resets a marker record and its twenty six-byte cells. */
typedef struct MarkerCell {
    unsigned char a;
    unsigned char b;
    unsigned char c;
    unsigned char d;
    short value;
} MarkerCell;

typedef struct Marker {
    short ids[4];               /* 0x00 */
    short field_08;
    short field_0A;
    short field_0C;
    short field_0E;
    float time;                 /* 0x10 */
    MarkerCell cells[20];       /* 0x14 */
} Marker;

void func_0891F5CC(Marker *m)
{
    int i;
    m->ids[0] = -1;
    m->ids[1] = -1;
    m->ids[2] = -1;
    m->ids[3] = -1;
    m->field_0A = 0;
    m->field_0C = -1;
    m->time = 0.0f;
    for (i = 0; i < 20; i++) {
        MarkerCell *cell = &m->cells[i];
        cell->a = 0;
        cell->b = 0;
        cell->c = 0;
        cell->d = 0;
        cell->value = 0;
    }
}
