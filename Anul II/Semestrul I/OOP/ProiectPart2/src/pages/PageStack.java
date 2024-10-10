package pages;

import java.util.ArrayList;

public final class PageStack extends Page {

    private PageStack() {

    }

    /**
     * Method used for the stack of pages that will be used for the back action.
     * Method will mimic the push action of the stack, adding a new element
     */
    public static void push(final ArrayList<String> stack, final String elem) {
        stack.add(0, elem);
    }

    /**
     * Method used for the stack of pages that will be used for the back action
     * Method will mimic the pop action of the stack, returning the element from the top of the
     * stack
     */
    public static String pop(final ArrayList<String> stack) {
        String first = null;
        if (!stack.isEmpty()) {
            first = stack.get(0);
            stack.remove(0);
        }
        return first;
    }
}
