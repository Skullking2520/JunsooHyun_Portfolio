# Assembly blackjack

An individual coursework card game written in NASM. The source filename is [BlackJack.asm](BlackJack.asm), including its capitalization.

The program uses 32-bit Linux system calls. It needs NASM, GNU ld, and a Linux environment with 32-bit executable support.

Run from this folder:

```sh
nasm -f elf32 BlackJack.asm -o BlackJack.o
ld -m elf_i386 BlackJack.o -o BlackJack
./BlackJack
```

These commands target Linux; no Windows or macOS executable is included.
