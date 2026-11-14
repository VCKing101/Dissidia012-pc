typedef struct StageSet {
    unsigned char active;
} StageSet;

extern void func_08AB89E0(StageSet *self);
extern void func_08AB8DC0(StageSet *self);
extern void func_08AB915C(StageSet *self);
extern void func_08AB93BC(StageSet *self);

/* Run the four stage updates of an active set. */
int func_08AB898C(StageSet *self)
{
    if (self->active == 0) {
        return -1;
    }
    func_08AB89E0(self);
    func_08AB8DC0(self);
    func_08AB915C(self);
    func_08AB93BC(self);
    return 0;
}
