package workflow;

import com.fasterxml.jackson.databind.node.ArrayNode;
import input.ActionInput;
import input.Const;
import input.Input;
import input.MovieInput;
import input.user.Notifications;
import input.user.UserInput;

import java.util.ArrayList;

public final class Database {

    private Database() {

    }

    /**
     * Method used for the database -> delete case.
     * If the added movie doesn't exist, an error will occur. Otherwise, the movie will be deleted
     * from the input data list, the current movie list, and from the user's watched, purchased,
     * liked, rated movie list.
     * In the end, if the action is completed, the user will be notified.
     */
    public static void deleteMovie(final Input inputData, final ActionInput action,
                                   final ArrayList<MovieInput> crtMovies,
                                   final UserInput crtUser,
                                   final ArrayNode output) {
        if (inputData.getMovies().stream().map(MovieInput::getName).
                noneMatch(name -> name.equals(action.getDeletedMovie()))) {
            OutPrint.printError(output);
        } else {
            inputData.getMovies().removeIf(movie -> movie.getName()
                    .equals(action.getDeletedMovie()));
            crtMovies.removeIf(movie -> movie.getName()
                    .equals(action.getDeletedMovie()));
            Database.deleteMovieFromUser(inputData, crtUser, action);
            Database.notifyDelete(action, inputData.getUsers());
        }
    }

    /**
     * Method used to notify the current user in the case of a movie deletion. If the account is
     * a premium one, if the user bought the said movie, one free premium movie will be added
     * to their account. Otherwise, 2 tokens will be added.
     */
    public static void notifyDelete(final ActionInput action, final ArrayList<UserInput> users) {
        for (UserInput crtUser: users) {
            for (MovieInput movie : crtUser.getPurchasedMovies()) {
                if (movie.getName().equals(action.getDeletedMovie())) {
                    if (crtUser.getCredentials().getAccountType().equals("premium")) {
                        crtUser.setNumFreePremiumMovies(crtUser.getNumFreePremiumMovies() + 1);
                    } else {
                        crtUser.setTokenCount(crtUser.getTokenCount() + Const.NO_TOKENS_MOVIE);
                    }
                    Notifications notifications = new Notifications();
                    notifications.setMessage("DELETE");
                    notifications.setMovieName(action.getDeletedMovie());
                    Notifications.enqueue(crtUser.getNotifications(), notifications);

                    break;
                }
            }
        }
    }

    /**
     * Method used for erasing the movie that will be deleted from the user's watched, purchased,
     * liked, rated movie list. The method will also erase the movie from the input data.
     */
    public static void deleteMovieFromUser(final Input inputData, final UserInput currentUser,
                                           final ActionInput action) {
        inputData.getMovies().removeIf(movie -> movie.getName()
                .equals(action.getDeletedMovie()));
        currentUser.getPurchasedMovies().removeIf(movie -> movie.getName()
                .equals(action.getDeletedMovie()));
        currentUser.getLikedMovies().removeIf(movie -> movie.getName()
                .equals(action.getDeletedMovie()));
        currentUser.getWatchedMovies().removeIf(movie -> movie.getName()
                .equals(action.getDeletedMovie()));
        currentUser.getRatedMovies().removeIf(movie -> movie.getName()
                .equals(action.getDeletedMovie()));
    }

    /**
     * Method used for the database -> add case.
     * If the added movie already exists, an error will occur. Otherwise, the movie will be added
     * to the input data list, and to the current movie list, if the user is not from one of the
     * banned countries.
     * In the end, if the action is completed, the user will be notified.
     */
    public static void addMovie(final Input inputData, final ActionInput action,
                                final ArrayList<MovieInput> crtMovies,
                                final UserInput crtUser,
                                final ArrayNode output) {
        if (inputData.getMovies().stream().map(MovieInput::getName).
                anyMatch(name -> name
                        .equals(action.getAddedMovie().getName()))) {
            OutPrint.printError(output);
        } else {
            inputData.getMovies().add(action.getAddedMovie());
            Database.notifyAdd(action, inputData.getUsers());

            if (!action.getAddedMovie().getCountriesBanned()
                    .contains(crtUser.getCredentials().getCountry())) {
                crtMovies.add(action.getAddedMovie());
            }
        }
    }

    /**
     * Method used to notify the current user in the case of a movie deletion. If the movie that
     * is going to be added has at least one genre that the user subscribed to, they will be
     * notified.
     */
    public static void notifyAdd(final ActionInput action, final ArrayList<UserInput> users) {
        for (UserInput crtUser : users) {
            for (int i = 0; i < action.getAddedMovie().getGenres().size(); i++) {
                if (crtUser.getSubscribedGenres()
                        .contains(action.getAddedMovie().getGenres().get(i))
                    && !action.getAddedMovie().getCountriesBanned()
                        .contains(crtUser.getCredentials().getCountry())) {

                    Notifications notifications = new Notifications();
                    notifications.setMessage("ADD");
                    notifications.setMovieName(action.getAddedMovie().getName());
                    Notifications.enqueue(crtUser.getNotifications(), notifications);
                    break;
                }
            }
        }
    }
}
