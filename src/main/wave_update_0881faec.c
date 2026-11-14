typedef struct Wave {
    char pad0[0x4];
    unsigned char active;
    char pad5[0x2B];
    float x;
    float y;
    float z;
    float volume;
} Wave;
extern void func_0881FE78(Wave *wave, float dt);
extern void func_0881FF98(Wave *wave, float dt);
extern void func_0881FF14(Wave *wave, float dt);
int func_0881FAEC(float dt, Wave *wave)
{
    if (wave->active == 0) {
        return -1;
    }
    func_0881FE78(wave, dt);
    func_0881FF98(wave, dt);
    func_0881FF14(wave, dt);
    wave->volume = wave->x * wave->y * wave->z;
    return 0;
}
