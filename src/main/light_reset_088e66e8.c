/* Resets a light; `keep` keeps the colour words at 8/0xC. */
typedef struct LightState {
    int target;
    int field_04;
    int color;
    int color2;
    int source;
    int field_14;
    float fade;
    float time;
    unsigned char active;
} LightState;

extern const float D_089CC728;

void func_088E66E8(LightState *light, unsigned char keep)
{
    light->target = -1;
    light->field_04 = 0;
    if (!keep) {
        light->color = 0;
        light->color2 = 0;
    }
    light->source = -1;
    light->field_14 = 0;
    light->fade = D_089CC728;
    light->time = 0.0f;
    light->active = 0;
}
