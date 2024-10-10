package pages;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.node.ArrayNode;
import com.fasterxml.jackson.databind.node.ObjectNode;
import input.ActionInput;
import input.MovieInput;
import input.user.UserInput;
import workflow.OutPrint;

import java.util.ArrayList;
import java.util.Objects;

public final class SeeDetails extends Page {

    private static SeeDetails instance = null;

    private SeeDetails() {

    }

    /**
     * Function used for creating the singleton in the SeeDetails class.
     * @return the instance used for singleton
     */
    public static SeeDetails getSeeDetails() {
        if (instance == null) {
            instance = new SeeDetails();
        }
        return instance;
    }

    /**
     * Used for the change page -> see details action.
     */
    public void changePageSeeDetails(final ArrayNode output, final ObjectMapper objectMapper,
                                    final UserInput crtUser, final ArrayList<MovieInput> crtMovies,
                                    final ActionInput action, final Page crtPage) {
        boolean ok = false;
        // searching for the desired movie
        for (MovieInput movie : crtMovies) {
            if (Objects.equals(movie.getName(), action.getMovie())) {
                ok = true;
                crtUser.setCurrentMovie(movie);
                break;
            }
        }
        if (!ok) {
            OutPrint.printError(output); // the movie wasn't found
        } else {
            crtPage.setPageType("see details");
            ArrayList<MovieInput> currentMovie = new ArrayList<>();
            currentMovie.add(crtUser.getCurrentMovie());
            // printing the parameters
            OutPrint.printNoError(objectMapper, output, crtUser,currentMovie);
        }
    }

}
