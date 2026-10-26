# 어셈블리 블랙잭

학교 수업에서 작성한 NASM 어셈블리 카드 게임입니다. 소스는 [BlackJack.asm](BlackJack.asm)입니다. 대소문자를 구분하는 환경에서는 이 파일명을 그대로 사용합니다.

## 환경과 실행

32비트 Linux 시스템 호출을 사용하는 코드입니다. NASM·GNU ld와 32비트 실행을 지원하는 Linux 환경이 필요합니다. Windows나 macOS에서 직접 실행하는 명령은 아닙니다.

```bash
nasm -f elf32 BlackJack.asm -o BlackJack.o
ld -m elf_i386 BlackJack.o -o BlackJack
./BlackJack
```

현재 환경에서의 실행 결과나 성능 지표는 별도로 측정하지 않았습니다.
