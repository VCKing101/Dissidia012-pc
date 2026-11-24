/* Constructs a draw-layer set: builds its members, then resets its four
   layers (white, full scale, enabled) and its state. */
struct Layer {
    int a;
    int pad_04;
    int b;
    int c;
    float scale;
    unsigned int color;
    int enabled;
};

struct LayerSet {
    int state;
    char pad_004[0x10 - 4];
    char m10[0x60 - 0x10];
    char m60[0x74 - 0x60];
    int count;
    char m78[0x84 - 0x78];
    unsigned char busy;
    char pad_085[3];
    int mode;
    int current;
    Layer layers[4];
    int last;
    char pad_104[0x120 - 0x104];
    void *vtable;

    static void ResetLayers(LayerSet *self)
    {
        for (int i = 0; i < 4; i++) {
            self->layers[i].a = 0;
            self->layers[i].b = 0;
            self->layers[i].c = 0;
            self->layers[i].scale = 1.0f;
            self->layers[i].color = 0x80808080;
            self->layers[i].enabled = 1;
        }
        self->current = 0;
    }
};

extern "C" {

extern char D_089CA868[];

void func_0886EE24(void *p);
void func_0886F028(void *p);
void func_08864C1C(void *p);

LayerSet *func_08894D34(LayerSet *self)
{
    self->vtable = D_089CA868;
    func_0886EE24(self->m10);
    func_0886F028(self->m60);
    func_08864C1C(self->m78);
    for (int i = 0; i < 4; i++) {
        self->layers[i].a = 0;
        self->layers[i].b = 0;
        self->layers[i].c = 0;
        self->layers[i].scale = 1.0f;
        self->layers[i].color = 0x80808080;
        self->layers[i].enabled = 1;
    }
    self->mode = 2;
    self->current = 0;
    self->state = 0;
    self->count = 0;
    self->last = 0;
    self->busy = 0;
    LayerSet::ResetLayers(self);
    return self;
}

}
