// Draws a UI element when the object's state byte is 1.
struct Obj { char pad[0x34]; unsigned char state; };
extern void *D_089D6AC8;
extern void *func_08899300(void);
extern void *func_08895420(void *);
extern void *func_089A04E8(void);
extern void func_089A0384(int);
extern void func_089A119C(int);
extern void *func_088985F0(void *, int);
extern void *func_08893A18(void *);
extern void func_0886C38C(void *, void *, int, int, void *, int);
extern void func_0899FDC4(void *);

void func_08B5DBB0(struct Obj *self)
{
    if (self->state == 1) {
        void *key = D_089D6AC8;
        void *a = func_08899300();
        void *dst = func_08895420(key);
        if (dst == 0) {
            return;
        }
        {
            void *sv = func_089A04E8();
            func_089A0384(0x15);
            func_089A119C(1);
            a = func_088985F0(a, 0x1A);
            {
                void *c = func_08893A18(key);
                func_0886C38C(dst, a, 1, 0xFF, c, 0);
            }
            func_089A119C(0);
            func_0899FDC4(sv);
        }
    }
}
