/* Returns the pointer stored at +0x196C of the global block, or a default object. */
struct Blk { char pad[0x196C]; void *cur; };
extern struct Blk *D_08B54330;
extern char D_09AFB89C;
void *func_08B53804(void)
{
    void *p;
    p = D_08B54330->cur;
    if (p) {
        return p;
    } else {
        return &D_09AFB89C;
    }
}
