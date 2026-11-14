extern int func_08905728(void *self, void *header, void *buffer, int size, int flags);
extern void func_089057BC(void *header);

void func_0890D3A8(void *self)
{
    char header[8];
    char buffer[0x1B8];

    func_089057BC(header);
    while (func_08905728(self, header, buffer, sizeof(buffer), 0) != 0) {
    }
}
