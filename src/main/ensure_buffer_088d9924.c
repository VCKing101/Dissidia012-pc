// Returns -1 if the check fails, -2 if allocating the buffer fails, else 0; allocates obj+4 (size from obj+8, 12 bytes each) on first use.
typedef struct { int a; int buf; int count; } Ob44;
extern int func_088D991C(void);
extern int func_088DAA40(int, int, int);
int func_088D9924(Ob44 *o)
{
    if (!func_088D991C()) return -1;
    if (o->buf == 0) {
        o->buf = func_088DAA40(o->count, 12, 0);
        if (o->buf == 0) return -2;
    }
    return 0;
}
