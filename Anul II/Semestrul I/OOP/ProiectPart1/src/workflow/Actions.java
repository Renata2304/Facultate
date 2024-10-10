package workflow;

import input.ActionInput;
import input.Const;
import input.Input;
import input.MovieInput;
import input.user.UserInput;
import pages.Page;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.node.ArrayNode;
import pages.Upgrades;

import java.util.ArrayList;
import java.util.Objects;

public final class Actions {

    private Actions() {

    }
    /**
     *
     */
    public static void action(final ObjectMapper objectMapper, final Input inputData,
                              final ArrayNode output) {
        Page crtPage = new Page();
        crtPage.setPageType("homepage neautentificat");
        ArrayList<MovieInput> crtMovies = new ArrayList<>();
        UserInput crtUser = new UserInput();

        for (ActionInput action: inputData.getActions()) {
            switch (action.getType()) {
                case "on page" -> {
                    // testing to see if there are any errors
                    boolean verif = Errors.checkErrorFeatureOnPage(crtPage.getPageType(), action);
                    if (!verif) {
                        // printing the error
                        OutPrint.printError(output);
                        break;
                    }
                    switch (action.getFeature()) {
                        case "register" -> {
                            crtUser = pages.Register.getRegister().initialOnPage(inputData, action,
                                    output, objectMapper, crtPage, crtUser, crtMovies);
                        }
                        case "login" -> {
                            crtUser = pages.Login.getLogin().initialOnPage(inputData, action,
                                    output, objectMapper, crtPage, crtUser, crtMovies);
                        }
                        case "search" -> {
                            pages.Movies.getMovies().onPageSearch(objectMapper, output, action,
                                    crtUser, crtMovies);
                        }
                        case "filter" -> {
                            crtMovies = pages.Movies.getMovies().onPageFilter(objectMapper,
                                    inputData, output, action, crtUser);
                        }
                        case "buy tokens" -> {
                            Upgrades.getUpgrades().onPageBuyTokens(action, crtUser);
                        }
                        case "buy premium account" -> {
                            Upgrades.getUpgrades().onPageBuyPremium(output, crtUser);
                        }
                        case "purchase" -> {
                            if (Objects.equals(crtUser.getCredentials().getAccountType(),
                                                "premium")) {
                                if (crtUser.getNumFreePremiumMovies() > 0) {
                                    crtUser.setNumFreePremiumMovies(crtUser.
                                            getNumFreePremiumMovies() - 1);
                                } else {
                                    crtUser.setTokenCount(crtUser.getTokenCount()
                                            - Const.NO_TOKENS_MOVIE);
                                }
                            } else {
                                crtUser.setTokenCount(crtUser.getTokenCount()
                                        - Const.NO_TOKENS_MOVIE);
                            }
                            crtUser.getCurrentMovie().setPurchased(true);
                            crtUser.getPurchasedMovies().add(crtUser.getCurrentMovie());

                            ArrayList<MovieInput> currentMovie = new ArrayList<>();
                            currentMovie.add(crtUser.getCurrentMovie());

                            OutPrint.printNoError(objectMapper, output, crtUser, currentMovie);
                        }
                        case "watch" -> {
                            pages.Movies.getMovies().onPageWatch(objectMapper, output, crtUser);
                        }
                        case "like" -> {
                            pages.Movies.getMovies().onPageLike(objectMapper, output, crtUser);
                        }
                        case "rate" -> {
                            pages.Movies.getMovies().onPageRate(objectMapper, output,
                                    action, crtUser);
                        }
                        default -> {
                        }
                    }
                }
                case "change page" -> {
                    boolean ok = Errors.checkErrorChangePage(crtPage.getPageType(),
                            action);
                    if (!ok) {
                        OutPrint.printError(output);
                    } else {
                        switch (action.getPage()) {
                            case "register" -> {
                                pages.Register.getRegister().
                                        changePage(output, action, crtPage);
                            }
                            case "login" -> {
                                pages.Login.getLogin().changePage(output, action, crtPage);
                            }
                            case "logout" -> {
                                crtMovies.removeAll(crtMovies);
                                crtUser = new UserInput();
                                crtPage.setPageType("homepage neautentificat");
                            }
                            case "movies" -> {
                                crtMovies = pages.Movies.getMovies().changePageMovie(objectMapper,
                                        output, crtPage, crtUser, inputData);
                            }
                            case "see details" -> {
                                pages.SeeDetails.getSeeDetails().changePageSeeDetails(output,
                                        objectMapper, crtUser, crtMovies, action, crtPage);
                            }
                            case "upgrades" -> {
                                crtPage.setPageType("upgrades");
                            }
                            default -> {
                            }
                        }
                    }
                }
                default -> {

                }
            }
        }
    }
}
