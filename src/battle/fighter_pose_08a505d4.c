typedef struct PoseVector {
    float x;
    float y;
    float z;
    float w;
} PoseVector;

typedef struct PoseFighter {
    char pad_000[0x384];
    unsigned int flags;
    char pad_388[0x35C];
    float field_6E4;
} PoseFighter;

extern float D_08A514F8;
extern void func_08837A38(PoseFighter *fighter);
extern float func_08834840(PoseFighter *fighter);
extern void func_08A5BF84(PoseFighter *fighter, PoseVector *pose);
extern void func_08834C10(PoseFighter *fighter, int mode, int flag);

void func_08A505D4(int unused, PoseFighter *self)
{
    PoseVector pose;
    func_08837A38(self);
    {
        float height = func_08834840(self);
        float zero = 0.0f;
        pose.y = height;
        pose.x = zero;
        pose.z = zero;
        pose.w = zero;
    }
    func_08A5BF84(self, &pose);
    func_08834C10(self, 1, 0);
    self->field_6E4 = D_08A514F8;
}
