/* Initialises a link: copies the source field 0x14, calls func_088371C4 when set, links the source. */
struct Src { char pad[0x14]; int h; };
struct Link { int a; struct Src *src; int h; float f; };
extern void func_088371C4(void);
void func_08B5F654(struct Link *self, struct Src *src)
{
    struct Link *me = self;
    int h = src->h;
    me->h = h;
    if (h != 0) {
        func_088371C4();
    }
    me->src = src;
    me->f = 0.0f;
    me->a = 0;
}
