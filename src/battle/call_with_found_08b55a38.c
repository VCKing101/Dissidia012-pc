/* Looks up an entry by name; if found, passes its fields 4 and 8 to func_08854254. */
struct Ent { int a; int b; int c; };
extern const char D_08B563A4[];
extern void *D_089D4DC8;
extern struct Ent *func_08869AB4(void *, const char *);
extern void func_08854254(void *, int, int);
void func_08B55A38(void *self, void *name)
{
    struct Ent *e = func_08869AB4(name, D_08B563A4);
    if (e != 0) {
        func_08854254(D_089D4DC8, e->b, e->c);
    }
}
