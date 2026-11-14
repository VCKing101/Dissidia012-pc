/* Battle setup root destructor: tears down each subobject, then frees. */
extern void func_089DD604(void *);
extern void func_089DD698(void *);
extern void func_089ECF78(void *, int);
extern void func_089E0708(void *, int);
extern void func_089E6928(void *, int);
extern void func_089E7180(void *, int);
extern void func_089E7A20(void *, int);
extern void func_089E3258(void *, int);
extern void func_089E8EA8(void *, int);

void func_089DD534(unsigned char *object, int flags)
{
    if (object) {
        func_089DD604(object);
        func_089ECF78(object + 0x1D04, 2);
        func_089E0708(object + 0x1C6C, 2);
        func_089E6928(object + 0xA8C, 2);
        func_089E7180(object + 0xA44, 2);
        func_089E7A20(object + 0x1DC, 2);
        func_089E3258(object + 0x18, 2);
        func_089E8EA8(object + 0x10, 2);
        if (flags & 1) func_089DD698(object);
    }
}
