/* Implement this class. */
import java.util.ArrayList;
import java.util.List;

public class MyHost extends Host {

    private volatile List<Task> tasksList = new ArrayList<Task>();

    private volatile Task taskCrt = null;
    private volatile boolean running = true;
    private long totalTime = 0;
    
    @Override
    public void run() {
        while(running) {
            try {
                if (taskCrt == null || taskCrt.isPreemptible())
                    getNextTask();

                sleep(500);
                taskCrt.setLeft(taskCrt.getLeft() - 500);
                totalTime -= 500;

                if (taskCrt.getLeft() <= 0) {
                    taskCrt.finish();
                    tasksList.remove(taskCrt);
                    taskCrt = null;
                }

            } catch (InterruptedException | IndexOutOfBoundsException ignored) {

            }
        }
    }

    public void getNextTask() {
        taskCrt = tasksList.get(0);

        tasksList.forEach(element -> {
            if (element.getPriority() > taskCrt.getPriority()) {
                taskCrt = element;
            }
        });
    }

    @Override
    public void addTask(Task task) {
        tasksList.add(task);
        totalTime += task.getDuration();
    }

    @Override
    public int getQueueSize() {
        return tasksList.size();
    }

    @Override
    public long getWorkLeft() {
        return totalTime;
    }

    @Override
    public void shutdown() {
        running = false;
    }
}
