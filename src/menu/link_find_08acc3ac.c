/* Looks up `id`; when that finds nothing, tries each id linked to it (up to
   16) and returns the first hit. */
struct Screen08ACC3AC;

extern int func_08ACC224(struct Screen08ACC3AC *self, int id, int arg);
extern int func_088B9DA8(short *out, int max, int id);

int func_08ACC3AC(struct Screen08ACC3AC *self, short id, int arg)
{
    short links[16];
    int found;
    int count;
    int i;
    found = func_08ACC224(self, id, arg);
    if (found == 0) {
        count = func_088B9DA8(links, 0x10, id);
        for (i = 0; i < count; i++) {
            found = func_08ACC224(self, links[i], arg);
            if (found != 0) {
                break;
            }
        }
    }
    return found;
}
