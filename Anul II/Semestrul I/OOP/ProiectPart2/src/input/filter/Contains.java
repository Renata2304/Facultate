package input.filter;

import java.util.ArrayList;

public final class Contains {
    private ArrayList<String> actors = new ArrayList<>();
    private ArrayList<String> genre = new ArrayList<>();

    private Contains() {

    }
    public ArrayList<String> getActors() {
        return actors;
    }

    public void setActors(final ArrayList<String> actors) {
        this.actors = actors;
    }

    public ArrayList<String> getGenre() {
        return genre;
    }

    public void setGenre(final ArrayList<String> genre) {
        this.genre = genre;
    }
}
