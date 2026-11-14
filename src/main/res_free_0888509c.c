/* Frees both resources of the object. */
typedef struct Obj0888509C {
    char pad_00[0xC];
    void *first;
    void *second;
} Obj0888509C;

void func_0887BA5C(void *res);

void func_0888509C(Obj0888509C *o)
{
    if (o->first != 0) {
        func_0887BA5C(o->first);
        o->first = 0;
    }
    if (o->second != 0) {
        func_0887BA5C(o->second);
        o->second = 0;
    }
}
