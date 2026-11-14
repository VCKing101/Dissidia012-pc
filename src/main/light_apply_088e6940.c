/* Applies an active light: selects its definition, then sets its value and strength. */
typedef struct LightDef {
    short id;
} LightDef;

typedef struct LightApply {
    int value;
    LightDef *def;
    char pad_08[0x18 - 8];
    float strength;
    char pad_1C[4];
    unsigned char active;
} LightApply;

extern void *D_089D6C30;
extern void func_0889F2D4(void *lights, int id);
extern void func_0889F37C(void *lights, int value, float strength);

void func_088E6940(LightApply *light)
{
    if (light->active == 0) {
        return;
    }
    func_0889F2D4(D_089D6C30, light->def->id);
    func_0889F37C(D_089D6C30, light->value, light->strength);
}
