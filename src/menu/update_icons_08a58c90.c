/* Updates the pair of icons for the current state: asks the owner through its table whether it is active and shows the matching sprites. */
typedef struct Ent58C90 { short adj; short pad; int (*fn)(void *); } Ent58C90;
typedef struct Own58C90 { char pad[0x38]; Ent58C90 e; } Own58C90;
typedef struct S58C90 { char pad0[8]; Own58C90 *own; int mode; char pad1[0x18 - 0x10]; int a; int b; char pad2[0x50 - 0x20]; int c; int d; } S58C90;
extern void func_088D9F5C(int, int, int);
extern void func_088D9CFC(int);
extern void func_088D9CD8(int);
extern void func_0887B924(int);
extern void func_0887B944(int);
extern void func_08A58D7C(S58C90 *);
void func_08A58C90(S58C90 *self)
{
    Ent58C90 *e = &self->own->e;
    if (e->fn((char *)self + e->adj)) {
        switch (self->mode) {
        case 0:
            func_088D9F5C(self->a, 7, 1);
            func_088D9CFC(self->b);
            func_0887B924(self->c);
            func_0887B944(self->d);
            break;
        case 1:
            func_088D9F5C(self->a, 7, 1);
            func_088D9CD8(self->b);
            func_0887B924(self->c);
            func_0887B944(self->d);
            break;
        }
    } else {
        func_088D9F5C(self->a, 6, 1);
        func_088D9CFC(self->b);
        func_0887B944(self->c);
        func_0887B924(self->d);
    }
    func_08A58D7C(self);
}
