/* Entry count of the loaded table header; 0 when none is loaded. */
typedef struct Header088C50B0 {
    short field_00;
    unsigned short count;
} Header088C50B0;

extern Header088C50B0 *D_089D76D8;

unsigned short func_088C50B0(void)
{
    if (D_089D76D8 != 0) {
        return D_089D76D8->count;
    }
    return 0;
}
