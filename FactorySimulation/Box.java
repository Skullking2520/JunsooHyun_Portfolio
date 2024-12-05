// Box.java
import java.util.List;

public class Box {
    private final List<Pack> packs;
    private final int id;

    public Box(List<Pack> packs, int id) {
        this.packs = packs;
        this.id = id;
    }

    public List<Pack> getPacks() {
        return packs;
    }

    public int getId() {
        return id;
    }
}
