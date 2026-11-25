/* For each 0x1C-byte entry of the list (entries start at +0x10, count at +8) whose +0x14 is zero, calls func_088A2664 on its handle at +4 with volume 0. */
typedef struct Ent { char p[4]; int h; char q[0xC]; int busy; char r[4]; } Ent;
typedef struct List { char p[8]; int count; char q[4]; Ent ents[1]; } List;
typedef struct S { List *list; } S;
extern void func_088A2664(int h, float f);
void func_08895FE4(S *self) {
    int i;
    Ent *e;
    if (self->list != 0) {
        for (e = self->list->ents, i = 0; i < self->list->count; i++, e++) {
            if (e->busy == 0) {
                func_088A2664(e->h, 0.0f);
            }
        }
    }
}
