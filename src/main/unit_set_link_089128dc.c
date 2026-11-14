extern void func_089A34C0(void *dst, const void *src);
typedef struct Vec3 { float x; float y; float z; } Vec3;
typedef struct UnitLinks {
    char pad_000[0x1D4];
    unsigned char flags[4];     /* 0x1D4 */
    Vec3 from[4];               /* 0x1D8 */
    Vec3 to[4];                 /* 0x208 */
    short ids[4];               /* 0x238 */
} UnitLinks;
void func_089128DC(UnitLinks *unit, int index, const Vec3 *from, const Vec3 *to, int id, unsigned char on)
{
    func_089A34C0(&unit->from[index], from);
    func_089A34C0(&unit->to[index], to);
    unit->ids[index] = id;
    if (on) {
        *((unsigned char *)unit + 0x1D4 + index) |= 1;
    }
}
