// Looks up key in the array at +4 (count in the short at +2, 4-byte records) with func_08B4AE04; returns the short at +2 of the found record, or 0.
typedef struct { short id; short val; } Rec08B4AB10;
extern Rec08B4AB10 *func_08B4AE04(Rec08B4AB10 *, Rec08B4AB10 *, int);
typedef struct { short pad; short count; Rec08B4AB10 *recs; } Obj08B4AB10;
int func_08B4AB10(Obj08B4AB10 *self, int key)
{
    Rec08B4AB10 *r = func_08B4AE04(self->recs, self->recs + self->count, key);
    if (r != 0) {
        return r->val;
    } else {
        return 0;
    }
}
