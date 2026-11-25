// Returns 1 when the float is infinity or NaN (exponent all ones).
union F08998278 { float f; unsigned u; };
unsigned func_08998278(float x)
{
    union F08998278 v;
    v.f = x;
    return (0x7F800000 - (v.u & 0x7FFFFFFF)) >> 31;
}
