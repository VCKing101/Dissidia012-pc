struct Image0887BB6C {
    int field_00;
    unsigned short width;
    char pad_06[0xA];
    short height;
};
struct Image2 { int field_00; unsigned short width; };
struct Sprite2 { Image2 *image; };
struct Sprite0887BB6C {
    Image0887BB6C *image;
    char pad_04[0xC];
    unsigned int flags;
    char pad_14[4];
    float scale;
    static bool has(Sprite0887BB6C *s, unsigned int mask) { return (s->flags & mask) != 0; }
    static int run(Sprite0887BB6C *s) {
        float sc = s->scale;
        if (!has(s, 0x2000)) {
            return (int)(s->image->height * sc);
        } else {
            return (int)(((Sprite2 *)s)->image->width * sc);
        }
    }
};
extern "C" {
int func_0887BB6C(Sprite0887BB6C *s) { return Sprite0887BB6C::run(s); }
}
