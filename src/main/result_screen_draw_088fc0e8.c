/* Draws the result layout and shows the cursor according to flag bit 0 at 0x16. */
typedef struct ResultScreen {
    char pad_000[8];
    int dirty;
    char pad_00C[0x16 - 0xC];
    unsigned short flags;
    char pad_018[0x65C - 0x18];
    char layout[0xA5C - 0x65C];
    char panels[0xABC - 0xA5C];
    void *cursor;
} ResultScreen;

typedef struct ResultOwner {
    char pad_00[0x30];
    ResultScreen *screen;
} ResultOwner;

extern char D_089CCE28[];
extern void func_088D3CF0(void *layout, void *panels, void *table);
extern void func_088D9F5C(void *widget, int arg, int on);

void func_088FC0E8(ResultOwner *owner)
{
    func_088D3CF0(owner->screen->layout, owner->screen->panels, D_089CCE28);
    if (owner->screen->flags & 1) {
        func_088D9F5C(owner->screen->cursor, 1, 1);
    } else {
        func_088D9F5C(owner->screen->cursor, 0, 1);
    }
    owner->screen->dirty = 0;
}
