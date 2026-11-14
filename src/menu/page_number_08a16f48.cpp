/* Screen181E0: rebuilds the page number text ("%d/%d" from D_08A18910
   with the page +0xC98 plus one and the page count +0xC94) in the text
   slot that D_08A188C6 names. Standalone: func_0893476C is called with
   four arguments, which the region header's prototype does not allow. */
typedef struct PageWork29 {
    char pad_000[0x880];
    void *texts[0xDF];
    char text[0x80]; /* +0xBFC */
    unsigned int field_C7C;
    char pad_C80[0x14];
    int count; /* +0xC94 */
    int page;  /* +0xC98 */
} PageWork29;

typedef struct PageScreen29 {
    void *vtable;
    char pad_04[0xD7C];
    PageWork29 *work; /* +0xD80 */
} PageScreen29;

typedef struct PageText29 {
    short field_00;
    short field_02;
    unsigned short slot;
} PageText29;

extern "C" {
extern void *D_089D6998;
extern const PageText29 D_08A188C6;
extern const char D_08A18910[];
void func_0887BA5C(void *text);
void func_0893476C(char *out, const char *format, int arg1, int arg2);
void func_0887DF14(void *owner, char *out, int size, const char *text);
void *func_0887D868(void *owner, int arg1, const char *text, int arg3, int arg4, int arg5, int arg6, int arg7);
}


/* Inlined calls on the object in D_089D6998. */
struct Object9D6998 {
    static void Convert(char *out, int size, const char *text) {
        func_0887DF14(D_089D6998, out, size, text);
    }
    static void *Make(const char *text) {
        return func_0887D868(D_089D6998, 0, text, -1, 0, 0, 0, 0);
    }
};

extern "C" void func_08A16F48(PageScreen29 *self)
{
    char text[0x40];
    const PageText29 *page = &D_08A188C6;

    if (self->work->texts[page->slot] != 0) {
        func_0887BA5C(self->work->texts[page->slot]);
        self->work->texts[page->slot] = 0;
    }
    func_0893476C(text, D_08A18910, self->work->page + 1, self->work->count);
    Object9D6998::Convert(self->work->text, 0x80, text);
    self->work->texts[page->slot] = Object9D6998::Make(self->work->text);
}
