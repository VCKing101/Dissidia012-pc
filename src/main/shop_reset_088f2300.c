/* Resets the shop with flag 1 and clears the stock ids. */
extern void func_088F229C(void *shop, int flag);
extern void func_088F2340(void);

void func_088F2300(void *shop)
{
    func_088F229C(shop, 1);
    func_088F2340();
}
