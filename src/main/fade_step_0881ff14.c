/* Advances a fade by dt and stores its level (rising when dir is 1). */
typedef struct Fade0881FF14 {
    char pad_00[0x24];
    unsigned char dir;
    char pad_25[3];
    float time;
    float duration;
    char pad_30[8];
    float level;
} Fade0881FF14;

void func_0881FF14(Fade0881FF14 *fade, float dt)
{
    float duration = fade->duration;
    unsigned char dir = fade->dir;
    float t;
    if (duration <= 0.0f) {
        fade->level = dir == 1 ? 1.0f : 0.0f;
    } else {
        t = fade->time + dt;
        fade->time = t;
        if (!(fade->time <= duration)) {
            fade->time = duration;
            t = duration;
        }
        float level;
        if (dir == 1) {
            level = t / duration;
        } else {
            level = 1.0f - t / duration;
        }
        fade->level = level;
    }
}
