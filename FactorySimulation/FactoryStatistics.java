import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class FactoryStatistics {
    private int totalCansProduced = 0;
    private int totalCansRejected = 0;
    private int totalBoxesLoaded = 0;
    private int incompletePacks = 0;
    private int incompleteBoxes = 0;
    private List<Long> vanLoadingTimes = new ArrayList<>();

    public synchronized void incrementCansProduced() {
        totalCansProduced++;
    }

    public synchronized void incrementCansRejected() {
        totalCansRejected++;
    }

    public synchronized void incrementBoxesLoaded() {
        totalBoxesLoaded++;
    }

    public synchronized void incrementIncompletePacks() {
        incompletePacks++;
    }

    public synchronized void incrementIncompleteBoxes() {
        incompleteBoxes++;
    }

    public synchronized void addVanLoadingTime(long time) {
        vanLoadingTimes.add(time);
    }

    public void printStatistics() {
        System.out.println("=== Factory Statistics ===");
        System.out.println("Total Cans Produced: " + totalCansProduced);
        System.out.println("Total Cans Rejected: " + totalCansRejected);
        System.out.println("Total Boxes Loaded: " + totalBoxesLoaded);
        System.out.println("Incomplete Packs Remaining: " + incompletePacks);
        System.out.println("Incomplete Boxes Remaining: " + incompleteBoxes);

        if (!vanLoadingTimes.isEmpty()) {
            long maxTime = Collections.max(vanLoadingTimes);
            long minTime = Collections.min(vanLoadingTimes);
            double avgTime = vanLoadingTimes.stream().mapToLong(Long::longValue).average().orElse(0);
            System.out.println("Van Loading Times (ms): Max=" + maxTime + ", Min=" + minTime + ", Avg=" + avgTime);
        }
    }
}
