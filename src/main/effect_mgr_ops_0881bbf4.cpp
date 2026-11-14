/* Effect manager operations that apply to every effect with a given id, of
   a given owner, or in a given state. All fail with -1 while the manager is
   not ready and do nothing while it is locked. */
struct EffectDef0881BBF4 {
    short id;
};

struct Effect0881BBF4 {
    Effect0881BBF4 *next;
    char pad_04[0x10 - 4];
    int state;
    void *owner;
    char pad_18[0x48 - 0x18];
    EffectDef0881BBF4 *def;
};

struct EffectMgr0881BBF4 {
    unsigned char ready;
    char pad_01[7];
    unsigned char locked;
    char pad_09[0x110 - 9];
    Effect0881BBF4 *first;

    static Effect0881BBF4 *next(EffectMgr0881BBF4 *m, Effect0881BBF4 *e) { return e == 0 ? m->first : e->next; }
};

extern "C" {

void func_08818E48(Effect0881BBF4 *e);
void func_08818F00(Effect0881BBF4 *e, int arg, int flags);
void func_08818F4C(Effect0881BBF4 *e);
void func_08819140(Effect0881BBF4 *e);

/* Stops every effect with the id. */
int func_0881BBF4(EffectMgr0881BBF4 *self, int id)
{
    if (!self->ready) {
        return -1;
    }
    if (self->locked == 1) {
        return 0;
    }
    Effect0881BBF4 *e = 0;
    while ((e = EffectMgr0881BBF4::next(self, e)) != 0) {
        if (e->def->id != id) {
            continue;
        }
        func_08818E48(e);
    }
    return 0;
}

/* Passes an argument to every effect with the id. */
int func_0881BC9C(EffectMgr0881BBF4 *self, int id, int arg)
{
    if (!self->ready) {
        return -1;
    }
    if (self->locked == 1) {
        return 0;
    }
    Effect0881BBF4 *e = 0;
    while ((e = EffectMgr0881BBF4::next(self, e)) != 0) {
        if (e->def->id != id) {
            continue;
        }
        func_08818F00(e, arg, 0);
    }
    return 0;
}

/* Ends every effect with the id. */
int func_0881BD58(EffectMgr0881BBF4 *self, int id)
{
    if (!self->ready) {
        return -1;
    }
    if (self->locked == 1) {
        return 0;
    }
    Effect0881BBF4 *e = 0;
    while ((e = EffectMgr0881BBF4::next(self, e)) != 0) {
        if (e->def->id != id) {
            continue;
        }
        func_08818F4C(e);
    }
    return 0;
}

/* Stops every effect of the owner. */
int func_0881BE00(EffectMgr0881BBF4 *self, void *owner)
{
    if (!self->ready) {
        return -1;
    }
    if (owner == 0) {
        return -2;
    }
    if (self->locked == 1) {
        return 0;
    }
    Effect0881BBF4 *e = 0;
    while ((e = EffectMgr0881BBF4::next(self, e)) != 0) {
        if (e->owner != owner) {
            continue;
        }
        func_08818E48(e);
    }
    return 0;
}

/* Ends every effect of the owner. */
int func_0881BEB4(EffectMgr0881BBF4 *self, void *owner)
{
    if (!self->ready) {
        return -1;
    }
    if (owner == 0) {
        return -2;
    }
    if (self->locked == 1) {
        return 0;
    }
    Effect0881BBF4 *e = 0;
    while ((e = EffectMgr0881BBF4::next(self, e)) != 0) {
        if (e->owner != owner) {
            continue;
        }
        func_08818F4C(e);
    }
    return 0;
}

/* Restarts every effect with the id. */
int func_0881BF68(EffectMgr0881BBF4 *self, int id)
{
    if (!self->ready) {
        return -1;
    }
    if (self->locked == 1) {
        return 0;
    }
    Effect0881BBF4 *e = 0;
    while ((e = EffectMgr0881BBF4::next(self, e)) != 0) {
        if (e->def->id != id) {
            continue;
        }
        func_08819140(e);
    }
    return 0;
}

/* Stops every effect that is in state 1. */
int func_0881C010(EffectMgr0881BBF4 *self)
{
    if (!self->ready) {
        return -1;
    }
    if (self->locked == 1) {
        return 0;
    }
    Effect0881BBF4 *e = 0;
    while ((e = EffectMgr0881BBF4::next(self, e)) != 0) {
        if (e->state == 1) {
            func_08818E48(e);
        }
    }
    return 0;
}

}
