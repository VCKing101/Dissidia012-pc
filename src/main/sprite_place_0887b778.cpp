/* Places a sprite from a matrix: scale, position, and one pixel up unless flag 0x2000. */
struct Mtx0887B778 {
    float m[16];
};

struct Sprite0887B778 {
    int field_00;
    char pad_04[0xC];
    unsigned int flags;
    short x;
    short y;
    float scale_x;
    float scale_y;

    static bool has(Sprite0887B778 *s, unsigned int mask) { return (s->flags & mask) != 0; }
};

extern "C" {

void func_0887B778(Sprite0887B778 *s, Mtx0887B778 *m)
{
    s->scale_x = m->m[0];
    s->scale_y = m->m[5];
    s->x = (int)m->m[12];
    s->y = (int)m->m[13];
    if (!Sprite0887B778::has(s, 0x2000)) {
        s->y--;
    }
}

}
