/* Bound timer: releases its models once the active one is hidden, then
 * attaches the mark to the model's bone. */
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

struct BoundTimer {
    unsigned char open;
    unsigned char pad_01[3];
    BoundModel *model;
    BoundModel *mark;
    unsigned char pad_0c[4];
    void *source;
    int name;
    void *mark_source;
    int mark_name;
    int bone;
    unsigned char closing;
};

extern "C" {

extern void func_088DA060(void *model);
extern void func_088DA0A8(void *model, int name, Matrix44 *matrix, Rgba *color);
extern void func_088D9B84(void *part, Matrix44 *matrix);
extern void func_088D9AF4(void *part, Rgba *color);

int func_08A10C88(BoundTimer *timer)
{
    if (!timer->open) {
        return -1;
    }
    unsigned char closing = timer->closing;
    if ((closing == 0 && timer->model != 0 && BoundModel::hidden(timer->model) == 1) ||
        (closing == 1 && timer->mark != 0 && BoundModel::hidden(timer->mark) == 1)) {
        if (timer->mark != 0) {
            func_088DA060(timer->mark);
            timer->mark = 0;
        }
        if (timer->model != 0) {
            func_088DA060(timer->model);
            timer->model = 0;
        }
    }
    if (timer->model != 0 && timer->mark != 0 && timer->bone >= 0) {
        Matrix44 matrix;
        Rgba color;
        func_088DA0A8(timer->model, timer->bone, &matrix, &color);
        func_088D9B84(timer->mark, &matrix);
        func_088D9AF4(timer->mark, &color);
    }
    return 0;
}

}
