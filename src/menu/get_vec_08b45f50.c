/* Copies the vector at +0x154 of the object pointed to by field +0x18 into the output. */
typedef struct V3 { float x, y, z; } V3;
typedef struct Owner { char pad[0x18]; char *obj; } Owner;
void func_08B45F50(V3 *out, Owner *owner)
{
    *out = *(V3 *)(owner->obj + 0x154);
}
