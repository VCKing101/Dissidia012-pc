/* Runs the linked or the plain handler for a record id; 0 when the record is missing. */
typedef struct Rec088B7790 {
    char pad_00[6];
    short link;
} Rec088B7790;

Rec088B7790 *func_088B1D9C(int id);
int func_088B93C8(int a, int b, int id);
int func_088B7698(int a, int b, int id);
int func_088B7790(int a, int b, int id)
{
    Rec088B7790 *rec = func_088B1D9C(id);
    if (rec != 0) {
        if (rec->link == -1) {
            return func_088B93C8(a, b, id);
        }
        return func_088B7698(a, b, id);
    }
    return 0;
}
