/* Deleting destructor of a sound stream base. */
extern "C" {
extern char D_089CAC38[];
void func_08992F04(void *p);
}

struct Stream088A30D8 {
    void *vtable;

    static void destroy(Stream088A30D8 *s, unsigned int flags)
    {
        if (s != 0) {
            s->vtable = D_089CAC38;
            if (flags & 1) {
                func_08992F04(s);
            }
        }
    }
};

extern "C" {

void func_088A30D8(Stream088A30D8 *s, unsigned int flags)
{
    Stream088A30D8::destroy(s, flags);
}

}
