/* Advances the voice's fade level by dt and applies it. */
typedef struct Voice088A4B58 {
    char pad_00[0x28];
    float level;
    float step;
} Voice088A4B58;

void func_088A46FC(Voice088A4B58 *v, void *param);

void func_088A4B58(Voice088A4B58 *v, float dt)
{
    v->level = v->level - v->step * dt;
    if (v->level < 0.0f) {
        v->level = 0.0f;
    }
    func_088A46FC(v, v);
}
