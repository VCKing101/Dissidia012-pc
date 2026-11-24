/* Advances a timer by the frame step and starts an effect once it reaches five frame-lengths. */
extern struct Step { char pad0[0x18]; float dt; } *D_089D69E0;
extern float D_089C9F54;
extern void func_08A6E2CC(void *, int, int, int);
void func_08A6DF50(char *self, char *other) {
    float *tp = (float *)(self + 0xC);
    float elapsed = *tp + D_089D69E0->dt;
    float five = 5.0f;
    float limit;
    *tp = elapsed;
    limit = D_089C9F54 * five;
    if (elapsed < limit) {
    } else {
        func_08A6E2CC(other + 0x1D4, 7, 0, 0);
    }
}
