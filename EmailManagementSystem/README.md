# Email management system

A C++ console application from a team assignment. It combines inbox and outbox lists, user management, filters, and file storage. My contribution is described in the [collection README](../README.md).

| Entry or module | Role |
| --- | --- |
| [main.cpp](main.cpp) | Console menu and integration |
| [Inbox.cpp](Inbox.cpp), [Outbox.cpp](Outbox.cpp) | Mailbox operations |
| [EL.cpp](EL.cpp), [EM.cpp](EM.cpp) | Linked lists and email records |
| [User.cpp](User.cpp), [UserManager.cpp](UserManager.cpp) | User records and authentication |
| [Filter.cpp](Filter.cpp) | Filtering |

Build and run from this folder with a C++ compiler:

```sh
g++ -std=c++11 main.cpp Inbox.cpp Outbox.cpp EL.cpp EM.cpp User.cpp UserManager.cpp Filter.cpp -o EmailSystem
./EmailSystem
```

This is a local simulation without a mail-server connection. Its coursework authentication has not been validated for protecting real accounts; use test credentials.
