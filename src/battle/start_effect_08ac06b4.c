/* Starts the effect on the object's target (offset 0x4C8) when the effect system is up; -1 inactive, -2 no system. */
typedef struct Sys38 { char pad[8]; void *fx; } Sys38;
typedef struct Obj38 { unsigned char active; char pad[0x37]; char *target; } Obj38;
extern void *func_088A9DE8(void *fx, int a, char *pos, int b, int c);
extern void func_088A6A38(float f, void *h, int mode, char *pos, int a, int b);
extern Sys38 *D_089D6DD0;
int func_08AC06B4(Obj38 *self)
{
    void *h;
    float neg;
    int m1;
    char *pos;
    if (self->active == 0) {
        return -1;
    }
    if (D_089D6DD0->fx == 0) {
        return -2;
    }
    h = func_088A9DE8(D_089D6DD0->fx, 0, self->target + 0x4C8, 0, 0);
    if (h != 0) {
        pos = self->target + 0x4C8;
        func_088A6A38(-1.0f, h, 2, pos, -1, -1);
    }
    return 0;
}
