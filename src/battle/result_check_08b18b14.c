// Calls func_08B18838(a, b); if it returns 1 and the byte at +0xBD of the record at *b + 8 is zero, returns 3 instead.
extern int func_08B18838(void *, void *);
typedef struct { char pad[0xBD]; unsigned char flag; } Rec08B18B14;
int func_08B18B14(void *a, Rec08B18B14 **b)
{
    int v = func_08B18838(a, b);
    if (v == 1) {
        Rec08B18B14 *r = (Rec08B18B14 *)((char *)*b + 8);
        if (r->flag == 0) {
            v = 3;
        }
    }
    return v;
}
