// Returns 0 if func_0889877C(func_08899300(), entry i) is true for any of the entries (stride 0x14, count at +4), else 1.
extern int func_08899300(void);
extern int func_0889877C(int, int);
typedef struct { int v; char pad[0x10]; } Ent08B51A14;
typedef struct { Ent08B51A14 *ents; int count; } Obj08B51A14;
int func_08B51A14(Obj08B51A14 *self)
{
    int i;
    for (i = 0; i < self->count; i++) {
        if (func_0889877C(func_08899300(), self->ents[i].v) != 0) {
            return 0;
        }
    }
    return 1;
}
