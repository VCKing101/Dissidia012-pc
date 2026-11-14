typedef struct PanelItem {
    char data[0x18];
} PanelItem;

typedef struct Panel {
    PanelItem items[3];
    void *handles[4];
    int field_58;
    int mode;
} Panel;

typedef struct PanelResources {
    char pad_000[0x17C];
    void *source;
} PanelResources;

extern "C" {

extern PanelResources *D_08B23058;
extern void *func_088DAA68(void *source);
extern void func_08ADF450(PanelItem *item);

void func_08ADFAE4(Panel *self, int value, int mode)
{
    self->field_58 = value;
    self->mode = mode;
    void *source = D_08B23058->source;
    if (source != 0) {
        for (int i = 0; i < 4; i++) {
            self->handles[i] = func_088DAA68(source);
        }
    }
    for (int i = 0; i < 3; i++) {
        func_08ADF450(&self->items[i]);
    }
}

}
