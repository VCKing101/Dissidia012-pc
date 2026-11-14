/* Resets all sixteen cursors of a menu. */
struct Cursor0884F5D8 {
    char data[0x20];
};

struct Menu0884F5D8 {
    Cursor0884F5D8 cursors[16];
    char pad_200[4];
    unsigned char field_204;
    char pad_205[3];
    unsigned char field_208;
};

extern "C" {

void func_088501D8(Cursor0884F5D8 *c, int arg);

void func_0884F5D8(Menu0884F5D8 *m)
{
    m->field_208 = 1;
    m->field_204 = 0;
    for (int i = 0; i < 16; i++) {
        Cursor0884F5D8 *c = &m->cursors[i];
        func_088501D8(c, 0);
    }
}

}
