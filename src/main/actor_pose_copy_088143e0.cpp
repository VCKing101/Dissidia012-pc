/* Copies the pose (a 64-byte matrix) of the object's actor out. */
struct Pose088143E0 {
    int m[16];
};

struct Obj088143E0 {
    int pad_0;
    void *actor;
};

extern "C" {

Pose088143E0 func_08836648(void *actor);
void func_08935524(void *dst, const void *src, int size);

void func_088143E0(Obj088143E0 *self, void *dst)
{
    Pose088143E0 pose;
    pose = func_08836648(self->actor);
    func_08935524(dst, &pose, sizeof(Pose088143E0));
}

}

/* The same copy for a second kind of object (identical code). */
extern "C" {

void func_0881449C(Obj088143E0 *self, void *dst)
{
    Pose088143E0 pose;
    pose = func_08836648(self->actor);
    func_08935524(dst, &pose, sizeof(Pose088143E0));
}

}
