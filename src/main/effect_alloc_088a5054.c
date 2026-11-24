/* Creates the "EFFECT_ALLOCATOR" block (func_089C5F38, 0x3DD000 bytes) into D_089D6D70; when valid, gets its base (func_089C5F20), builds the heap at D_09B4D420 (func_08867310), allocates 0x60000 (func_088673CC) into D_089D6D6C and registers it as "EFFECT SYSTEM MEMORY" (func_0885C40C, align 0x10) into D_089D6D68. Returns whether the block is valid. */
extern int D_089D6D70;
extern int D_089D6D6C;
extern int D_089D6D68;
extern char D_09B4D420[];
extern char D_089CAEF8[];
extern char D_089CAF0C[];
extern int func_089C5F38(char *name, int a, int size, int b);
extern void func_089C5F20(int h, void *out);
extern void func_08867310(void *heap, int base, unsigned size, int align);
extern int func_088673CC(void *heap, int size);
extern int func_088676B4(void *heap, int blk);
extern int func_0885C40C(int mem, unsigned size, char *name, int align);
int func_088A5054(void)
{
    D_089D6D70 = func_089C5F38(D_089CAEF8, 0, 0x3DD000, 0);
    if (D_089D6D70 < 0) {
        return 0;
    }
    {
        int base[1];
        int blk;
        func_089C5F20(D_089D6D70, base);
        func_08867310(D_09B4D420, base[0], 0x3DD000, 0x40);
        D_089D6D6C = func_088673CC(D_09B4D420, 0x60000);
        blk = func_088676B4(D_09B4D420, D_089D6D6C);
        D_089D6D68 = func_0885C40C(blk, 0x60000, D_089CAF0C, 0x10);
    }
    return 1;
}
