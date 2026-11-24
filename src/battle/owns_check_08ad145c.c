// Returns 1 when the word at +0x68 of self equals other, other's word at +0x1108, or any of other's 16 entries (func_0884F6B8 on +0xD64); else 0.
extern int func_0884F6B8(void *, int);
typedef struct { char pad[0x68]; void *owner; } Obj08AD145C;
typedef struct { char pad0[0xD64]; char list[0x1108 - 0xD64]; void *parent; } Other08AD145C;
int func_08AD145C(Obj08AD145C *self, Other08AD145C *other0)
{
    void *id = self->owner;
    Other08AD145C *other = other0;
    int i;
    if (id == other) {
        return 1;
    } else if (id == other->parent) {
        return 1;
    } else {
        i = 0;
        {
            char *list = other->list;
            for (; i < 16; i++) {
                if (id == (void *)func_0884F6B8(list, i)) {
                    return 1;
                }
            }
        }
    }
    return 0;
}
