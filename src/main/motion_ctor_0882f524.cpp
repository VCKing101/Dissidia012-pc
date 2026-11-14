/* Motion player constructor. */
struct Motion0882F524 {
    char pad_00[0x10];
    float pos[4];               /* 0x10 */
    char pad_20[0x10];
    void *actor;                /* 0x30 */
    char anim[0x14];            /* 0x34 */
    void *table;                /* 0x48 */
    void *entry;                /* 0x4C */
    short current;              /* 0x50 */
    short next;                 /* 0x52 */
    int time;                   /* 0x54 */
    unsigned int flags;         /* 0x58 */
    char pad_5C[4];
    char events[0x10];          /* 0x60 */
    float blend[8];             /* 0x70 */
    int event_count;            /* 0x90 */
    int a;                      /* 0x94 */
    int b;                      /* 0x98 */
    int c;                      /* 0x9C */
    char pad_A0[0x178 - 0xA0];
    float speed;                /* 0x178 */
};

extern "C" {

void func_0886F028(char *anim);
void func_0886F074(char *anim);
void func_08935828(char *dst, int value, int size);
void func_0882F470(Motion0882F524 *self);
void func_089A3890(float *v);
void func_0882FE00(Motion0882F524 *self);

Motion0882F524 *func_0882F524(Motion0882F524 *self, void *actor)
{
    float *pos = self->pos;
    char *anim = self->anim;
    self->actor = actor;
    func_0886F028(anim);
    self->table = 0;
    short none = -1;
    self->entry = 0;
    self->current = none;
    self->next = none;
    self->time = 0;
    char *events = self->events;
    self->flags = 0;
    func_08935828(events, 0, 0x10);
    float zero = 0.0f;
    self->event_count = 0;
    self->blend[0] = zero;
    self->blend[1] = zero;
    self->blend[2] = zero;
    self->blend[3] = zero;
    self->blend[4] = zero;
    self->blend[5] = zero;
    self->blend[6] = zero;
    self->blend[7] = zero;
    self->a = 0;
    self->b = 0;
    self->c = 0;
    self->speed = zero;
    func_0886F074(anim);
    func_0882F470(self);
    func_089A3890(pos);
    self->time = 0;
    self->flags = 0;
    func_08935828(events, 0, 0x10);
    func_0882FE00(self);
    return self;
}

}
