/* Creates the effect handle on first use (target offset 0x4C8), then stores the given value in the object. */
typedef struct Sys38 { char pad[8]; void *fx; } Sys38;
typedef struct Obj38 { char pad[0x30]; void *handle; float value; char *target; } Obj38;
extern void *func_088A9DE8(void *fx, int a, char *pos, int b, int c);
extern void func_088A6A38(float f, void *h, int mode, char *pos, int a, int b);
extern Sys38 *D_089D6DD0;
int func_08AC0750(float value, Obj38 *self)
{
    char *pos;
    void *h;
    if (self->handle == 0 && D_089D6DD0->fx != 0) {
        pos = self->target + 0x4C8;
        h = self->handle = func_088A9DE8(D_089D6DD0->fx, 1, pos, 0, 0);
        if (h != 0) {
            func_088A6A38(-1.0f, h, 2, pos, -1, -1);
        }
    }
    self->value = value;
    return 0;
}
