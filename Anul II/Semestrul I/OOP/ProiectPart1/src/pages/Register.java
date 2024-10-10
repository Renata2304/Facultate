package pages;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.node.ArrayNode;
import com.fasterxml.jackson.databind.node.ObjectNode;
import input.ActionInput;
import input.Input;
import input.MovieInput;
import input.user.UserInput;
import workflow.Errors;
import workflow.OutPrint;

import java.util.ArrayList;
import java.util.Objects;

public final class Register extends Page {

    private static Register instance = null;

    Register() {

    }

    /**
     * Function used for creating the singleton in the Login class.
     * @return the instance used for singleton
     */
    public static Register getRegister() {
        if (instance == null) {
            instance = new Register();
        }
        return instance;
    }

    /**
     * Function used for the case when a user is trying to register.
     * @return the current user that is going to be returned, if the action is finished
     * successfully or null otherwise (+ an error will be printed).
     */
    @Override
    public UserInput initialOnPage(final Input inputData, final ActionInput action,
                            final ArrayNode output, final ObjectMapper objectMapper,
                            final Page crtPage, UserInput crtUser,
                            final ArrayList<MovieInput> crtMovies) {
        boolean ok = true;
        // searching to see if the user already exists
        for (UserInput user : inputData.getUsers()) {
            if (Objects.equals(user.getCredentials().getName(),
                    action.getCredentials().getName())) {
                ok = false;
                break;
            }
        }
        if (ok) {
            // if the user doesn't already exist, a new user will be created and added
            // to the users list.
            crtPage.setPageType("homepage autentificat");
            UserInput newUser = new UserInput(action.getCredentials());
            inputData.getUsers().add(newUser);
            crtUser = newUser;
            ObjectNode outputNode = output.addObject();
            outputNode.putPOJO("error", null);
            OutPrint.printCurrentMoviesList(crtMovies, outputNode);
            OutPrint.printCurrentUser(objectMapper, crtUser, outputNode);

        } else {
            // the user already exists, so an error will be printed
            OutPrint.printError(output);
            crtPage.setPageType("homepage neautentificat");
        }

        return crtUser;
    }

    /**
     * Function used for the case change page -> login. If the action is finished successfully, the
     * page type will be changed, otherwise an error will be printed.
     */
    @Override
    public void changePage(final ArrayNode output, final ActionInput action, final Page crtPage) {
        boolean ok = Errors.checkErrorChangePage(crtPage.getPageType(), action);
        if (ok) {
            // if the action is finished successfully, the page type will be changed to register
            crtPage.setPageType("register");
        } else {
            // if an error occurred
            OutPrint.printError(output);
        }
    }

}
