/*
encode: plain + key = cipher
decode: cipher + key = plain

> encode file.key < foo.txt > foo.cipher // encode foo.txt (stdin) w/key in file.key
-- for i = 0, each char in foo.txt, encode it w/a char from file.key, i++
---- encode( foo.txt[i], file.key[i % len(file.key)])

> decode file.key < foo.cipher > bar.txt // decode foo.cipher (stdin) w/key in file.key
-- for each char in foo.cipher, decode it w/a char from file.key

> diff foo.txt bar.txt
>

file.key???
  
> keygen 256 > file.key
-- alphabet = "ABC... "
-- key string
-- for i = 0 to command-line arg (e.g., 256)
---- key += alphabet[ rand() % len(alphabet) ]
-- write key to stdout
*/