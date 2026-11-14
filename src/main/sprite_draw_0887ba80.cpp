/* Draws the sprite's image unless flag 0x2000 is set; 0 when nothing is drawn. */
struct Sprite0887BA80 {
    void *image;
    char pad_04[0xC];
    unsigned int flags;

    static bool has(Sprite0887BA80 *s, unsigned int mask) { return (s->flags & mask) != 0; }
};

extern "C" {

int func_0887A5F8(void *image, int x, int y);

int func_0887BA80(Sprite0887BA80 *s, int x, int y)
{
    int result = 0;
    if (!Sprite0887BA80::has(s, 0x2000) && s->image != 0) {
        result = func_0887A5F8(s->image, x, y);
    }
    return result;
}

}
