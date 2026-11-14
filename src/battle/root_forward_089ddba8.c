/* Forwarders that resolve a member of the 0x1DC subobject, then call it. */
extern void *func_089E804C(void *);
extern void *func_089E8044(void *);
extern int func_089DE418(void *, int);
extern int func_089DE478(void *, int);
extern int func_089DE568(void *, int, int);
extern int func_089DE5E4(void *, int, int);
extern int func_089DE660(void *, int, int, unsigned char);
extern int func_089DE3AC(void *);
extern int func_089DE4D8(void *, int, int, int, int);

int func_089DDBA8(unsigned char *object, int value)
{
    return func_089DE418(func_089E804C(object + 0x1DC), value);
}

int func_089DDBDC(unsigned char *object, int value)
{
    return func_089DE478(func_089E804C(object + 0x1DC), value);
}

int func_089DDC10(unsigned char *object, int a, int b)
{
    return func_089DE568(func_089E8044(object + 0x1DC), a, b);
}

int func_089DDC54(unsigned char *object, int a, int b)
{
    return func_089DE5E4(func_089E8044(object + 0x1DC), a, b);
}

int func_089DDC98(unsigned char *object, int a, int b, unsigned char c)
{
    return func_089DE660(func_089E8044(object + 0x1DC), a, b, c);
}

int func_089DDCEC(unsigned char *object)
{
    return func_089DE3AC(func_089E8044(object + 0x1DC));
}

int func_089DDD10(unsigned char *object, int a, int b, int c, int d)
{
    return func_089DE4D8(func_089E8044(object + 0x1DC), a, b, c, d);
}
