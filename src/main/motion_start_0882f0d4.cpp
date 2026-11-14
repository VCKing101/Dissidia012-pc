/* Starts motion `id` on the motion player: tells the actor, looks the
   motion up in the animation set and starts its animation. */
struct Actor0882F0D4 {
    char data[0x544];
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void motion_changed(int id);
};

struct MotionEntry0882F0D4 {
    short key;
    unsigned short anim;
    int data;
};

struct MotionTable0882F0D4 {
    int pad_0;
    int count;
    MotionEntry0882F0D4 entries[1];
};

struct Motion0882F0D4 {
    char pad_00[0x30];
    Actor0882F0D4 *actor;
    char pad_34[0x48 - 0x34];
    MotionTable0882F0D4 *table;
    MotionEntry0882F0D4 *entry;
    short current;
};

extern "C" {

int func_0882EF04(const void *a, const void *b);
int func_0882FE28(Motion0882F0D4 *self, int id);
MotionEntry0882F0D4 *func_08934944(int key, MotionEntry0882F0D4 *base, int count, int size, int (*cmp)(const void *, const void *));
void func_0882EF10(Motion0882F0D4 *self, int anim, float time, float start);

void func_0882F0D4(Motion0882F0D4 *self, int id, float time, float start)
{
    self->actor->motion_changed(id);
    self->current = id;
    int key = func_0882FE28(self, id);
    self->entry = func_08934944(key, self->table->entries, self->table->count, sizeof(MotionEntry0882F0D4), func_0882EF04);
    func_0882EF10(self, self->entry->anim, time, start);
}

}
