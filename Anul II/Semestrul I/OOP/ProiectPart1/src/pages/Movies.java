package pages;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.node.ArrayNode;
import input.ActionInput;
import input.Const;
import input.Input;
import input.MovieInput;
import input.filter.Sort;
import input.user.UserInput;
import workflow.OutPrint;

import java.util.ArrayList;
import java.util.Comparator;

public final class Movies extends Page {
    private static Movies instance = null;

    public Movies() {

    }

    /**
     * Function used for creating the singleton in the Movies class.
     * @return the instance used for singleton
     */
    public static Movies getMovies() {
        if (instance == null) {
            instance = new Movies();
        }
        return instance;
    }

    /**
     * Function used for the change page -> movie case. The program will go through all the movies
     * and add those that aren't banned in the user's country.
     * @return the list of movies that aren't banned in the user's country.
     */
    public ArrayList<MovieInput> changePageMovie(final ObjectMapper objectMapper,
                                                 final ArrayNode output, final Page crtPage,
                                                 final UserInput crtUser, final Input inputData) {
        crtPage.setPageType("movies"); // changing the page type
        ArrayList<MovieInput> crtMovies = new ArrayList<>();
        // going through all the movies and adding those that aren't banned in the user's country
        for (MovieInput movie : inputData.getMovies()) {
            if (!movie.getCountriesBanned().
                    contains(crtUser.getCredentials().getCountry())) {
                crtMovies.add(movie);
            }
        }
        // printing the fields
        OutPrint.printNoError(objectMapper, output, crtUser, crtMovies);

        // return the list of movies that aren't banned in the user's country
        return crtMovies;
    }

    /**
     * Function used for the on page -> search case. The program will go throug all the current
     * movies and print all the movies that start with a certain string.
     */
    public void onPageSearch(final ObjectMapper objectMapper, final ArrayNode output,
                             final ActionInput action, final UserInput crtUser,
                             final ArrayList<MovieInput> crtMovies) {
        ArrayList<MovieInput> search = new ArrayList<>();
        for (MovieInput movie : crtMovies) {
            if (movie.getName().startsWith(action.getStartsWith())) {
                search.add(movie);
            }
        }
        // printing the fields
        OutPrint.printNoError(objectMapper, output, crtUser, search);
    }

    /**
     * Function used for the on page -> filter case. The program will recreate the movie of
     * lists that aren't banned in the user's country. Afterwards, the list will be sorted
     * and filters will be applied over it.
     * @return the list will be returned.
     */
    public ArrayList<MovieInput> onPageFilter(final ObjectMapper objectMapper,
                                              final Input inputData,
                                              final ArrayNode output, final ActionInput action,
                                              final UserInput crtUser) {
        // creating a new array of movies that aren't banned in the user's country.
        ArrayList<MovieInput> crtMovies = new ArrayList<>();
        for (MovieInput movie : inputData.getMovies()) {
            if (!movie.getCountriesBanned().
                    contains(crtUser.getCredentials().getCountry())) {
                crtMovies.add(movie);
            }
        }
        // if there are any filters given
        if (action.getFilters() != null) {
            if (action.getFilters().getSort() != null) {
                crtMovies.sort(Movies.getComparator(action.getFilters().getSort()));
            }
            if (action.getFilters().getContains() != null) {
                // sorting the movie list
                ArrayList<String> genreFilter = action.getFilters().
                        getContains().getGenre();
                ArrayList<String> actorsFilter = action.getFilters().
                        getContains().getActors();
                // removing the movies that don't contain the genres given
                if (genreFilter != null) {
                    crtMovies.removeIf(movie -> !movie.getGenres().
                            containsAll(genreFilter));
                }
                // removing the movies that don't contain the actors given
                if (actorsFilter != null) {
                    crtMovies.removeIf(movie -> !movie.getActors().
                            containsAll(actorsFilter));
                }
            }
            // printing the fields
            OutPrint.printNoError(objectMapper, output, crtUser, crtMovies);
        }
        // returning the list of movies after the filters have been applied
        return crtMovies;
    }

    /**
     * Function used for the on page -> watch case. If the movie isn't purchased, an error will
     * be displayed. Otherwise, the movies will be added to the watched list and the parameters
     * will be displayed.
     */
    public void onPageWatch(final ObjectMapper objectMapper, final ArrayNode output,
                            final UserInput crtUser) {
        // if the movie hasn't been burchased, an error will be displayed
        if (!crtUser.getPurchasedMovies().contains(crtUser.getCurrentMovie())) {
            OutPrint.printError(output);
        } else {
            // the movie will be added to the watched movies list
            crtUser.getWatchedMovies().add(crtUser.getCurrentMovie());
            crtUser.getCurrentMovie().setWatched(true);
            // printing the parameters
            ArrayList<MovieInput> currentMovie = new ArrayList<>();
            currentMovie.add(crtUser.getCurrentMovie());
            OutPrint.printNoError(objectMapper, output, crtUser, currentMovie);
        }
    }

    /**
     * Function used for the on page -> like case. If the movie isn't purchased or watched,
     * an error will be displayed. Otherwise, the movies will be added to the liked list and
     * the parameters will be displayed.
     */
    public void onPageLike(final ObjectMapper objectMapper, final ArrayNode output,
                           final UserInput crtUser) {
        // if the movie isn't purchased or watched -> error
        if (!crtUser.getPurchasedMovies().contains(crtUser.getCurrentMovie())) {
            OutPrint.printError(output);
        } else if (!crtUser.getCurrentMovie().isWatched()) {
            OutPrint.printError(output);
        } else {
            // adding the movie to the liked list
            crtUser.getLikedMovies().add(crtUser.getCurrentMovie());
            crtUser.getCurrentMovie().
                    setNumLikes(crtUser.getCurrentMovie().getNumLikes() + 1);
            // printing the parameters
            ArrayList<MovieInput> currentMovie = new ArrayList<>();
            currentMovie.add(crtUser.getCurrentMovie());
            OutPrint.printNoError(objectMapper, output, crtUser, currentMovie);
        }
    }

    /**
     * Function used for the on page -> rate case. If the movie isn't purchased or watched,
     * an error will be displayed. Otherwise, the movies will be added to the rated list and
     * the parameters will be displayed.
     */
    public void onPageRate(final ObjectMapper objectMapper, final ArrayNode output,
                           final ActionInput action, final UserInput crtUser) {
        // if the movie isn't purchased or watched -> error
        if (!crtUser.getPurchasedMovies().contains(crtUser.getCurrentMovie())) {
            OutPrint.printError(output);
        } else if (!crtUser.getWatchedMovies().contains(crtUser.getCurrentMovie())) {
            OutPrint.printError(output);
        } else if (action.getRate() > Const.MAX_RATING) {
            OutPrint.printError(output); // if the user gives a rating over 5
        } else {
            // adding the movie to the rated list
            crtUser.getRatedMovies().add(crtUser.getCurrentMovie());
            // increasing the number of ratings given to the movie
            crtUser.getCurrentMovie().setNumRatings(crtUser.
                    getCurrentMovie().getNumRatings() + 1);
            crtUser.getCurrentMovie().setRating(crtUser.
                    getCurrentMovie().getRating() + action.getRate());
            // printing the parameters
            ArrayList<MovieInput> currentMovie = new ArrayList<>();
            currentMovie.add(crtUser.getCurrentMovie());
            OutPrint.printNoError(objectMapper, output, crtUser, currentMovie);
        }
    }

    /**
     * Function used for sorting the movie lists, based on the filters given.
     */
    public static Comparator<MovieInput> getComparator(final Sort sort) {
        Comparator<MovieInput> comparator;
        if (sort.getDuration() == null) {
            comparator = Comparator.comparing(MovieInput :: getRating);
            if (sort.getRating().equals("decreasing")) {
                comparator = Comparator.comparing(MovieInput::getRating,
                        Comparator.reverseOrder());
            }
            return comparator;
        }
        comparator = Comparator.comparing(MovieInput::getDuration);
        if (sort.getDuration().equals("decreasing")) {
            comparator = Comparator.comparing(MovieInput::getDuration, Comparator.reverseOrder());
        }
        if (sort.getRating() != null) {
            comparator = comparator.thenComparing(MovieInput::getRating);
            if (sort.getRating().equals("decreasing")) {
                comparator = comparator.reversed();
            }
        }
        return comparator;
    }
}
