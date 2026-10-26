# 공장 생산라인 시뮬레이션

Java로 공정별 작업과 생산라인의 동시 실행을 표현한 학교 수업 과제입니다.

## 코드 구성

[Main.java](Main.java)에서 시작하며 [Factory.java](Factory.java)가 공정을 연결합니다. `Sterilizer`, `Filler`, `Sealer`, `Labeler`, `DefectScanner`, `Packager`가 공정을, `LoadingBay`, `Forklift`, `Van`이 출하 흐름을 다룹니다. [FactoryStatistics.java](FactoryStatistics.java)에서 통계를 확인할 수 있습니다.

## 실행

JDK를 설치하고 이 폴더에서 실행합니다.

```bash
javac *.java
java Main
```

실행 시간·생산 수량은 스레드 스케줄링과 코드 설정에 영향을 받습니다. 실제 공장의 처리 성능을 측정한 자료는 아닙니다.
