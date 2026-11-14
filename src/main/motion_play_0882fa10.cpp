/* Motion player: start requests. Nothing happens without an animation set
   or while a forced motion is running; `force` makes the new one forced. */
struct Motion0882FA10 {
    char pad_00[0x48];
    void *anims;
    char pad_4C[4];
    short current;
    char pad_52[6];
    unsigned int flags;

    static bool loaded(Motion0882FA10 *m) { return m->anims != 0; }
    static bool locked(Motion0882FA10 *m) { return (m->flags & 8) != 0; }
    static bool differs(Motion0882FA10 *m, int id) { return m->current != id; }
};

extern "C" {

void func_0882FBF0(Motion0882FA10 *self);
void func_0882F0D4(Motion0882FA10 *self, int id, float time, float start);

/* Starts a motion. */
void func_0882FA10(Motion0882FA10 *self, int id, unsigned char force, float time, float start)
{
    if (!Motion0882FA10::loaded(self)) {
        return;
    }
    if (Motion0882FA10::locked(self)) {
        return;
    }
    if (force) {
        self->flags |= 8;
    }
    func_0882FBF0(self);
    func_0882F0D4(self, id, time, start);
}

/* Starts a motion unless it is already the current one. */
void func_0882FAC4(Motion0882FA10 *self, int id, unsigned char force, float time, float start)
{
    if (!Motion0882FA10::loaded(self)) {
        return;
    }
    if (Motion0882FA10::locked(self)) {
        return;
    }
    if (Motion0882FA10::differs(self, id)) {
        if (force) {
            self->flags |= 8;
        }
        func_0882FBF0(self);
        func_0882F0D4(self, id, time, start);
    }
}

}
