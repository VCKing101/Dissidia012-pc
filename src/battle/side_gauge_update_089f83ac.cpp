/* Updates a side gauge: pushes a pending value to the root and attaches the
 * gauge's four parts to the root's bones for the side. */
struct Matrix44 {
    float m[16];
};

struct Rgba {
    unsigned char r, g, b, a;
};

struct RootHolder {
    unsigned char pad_000[0x7A4];
    void *root;
};

struct SideGauge {
    unsigned char open;
    unsigned char pad_01[7];
    unsigned int side;
    unsigned char pending;
    unsigned char pad_0d[3];
    int value;
    unsigned char parts[0x24];
};

extern "C" {

extern RootHolder *D_089D6A10;
extern int func_089E9AA8(unsigned int side, int value);
extern void func_089DDC98(void *root, unsigned int side, int flags, int value);
extern void *func_089F7DDC(void *parts, int index);
extern int func_089DDD10(void *root, unsigned int side, int index, Matrix44 *matrix, Rgba *color);
extern void func_088D9B84(void *part, Matrix44 *matrix);
extern void func_088D9AF4(void *part, Rgba *color);

int func_089F83AC(SideGauge *gauge)
{
    if (!gauge->open) {
        return -1;
    }
    void *root = D_089D6A10->root;
    if (root == 0) {
        return -2;
    }
    if (gauge->pending == 1 && gauge->side < 2) {
        int value = func_089E9AA8(gauge->side, gauge->value);
        func_089DDC98(root, gauge->side, 1, value);
    }
    Matrix44 matrix;
    Rgba color;
    for (int i = 0; i < 4; i++) {
        void *part = func_089F7DDC(gauge->parts, i);
        if (part == 0) {
            continue;
        }
        if (func_089DDD10(root, gauge->side, i, &matrix, &color) != 0) {
            continue;
        }
        func_088D9B84(part, &matrix);
        func_088D9AF4(part, &color);
    }
    return 0;
}

}
