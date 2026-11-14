/* Releases slot id from both pools: -1 when inactive, -2/-3 when a pool fails. */
typedef struct Owner0881F288 {
    unsigned char active;
    char pad_01[7];
    unsigned char first[0x1C];
    unsigned char second[4];
} Owner0881F288;

int func_0881F79C(void *pool, unsigned char id);
int func_088204B0(void *pool, unsigned char id);

int func_0881F288(Owner0881F288 *self, unsigned char id)
{
    if (!self->active) {
        return -1;
    }
    if (func_0881F79C(self->first, id) != 0) {
        return -2;
    }
    if (func_088204B0(self->second, id) != 0) {
        return -3;
    }
    return 0;
}
