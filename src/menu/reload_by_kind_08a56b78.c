/* Frees three effect handles, picks an effect id by kind (0..4, else -1), creates it and refreshes the panel. */
typedef struct Panel {
    char g0[0x54];
    int owner;
    char g1[4];
    int h0;
    int h1;
    int h2;
    char g2[0x8C - 0x68];
} Panel;
extern void func_0887BA5C(void);
extern int func_0887B3EC(int owner, int a, int b, int c, int d, int e);
extern int func_0887E534(int h);
extern void func_088D428C(void *a, void *b, void *c, int d);
extern void func_08A55174(void *p);
void func_08A56B78(Panel *arg, unsigned int kind) {
    Panel *self = arg;
    int id;
    int h;
    if (self->h0 != 0) {
        func_0887BA5C();
        self->h0 = 0;
    }
    if (self->h1 != 0) {
        func_0887BA5C();
        self->h1 = 0;
    }
    if (self->h2 != 0) {
        func_0887BA5C();
        self->h2 = 0;
    }
    id = -1;
    switch (kind) {
    case 0:
        id = 0x1A;
        break;
    case 1:
        id = 0x1B;
        break;
    case 2:
        id = 0x1C;
        break;
    case 3:
        id = 0x1D;
        break;
    case 4:
        id = 0x1E;
        break;
    }
    h = func_0887B3EC(self->owner, 3, 0, id, 0, 0);
    self->h0 = h;
    h = func_0887E534(h);
    self->h1 = h;
    h = func_0887E534(self->h0);
    self->h2 = h;
    func_088D428C(self, &self->h0, (char *)self + 0x8C, 1);
    func_08A55174(self);
}
