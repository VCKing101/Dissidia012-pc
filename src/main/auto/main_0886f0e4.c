#include "main/auto_region2.h"

/* HMAC (inner pad 0x36, outer 0x5C) over data with key: SHA-1-style init/update/final
 * (func_0899F3FC, func_0899F030, func_0899F200); keys over 64 bytes are hashed first. */
void func_0886F0E4(const void *key, unsigned int key_size, const void *data, int data_size, void *digest)
{
    int next;
    int i;
    char context[96];
    char inner[20];
    unsigned char pad[64];

    func_08935828(pad, 0, 0x40);
    if (key_size < 0x41) {
        func_08935524(pad, key, key_size);
    } else {
        func_089400B0(key, key_size, pad);
    }
    i = 0;
    do {
        next = i + 1;
        pad[i] = pad[i] ^ 0x36;
        i = next;
    } while (next < 0x40);
    func_0899F3FC(context);
    func_0899F030(context, pad, 0x40);
    func_0899F030(context, data, data_size);
    func_0899F200(context, inner);
    i = 0;
    do {
        next = i + 1;
        pad[i] = pad[i] ^ 0x6a;
        i = next;
    } while (next < 0x40);
    func_0899F3FC(context);
    func_0899F030(context, pad, 0x40);
    func_0899F030(context, inner, 0x14);
    func_0899F200(context, digest);
}
