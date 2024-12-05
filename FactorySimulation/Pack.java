// Pack.java
import java.util.List;

public class Pack {
    private final List<Can> cans;
    private final int id;

    public Pack(List<Can> cans, int id) {
        this.cans = cans;
        this.id = id;
    }

    public List<Can> getCans() {
        return cans;
    }

    public int getId() {
        return id;
    }
}
