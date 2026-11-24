// If the byte at +0x10 is set: looks up the object for short +0x20 of the record at +4, notifies it with func_08A6419C, runs func_08B44E90 and returns 0; else returns 1.
extern void *func_08916FB0(int);
extern void func_08A6419C(int, int);
extern void func_08B44E90(void *);
typedef struct { char pad[0x20]; short id; short arg; } Rec08B445A0;
typedef struct { char pad0[4]; Rec08B445A0 *rec; char pad1[8]; unsigned char on; } Obj08B445A0;
typedef struct { char pad[0x5C4]; int w; } Tgt08B445A0;
int func_08B445A0(Obj08B445A0 *self)
{
    if (self->on != 0) {
        Tgt08B445A0 *t = (Tgt08B445A0 *)func_08916FB0(self->rec->id);
        if (t != 0) {
            func_08A6419C(t->w, self->rec->arg);
        }
        func_08B44E90(self);
        return 0;
    }
    return 1;
}
