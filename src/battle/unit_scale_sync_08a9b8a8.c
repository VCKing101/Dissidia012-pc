typedef struct ScaleSync {
    char pad_000[0x160];
    int id;
} ScaleSync;

extern void *func_08AB3264(int id);
extern void *func_08AB327C(int id);
extern int func_08AAFE14(void *unit);
extern float func_08AAFEB8(void *unit);
extern int func_08A975E0(void *holder, float scale);

int func_08A9B8A8(ScaleSync *self)
{
    void *holder = func_08AB3264(self->id);
    void *unit = func_08AB327C(self->id);

    if (func_08AAFE14(unit) == 1) {

        func_08A975E0(holder, func_08AAFEB8(unit));
    } else {
        func_08A975E0(holder, 0.0f);
    }
    return 0;
}
