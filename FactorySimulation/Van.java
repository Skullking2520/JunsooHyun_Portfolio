// Van.java
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.TimeUnit;
import java.util.ArrayList;
import java.util.List;

public class Van implements Runnable {
    private boolean closingTime = false;
    private BlockingQueue<Box> loadingArea;
    private FactoryStatistics stats;
    private String name;
    private List<Box> loadedBoxes = new ArrayList<>();
    private static final int MAX_BOXES = 18;
    private LoadingBay loadingBay;

    public Van(BlockingQueue<Box> loadingArea, FactoryStatistics stats, String name, LoadingBay loadingBay) {
        this.loadingArea = loadingArea;
        this.stats = stats;
        this.name = name;
        this.loadingBay = loadingBay;
    }

    private static final int MAX_WAIT_TIME = 60000; // max wait time
    private long startTime;

    public void run() {
        System.out.println(name + " arrived and waiting for loading bay.");
        startTime = System.currentTimeMillis();
        try {
            loadingBay.acquireBay(name);
            System.out.println(name + ": Acquired loading bay.");
            while (!closingTime) {
                if (loadedBoxes.size() >= MAX_BOXES) {
                    System.out.println(name + ": Fully loaded and ready to depart.");
                    break;
                }
                if (System.currentTimeMillis() - startTime >= MAX_WAIT_TIME) {
                    System.out.println(name + ": Waited too long, departing early with " + loadedBoxes.size() + " boxes.");
                    break;
                }
                Box box = loadingArea.poll(1, TimeUnit.SECONDS);
                if (box != null) {
                    Thread.sleep(1000); // loading time
                    loadedBoxes.add(box);
                    System.out.println(name + ": Loaded box " + box.getId() + ".");
                } else if (closingTime) {
                    break;
                }
            }
            long endTime = System.currentTimeMillis();
            stats.addVanLoadingTime(endTime - startTime);
            System.out.println(name + ": Departing with " + loadedBoxes.size() + " boxes.");
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        } finally {
            loadingBay.releaseBay(name);
        }
    }


    public void setClosingTime(boolean closingTime) {
        this.closingTime = closingTime;
    }
}
