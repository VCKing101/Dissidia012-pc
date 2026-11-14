/* True when the network status query fails. */
extern char D_09B698D8[];
extern int func_0891A44C(void *status);

int func_088F29F4(void)
{
    int failed = 0;

    if (func_0891A44C(D_09B698D8) < 0) {
        failed = 1;
    }
    return failed;
}
