typedef struct Effect { char data_00[0x10]; unsigned int flags; } Effect;
typedef struct Owner { char data_00[0xC]; void *resource; Effect *effect; } Owner;
extern void func_0887BA5C(Effect *e);
extern Effect *func_0887E534(void *resource);
extern void func_0887B9C4(Effect *e);
extern void func_0887B98C(Effect *e);

void func_08884F98(Owner *o)
{
    if (o->effect != 0) {
        func_0887BA5C(o->effect);
        o->effect = 0;
    }
    o->effect = func_0887E534(o->resource);
    if (o->effect != 0) {
        func_0887B9C4(o->effect);
        func_0887B98C(o->effect);
        o->effect->flags |= 0x800;
    }
}
