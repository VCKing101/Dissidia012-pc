typedef struct Owner { int field_00; void *a; void *c; void *b; void *d; } Owner;
extern void func_08884BC0(void *p);
extern void func_0888630C(void *p);
extern void func_08885EF0(void *p);
extern void func_088844C4(void *p);

void func_088853CC(Owner *o, unsigned char keep)
{
    if (o->a != 0) {
        func_08884BC0(o->a);
    }
    if (o->b != 0) {
        func_0888630C(o->b);
    }
    if (o->c != 0) {
        func_08885EF0(o->c);
    }
    if (!keep && o->d != 0) {
        func_088844C4(o->d);
    }
}
