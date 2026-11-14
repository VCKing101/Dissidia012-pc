/* Releases the effect id and resets the record to full scale. */
typedef struct Rec088A94F0 {
    int field_00;
    int field_04;
    float scale;
} Rec088A94F0;

void func_088A7EE8(int id);

void func_088A94F0(Rec088A94F0 *r, int id)
{
    func_088A7EE8(id);
    r->field_04 = 0;
    r->field_00 = 0;
    r->scale = 1.0f;
}
