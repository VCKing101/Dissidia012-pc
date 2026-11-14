/* Stores the value (or null) and marks the global as changed. */
extern unsigned char D_089D7434;
void func_088C1B48(void **out, void *value)
{
    void *none = 0;
    *out = value != 0 ? value : none;
    D_089D7434 = 1;
}
