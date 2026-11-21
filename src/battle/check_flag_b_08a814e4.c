/* Returns -1 when flag 11 is set on the object's resource, else 0. */
typedef struct Obj38 { char pad[8]; void *res; } Obj38;
extern int func_08A97060(void *obj, int id);
int func_08A814E4(Obj38 *self)
{
    if (func_08A97060(self->res, 11)) {
        return -1;
    } else {
        return 0;
    }
}
