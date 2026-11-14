/* Works out how far one hit shape pushes another, by shape kind; a
   capsule against a sphere is worked out the other way round and flipped. */
struct Vec4_08812EF0 {
    float x, y, z, w;
};

struct Shape08812EF0 {
    int kind;
};

extern "C" {

void func_08812414(Shape08812EF0 *a, Shape08812EF0 *b, Vec4_08812EF0 *out);
void func_088127B8(Shape08812EF0 *a, Shape08812EF0 *b, Vec4_08812EF0 *out);
void func_089A3A64(Vec4_08812EF0 *dst, const Vec4_08812EF0 *src, float scale);

void func_08812EF0(Shape08812EF0 *a, Shape08812EF0 *b, Vec4_08812EF0 *out)
{
    switch (a->kind) {
    case 0:
    case 3:
        switch (b->kind) {
        case 0:
        case 3:
            func_08812414(a, b, out);
            break;
        case 1:
            func_088127B8(a, b, out);
            break;
        default:
            break;
        }
        break;
    case 1:
        switch (b->kind) {
        case 0:
        case 3:
            func_088127B8(b, a, out);
            func_089A3A64(out, out, -1.0f);
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

}
