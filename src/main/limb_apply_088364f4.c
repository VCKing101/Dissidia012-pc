/* Applies the second limb's pose to the unit when it has one. */
typedef struct Pose088364F4 {
    float v[4];
} Pose088364F4;

typedef struct Unit088364F4 {
    char pad_000[0x2FC];
    void *limb;
    void *limb2;
} Unit088364F4;

void func_08836DAC(Unit088364F4 *u, void *limb, Pose088364F4 *out);
void func_08836120(Unit088364F4 *u, float t, Pose088364F4 *pose, unsigned char flag);

void func_088364F4(Unit088364F4 *u, float t, unsigned char flag)
{
    Pose088364F4 pose;
    if (u->limb2 == 0) {
        return;
    }
    func_08836DAC(u, u->limb2, &pose);
    func_08836120(u, t, &pose, flag);
}
