package workflow;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.node.ArrayNode;
import com.fasterxml.jackson.databind.node.ObjectNode;
import input.MovieInput;
import input.user.UserInput;

import java.util.ArrayList;

public final class OutPrint {

    private OutPrint() {

    }

    /**
     * Prints all the parameters if no error occurred.
     */
    public static void printNoError(final ObjectMapper objectMapper, final ArrayNode output,
                                    final UserInput crtUser,
                                    final ArrayList<MovieInput> crtMovies) {
        ObjectNode outputNode = output.addObject();
        outputNode.putPOJO("error", null);
        OutPrint.printCurrentMoviesList(crtMovies, outputNode);
        OutPrint.printCurrentUser(objectMapper, crtUser, outputNode);
    }

    /**
     * Prints the current user's fields.
     */
    public static void printCurrentUser(final ObjectMapper objectMapper,
                                        final UserInput user, final ObjectNode output) {
        ObjectNode currentUsers = objectMapper.createObjectNode();

        ObjectNode jsonNodes2 = objectMapper.createObjectNode();
        jsonNodes2.put("name", user.getCredentials().getName());
        jsonNodes2.put("password", user.getCredentials().getPassword());
        jsonNodes2.put("accountType", user.getCredentials().getAccountType());
        jsonNodes2.put("country", user.getCredentials().getCountry());
        jsonNodes2.put("balance", user.getCredentials().getBalance());

        currentUsers.putPOJO("credentials", jsonNodes2);

        currentUsers.put("tokensCount", user.getTokenCount());
        currentUsers.put("numFreePremiumMovies", user.getNumFreePremiumMovies());

        ArrayNode purchasedMovies = currentUsers.putArray("purchasedMovies");
        if (user.getPurchasedMovies() != null) {
            for (MovieInput movie: user.getPurchasedMovies()) {
                printMovie(movie, purchasedMovies);
            }
        }

        ArrayNode watchedMovies = currentUsers.putArray("watchedMovies");
        if (user.getWatchedMovies() != null) {
            for (MovieInput movie : user.getWatchedMovies()) {
                printMovie(movie, watchedMovies);
            }
        }
        ArrayNode likedMovies = currentUsers.putArray("likedMovies");
        if (user.getLikedMovies() != null) {
            for (MovieInput movie : user.getLikedMovies()) {
                printMovie(movie, likedMovies);
            }
        }
        ArrayNode ratedMovies = currentUsers.putArray("ratedMovies");
        if (user.getRatedMovies() != null) {
            for (MovieInput movie : user.getRatedMovies()) {
                printMovie(movie, ratedMovies);
            }
        }

        output.putPOJO("currentUser", currentUsers);
    }

    /**
     * Prints a single movie's fields.
     */
    public static void printMovie(final MovieInput movie, final ArrayNode allMoviesNode) {
        ObjectNode movieNode = allMoviesNode.addObject();
        movieNode.put("name", movie.getName());
        movieNode.put("year", movie.getYear());
        movieNode.put("duration", movie.getDuration());
        movieNode.put("numLikes", movie.getNumLikes());
        // different printing, based on the rating number
        if (movie.getNumRatings() != 0) {
            movieNode.put("rating", movie.getRating() / movie.getNumRatings());
        } else {
            movieNode.put("rating", movie.getRating());
        }
        movieNode.put("numRatings", movie.getNumRatings());

        ArrayNode genresNode = movieNode.putArray("genres");
        for (String genres : movie.getGenres()) {
            genresNode.add(genres);
        }
        ArrayNode actorsNode = movieNode.putArray("actors");
        for (String actors : movie.getActors()) {
            actorsNode.add(actors);
        }
        ArrayNode countriesBannedNode = movieNode.putArray("countriesBanned");
        for (String countriesBanned : movie.getCountriesBanned()) {
            countriesBannedNode.add(countriesBanned);
        }
    }

    /**
     * Prints the current movie list's fields, one movie at a time.
     */
    public static void printCurrentMoviesList(final ArrayList<MovieInput> movies,
                                              final ObjectNode output) {
        if (movies == null) {
            return;
        }

        ArrayNode allMoviesNode = output.putArray("currentMoviesList");

        for (MovieInput movie: movies) {
            printMovie(movie, allMoviesNode);
        }
    }

    /**
     * Prints the output of an error occurred.
     */
    public static void printError(final ArrayNode output) {
        ObjectNode outputNode = output.addObject();
        outputNode.putPOJO("error", "Error");
        OutPrint.printCurrentMoviesList(new ArrayList<MovieInput>(),
                outputNode);
        outputNode.putPOJO("currentUser", null);
    }
}
