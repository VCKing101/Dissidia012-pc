/* Course gauge: advances the gauge by speed * time up to its end; at the
   end stops the sound, plays sound 0x61A82 and moves to mode 2. Then sets
   the motions of its six models for the new position. */
struct Gauge29 {
    char pad_00[0x8];
    void *icon;       /* +0x08 */
    void *models[6];  /* +0x0C, used last to first */
    void *digits[6];  /* +0x24, used last to first */
    int mode;         /* +0x3C */
    int end;          /* +0x40 */
    int pos;          /* +0x44 */
    int speed;        /* +0x48 */
    char pad_4C[0x4];
    int sound;        /* +0x50 */
};

struct Min29 {
    static int Of(int a, int b) {
        return b < a ? b : a;
    }
};

extern "C" {
extern const unsigned char D_089EC2D0[];
void func_088A1E44(float time);
int func_088A24D4(int arg1, int id, int arg2, int arg3);
void func_089EB50C(Gauge29 *self, int *motions, int pos, const unsigned char *table);
void func_088D9F5C(void *model, int motion, bool arg);

void func_089EB2F8(Gauge29 *self, float time)
{
    self->pos = Min29::Of(self->pos + (int)((float)self->speed * time), self->end);
    int pos = self->pos;
    if (pos == self->end) {
        if (self->sound != 0) {
            func_088A1E44(0.0f);
            self->sound = 0;
        }
        func_088A24D4(0, 0x61A82, 1, 0);
        self->mode = 2;
    }
    void *models[6] = {self->models[5], self->models[4], self->models[3],
                       self->models[2], self->models[1], self->models[0]};
    int motions[6];
    func_089EB50C(self, motions, self->pos, D_089EC2D0);
    for (int i = 0; i < 6; i++) {
        func_088D9F5C(models[i], motions[i], true);
    }
}
}
