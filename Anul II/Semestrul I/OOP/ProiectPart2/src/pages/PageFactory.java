package pages;

public final class PageFactory extends Page {
    private PageFactory() {
    }

    /**
     * Method used for the factory coding style
     * @return the new page with the said name
     */
    public static Page createPage(final String pageName) {
        switch (pageName) {
            case "login" -> {
                return new Login();
            }
            case "register" -> {
                return new Register();
            }
            case "logout" -> {
                return new NotAuthenticated();
            }
            case "movies" -> {
                return new Movies();
            }
            case "see details" -> {
                return new SeeDetails();
            }
            case "upgrades" -> {
                return new Upgrades();
            }
            default -> {
                return null;
            }
        }
    }
}
