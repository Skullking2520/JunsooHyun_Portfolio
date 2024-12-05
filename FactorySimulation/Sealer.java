// Sealer.java
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.TimeUnit;
import java.util.List;
import java.util.ArrayList;

public class Sealer implements Runnable {
    private boolean closingTime = false;
    private BlockingQueue<Can> inputQueue;
    private BlockingQueue<Can> outputQueue;

    public Sealer(BlockingQueue<Can> inputQueue, BlockingQueue<Can> outputQueue) {
        this.inputQueue = inputQueue;
        this.outputQueue = outputQueue;
    }

    public void run() {
        System.out.println("Sealer started.");
        while (!closingTime || !inputQueue.isEmpty()) {
            try {
                List<Can> batch = new ArrayList<>();
                // max 12 cans are batched
                for (int i = 0; i < 12; i++) {
                    Can can = inputQueue.poll(1, TimeUnit.SECONDS);
                    if (can != null) {
                        batch.add(can);
                    } else {
                        break;
                    }
                }
                if (!batch.isEmpty()) {
                    System.out.println("Sealer: Sealing batch of " + batch.size() + " cans.");
                    Thread.sleep(300); // sealing time
                    for (Can can : batch) {
                        outputQueue.put(can);
                        System.out.println("Sealer: Can " + can.getId() + " sealed.");
                    }
                } else if (closingTime) {
                    break;
                }
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }
        System.out.println("Sealer: Closing time, no cans left to process.");
    }

    public void setClosingTime(boolean closingTime) {
        this.closingTime = closingTime;
    }
}
