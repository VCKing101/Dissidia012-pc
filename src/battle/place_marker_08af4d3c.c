// Same as 08AF46F4 but the model lookup uses (joint - base at +0xC).
extern int func_08AF4A48(void *, int);
extern void func_088DA0A8(int, int, void *, void *);
extern void func_088D9C60(void *, int, int);
extern void func_088D9CD8(void *);
extern void func_088D9CFC(void *);
typedef struct Own57 {
    char pad0[8];
    int joint;
    int base;
    char pad1[4];
    int model;
} Own57;
void func_08AF4D3C(Own57 *self, void *marker)
{
    if (self->joint != -1) {
        float m[16];
        char tail[4];
        int j = func_08AF4A48(self, self->joint - self->base);
        func_088DA0A8(self->model, j, m, tail);
        func_088D9C60(marker, (int)m[12], (int)m[13]);
        func_088D9CD8(marker);
    } else {
        func_088D9CFC(marker);
    }
}
