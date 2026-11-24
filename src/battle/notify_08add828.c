/* When the event's id matches func_08889E44's and its table entry has a valid short, builds a request and sends it. */
typedef struct Request { int a; int b; char pad[0x18]; int mode; unsigned char flag; char pad2[0xB]; } Request;
typedef struct Entry { char pad[6]; short value; } Entry;
typedef struct Obj { char pad[0xC]; int index; } Obj;
typedef struct Table { char pad[0x54]; int owner; } Table;
extern Entry *D_089D77D8;
extern Table *D_089D6A10;
extern int func_08889E44(void);
extern void func_0888ACE4(Request *r);
extern void func_089EB798(Request *r);
extern void func_089EB7D0(Request *r, int mode);

void func_08ADD828(Obj *self, int *evt)
{
    Request req;
    int id = *evt;
    Entry *e;
    if (id == func_08889E44() && (e = &D_089D77D8[self->index]) != 0 && e->value != -1) {
        func_089EB798(&req);
        req.a = D_089D6A10->owner;
        req.b = e->value;
        req.mode = 3;
        req.flag = 1;
        func_0888ACE4(&req);
        func_089EB7D0(&req, 2);
    }
}
