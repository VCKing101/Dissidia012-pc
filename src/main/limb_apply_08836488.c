/* Applies the first limb's pose to the unit when it has one. */
typedef struct Pose08836488 {
    float v[4];
} Pose08836488;

typedef struct Unit08836488 {
    char pad_000[0x2FC];
    void *limb;
    void *limb2;
} Unit08836488;

void func_08836DAC(Unit08836488 *u, void *limb, Pose08836488 *out);
void func_08836120(Unit08836488 *u, float t, Pose08836488 *pose, unsigned char flag);

void func_08836488(Unit08836488 *u, float t, unsigned char flag)
{
    Pose08836488 pose;
    if (u->limb == 0) {
        return;
    }
    func_08836DAC(u, u->limb, &pose);
    func_08836120(u, t, &pose, flag);
}
