struct Vec4 {
    float x;
    float y;
    float z;
    float w;
    Vec4() {}
};

typedef struct FollowTask {
    char pad_00[0x4];
    void *fighter;
    char pad_08[0x10];
    char camera[0x44];
    void *target;
} FollowTask;

extern "C" Vec4 func_08837030(void *fighter);
extern "C" float func_08837014(void *fighter);
extern "C" void func_08870C94(void *camera, Vec4 *at);

/* Points the camera at the fighter's position, with its facing as w. */
extern "C" void func_08A8124C(FollowTask *self)
{
    Vec4 pos;
    Vec4 at;
    float facing;
    pos = func_08837030(self->fighter);
    facing = func_08837014(self->fighter);
    Vec4 *target = &at;
    at.x = pos.x;
    at.y = pos.y;
    at.z = pos.z;
    at.w = facing;
    func_08870C94(self->camera, target);
}
