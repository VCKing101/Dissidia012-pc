struct Extra { char pad_00[0x2]; unsigned char enabled; char pad_03[0x1]; };
struct Owner { char pad_00[0x10]; int source; };
extern "C" {
extern void func_088DA0A8(int source, int id, char *params, Extra *extra);
extern int func_089EC318(Owner *self);
extern void func_0887B778(void *target, char *params);
extern void func_0887B828(void *target, Extra *extra);

/* Copies the sound parameters for id into target; -1 bad arguments, -2 no source. */
int func_089ECA94(Owner *self, void *target, int id)
{
    char params[0x40];
    Extra extra;
    if (target == 0 || id < 0) {
        return -1;
    }
    if (self->source == 0) {
        return -2;
    }
    func_088DA0A8(self->source, id, params, &extra);
    if (func_089EC318(self) == 1) {
        extra.enabled = 0;
    }
    func_0887B778(target, params);
    func_0887B828(target, &extra);
    return 0;
}
}
