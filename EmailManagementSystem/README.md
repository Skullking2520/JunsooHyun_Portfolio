# C++ 이메일 관리 시스템

학교 팀 과제로 작성한 콘솔 프로그램입니다. 받은편지함·보낸편지함, 사용자 관리와 필터를 연결합니다.

## 코드 구성

| 파일 | 역할 |
| --- | --- |
| [main.cpp](main.cpp) | 콘솔 메뉴와 기능 연결 |
| [Inbox.cpp](Inbox.cpp) · [Outbox.cpp](Outbox.cpp) | 받은편지함·보낸편지함 |
| [EL.cpp](EL.cpp) · [EM.cpp](EM.cpp) | 이메일 목록과 이메일 객체 |
| [UserManager.cpp](UserManager.cpp) · [User.cpp](User.cpp) | 사용자 처리 |
| [Filter.cpp](Filter.cpp) | 필터 |

## 빌드

이 폴더에서 C++ 컴파일러로 빌드합니다.

```bash
g++ -std=c++11 main.cpp Inbox.cpp Outbox.cpp EL.cpp EM.cpp User.cpp UserManager.cpp Filter.cpp -o EmailSystem
./EmailSystem
```

학습용 사용자 인증이며 운영용 보안 시스템으로 검증된 구현은 아닙니다. 실제 이메일이나 비밀번호 대신 테스트 값을 사용합니다.
