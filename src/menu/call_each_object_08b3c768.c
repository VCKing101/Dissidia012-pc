/* Clears a global flag byte, then calls the member function (table +0xF8) on every object from the iterator. */
typedef struct { short adj; short pad; void (*fn)(char *); } Entry;
typedef struct { char d[0x6B2]; char flag; } Glob;
typedef struct { char d[0x544]; char *table; } Obj;
extern Glob *D_089D6AC8;
extern Obj *func_0881CBB8(Obj *, int);
extern void func_0881CB5C(void);
void func_08B3C768(void)
{
    Obj *o;
    char z = 0;
    D_089D6AC8->flag = z;
    o = 0;
    while ((o = func_0881CBB8(o, -1)) != 0) {
        Entry *e = (Entry *)(o->table + 0xF8);
        e->fn((char *)o + e->adj);
    }
    func_0881CB5C();
}
