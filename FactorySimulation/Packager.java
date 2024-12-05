// Packager.java
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.TimeUnit;
import java.util.ArrayList;
import java.util.List;

public class Packager implements Runnable {
    private boolean closingTime = false;
    private BlockingQueue<Can> inputQueue;
    private BlockingQueue<Box> outputQueue;
    private FactoryStatistics stats;
    private List<Can> currentPack = new ArrayList<>();
    private List<Pack> currentBox = new ArrayList<>();
    private int packId = 1;
    private int boxId = 1;

    public Packager(BlockingQueue<Can> inputQueue, BlockingQueue<Box> outputQueue, FactoryStatistics stats) {
        this.inputQueue = inputQueue;
        this.outputQueue = outputQueue;
        this.stats = stats;
    }

    public void run() {
        System.out.println("Packager started.");
        while (!closingTime || !inputQueue.isEmpty() || !currentPack.isEmpty() || !currentBox.isEmpty()) {
            try {
                Can can = inputQueue.poll(1, TimeUnit.SECONDS);
                if (can != null) {
                    currentPack.add(can);
                    if (currentPack.size() == 6) {
                        createPack();
                    }
                } else if (closingTime) {
                    break;
                }
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }
        // ³²Àº Äµ°ú ÆÑ Ã³¸®
        if (!currentPack.isEmpty()) {
            createIncompletePack();
        }
        if (!currentBox.isEmpty()) {
            createIncompleteBox();
        }
        System.out.println("Packager: Closing time, no cans left to pack.");
    }

    private void createPack() {
        Pack pack = new Pack(new ArrayList<>(currentPack), packId++);
        currentBox.add(pack);
        currentPack.clear();
        System.out.println("Packager: Pack " + pack.getId() + " created.");
        if (currentBox.size() == 2) {
            createBox();
        }
    }

    private void createBox() {
        Box box = new Box(new ArrayList<>(currentBox), boxId++);
        try {
            while (true) {
                if (outputQueue.offer(box, 1, TimeUnit.SECONDS)) {
                    System.out.println("Packager: Box " + box.getId() + " created and placed in loading area.");
                    break;
                } else {
                    System.out.println("Packager: Loading area full, waiting to place box " + box.getId() + ".");
                }
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
        currentBox.clear();
    }

    private void createIncompletePack() {
        Pack pack = new Pack(new ArrayList<>(currentPack), packId++);
        currentBox.add(pack);
        currentPack.clear();
        stats.incrementIncompletePacks();
        System.out.println("Packager: Incomplete Pack " + pack.getId() + " created.");
        if (currentBox.size() == 2) {
            createBox();
        }
    }

    private void createIncompleteBox() {
        Box box = new Box(new ArrayList<>(currentBox), boxId++);
        try {
            outputQueue.put(box);
            stats.incrementIncompleteBoxes();
            System.out.println("Packager: Incomplete Box " + box.getId() + " created.");
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
        currentBox.clear();
    }

    public void setClosingTime(boolean closingTime) {
        this.closingTime = closingTime;
    }
}
