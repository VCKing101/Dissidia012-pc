/* Attaches the four parts of a part set to their bones on the owner model,
 * or releases them once the model is hidden. */
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

struct PartSet {
    unsigned char open;
    unsigned char pad_01[3];
    BoundModel *model;
    unsigned char parts[0x28];
    Matrix44 matrices[4];
    int owner;
};

extern "C" {

extern int D_089F3838[2][4];
extern unsigned int func_08A22CA8(int owner);
extern void *func_089F7DDC(void *parts, int index);
extern void func_088DA0A8(void *model, int name, Matrix44 *matrix, Rgba *color);
extern void func_088D9C0C(void *part, Matrix44 *matrix);
extern void func_088D9AF4(void *part, Rgba *color);
extern void func_089F7E40(void *parts);
extern void func_088DA060(void *model);

int func_089F35C0(PartSet *set)
{
    if (!set->open) {
        return -1;
    }
    if (set->model != 0) {
        if (!BoundModel::hidden(set->model)) {
            unsigned int side = func_08A22CA8(set->owner);
            if (side >= 2) {
                return -2;
            } else {
                Rgba color;
                for (int i = 0; i < 4; i++) {
                    void *part = func_089F7DDC(set->parts, i);
                    if (part == 0) {
                        continue;
                    }
                    func_088DA0A8(set->model, D_089F3838[side][i], &set->matrices[i], &color);
                    func_088D9C0C(part, &set->matrices[i]);
                    func_088D9AF4(part, &color);
                }
            }
        } else {
            func_089F7E40(set->parts);
            if (set->model != 0) {
                func_088DA060(set->model);
                set->model = 0;
            }
        }
    }
    return 0;
}

}
