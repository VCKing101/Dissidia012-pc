typedef struct MotionModel4FFD8 {
    char pad_000[0xC];
    short motion;
} MotionModel4FFD8;

typedef struct MotionOwner4FFD8 {
    char pad_000[0x54];
    MotionModel4FFD8 *model;
} MotionOwner4FFD8;

extern void func_088D9F5C(MotionModel4FFD8 *model, int motion, int loop);

void func_08A4FFD8(MotionOwner4FFD8 *self)
{
    MotionModel4FFD8 *model = self->model;
    if (!model) {
        return;
    }
    if (model->motion == 12) {
        return;
    }
    func_088D9F5C(model, 12, 1);
}
