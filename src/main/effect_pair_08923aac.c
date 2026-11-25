typedef struct Eff { char pad[0xE0]; float scale; } Eff;
typedef struct Vec3 { float x, y, z; } Vec3;
typedef struct Body {
    char pad0[0x16];
    short kind;
    char pad1[0x44 - 0x18];
    short alt;
    char pad2[0x5C - 0x46];
    Vec3 pos;
    char pad3[0xBC - 0x68];
    void *h0;
    Eff *h1;
    void *h2;
} Body;
typedef struct Owner { char pad[0x20]; Body *body; } Owner;

extern void *D_08B42288;
extern void *func_08B421FC(void *tbl, int id);
extern void func_08B347D4(float f, void *h);
extern void func_088A72E8(void *h, float x, float y, float z);

void func_08923AAC(Owner *self) {
    void *h = self->body->h0;
    if (h != 0) {
        func_08B347D4(60.0f, h);
        if (self->body->alt != -1) {
            self->body->h2 = func_08B421FC(D_08B42288, self->body->alt);
            {
                Vec3 *pp = &self->body->pos;
                func_088A72E8(self->body->h2, pp->x, pp->y, pp->z);
            }
        }
    } else {
        int id;
        self->body->h1 = func_08B421FC(D_08B42288, self->body->kind);
        id = -1;
        switch (self->body->kind) {
        case 0x14:
            id = 0x15;
            break;
        case 0x19:
            id = 0x18;
            break;
        case 0x1C:
            id = 0x1B;
            break;
        case 0x1F:
            id = 0x1E;
            break;
        case 0x22:
            id = 0x21;
        }
        if (id != -1) {
            self->body->h1->scale = 20.0f;
            self->body->h2 = func_08B421FC(D_08B42288, id);
        }
    }
}
