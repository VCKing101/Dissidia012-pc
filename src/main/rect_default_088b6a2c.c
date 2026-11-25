/* Returns the shared 4-short rectangle: reset to a 100x100 default, then (when given a source) replaced by the converted source values. */
typedef struct R { unsigned short a, b, c, d; } R;
typedef struct T { short a, b, c, d; } T;
extern T D_09B4D488;
extern R *func_088B69AC(unsigned short v);
T *func_088B6A2C(R *in)
{
    T tmp;
    D_09B4D488.a = 0;
    D_09B4D488.b = 0;
    D_09B4D488.d = 100;
    D_09B4D488.c = 100;
    if (in != 0) {
        tmp.a = func_088B69AC(in->a)->a;
        tmp.b = func_088B69AC(in->b)->b;
        tmp.c = func_088B69AC(in->c)->c;
        tmp.d = func_088B69AC(in->d)->d;
        D_09B4D488 = tmp;
    }
    return &D_09B4D488;
}
