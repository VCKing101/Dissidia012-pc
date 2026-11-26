/* Finishes a save's name and texts: copies the title, then builds the detail text (optional extra line, separator, text, separator, stamp line) into the 0x400-byte buffer at +0x100 and sets the two trailing kind bytes. */
typedef struct Obj {
    char pad[0xEE0];
    int unk_EE0;
} Obj;
typedef struct Text {
    char title[0x100];
    char detail[0x400];
    char pad[0x500 - 0x500];
    unsigned char kind;
    unsigned char kind2;
} Text;
extern char D_089CB434[];
extern int D_089D712C;
char *func_0887AFBC(int a, int b, int c);
void func_08935BB4(void *dst, void *src, int n);
int func_089359E8(void *s);
void func_0893FDCC(void *dst, int n, void *src);
void func_088ABAF4(Obj *obj, void *buf, int n, int c);

void func_088ADC08(Obj *obj, Text *text, char *extra)
{
    char buf[128];
    int len;
    int tag;
    char *detail;
    int size;
    int dead;
    func_08935BB4(text->title, func_0887AFBC(obj->unk_EE0, 0, 0), 0x80);
    len = 0;
    size = 0x400;
    tag = obj->unk_EE0;
    detail = text->detail;
    if (extra != 0) {
        func_08935BB4((char *)(len + (int)text + 0x100), func_0887AFBC(tag, 4, 0), size);
        len = func_089359E8(detail);
        func_08935BB4((char *)(len + (int)text + 0x100), extra, size - len);
        len = func_089359E8(detail);
        func_08935BB4((char *)(len + (int)text + 0x100), D_089CB434, size - len);
        len = func_089359E8(detail);
        dead = len;
        tag = obj->unk_EE0;
    }
    func_08935BB4((char *)(len + (int)text + 0x100), func_0887AFBC(tag, 2, 0), size - len);
    len = func_089359E8(detail);
    func_0893FDCC((char *)(len + (int)text + 0x100), size - len, (char *)D_089D712C + 0x1FE08);
    len = func_089359E8(detail);
    func_08935BB4((char *)(len + (int)text + 0x100), D_089CB434, size - len);
    len = func_089359E8(detail);
    func_08935BB4((char *)(len + (int)text + 0x100), func_0887AFBC(obj->unk_EE0, 3, 0), size - len);
    len = func_089359E8(detail);
    func_088ABAF4(obj, buf, 0x80, -1);
    func_0893FDCC((char *)(len + (int)text + 0x100), size - len, buf);
    text->kind = 5;
    text->kind2 = 7;
}
