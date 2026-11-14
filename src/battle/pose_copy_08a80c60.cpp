struct Vec3 {
    float x;
    float y;
    float z;
};

struct PoseData {
    Vec3 pos;
    Vec3 axes[3];
    Vec3 scale;
};

struct PoseState {
    unsigned char active;
    char pad_01[0x13];
    int mode;
    PoseData pose;
};

extern "C" {

extern void func_089A34C0(Vec3 *to, Vec3 *from);

/* Copies a pose and switches to mode 1; -1 inactive, -2 no pose. */
int func_08A80C60(PoseState *self, PoseData *pose)
{
    unsigned int i;
    if (self->active == 0) {
        return -1;
    }
    if (pose == 0) {
        return -2;
    }
    self->mode = 1;
    func_089A34C0(&self->pose.pos, &pose->pos);
    for (i = 0; i < 3; i++) {
        func_089A34C0(&self->pose.axes[i], &pose->axes[i]);
    }
    func_089A34C0(&self->pose.scale, &pose->scale);
    return 0;
}

}
