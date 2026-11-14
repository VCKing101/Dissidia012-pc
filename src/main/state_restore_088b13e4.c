/* Restores the saved state block and applies it. */
typedef struct State088B13E4 {
    int words[0xB1];
} State088B13E4;

typedef struct Obj088B13E4 {
    State088B13E4 current;
    State088B13E4 saved;
} Obj088B13E4;

void func_088B1054(Obj088B13E4 *obj);

void func_088B13E4(Obj088B13E4 *obj)
{
    obj->current = obj->saved;
    func_088B1054(obj);
}
