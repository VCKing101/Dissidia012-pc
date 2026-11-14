/* Attaches a pair meter's model and its two parts to the owner animation while
 * the animation plays the meter's motion, or hides the meter otherwise. */
struct BoundModel {
    unsigned char pad_00[0x20];
    unsigned short flags;

    static bool hidden(BoundModel *model) { return (model->flags & 0x20) != 0; }
};

struct MeterAnim {
    unsigned char pad_00[0xC];
    short motion;
};

struct Matrix44 {
    float m[16];
};

struct Rgba {
    unsigned char r, g, b, a;
};

struct Meter {
    unsigned char open;
    unsigned char pad_01[3];
    BoundModel *model;
    void *parts[2];
    int owner;
    unsigned char pad_14[4];
    int motion;
    int bone;
    unsigned char pad_20[4];
    int *bones;
};

extern "C" {

extern MeterAnim *func_08A01AC0(int owner);
extern void func_088DA0A8(void *model, int name, Matrix44 *matrix, Rgba *color);
extern void func_088D9B84(void *part, Matrix44 *matrix);
extern void func_088D9AF4(void *part, Rgba *color);
extern void func_08A08E0C(Meter *meter);

int func_08A08AEC(Meter *meter)
{
    if (!meter->open) {
        return -1;
    }
    if (meter->model != 0) {
        MeterAnim *anim = func_08A01AC0(meter->owner);
        int playing = 0;
        if (anim != 0 && anim->motion == meter->motion) {
            playing = 1;
        }
        if (!BoundModel::hidden(meter->model) && (unsigned char)playing == 1) {
            Matrix44 matrix;
            Rgba color;
            func_088DA0A8(anim, meter->bone, &matrix, &color);
            func_088D9B84(meter->model, &matrix);
            func_088D9AF4(meter->model, &color);
            for (int i = 0; i < 2; i++) {
                if (meter->parts[i] == 0) {
                    continue;
                }
                func_088DA0A8(meter->model, meter->bones[i], &matrix, &color);
                func_088D9B84(meter->parts[i], &matrix);
                func_088D9AF4(meter->parts[i], &color);
            }
        } else {
            func_08A08E0C(meter);
        }
    }
    return 0;
}

}
