typedef struct PlayTask {
    void *vtable;
    int field_04;
    int state;
    char pad_0C[0x4];
    float elapsed;
    char player[0x28];
    char rate[0x6C];
    void *anim;
} PlayTask;

extern float func_08891348(void *anim);
extern void func_08A16CC4(void *rate, float value);
extern void func_08A01C74(void *player, int value);
extern void func_08A16C58(void *rate);

/* Plays the animation at its own length; at the end restarts the parts and moves to state 3. */
int func_08A108AC(PlayTask *self, float dt)
{
    float length;
    self->elapsed = self->elapsed + dt;
    length = func_08891348(self->anim);
    if (length != 0.0f) {
        func_08A16CC4(self->rate, self->elapsed / length);
    }
    if (self->elapsed >= length) {
        func_08A01C74(self->player, 3);
        func_08A16C58(self->rate);
        self->state = 3;
        self->elapsed = 0.0f;
    }
    return 0;
}
