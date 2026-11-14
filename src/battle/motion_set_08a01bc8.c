typedef struct MotionModel {
    char pad_00[0xC];
    short motion;
    short frame;
} MotionModel;

typedef struct MotionPlayer {
    unsigned char active;
    char pad_01[0x3];
    void *owner;
    MotionModel *model;
    unsigned char locked;
} MotionPlayer;

extern void func_088D9F5C(MotionModel *model, int motion, int restart);
extern void func_08A01948(MotionPlayer *self);

/* Switches the model to motion (if needed) at frame; -1 inactive, -2 locked or no model. */
int func_08A01BC8(MotionPlayer *self, int motion, int frame)
{
    if (self->active == 0) {
        return -1;
    }
    if (self->locked == 1 || self->owner == 0 || self->model == 0) {
        return -2;
    }
    if (self->model->motion != motion) {
        func_088D9F5C(self->model, motion, 1);
    }
    self->model->frame = frame;
    func_08A01948(self);
    return 0;
}
