package pages;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.node.ArrayNode;
import input.ActionInput;
import input.Input;
import input.MovieInput;
import input.user.UserInput;

import java.util.ArrayList;

/**
 * The template used for all the pages to extend.
 */
public class Page {

    private String pageType;

    /**
     * Function used for the case when a user is trying to log in or register.
     * @return the new user that is going to be saved in crtUser.
     */
    public UserInput initialOnPage(final Input inputData, final ActionInput action,
                            final ArrayNode output, final ObjectMapper objectMapper,
                            final Page crtPage, final UserInput crtUser,
                            final ArrayList<MovieInput> crtMovies) {
        return new UserInput();
    }

    /**
     * Function used for the case change page -> login / register. If the action is finished
     * successfully, the page type will be changed, otherwise an error will be printed.
     */
    public void changePage(final ArrayNode output, final ActionInput action,
                           final Page crtPage) {

    }

    /**
     * @return the page type.
     */
    public String getPageType() {
        return pageType;
    }

    /**
     * @param pageType -> changing the type of the page.
     */
    public void setPageType(final String pageType) {
        this.pageType = pageType;
    }


}
