int check(long *data, long count)
{
    for (long i = 0; i < count - 1; ++i)
        if (data[i] > data[i + 1])
            return 0; // oops
    return 1; // ok
}
