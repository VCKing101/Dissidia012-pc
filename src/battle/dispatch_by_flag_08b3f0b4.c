// Calls func_08B3E8B0 then, by bit 0 of the flag word of the part at +0x1E0 of b, func_08B3D8C0 or func_08B3E8D0 with the same two arguments.
extern void func_08B3E8B0(void *, void *);
extern void func_08B3D8C0(void *, void *);
extern void func_08B3E8D0(void *, void *);
typedef struct { char pad[0xBC]; unsigned flags; } Part08B3F0B4;
typedef struct { char pad[0x1E0]; char part[1]; } Obj08B3F0B4;
void func_08B3F0B4(void *a, Obj08B3F0B4 *b)
{
    void *aa = a;
    Obj08B3F0B4 *bb = b;
    func_08B3E8B0(aa, bb);
    if (((Part08B3F0B4 *)bb->part)->flags & 1) {
        func_08B3D8C0(aa, bb);
    } else {
        func_08B3E8D0(aa, bb);
    }
}
