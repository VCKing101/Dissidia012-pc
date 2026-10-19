extern void func_088A1BEC(void *, float, float);

/* Forward signed integer settings as floats to the observed subobject. */
void func_088A18B0(void *owner, int first, int second) {
    func_088A1BEC((unsigned char *)owner + 0x7C,(float)first,(float)second);
}
