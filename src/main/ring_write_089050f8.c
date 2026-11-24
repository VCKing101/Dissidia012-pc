/* Ring buffer write: when the free space (fields 0x18 size, 0x1C head, 0x20 tail) allows a 12-byte header plus len bytes, writes the header {'AMSG', id, len} and the data with func_08905354(self + 0x14, ...) and returns 1; else 0. */
typedef struct Ring089050f8 { unsigned char pad[0x18]; int size; int head; int tail; } Ring089050f8;
typedef struct Hdr089050f8 { unsigned magic; int id; int len; } Hdr089050f8;
extern void func_08905354(void *ring, void *data, int n);
int func_089050F8(Ring089050f8 *self, int id, void *data, int len)
{
    unsigned avail = (unsigned)((self->tail + self->size - (self->head + 1)) % self->size);
    unsigned need = len + 0xC;
    if (need <= avail) {
        Hdr089050f8 hdr;
        hdr.magic = 0x47534D41;
        hdr.id = id;
        hdr.len = len;
        func_08905354((char *)self + 0x14, &hdr, 0xC);
        func_08905354((char *)self + 0x14, data, len);
        return 1;
    } else {
        return 0;
    }
}
