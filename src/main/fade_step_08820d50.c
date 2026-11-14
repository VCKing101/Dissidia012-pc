/* Advances a fade by dt and stores its level (rising when dir is 1);
   nothing happens without a duration. */
typedef struct Fade08820D50 {
    char pad_00[0x1C];
    unsigned char dir;
    char pad_1D[3];
    float time;
    float duration;
    char pad_28[4];
    float level;
} Fade08820D50;

void func_08820D50(Fade08820D50 *fade, float dt)
{
    float duration = fade->duration;
    float t;
    float level;
    if (duration <= 0.0f) {
        return;
    }
    t = fade->time + dt;
    fade->time = t;
    if (!(fade->time <= duration)) {
        fade->time = duration;
        t = duration;
    }
    if (fade->dir == 1) {
        level = t / duration;
    } else {
        level = 1.0f - t / duration;
    }
    fade->level = level;
}
