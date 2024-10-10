package input.user;

import java.util.ArrayList;

public final class Notifications {

    private String movieName;
    private String message;

    public Notifications() {

    }

    /**
     * Method used for the queue of notifications that will be used after the action iteration.
     * Method will mimic the enqueue action of the queue, adding a new element.
     */
    public static void enqueue(final ArrayList<Notifications> arrayList,
                               final Notifications elem) {
        arrayList.add(elem);
    }

    public String getMovieName() {
        return movieName;
    }

    public void setMovieName(final String movieName) {
        this.movieName = movieName;
    }

    public String getMessage() {
        return message;
    }

    public void setMessage(final String message) {
        this.message = message;
    }
}
