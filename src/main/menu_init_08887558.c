/* Initialises a menu: no cursor or selection, cleared counters and list. */
typedef struct Menu08887558 {
    int field_00;
    int field_04;
    int cursor;
    int selection;
    int field_10;
    int field_14;
    unsigned char field_18;
    unsigned char field_19;
    char pad_1A[2];
    int field_1C;
    int hover;
    char list[0x778];
    int field_79C;
    int field_7A0;
} Menu08887558;

void func_088CB9E8(void *list);

Menu08887558 *func_08887558(Menu08887558 *m)
{
    int none = -1;
    m->field_00 = 0;
    m->field_04 = 0;
    m->cursor = none;
    m->selection = none;
    m->field_10 = 0;
    m->field_14 = 0;
    m->field_18 = 0;
    m->field_19 = 0;
    m->field_1C = 0;
    m->hover = none;
    func_088CB9E8(m->list);
    m->field_79C = 0;
    m->field_7A0 = 0;
    return m;
}
