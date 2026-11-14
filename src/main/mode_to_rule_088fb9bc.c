/* Maps mode 1 to 2, mode 2 to 3, anything else to 1. */
int func_088FB9BC(int mode)
{
    switch (mode) {
    case 1:
        mode = 2;
        break;
    case 2:
        mode = 3;
        break;
    default:
        mode = 1;
        break;
    }
    return mode;
}
