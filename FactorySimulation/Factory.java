// Factory.java
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.TimeUnit;

public class Factory {
    private static final int TOTAL_CANS = 600;
    private static final int CAN_ARRIVAL_RATE_MS = 300;

    // Creating Queue
    private BlockingQueue<Can> sterilizerInputQueue = new LinkedBlockingQueue<>();
    private BlockingQueue<Can> sterilizerOutputQueue = new LinkedBlockingQueue<>();
    private BlockingQueue<Can> fillerInputQueue = new LinkedBlockingQueue<>();
    private BlockingQueue<Can> fillerOutputQueue = new LinkedBlockingQueue<>();
    private BlockingQueue<Can> sealerInputQueue = new LinkedBlockingQueue<>();
    private BlockingQueue<Can> sealerOutputQueue = new LinkedBlockingQueue<>();
    private BlockingQueue<Can> labelerInputQueue = new LinkedBlockingQueue<>();
    private BlockingQueue<Can> labelerOutputQueue = new LinkedBlockingQueue<>();
    private BlockingQueue<Can> defectiveCansQueue = new LinkedBlockingQueue<>();
    private BlockingQueue<Box> loadingAreaQueue = new LinkedBlockingQueue<>(); // Max 10 boxes are stored

    private FactoryStatistics stats = new FactoryStatistics();

    // worker thread
    private Thread sterilizerThread;
    private Thread fillerThread;
    private Thread sealerThread;
    private Thread defectScanner1Thread;
    private Thread defectScanner2Thread;
    private Thread defectScanner3Thread;
    private Thread labelerThread;
    private Thread packagerThread;
    private Thread forklift1Thread;
    private Thread forklift2Thread;
    private Thread forklift3Thread;
    private Thread van1Thread;
    private Thread van2Thread;
    private Thread van3Thread;

    private LoadingBay loadingBay = new LoadingBay(2);
    
    public void startFactory() {
        // Create worker object
        DefectScanner defectScanner1 = new DefectScanner(sterilizerInputQueue, sterilizerOutputQueue, defectiveCansQueue, "DefectScanner1", stats);
        Sterilizer sterilizer = new Sterilizer(sterilizerOutputQueue, fillerInputQueue);
        Filler filler = new Filler(fillerInputQueue, fillerOutputQueue);
        DefectScanner defectScanner2 = new DefectScanner(fillerOutputQueue, sealerInputQueue, defectiveCansQueue, "DefectScanner2", stats);
        Sealer sealer = new Sealer(sealerInputQueue, sealerOutputQueue);
        DefectScanner defectScanner3 = new DefectScanner(sealerOutputQueue, labelerInputQueue, defectiveCansQueue, "DefectScanner3", stats);
        Labeler labeler = new Labeler(labelerInputQueue, labelerOutputQueue);
        Packager packager = new Packager(labelerOutputQueue, loadingAreaQueue, stats); // Modifying to loadingAreaQueue here
        Forklift forklift1 = new Forklift(loadingAreaQueue, stats, "Forklift 1");
        Forklift forklift2 = new Forklift(loadingAreaQueue, stats, "Forklift 2");
        Forklift forklift3 = new Forklift(loadingAreaQueue, stats, "Forklift 3");
        Van van1 = new Van(loadingAreaQueue, stats, "Van 1", loadingBay);
        Van van2 = new Van(loadingAreaQueue, stats, "Van 2", loadingBay);
        Van van3 = new Van(loadingAreaQueue, stats, "Van 3", loadingBay);

        // Creation of worker threads
        defectScanner1Thread = new Thread(defectScanner1);
        sterilizerThread = new Thread(sterilizer);
        fillerThread = new Thread(filler);
        defectScanner2Thread = new Thread(defectScanner2);
        sealerThread = new Thread(sealer);
        defectScanner3Thread = new Thread(defectScanner3);
        labelerThread = new Thread(labeler);
        packagerThread = new Thread(packager);
        forklift1Thread = new Thread(forklift1);
        forklift2Thread = new Thread(forklift2);
        forklift3Thread = new Thread(forklift3);
        van1Thread = new Thread(van1);
        van2Thread = new Thread(van2);
        van3Thread = new Thread(van3);

        // Starting the threads
        defectScanner1Thread.start();
        sterilizerThread.start();
        fillerThread.start();
        defectScanner2Thread.start();
        sealerThread.start();
        defectScanner3Thread.start();
        labelerThread.start();
        packagerThread.start();
        forklift1Thread.start();
        forklift2Thread.start();
        forklift3Thread.start();
        van1Thread.start();
        van2Thread.start();
        van3Thread.start();

        // Create a can
        for (int i = 1; i <= TOTAL_CANS; i++) {
            try {
                Can can = new Can(i);
                boolean added = sterilizerInputQueue.offer(can, 1, TimeUnit.SECONDS);
                if (added) {
                    System.out.println("Main Thread: Can " + can.getId() + " entered the factory.");
                    stats.incrementCansProduced();
                } else {
                    System.out.println("Main Thread: sterilizerInputQueue is full, cannot add Can " + can.getId());
                    // Can be moved on without repetition if necessary
                }
                Thread.sleep(CAN_ARRIVAL_RATE_MS);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }

        // Termination Signaling
        defectScanner1.setClosingTime(true);
        sterilizer.setClosingTime(true);
        filler.setClosingTime(true);
        defectScanner2.setClosingTime(true);
        sealer.setClosingTime(true);
        defectScanner3.setClosingTime(true);
        labeler.setClosingTime(true);
        packager.setClosingTime(true);
        forklift1.setClosingTime(true);
        forklift2.setClosingTime(true);
        forklift3.setClosingTime(true);
        van1.setClosingTime(true);
        van2.setClosingTime(true);
        van3.setClosingTime(true);

        // Waiting to end the thread
        try {
            defectScanner1Thread.join();
            sterilizerThread.join();
            fillerThread.join();
            defectScanner2Thread.join();
            sealerThread.join();
            defectScanner3Thread.join();
            labelerThread.join();
            packagerThread.join();
            forklift1Thread.join();
            forklift2Thread.join();
            forklift3Thread.join();
            van1Thread.join();
            van2Thread.join();
            van3Thread.join();
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }

        try {
            Thread.sleep(10000); // Wait time to complete additional tasks
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }

        // Termination Signaling
        defectScanner1.setClosingTime(true);
        sterilizer.setClosingTime(true);
        filler.setClosingTime(true);
        defectScanner2.setClosingTime(true);
        sealer.setClosingTime(true);
        defectScanner3.setClosingTime(true);
        labeler.setClosingTime(true);
        packager.setClosingTime(true);
        forklift1.setClosingTime(true);
        forklift2.setClosingTime(true);
        forklift3.setClosingTime(true);
        van1.setClosingTime(true);
        van2.setClosingTime(true);
        van3.setClosingTime(true);
        
        // Statistics output
        stats.printStatistics();
        System.out.println("Factory simulation completed.");
    }
}
