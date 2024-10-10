package workflow;

import input.ActionInput;
import java.util.Objects;

public final class Errors {

    private Errors() {
    }

    /**
     * Checking to see if there may be any cases that can prevent the user from changing the page.
     * @return false -> there is an error / true -> otherwise
     */
    public static boolean checkErrorChangePage(final String currPage, final ActionInput action) {
        // comparing the current page to the next desired page
        String nextPage = action.getPage();
        switch (currPage) {
            case "homepage neautentificat" -> {
                if (nextPage.equals("login")) {
                    return true;
                }
                if (nextPage.equals("register")) {
                    return true;
                }
                return false;
            }
            case "movies" -> {
                if (Objects.equals(nextPage, "upgrades") || Objects.equals(nextPage, "login")
                    || nextPage.equals("register")) {
                    return false;
                }
                return true;
            }
            case "homepage autentificat", "see details", "upgrades" -> {
                if (Objects.equals(nextPage, "login") || nextPage.equals("register")) {
                    return false;
                }
                return true;
            }
            default -> {
                return true;
            }
        }
    }

    /**
     * Checking to see if there are any errors that can prvent the user from applying a fearture
     * during the on page action.
     * @return true -> no error occurred / false -> an error occurred
     */
    public static boolean checkErrorFeatureOnPage(final String currPage,
                                                  final ActionInput action) {
        if (currPage.equals("login") && Objects.equals(action.getFeature(), "login")) {
            return true;
        }
        if (currPage.equals("register") && Objects.equals(action.getFeature(), "register")) {
            return true;
        }
        if (currPage.equals("movies") && (Objects.equals(action.getFeature(), "search")
            || Objects.equals(action.getFeature(), "filter"))) {
            return true;
        }
        if (currPage.equals("upgrades") && (Objects.equals(action.getFeature(), "buy tokens")
            || Objects.equals(action.getFeature(), "buy premium account"))) {
            return true;
        }
        if (currPage.equals("see details") && (Objects.equals(action.getFeature(), "purchase")
                || Objects.equals(action.getFeature(), "watch")
                || Objects.equals(action.getFeature(), "like")
                || Objects.equals(action.getFeature(), "rate"))) {
            return true;
        }
        return false;
    }

}
