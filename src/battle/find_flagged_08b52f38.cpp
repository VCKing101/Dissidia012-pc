/* Follows func_08841F64 links from the object until one has byte 0x542 clear; returns it, or 0. */
struct Node {
    char pad[0x542];
    unsigned char flag;
    static bool Clear(Node *n) { return n->flag == 0; }
};
extern "C" {
extern Node *func_08841F64(Node *);
Node *func_08B52F38(Node *n)
{
    while (1) {
        n = func_08841F64(n);
        if (n == 0) {
            return 0;
        }
        if (Node::Clear(n)) {
            return n;
        }
    }
}
}
