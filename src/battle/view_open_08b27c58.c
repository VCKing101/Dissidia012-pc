typedef struct ViewSource {
    int field_00;
    void *data;     /* 0x04 */
} ViewSource;

typedef struct ViewContext {
    char pad0000[0xBDB0];
    unsigned char autoStart;    /* 0xBDB0 */
} ViewContext;

typedef struct View {
    unsigned char active;   /* 0x00 */
    char pad01[3];
    ViewSource *source;     /* 0x04 */
    ViewContext *context;   /* 0x08 */
} View;

extern void func_08B27A9C(View *view);
extern void func_08B283E8(View *view);

int func_08B27C58(View *view, ViewContext *context, ViewSource *source)
{
    func_08B27A9C(view);
    if (context == 0 || source == 0) {
        return -1;
    }
    if (source->data == 0) {
        return -2;
    }
    view->source = source;
    view->context = context;
    view->active = 1;
    if (context->autoStart == 1) {
        func_08B283E8(view);
    }
    return 0;
}
