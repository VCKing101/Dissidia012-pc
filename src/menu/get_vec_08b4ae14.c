/* Copies the vector at +0x7C of the object pointed to by the first field into the output. */
typedef struct V3 { float x, y, z; } V3;
typedef struct Owner { char *obj; } Owner;
void func_08B4AE14(V3 *out, Owner *owner)
{
    *out = *(V3 *)(owner->obj + 0x7C);
}
