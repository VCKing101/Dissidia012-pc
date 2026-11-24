/* Teardown: base release, free handle at 0x2C, then calls the member function in the table entry at +8. */
extern void func_0892ACAC();
extern void func_088DAB44(int);
typedef struct { short adj; short pad; void (*fn)(char *); } Entry;
typedef struct { char data[0x24]; char *table; int pad; int h; } S;
void func_08B4C8E4(S *self)
{
    func_0892ACAC();
    if (self->h) { func_088DAB44(self->h); self->h = 0; }
    Entry *e = (Entry *)(self->table + 8);
    e->fn((char *)self + e->adj);
}
