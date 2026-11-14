typedef struct PlayTask {
    void *vtable;
    int field_04;
    int state;
    char pad_0C[0x4];
    float elapsed;
    char pad_14[0x2C];
    char part_40[0x28];
    char part_68[0x6C];
    void *anim;
} PlayTask;

extern float func_08891348(void *anim);
extern void func_08A16CC4(void *part, float rate);
extern void func_08A01C74(void *part, int value);
extern void func_08A16C58(void *part);

/* Plays the animation at its own length; at the end restarts the parts and moves to state 4. */
int func_08A0B930(PlayTask *self, float dt)
{
    float length;
    self->elapsed = self->elapsed + dt;
    length = func_08891348(self->anim);
    if (length != 0.0f) {
        func_08A16CC4(self->part_68, self->elapsed / length);
    }
    if (self->elapsed >= length) {
        func_08A01C74(self->part_40, 6);
        func_08A16C58(self->part_68);
        self->state = 4;
        self->elapsed = 0.0f;
    }
    return 0;
}
