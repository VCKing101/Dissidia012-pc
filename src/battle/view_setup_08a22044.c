typedef struct ViewSetup {
    unsigned char active;
    char pad_01[0x3];
    char link[0xFC];
    char view[0x388];
    void *source;
} ViewSetup;

extern void func_08A2200C(ViewSetup *self);
extern int func_08A232F0(void *link, void *source, void *view);
extern int func_08A06DE8(void *view, void *target, void *source);

/* Links source into the view and the view to target; -1 bad arguments, -2/-3 when a link fails. */
int func_08A22044(ViewSetup *self, void *source, void *target)
{
    func_08A2200C(self);
    if (source == 0 || target == 0) {
        return -1;
    }
    if (func_08A232F0(self->link, source, self->view) != 0) {
        return -2;
    }
    if (func_08A06DE8(self->view, target, source) != 0) {
        return -3;
    }
    self->source = source;
    self->active = 1;
    return 0;
}
