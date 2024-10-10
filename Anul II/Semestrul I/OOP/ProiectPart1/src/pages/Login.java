package pages;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.node.ArrayNode;
import input.ActionInput;
import input.Input;
import input.MovieInput;
import input.user.UserInput;
import workflow.Errors;
import workflow.OutPrint;

import java.util.ArrayList;
import java.util.Objects;

public final class Login extends Page {

    private static Login instance = null;

    private Login() {

    }

    /**
     * Function used for creating the singleton in the Login class.
     * @return the instance used for singleton
     */
    public static Login getLogin() {
        if (instance == null) {
            instance = new Login();
        }
        return instance;
    }

    /**
     * Function used for the case when a user is trying to log in.
     * @return the current user that is going to be returned, if the action is finished
     * successfully or null otherwise (+ an error will be printed).
     */
    @Override
    public UserInput initialOnPage(final Input inputData, final ActionInput action,
                            final ArrayNode output, final ObjectMapper objectMapper,
                            final Page crtPage, final UserInput crtUser,
                            final ArrayList<MovieInput> crtMovies) {
        // going through all the users in the input list
        for (UserInput user: inputData.getUsers()) {
            // if the user's account already exists, they will be logged in
            if (Objects.equals(user.getCredentials().getName(), action.getCredentials().getName())
                && Objects.equals(user.getCredentials().getPassword(),
                    action.getCredentials().getPassword())) {
                // changing the page to the authenticated one and printing the output
                crtPage.setPageType("homepage autentificat");
                OutPrint.printNoError(objectMapper, output, user, crtMovies);
                return user; // returning the current user's account
            }
        }
        // if the user tried to log in with an account that didn't exist an error will be
        // displayed
        OutPrint.printError(output);
        crtPage.setPageType("homepage neautentificat");
        return null;
    }

    /**
     * Function used for the case change page -> login. If the action is finished successfully, the
     * page type will be changed, otherwise an error will be printed.
     */
    @Override
    public void changePage(final ArrayNode output, final ActionInput action, final Page crtPage) {
        boolean ok = Errors.checkErrorChangePage(crtPage.getPageType(), action);
        if (ok) {
            // if the action is finished successfully, the page type will be changed to login
            crtPage.setPageType("login");
        } else {
            // if an error occurred
            OutPrint.printError(output);
        }
    }
}
