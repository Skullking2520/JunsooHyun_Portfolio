import java.util.concurrent.BlockingQueue;
import java.util.concurrent.TimeUnit;
import java.util.Random;

public class Forklift implements Runnable {
    private boolean closingTime = false;
    private BlockingQueue<Box> inputQueue;
    private FactoryStatistics stats;
    private String name;
    private Random random = new Random();
    private boolean isBroken = false;

    public Forklift(BlockingQueue<Box> inputQueue, FactoryStatistics stats, String name) {
        this.inputQueue = inputQueue;
        this.stats = stats;
        this.name = name;
    }

    public void run() {
        System.out.println(name + " started.");
        while (!closingTime || !inputQueue.isEmpty()) {
            try {
                if (isBroken) {
                    System.out.println(name + ": Broken down! Waiting for repair.");
                    Thread.sleep(3000); // repairing time
                    isBroken = false;
                    System.out.println(name + ": Repaired and back to work.");
                }
                Box box = inputQueue.poll(1, TimeUnit.SECONDS);
                if (box != null) {
                    Thread.sleep(500); // loading time
                    System.out.println(name + ": Loading box " + box.getId() + " onto van.");
                    stats.incrementBoxesLoaded();
                    // Check if broken
                    if (random.nextInt(100) < 5) { // broken by 5%
                        isBroken = true;
                    }
                } else if (closingTime && inputQueue.isEmpty()) {
                    break;
                }
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }
        System.out.println(name + ": Closing time, no boxes left to load.");
    }


    public void setClosingTime(boolean closingTime) {
        this.closingTime = closingTime;
    }
}
