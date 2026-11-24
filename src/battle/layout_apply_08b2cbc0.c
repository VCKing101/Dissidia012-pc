/* Applies a layout to the object at +4 and positions the two child objects at +8 / +0xC from the table at 0x08B2CCB8. */
typedef struct LSelf { char pad[4]; int obj; int child[2]; } LSelf;
extern int D_08B2CCB8[2];
extern void func_088D9B84(int obj, int a);
extern void func_088D9AF4(int obj, int b);
extern void func_088DA0A8(int obj, int key, void *m, void *v);
extern void func_0887B778(int child, void *m);
extern void func_0887B828(int child, void *v);
int func_08B2CBC0(LSelf *self, int a, int b)
{
    char matrix[0x40];
    char vec[4];
    int i;
    if (b == 0) {
        return -1;
    }
    if (self->obj != 0) {
        func_088D9B84(self->obj, a);
        func_088D9AF4(self->obj, b);
        for (i = 0; i < 2; i++) {
            if (self->child[i] == 0) {
                continue;
            }
            func_088DA0A8(self->obj, D_08B2CCB8[i], matrix, vec);
            func_0887B778(self->child[i], matrix);
            func_0887B828(self->child[i], vec);
        }
    }
    return 0;
}
