/* Returns whether bit 0x10 is set in the halfword at offset 0xE of the object func_08900D68 returns (0 if none). */
typedef struct Obj08900ed8 { unsigned char pad[0xE]; unsigned short flags; } Obj08900ed8;
extern Obj08900ed8 *func_08900D68(void);
int func_08900ED8(void)
{
    Obj08900ed8 *o = func_08900D68();
    if (o != 0) {
        return (o->flags & 0x10) != 0;
    } else {
        return 0;
    }
}
