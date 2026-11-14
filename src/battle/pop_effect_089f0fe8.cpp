/* Pop-up effect: counts down a shown pop-up and releases a hidden one, and
 * (re)starts the pop-up with a display time. */
struct PopModel {
    float length;
    unsigned char pad_04[8];
    short kind;
    short playing;
    unsigned char pad_10[0x10];
    unsigned short flags;

    static bool hidden(PopModel *model) { return (model->flags & 0x20) != 0; }
};

struct FrameClock {
    unsigned char pad_00[0x18];
    float frame_scale;
};

struct PopEffect {
    unsigned char open;
    unsigned char pad_01[3];
    void *source;
    PopModel *model;
    float time;
};

extern "C" {

extern FrameClock *D_089D69E0;
extern float D_089F12E8;
extern float D_089F12EC;
extern void func_089F11C4(PopEffect *pop);
extern void func_088DA060(PopModel *model);
extern PopModel *func_088DAA68(void *source);
extern void func_088D9F5C(PopModel *model, int motion, int loop);
extern void func_088D9AEC(PopModel *model, float frame);

int func_089F0FE8(PopEffect *pop)
{
    if (!pop->open) {
        return -1;
    }
    PopModel *model = pop->model;
    if (model != 0) {
        switch (model->kind) {
        case 1:
            if (pop->time != D_089F12E8) {
                pop->time -= D_089D69E0->frame_scale;
                if (pop->time <= 0.0f) {
                    func_089F11C4(pop);
                }
            }
            break;
        case 2:
            if (PopModel::hidden(model) == 1) {
                if (pop->model != 0) {
                    func_088DA060(pop->model);
                    pop->model = 0;
                }
            }
            break;
        }
    }
    return 0;
}

int func_089F10DC(PopEffect *pop, float time)
{
    if (!pop->open) {
        return -1;
    }
    if (pop->model == 0) {
        pop->model = func_088DAA68(pop->source);
        if (pop->model == 0) {
            return -2;
        }
    }
    if (pop->model->kind == 2) {
        float start = D_089F12EC - pop->model->length;
        func_088D9F5C(pop->model, 0, 1);
        pop->model->playing = 1;
        func_088D9AEC(pop->model, start);
    } else {
        func_088D9F5C(pop->model, 0, 1);
        pop->model->playing = 1;
    }
    pop->time = time;
    return 0;
}

}
