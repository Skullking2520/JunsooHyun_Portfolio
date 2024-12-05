// LoadingBay.java
import java.util.concurrent.Semaphore;

public class LoadingBay {
    private Semaphore bays;

    public LoadingBay(int numberOfBays) {
        bays = new Semaphore(numberOfBays);
    }

    public void acquireBay(String vanName) throws InterruptedException {
        bays.acquire();
        System.out.println(vanName + ": Acquired a loading bay.");
    }

    public void releaseBay(String vanName) {
        bays.release();
        System.out.println(vanName + ": Released a loading bay.");
    }
}
