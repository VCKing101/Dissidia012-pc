struct Obj0884F48C {
    int id;                            /* +0x00 */
    unsigned char unk_04[2];
    unsigned short flags;              /* +0x06 bit 4 */
};

struct Owner0884F48C {
    unsigned char unk_000[0x5A4];
    unsigned char *kind;               /* +0x5A4 */
};

struct Entry0884F48C {
    unsigned char unk_00[0x12];
    short id;                          /* +0x12 0 = unused */
    unsigned char unk_14[0xC];
};

struct Fighters0884F48C {
    Entry0884F48C entries[16];         /* +0x000 */
    Owner0884F48C *owner;              /* +0x200 */
};

extern "C" {
void func_0884F010(Entry0884F48C *e);
Obj0884F48C *func_08850A90(Obj0884F48C *prev, unsigned char kind);
void func_08850574(Fighters0884F48C *self, int id, int a, int b, int c, int d, int e, int f, int g);

void func_0884F48C(Fighters0884F48C *self)
{
    int i = 0;
    unsigned char kind;
    Obj0884F48C *obj;

    for (Entry0884F48C *e = &self->entries[i]; i < 16; e++, i++) {
        if (!(unsigned char)(e->id == 0)) {
            func_0884F010(e);
        }
    }
    obj = 0;
    kind = *self->owner->kind;
    while ((obj = func_08850A90(obj, kind)) != 0) {
        if ((unsigned char)((obj->flags & 0x10) != 0)) {
            func_08850574(self, obj->id, -1, 0, 0, 0, 0, 5, 0);
        }
    }
}
}
