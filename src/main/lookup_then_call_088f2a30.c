/* Looks up an entry for the byte in the object's first field; returns func_088C09FC of its second byte, or 0. */
extern void *func_088B70C8(int id);
extern int func_088C09FC(unsigned char v);
typedef struct Obj088F2A30 { unsigned char pad[4]; char *p; } Obj088F2A30;
int func_088F2A30(Obj088F2A30 *self)
{
    unsigned char *e = func_088B70C8(*self->p);
    int r = 0;
    if (e) {
        r = func_088C09FC(e[1]);
    }
    return r;
}
