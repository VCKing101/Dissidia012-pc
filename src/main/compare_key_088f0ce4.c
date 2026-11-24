/* Returns whether func_088F0C80() equals func_089C5D70 of the three halfwords at D_09B698C8. */
typedef struct Key088f0ce4 { unsigned short a, b, c; } Key088f0ce4;
extern Key088f0ce4 D_09B698C8;
extern int func_088F0C80(void);
extern int func_089C5D70(unsigned a, unsigned b, unsigned c);
int func_088F0CE4(void)
{
    int cur = func_088F0C80();
    return cur == func_089C5D70(D_09B698C8.a, D_09B698C8.b, D_09B698C8.c);
}
