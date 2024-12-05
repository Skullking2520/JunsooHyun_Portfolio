// Labeler.java
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.TimeUnit;

public class Labeler implements Runnable {
    private boolean closingTime = false;
    private BlockingQueue<Can> inputQueue;
    private BlockingQueue<Can> outputQueue;

    public Labeler(BlockingQueue<Can> inputQueue, BlockingQueue<Can> outputQueue) {
        this.inputQueue = inputQueue;
        this.outputQueue = outputQueue;
    }

    public void run() {
        System.out.println("Labeler started.");
        while (!closingTime || !inputQueue.isEmpty()) {
            try {
                Can can = inputQueue.poll(1, TimeUnit.SECONDS);
                if (can != null) {
                    outputQueue.put(can);
                    System.out.println("Labeler: Can " + can.getId() + " labeled.");
                    Thread.sleep(80); // 작업 시간
                } else if (closingTime) {
                    break;
                }
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }
        System.out.println("Labeler: Closing time, no cans left to label.");
    }

    public void setClosingTime(boolean closingTime) {
        this.closingTime = closingTime;
    }
}
