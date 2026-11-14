typedef struct ResultScreen {
    char pad_000[0xABC];
    void *cursor;
} ResultScreen;

typedef struct ResultOwner {
    char pad_00[0x30];
    ResultScreen *screen;
} ResultOwner;

extern void func_088D9CFC(void *cursor);

void func_088FCBBC(ResultOwner *owner)
{
    if (owner->screen == 0) {
        return;
    }
    func_088D9CFC(owner->screen->cursor);
}
