// Copies a world position from a table entry (via manager) into a point with w = 1; returns -1..-4 on failures.
struct A08A946AC { unsigned char on; char pad[0x13]; unsigned idx; };
struct M08A946AC { unsigned char on; char pad[0xBB]; unsigned count; short (*tbl)[4]; };
struct P08A946AC { float x, y, z; };
struct V08A946AC { float x, y, z, w; };
extern struct M08A946AC *func_08AA79C4(void *);
extern void *func_08899300(void);
extern struct P08A946AC *func_08897DEC(void *, int);
extern void func_089A3868(void *dst, struct V08A946AC *v);
extern void *D_08AAAD80;
int func_08A946AC(struct A08A946AC *self, void *dst)
{
    struct V08A946AC v;
    struct M08A946AC *m;
    struct P08A946AC *p;
    if (self->on == 0) {
        return -1;
    }
    m = func_08AA79C4(D_08AAAD80);
    if (m->on == 0) {
        return -2;
    }
    if (!(self->idx < m->count)) {
        return -3;
    }
    p = func_08897DEC(func_08899300(), m->tbl[self->idx][0]);
    if (p == 0) {
        return -4;
    }
    v.x = p->x;
    v.y = p->y;
    v.z = p->z;
    v.w = 1.0f;
    func_089A3868(dst, &v);
    return 0;
}
