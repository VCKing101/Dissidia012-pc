/* Creates the 0x220-byte part on the heap and stores it with its heap.
   Retail keeps the new-expression's null result in the frame (0($sp)). */
extern "C" void *func_0885C6CC(void *heap, unsigned size, int align, int offset, int kind, int flags);

struct NewPart;

struct NewPartHolder {
    NewPart *field_00;
    void *field_04;
};

extern "C" void func_08A6F940(NewPart *part);

extern "C" {
void func_08A6D718(NewPartHolder *holder, void *heap)
{
    NewPart *none = 0;
    NewPart *result[1];
    result[0] = none;
    int one = 1;
    NewPart *part = (NewPart *)func_0885C6CC(heap, 0x220, 0, 0, one, 0);
    NewPart *made = result[0];
    if (part != 0) {
        func_08A6F940(part);
        made = part;
    }
    holder->field_00 = made;
    holder->field_04 = heap;
}
}
