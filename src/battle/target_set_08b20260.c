/* Stores a target pointer on the side's battle object and its sub-object; -1 null, -2 no object, -3 no sub-object. */
typedef struct TSub { char pad[0x120]; void *target; } TSub;
typedef struct TObj { char pad[0x5A0]; void *target; char pad_5a4[8]; TSub *sub; } TObj;
typedef struct TRoot { char pad[0x38]; TObj *obj[2]; } TRoot;
typedef struct TSelf { char pad[0x10]; int side; } TSelf;
extern TRoot *D_089D6A10;
int func_08B20260(TSelf *self, void *target)
{
    TObj *o;
    TSub *sub;
    if (target == 0) {
        return -1;
    }
    o = D_089D6A10->obj[self->side];
    if (o == 0) {
        return -2;
    }
    sub = o->sub;
    if (sub == 0) {
        return -3;
    }
    o->target = target;
    sub->target = target;
    return 0;
}
