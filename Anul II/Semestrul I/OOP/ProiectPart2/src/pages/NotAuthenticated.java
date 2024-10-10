package pages;

public final class NotAuthenticated extends Page {

    private static NotAuthenticated instance = null;

    NotAuthenticated() {

    }

    /**
     * Function used for creating the singleton in the Movies class.
     * @return the instance used for singleton
     */
    public static NotAuthenticated getNonAuthenticated() {
        if (instance == null) {
            instance = new NotAuthenticated();
        }
        return instance;
    }
}
