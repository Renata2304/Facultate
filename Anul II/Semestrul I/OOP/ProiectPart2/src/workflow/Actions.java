package workflow;

import input.ActionInput;
import input.Input;
import input.MovieInput;
import input.user.Notifications;
import input.user.UserInput;
import pages.Page;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.node.ArrayNode;
import pages.PageStack;
import pages.Upgrades;

import java.util.ArrayList;

public final class Actions {

    private Actions() {

    }

    public static void action(final ObjectMapper objectMapper, Input inputData,
                              final ArrayNode output) {
        Page crtPage = new Page();
        crtPage.setPageType("homepage neautentificat");
        ArrayList<MovieInput> crtMovies = new ArrayList<>();
        UserInput crtUser = new UserInput();
        ArrayList<String> stack = new ArrayList<>();
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
                            pages.Movies.getMovies().onPagePurchase(objectMapper, output, crtUser);
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
                        case "subscribe" -> {
                            if (!crtUser.getCurrentMovie().getGenres()
                                    .contains(action.getSubscribedGenre())) {
                                OutPrint.printError(output);
                            } else {
                                crtUser.getSubscribedGenres().add(action.getSubscribedGenre());
                            }
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
                                stack.clear();
                            }
                            case "movies" -> {
                                crtMovies = pages.Movies.getMovies().changePageMovie(objectMapper,
                                        output, crtPage, crtUser, inputData, stack);
                            }
                            case "see details" -> {
                                pages.SeeDetails.getSeeDetails().changePageSeeDetails(output,
                                        objectMapper, crtUser, crtMovies, action, crtPage, stack);
                            }
                            case "upgrades" -> {
                                crtPage.setPageType("upgrades");
                                PageStack.push(stack, "upgrades");
                            }
                            default -> {
                            }
                        }
                    }
                }
                case "back" -> {
                    if (stack.isEmpty()
                            || crtPage.getPageType().equals("homepage neautentificat")) {
                        OutPrint.printError(output);
                    } else {
                        if (crtPage.getPageType().equals("see details")) {
                            crtMovies = pages.Movies.getMovies().changePageMovie(objectMapper,
                                output, crtPage, crtUser, inputData, stack);
                        }
                        crtPage.setPageType(PageStack.pop(stack));
                    }
                }
                case "database" -> {
                    switch (action.getFeature()) {
                        case "add" -> {
                            Database.addMovie(inputData, action, crtMovies, crtUser, output);
                        }
                        case "delete" -> {
                            Database.deleteMovie(inputData, action, crtMovies, crtUser, output);
                        }
                        default -> {
                        }
                    }
                }
                default -> {
                }
            }
        }
        // if the current user isn't logged out of their account and if the account is a premium
        // one, a recommendation will be delivered.
        if (!crtPage.getPageType().equals("homepage neautentificat")) {
            if (crtUser.getCredentials().getAccountType().equals("premium")) {
                if (crtUser.getLikedMovies().isEmpty()) {
                    Notifications notifications = new Notifications();
                    notifications.setMessage("Recommendation");
                    notifications.setMovieName("No recommendation");
                    Notifications.enqueue(crtUser.getNotifications(), notifications);

                    OutPrint.printNoErrorNotif(objectMapper, output, crtUser);
                }
            }
        }
    }
}
