package pages;

import com.fasterxml.jackson.databind.node.ArrayNode;
import input.ActionInput;
import input.Const;
import input.user.UserInput;
import workflow.OutPrint;

import java.util.Objects;

public final class Upgrades extends Page {

    private static Upgrades instance = null;

    private Upgrades() {

    }

    /**
     * Function used for creating the singleton in the Upgrades class.
     * @return the instance used for singleton
     */
    public static Upgrades getUpgrades() {
        if (instance == null) {
            instance = new Upgrades();
        }
        return instance;
    }

    /**
     * Function used for the on page -> buy tokens case.
     */
    public void onPageBuyTokens(final ActionInput action, final UserInput crtUser) {
        crtUser.getCredentials().setBalance(Integer.toString(Integer.parseInt(crtUser.
                getCredentials().getBalance()) - Integer.parseInt(action.getCount())));
        crtUser.setTokenCount(crtUser.getTokenCount() + Integer.parseInt(action.getCount()));
    }

    /**
     * Function used for the on page -> buy premium account case. The program will test if the
     * user already has a premium account, otherwise, the account will be upgraded to the
     * premium one.
     */
    public void onPageBuyPremium(final ArrayNode output, final UserInput crtUser) {
        if (Objects.equals(crtUser.getCredentials().getAccountType(), "premium")) {
            OutPrint.printError(output);
        } else {
            crtUser.setTokenCount(crtUser.getTokenCount() - Const.PREMIUM_UPGRADE);
            crtUser.getCredentials().setAccountType("premium");
            crtUser.setNumFreePremiumMovies(Const.FREE_PREMIUM_MOVIES);
        }
    }
}
