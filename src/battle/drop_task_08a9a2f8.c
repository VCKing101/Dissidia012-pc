typedef struct DropBody {
    char pad_000[0x188];
    unsigned int flags;
} DropBody;

typedef struct DropSlot {
    char pad_00[0x4];
    DropBody *body;
} DropSlot;

typedef struct DropTask {
    unsigned char active;
    char pad_01[0x3];
    int id;
    int sound;
    int effect;
} DropTask;

extern DropSlot *func_08AB3264(int id);
extern int func_08A65904(int sound);
extern void func_08834DF4(DropBody *body, float scale);
extern void func_08B34ED0(DropBody *body, int mode);
extern void func_08A65A84(int sound);
extern void func_08A66390(int effect);

/* Lets the body fall when its sound has finished, then restarts the sound and effect; -1 inactive, -2 no body. */
int func_08A9A2F8(DropTask *self)
{
    DropBody *body;
    if (self->active == 0) {
        return -1;
    }
    body = func_08AB3264(self->id)->body;
    if (body == 0) {
        return -2;
    }
    if (func_08A65904(self->sound) == 0) {
        body->flags &= ~0x10;
        func_08834DF4(body, 1.0f);
        func_08B34ED0(body, 2);
    }
    func_08A65A84(self->sound);
    func_08A66390(self->effect);
    return 0;
}
