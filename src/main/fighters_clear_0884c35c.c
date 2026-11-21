typedef struct Entry0884C35C {
    unsigned char unk_00[0xC];
    int a;                             /* +0x0C */
    int b;                             /* +0x10 */
} Entry0884C35C;

typedef struct Fighters0884C35C {
    unsigned char unk_0000[0x2580];
    int unk_2580;
    int unk_2584;
    int unk_2588;
    Entry0884C35C entries[0x20];       /* +0x258C */
} Fighters0884C35C;

typedef struct ObjA0884C35C {
    unsigned char unk_000[0x77C];
    int a;                             /* +0x77C */
    int b;                             /* +0x780 */
} ObjA0884C35C;

typedef struct ObjB0884C35C {
    unsigned char unk_000[0x5E8];
    int a;                             /* +0x5E8 */
    int b;                             /* +0x5EC */
} ObjB0884C35C;

extern ObjA0884C35C *func_08841F84(ObjA0884C35C *prev);   /* next object of the first list */
extern ObjB0884C35C *func_08842068(ObjB0884C35C *prev);   /* next object of the second list */

void func_0884C35C(Fighters0884C35C *self)
{
    ObjA0884C35C *a = 0;
    ObjB0884C35C *b = 0;
    int i;

    self->unk_2588 = 0;
    self->unk_2584 = 0;
    self->unk_2580 = 0;
    for (i = 1; i < 0x20; i++) {
        Entry0884C35C *e = &self->entries[i];
        e->b = 0;
        e->a = 0;
    }
    while ((a = func_08841F84(a)) != 0) {
        a->b = 0;
        a->a = 0;
    }
    while ((b = func_08842068(b)) != 0) {
        b->b = 0;
        b->a = 0;
    }
}
