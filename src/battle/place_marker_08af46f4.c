// If the owner has a joint index (+8, -1 = none), looks up its matrix and sets the marker position from the matrix floats at 0x30/0x34; otherwise clears the marker.
extern int func_08AF43EC(void *, int);
extern void func_088DA0A8(int, int, void *, void *);
extern void func_088D9C60(void *, int, int);
extern void func_088D9CD8(void *);
extern void func_088D9CFC(void *);
typedef struct Own57 {
    char pad0[8];
    int joint;
    char pad1[8];
    int model;
} Own57;
void func_08AF46F4(Own57 *self, void *marker)
{
    if (self->joint != -1) {
        float m[16];
        char tail[4];
        int j = func_08AF43EC(self, self->joint);
        func_088DA0A8(self->model, j, m, tail);
        func_088D9C60(marker, (int)m[12], (int)m[13]);
        func_088D9CD8(marker);
    } else {
        func_088D9CFC(marker);
    }
}
