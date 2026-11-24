/* Initialises the global object at 0x09B6C2C8 and stores its address at +0x1974. */
struct Blk { char pad[0x1974]; void *obj; };
extern char D_09B6C2C8[];
extern void func_08935524(void *);
void func_08B532B4(struct Blk *self)
{
    char *p = D_09B6C2C8;
    func_08935524(p);
    self->obj = p ? p : 0;
}
