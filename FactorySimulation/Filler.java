// Filler.java
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.TimeUnit;

public class Filler implements Runnable {
    private boolean closingTime = false;
    private BlockingQueue<Can> inputQueue;
    private BlockingQueue<Can> outputQueue;

    public Filler(BlockingQueue<Can> inputQueue, BlockingQueue<Can> outputQueue) {
        this.inputQueue = inputQueue;
        this.outputQueue = outputQueue;
    }

    public void run() {
        System.out.println("Filler started.");
        while (!closingTime || !inputQueue.isEmpty()) {
            try {
                Can can = inputQueue.poll(1, TimeUnit.SECONDS);
                if (can != null) {
                    outputQueue.put(can);
                    System.out.println("Filler: Can " + can.getId() + " filled with fruit.");
                    Thread.sleep(100); // working time
                } else if (closingTime) {
                    break;
                }
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }
        System.out.println("Filler: Closing time, no cans left to process.");
    }

    public void setClosingTime(boolean closingTime) {
        this.closingTime = closingTime;
    }
}
