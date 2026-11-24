/* Returns 1 when the selected id matches the stored halfword and func_088856DC(D_089D69E8) is nonzero. */
typedef struct { char p0[0x41C]; short **p41C; } X;
typedef struct { char p0[0x41C]; struct { char p0[0xC]; short id; } *p41C; char p1[0x2E08 - 0x420]; unsigned short cur; } Sub;
typedef struct { char p0[0x140]; Sub *p140; } Obj;
extern int D_089D69E8;
extern int func_088856DC(int a);
int func_08AF1708(Obj *self) {
    Sub *s = self->p140;
    if (s->p41C->id == s->cur && func_088856DC(D_089D69E8) != 0) {
        return 1;
    } else {
        return 0;
    }
}
