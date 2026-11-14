/* Side counter: attaches its mark to the counter model's bone for the side,
 * releasing both once the model is hidden; and reloads both models. */
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

struct SideCounterModels {
    unsigned char open;
    unsigned char pad_01[3];
    unsigned int side;
    BoundModel *model;
    void *mark;
};

extern "C" {

extern int D_08A11650[2];
extern int D_08A11658[2];
extern unsigned int func_08A22CA8(unsigned int side);
extern void func_088DA060(void *model);
extern BoundModel *func_088DAA40(void *source, int name, int flags);
extern void func_088DA0A8(void *model, int name, Matrix44 *matrix, Rgba *color);
extern void func_088D9B84(void *part, Matrix44 *matrix);
extern void func_088D9AF4(void *part, Rgba *color);

int func_08A113D8(SideCounterModels *counter)
{
    if (!counter->open) {
        return -1;
    }
    if (counter->model != 0 && counter->mark != 0) {
        if (BoundModel::hidden(counter->model) == 1) {
            if (counter->mark != 0) {
                func_088DA060(counter->mark);
                counter->mark = 0;
            }
            if (counter->model != 0) {
                func_088DA060(counter->model);
                counter->model = 0;
            }
        } else {
            if (counter->side >= 2) {
                return -2;
            }
            unsigned int colour = func_08A22CA8(counter->side);
            if (colour >= 2) {
                return -3;
            } else {
                Matrix44 matrix;
                Rgba color;
                func_088DA0A8(counter->model, D_08A11658[colour], &matrix, &color);
                func_088D9B84(counter->mark, &matrix);
                func_088D9AF4(counter->mark, &color);
            }
        }
    }
    return 0;
}

int func_08A114F8(SideCounterModels *counter, unsigned int side, void *source, void *mark_source)
{
    if (!counter->open) {
        return -1;
    }
    if (side >= 2 || source == 0 || mark_source == 0) {
        return -2;
    }
    if (counter->mark != 0) {
        func_088DA060(counter->mark);
        counter->mark = 0;
    }
    if (counter->model != 0) {
        func_088DA060(counter->model);
        counter->model = 0;
    }
    unsigned int colour = func_08A22CA8(side);
    if (colour >= 2) {
        return -3;
    }
    counter->model = func_088DAA40(source, D_08A11650[colour], 0);
    if (counter->model == 0) {
        return -4;
    }
    counter->mark = func_088DAA40(mark_source, 0, 0);
    if (counter->mark == 0) {
        if (counter->model != 0) {
            func_088DA060(counter->model);
            counter->model = 0;
        }
        return -5;
    }
    counter->side = side;
    func_08A113D8(counter);
    return 0;
}

}
