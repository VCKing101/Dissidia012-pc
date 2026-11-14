typedef struct LightApply {
    char pad_00[0x20];
    unsigned char active;
} LightApply;

extern void *D_089D6C30;
extern void func_0889F340(void *lights, int value);

void func_088E699C(LightApply *light)
{
    if (light->active == 0) {
        return;
    }
    func_0889F340(D_089D6C30, 0);
}
