typedef struct Target { char data_00[0x30]; int field_30; int field_34; int field_38; } Target;
typedef struct Owner { char data_00[0x10]; Target *target; void **tasks; } Owner;
extern void func_0887BA5C(void *task);

void func_088D372C(Owner *o)
{
    int i;
    Target *t = o->target;
    if (t != 0) {
        t->field_34 = 0;
        t->field_30 = 0;
        t->field_38 = 0;
    }
    for (i = 0; i < 6; i++) {
        if (o->tasks[i] != 0) {
            func_0887BA5C(o->tasks[i]);
            o->tasks[i] = 0;
        }
    }
}
