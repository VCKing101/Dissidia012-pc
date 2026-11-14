/* Attaches the trigger set's model to its source by the side's bone name and
 * the four trigger parts to the model, or releases them once it is hidden. */
struct BoundModel {
    unsigned char pad_00[0x20];
    unsigned short flags;

    static bool hidden(BoundModel *model) { return (model->flags & 0x20) != 0; }
};

struct Matrix44 {
    float m[16];
} __attribute__((aligned(16)));

struct Rgba {
    unsigned char r, g, b, a;
};

struct TriggerSet {
    unsigned char open;
    unsigned char pad_01[3];
    void *source;
    BoundModel *model;
    void *parts[4];
    int owner;
    int pad_20[3];
    int (*names)[4];
};

extern "C" {

extern int D_089E4B38[2];
extern unsigned int func_089E95E0(int owner);
extern void func_088DA0A8(void *model, int name, Matrix44 *matrix, Rgba *color);
extern void func_088D9B84(void *part, Matrix44 *matrix);
extern void func_088D9AF4(void *part, Rgba *color);
extern int func_089E4A94(TriggerSet *set);

int func_089E4674(TriggerSet *set)
{
    if (!set->open) {
        return -1;
    }
    unsigned int side = func_089E95E0(set->owner);
    if (side >= 2) {
        return -2;
    }
    if (set->model != 0) {
        if (!BoundModel::hidden(set->model)) {
            Matrix44 matrix;
            Rgba color;
            func_088DA0A8(set->source, D_089E4B38[side], &matrix, &color);
            func_088D9B84(set->model, &matrix);
            func_088D9AF4(set->model, &color);
            for (int i = 0; i < 4; i++) {
                if (set->parts[i] == 0) {
                    continue;
                }
                Matrix44 part_matrix;
                Rgba part_color;
                func_088DA0A8(set->model, set->names[side][i], &part_matrix, &part_color);
                func_088D9B84(set->parts[i], &part_matrix);
                func_088D9AF4(set->parts[i], &part_color);
            }
        } else {
            func_089E4A94(set);
        }
    }
    return 0;
}

}
