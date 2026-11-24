/* Returns -1 if func_088DEC0C fails; otherwise creates the member at +4 with func_088DAA40(field 8, 0, 0) when missing (-2 on failure); returns 0. */
typedef struct Obj088dec14 { int pad; void *member; void *arg; } Obj088dec14;
extern int func_088DEC0C(void);
extern void *func_088DAA40(void *arg, int a, int b);
int func_088DEC14(Obj088dec14 *self)
{
    
    if (func_088DEC0C() == 0) {
        return -1;
    } else {
        if (self->member == 0) {
            self->member = func_088DAA40(self->arg, 0, 0);
            if (self->member == 0) {
                return -2;
            }
        }
        return 0;
    }
}
