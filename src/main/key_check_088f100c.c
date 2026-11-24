/* Returns 1 when func_088F0D98 is nonzero; otherwise 1 if func_088F0E04() equals func_089C5D70 of the key at D_09B698C8, or if func_088F0E34() is nonzero; else 0. */
typedef struct Key088f100c { unsigned short a, b, c; } Key088f100c;
extern Key088f100c D_09B698C8;
extern int func_088F0D98(void);
extern int func_088F0E04(void);
extern int func_088F0E34(void);
extern int func_089C5D70(unsigned a, unsigned b, unsigned c);
int func_088F100C(void)
{
    int result;
    int cur;
    int want;
    if (func_088F0D98() != 0) {
        return 1;
    }
    result = 0;
    cur = func_088F0E04();
    want = func_089C5D70(D_09B698C8.a, D_09B698C8.b, D_09B698C8.c);
    if (cur == want) {
        result = 1;
    } else {
        if (func_088F0E34() != 0) {
            result = 1;
        }
    }
    return result;
}
