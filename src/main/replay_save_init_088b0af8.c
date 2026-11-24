// Fills the replay save descriptor at obj+0x2B84 (file, titles, icons, callback) and clears its 0xBEA0-byte buffer, then calls func_088B04AC.
typedef struct {
    char pad[0x2B84];
    int enabled;
    int size;
    const char *file;
    const char *title;
    const char *detail;
    const char *icon;
    const char *icon_new;
    void *callback;
    int pad2;
    char buffer[1];
} Save47;
extern char D_089CB718[], D_089CB724[], D_089CB708[], D_089CB72C[], D_089CB740[];
extern int D_089D7260;
extern void func_088B0CAC(void);
extern void func_08935828(void *, int, unsigned);
extern void func_088B04AC(int, void *, void *, void *, void *);
void func_088B0AF8(Save47 *s)
{
    char *buf;
    s->enabled = 1;
    s->size = 0xBEA0;
    s->file = D_089CB718;
    s->title = D_089CB724;
    s->detail = D_089CB708;
    s->icon = D_089CB72C;
    s->icon_new = D_089CB740;
    s->callback = (void *)func_088B0CAC;
    buf = s->buffer;
    func_08935828(buf, 0, 0xBEA0);
    func_088B04AC(D_089D7260, s, buf, buf, s);
}
