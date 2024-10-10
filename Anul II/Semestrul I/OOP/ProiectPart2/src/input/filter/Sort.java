package input.filter;

public final class Sort {
    private String rating;
    private String duration;

    private Sort() {

    }
    public String getRating() {
        return rating;
    }

    public void setRating(final String rating) {
        this.rating = rating;
    }

    public String getDuration() {
        return duration;
    }

    public void setDuration(final String duration) {
        this.duration = duration;
    }
}
