/* Returns whether the byte at offset 8 of the block pointed to by field 0xC of the object func_08900D84 returns is nonzero (0 if none). */
typedef struct Inner08900f14 { unsigned char pad[8]; char flag; } Inner08900f14;
typedef struct Obj08900f14 { unsigned char pad[0xC]; Inner08900f14 *inner; } Obj08900f14;
extern Obj08900f14 *func_08900D84(void);
int func_08900F14(void)
{
    Obj08900f14 *o = func_08900D84();
    if (o != 0) {
        return o->inner->flag != 0;
    } else {
        return 0;
    }
}
