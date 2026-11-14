extern "C" {

/* Append value unless the list already has it; returns the new count. */
int func_08ADCE4C(int *list, int capacity, int count, int value)
{
    for (int i = 0; i < count; i++) {
        if (list[i] == value) {
            return count;
        }
    }
    if (count < capacity) {
        list[count] = value;
        count++;
    }
    return count;
}

}
