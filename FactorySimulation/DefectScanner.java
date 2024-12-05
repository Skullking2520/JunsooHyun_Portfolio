// DefectScanner.java
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.TimeUnit;
import java.util.Random;

public class DefectScanner implements Runnable {
    private boolean closingTime = false;
    private BlockingQueue<Can> inputQueue;
    private BlockingQueue<Can> outputQueue;
    private BlockingQueue<Can> defectiveQueue;
    private String name;
    private FactoryStatistics stats;
    private Random random = new Random();

    public DefectScanner(BlockingQueue<Can> inputQueue, BlockingQueue<Can> outputQueue,
                         BlockingQueue<Can> defectiveQueue, String name, FactoryStatistics stats) {
        this.inputQueue = inputQueue;
        this.outputQueue = outputQueue;
        this.defectiveQueue = defectiveQueue;
        this.name = name;
        this.stats = stats;
    }

    public void run() {
        System.out.println(name + " started.");
        while (!closingTime || !inputQueue.isEmpty()) {
            try {
                Can can = inputQueue.poll(1, TimeUnit.SECONDS);
                if (can != null) {
                    Thread.sleep(50); // checking time
                    if (random.nextInt(100) < 15) { // defective by 15%
                        defectiveQueue.put(can);
                        stats.incrementCansRejected();
                        System.out.println(name + ": Can " + can.getId() + " rejected due to defect.");
                    } else {
                        outputQueue.put(can);
                        System.out.println(name + ": Can " + can.getId() + " passed inspection.");
                    }
                } else if (closingTime) {
                    break;
                }
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }
        System.out.println(name + ": Closing time, no cans left to inspect.");
    }

    public void setClosingTime(boolean closingTime) {
        this.closingTime = closingTime;
    }
}
