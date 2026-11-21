extern unsigned char D_089D5F1F;
extern unsigned char D_089D5F20;
extern int func_089C5D30(int a);

int func_0886F378(void *self, int a)
{
    if (D_089D5F1F != 0 || D_089D5F20 != 0) {
        return 0x80010002;
    }
    return func_089C5D30(a);
}
