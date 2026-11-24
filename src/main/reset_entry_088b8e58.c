// Marks the object from func_088B9044 (bit 1 of byte 3), then loads its template row from func_088C6CA4 into byte 1 and clears byte 2.
typedef struct { char id; unsigned char b1; unsigned char b2; unsigned char flags; } E44;
extern E44 *func_088B9044(void);
extern unsigned char *func_088C6CA4(int);
void func_088B8E58(void)
{
    E44 *e = func_088B9044();
    if (e) {
        unsigned char *row;
        e->flags = e->flags | 2;
        row = func_088C6CA4(e->id);
        if (row) {
            unsigned char v = row[8];
            e->b2 = 0;
            e->b1 = v;
        }
    }
}
