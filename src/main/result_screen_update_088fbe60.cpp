/* Result screen update: draws the panels and closes the bonus window when it finishes. */
struct ResultNode {
    char pad_00[0xC];
    short kind;
    char pad_0E[0x20 - 0xE];
    unsigned short flags;

    static bool finished(ResultNode *n)
    {
        return (n->flags & 0x20) != 0;
    }
};

struct ResultScreen {
    unsigned int flags;
    char pad_004[0x65C - 4];
    char layout[0xA5C - 0x65C];
    char panels[0xAAC - 0xA5C];
    ResultNode *bonus;
    char pad_AB0[0xEF0 - 0xAB0];
    char effects[4];
};

struct ResultOwner {
    char pad_00[0x30];
    ResultScreen *screen;
};

extern "C" {
extern char D_089CCFAC[];
extern char D_089CCFD4[];
extern void func_088D3F00(void *layout, void *panels, void *table);
extern void func_088D44EC(void *panels, void *effects, void *table);
extern void func_088FBF8C(ResultOwner *owner);

void func_088FBE60(ResultOwner *owner)
{
    func_088D3F00(owner->screen->layout, owner->screen->panels, D_089CCFAC);
    if (owner->screen->flags & 8) {
        func_088D44EC(owner->screen->panels, owner->screen->effects, D_089CCFD4);
    }
    if (owner->screen->bonus->kind == 8 && ResultNode::finished(owner->screen->bonus)) {
        func_088FBF8C(owner);
        owner->screen->flags &= ~8;
    }
}
}
