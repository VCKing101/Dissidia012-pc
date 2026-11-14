/* Bone parts: a model with four parts attached to bones by name; open,
 * attach and reload. */
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

struct BoneParts {
    unsigned char open;
    unsigned char pad_01[3];
    BoundModel *model;
    unsigned char parts[0x28];
    Matrix44 matrices[4];
    unsigned int side;
    void *source;
    int name;
    int *bones;
};

extern "C" {

extern void func_08A11724(BoneParts *parts);
extern int func_089F7D74(void *parts, void *models, void *names);
extern void *func_089F7DDC(void *parts, int index);
extern void func_088DA0A8(void *model, int name, Matrix44 *matrix, Rgba *color);
extern void func_088D9C0C(void *part, Matrix44 *matrix);
extern void func_088D9AF4(void *part, Rgba *color);
extern void func_089F7E40(void *parts);
extern void func_088DA060(void *model);
extern BoundModel *func_088DAA40(void *source, int name, int flags);
extern void func_089F8100(void *parts, int kind, unsigned char flag);

int func_08A1176C(BoneParts *parts, unsigned int side, void *source, void *models, void *names, int name,
                  int *bones)
{
    func_08A11724(parts);
    if (side >= 2 || source == 0 || models == 0 || names == 0 || name < 0 || bones == 0) {
        return -1;
    }
    if (func_089F7D74(parts->parts, models, names) != 0) {
        return -2;
    }
    parts->side = side;
    parts->source = source;
    parts->name = name;
    parts->bones = bones;
    parts->open = 1;
    return 0;
}

int func_08A11850(BoneParts *parts)
{
    if (!parts->open) {
        return -1;
    }
    if (parts->model != 0) {
        if (!BoundModel::hidden(parts->model)) {
            Rgba color;
            for (int i = 0; i < 4; i++) {
                void *part = func_089F7DDC(parts->parts, i);
                if (part == 0) {
                    continue;
                }
                func_088DA0A8(parts->model, parts->bones[i], &parts->matrices[i], &color);
                func_088D9C0C(part, &parts->matrices[i]);
                func_088D9AF4(part, &color);
            }
        } else {
            func_089F7E40(parts->parts);
            if (parts->model != 0) {
                func_088DA060(parts->model);
                parts->model = 0;
            }
        }
    }
    return 0;
}

int func_08A1197C(BoneParts *parts, int kind, unsigned char flag)
{
    if (!parts->open) {
        return -1;
    }
    func_089F7E40(parts->parts);
    if (parts->model != 0) {
        func_088DA060(parts->model);
        parts->model = 0;
    }
    parts->model = func_088DAA40(parts->source, parts->name, 0);
    if (parts->model == 0) {
        return -2;
    }
    func_089F8100(parts->parts, kind, flag);
    func_08A11850(parts);
    return 0;
}

}
