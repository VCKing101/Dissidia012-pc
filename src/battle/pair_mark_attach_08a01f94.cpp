/* Attaches a pair mark to its owner model's bone for the side and kind, or
 * releases both once the mark is hidden. */
struct BoundModel {
    unsigned char pad_00[0x20];
    unsigned short flags;

    static bool hidden(BoundModel *model) { return (model->flags & 0x20) != 0; }
};

struct Matrix44 {
    float m[16];
};

struct Rgba {
    unsigned char r, g, b, a;
};

struct PairMark {
    unsigned char open;
    unsigned char pad_01[3];
    void *model;
    BoundModel *mark;
    int owner;
    int kind;
};

extern "C" {

extern int D_08A021A8[2][2];
extern unsigned int func_08A22CA8(int owner);
extern void func_088DA060(void *model);
extern void func_088DA0A8(void *model, int name, Matrix44 *matrix, Rgba *color);
extern void func_088D9B84(void *part, Matrix44 *matrix);
extern void func_088D9AF4(void *part, Rgba *color);

int func_08A01F94(PairMark *pair)
{
    if (!pair->open) {
        return -1;
    }
    if (pair->model != 0 && pair->mark != 0) {
        if (!BoundModel::hidden(pair->mark)) {
            unsigned int side = func_08A22CA8(pair->owner);
            if (side >= 2) {
                return -2;
            } else {
                Matrix44 matrix;
                Rgba color;
                func_088DA0A8(pair->model, D_08A021A8[side][pair->kind], &matrix, &color);
                func_088D9B84(pair->mark, &matrix);
                func_088D9AF4(pair->mark, &color);
            }
        } else {
            if (pair->mark != 0) {
                func_088DA060(pair->mark);
                pair->mark = 0;
            }
            if (pair->model != 0) {
                func_088DA060(pair->model);
                pair->model = 0;
            }
        }
    }
    return 0;
}

}
