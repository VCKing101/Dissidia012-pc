/* Returns whether bit 8 is set in the halfword at offset 0xE of the object func_08900D68 returns (0 if none). */
typedef struct Obj08900e9c { unsigned char pad[0xE]; unsigned short flags; } Obj08900e9c;
extern Obj08900e9c *func_08900D68(void);
int func_08900E9C(void)
{
    Obj08900e9c *o = func_08900D68();
    if (o != 0) {
        return (o->flags & 8) != 0;
    } else {
        return 0;
    }
}
