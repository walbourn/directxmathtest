// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

extern int core();
extern int packed();
extern int colors();
extern int collision();

int main()
{
    int ret = core();
    ret |= packed();
    ret |= colors();
    ret |= collision();
    return ret;
}
