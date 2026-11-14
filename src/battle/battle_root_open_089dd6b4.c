/* Opens the battle root: resets it, sets up every subsystem against the
 * context, and registers the per-frame update task. */
typedef struct RootTask {
    int field_00;
    void *owner;
} RootTask;

typedef struct BattleRoot {
    unsigned char open;
    unsigned char pad_01[3];
    RootTask *task;
    char pad_08[0x8];
    char field_10[0x8];
    char field_18[0x1C4];
    char field_1DC[0x868];
    char field_A44[0x48];
    char field_A8C[0x11E0];
    char field_1C6C[0x98];
    char field_1D04[0x4];
} BattleRoot;

extern void func_089DD604(BattleRoot *self);
extern void func_089E3358(void *sub, void *context);
extern void func_089E7B9C(void *sub, void *context);
extern void func_089E7258(void *sub, void *context, void *models);
extern void func_089E69F8(void *sub, void *context, void *link);
extern void func_089E082C(void *sub);
extern void func_089ED064(void *sub);
extern RootTask *func_088823C4(void *parent, int kind, int priority, void (*update)(RootTask *), char *name);
extern void func_089DDE7C(RootTask *task);
extern char D_089DDFC0[];
extern int func_089DDECC(BattleRoot *self);

int func_089DD6B4(BattleRoot *self, void *parent, void *context)
{
    func_089DD604(self);
    if (parent == 0 || context == 0) {
        return -1;
    }
    func_089E3358(self->field_18, context);
    func_089E7B9C(self->field_1DC, context);
    func_089E7258(self->field_A44, context, self->field_18);
    func_089E69F8(self->field_A8C, context, self->field_10);
    func_089E082C(self->field_1C6C);
    func_089ED064(self->field_1D04);
    self->task = func_088823C4(parent, 3, 0xD297, func_089DDE7C, D_089DDFC0);
    if (self->task != 0) {
        self->task->owner = self;
    }
    self->open = 1;
    func_089DDECC(self);
    return 0;
}
