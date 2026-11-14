typedef struct Header0887A8F0 {
    int magic;
} Header0887A8F0;

int func_0887A8F0(Header0887A8F0 *header) {
    if (header->magic == 0x3446464F) {
        return 1;
    }
    return 0;
}
