/* Effect manager: loads or unloads the resources of an effect definition
   and updates the manager's table for it. */
struct EffectDef0881C0A8 {
    short id;
    char pad_02[0x18 - 2];
    short parts;
};

struct EffectMgr0881C0A8 {
    unsigned char ready;
    char pad_01[7];
    unsigned char locked;
};

extern "C" {

void *func_08899300(void);
void func_088981B4(void *cache, int id, unsigned char flag, int load);
void func_0881C3F0(EffectMgr0881C0A8 *self, int id, unsigned char flag);
void func_088983C8(void *cache, int id, unsigned char flag);
int func_0881E83C(void *table, EffectDef0881C0A8 *def, int index);

extern void *D_089D4910;

/* Releases the definition's resources. */
int func_0881C0A8(EffectMgr0881C0A8 *self, EffectDef0881C0A8 *def, unsigned char flag)
{
    if (!self->ready) {
        return -1;
    }
    if (def == 0) {
        return -2;
    }
    if (self->locked == 1) {
        return 0;
    }
    func_088981B4(func_08899300(), def->id, flag, 0);
    func_0881C3F0(self, def->id, flag);
    return 0;
}

/* Loads the definition's resources. */
int func_0881C148(EffectMgr0881C0A8 *self, EffectDef0881C0A8 *def, unsigned char flag)
{
    if (!self->ready) {
        return -1;
    }
    if (def == 0) {
        return -2;
    }
    if (self->locked == 1) {
        return 0;
    }
    func_088981B4(func_08899300(), def->id, flag, 1);
    func_0881C3F0(self, def->id, flag);
    return 0;
}

/* Marks the definition and its parts as used in the cache. */
int func_0881C1E8(EffectMgr0881C0A8 *self, EffectDef0881C0A8 *def, unsigned char flag)
{
    if (!self->ready) {
        return -1;
    }
    if (def == 0) {
        return -2;
    }
    if (self->locked == 1) {
        return 0;
    }
    void *cache = func_08899300();
    func_088983C8(cache, def->id, flag);
    for (int i = 0; i < def->parts; i++) {
        func_088983C8(cache, func_0881E83C(D_089D4910, def, i), flag);
    }
    return 0;
}

/* Loads the definition and all its parts. */
int func_0881C2CC(EffectMgr0881C0A8 *self, EffectDef0881C0A8 *def, unsigned char flag)
{
    if (!self->ready) {
        return -1;
    }
    if (def == 0) {
        return -2;
    }
    if (self->locked == 1) {
        return 0;
    }
    void *cache = func_08899300();
    func_088981B4(cache, def->id, flag, 1);
    func_0881C3F0(self, def->id, flag);
    for (int i = 0; i < def->parts; i++) {
        int part = func_0881E83C(D_089D4910, def, i);
        func_088981B4(cache, part, flag, 1);
        func_0881C3F0(self, part, flag);
    }
    return 0;
}

}
