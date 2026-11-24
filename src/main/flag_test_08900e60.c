/* Returns whether bit 2 is set in the halfword at offset 0xE of the object func_08900D68 returns (0 if none). */
typedef struct Obj08900e60 { unsigned char pad[0xE]; unsigned short flags; } Obj08900e60;
extern Obj08900e60 *func_08900D68(void);
int func_08900E60(void)
{
    Obj08900e60 *o = func_08900D68();
    if (o != 0) {
        return (o->flags & 2) != 0;
    } else {
        return 0;
    }
}
