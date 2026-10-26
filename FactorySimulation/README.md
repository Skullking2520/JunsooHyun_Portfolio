# Factory simulation

An individual Java coursework assignment that models concurrent production stages and dispatch.

[Main.java](Main.java) starts the application; [Factory.java](Factory.java) connects the stages. Sterilizing, filling, sealing, labeling, inspection, and packaging have separate classes. LoadingBay, Forklift, and Van handle dispatch, while [FactoryStatistics.java](FactoryStatistics.java) records statistics.

Install a JDK and run from this folder:

```sh
javac *.java
java Main
```

Counts and elapsed times depend on the configured delays and thread scheduling. They are simulation output, not measured factory performance.
