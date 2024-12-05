// Sterilizer.java
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.TimeUnit;
import java.util.List;
import java.util.ArrayList;

public class Sterilizer implements Runnable {
    private boolean closingTime = false;
    private BlockingQueue<Can> inputQueue;
    private BlockingQueue<Can> outputQueue;

    public Sterilizer(BlockingQueue<Can> inputQueue, BlockingQueue<Can> outputQueue) {
        this.inputQueue = inputQueue;
        this.outputQueue = outputQueue;
    }

    public void run() {
        System.out.println("Sterilizer started.");
        while (!closingTime || !inputQueue.isEmpty()) {
            try {
                List<Can> batch = new ArrayList<>();
                // Adds max 4 cans
                for (int i = 0; i < 4; i++) {
                    Can can = inputQueue.poll(1, TimeUnit.SECONDS);
                    if (can != null) {
                        batch.add(can);
                    } else {
                        break;
                    }
                }
                if (!batch.isEmpty()) {
                    System.out.println("Sterilizer: Sterilizing batch of " + batch.size() + " cans.");
                    Thread.sleep(200); // time for Sterilizing
                    for (Can can : batch) {
                        outputQueue.put(can);
                        System.out.println("Sterilizer: Can " + can.getId() + " sterilized.");
                    }
                } else if (closingTime) {
                    break;
                }
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }
        System.out.println("Sterilizer: Closing time, no cans left to process.");
    }

    public void setClosingTime(boolean closingTime) {
        this.closingTime = closingTime;
    }
}
