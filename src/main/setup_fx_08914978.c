/* Starts effect 2 with scale 16, clears a 4-float vector, then runs two follow-up steps on self. */
extern void func_08873D34(int a, int b, int c, int d, int e, float f);
extern void func_08873DB0(float *v, int n);
extern void func_08914BC8(void *self);
extern void func_08914B64(void *self, int n);
void func_08914978(void *self) {
    float v[4];
    func_08873D34(2, 0, 0, 0, 2, 16.0f);
    v[0] = 0.0f; v[1] = 0.0f; v[2] = 0.0f; v[3] = 0.0f;
    func_08873DB0(v, 2);
    func_08914BC8(self);
    func_08914B64(self, 2);
}
