/* Implement this class. */

import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;

public class MyDispatcher extends Dispatcher {
    //protected AtomicInteger lastIndex = new AtomicInteger(0);
    private int lastIndex = 0;

    public MyDispatcher(SchedulingAlgorithm algorithm, List<Host> hosts) {
        super(algorithm, hosts);
        
    }

    @Override
    public void addTask(Task task) {
        // case RR
        if (algorithm == SchedulingAlgorithm.ROUND_ROBIN) {
            int index = (lastIndex + 1) % hosts.size();
            hosts.get(index).addTask(task);
            lastIndex = index;
        } else if(algorithm == SchedulingAlgorithm.SHORTEST_QUEUE) {
            int shortestQ = hosts.get(0).getQueueSize(), node = 0;
            for (int i = 1; i < hosts.size(); i++) {
                if (hosts.get(i).getQueueSize() < shortestQ) {
                    node = i;
                    shortestQ = hosts.get(i).getQueueSize();
                }
            }
            hosts.get(node).addTask(task);

        } else if(algorithm == SchedulingAlgorithm.SIZE_INTERVAL_TASK_ASSIGNMENT) {
            switch (task.getType()) {
                case SHORT:
                    hosts.get(0).addTask(task);
                    break;
                case MEDIUM:
                    hosts.get(1).addTask(task);
                    break;
                case LONG:
                    hosts.get(2).addTask(task);
                    break;
                default:
                    break;
            }
        } else if(algorithm == SchedulingAlgorithm.LEAST_WORK_LEFT) {
            long leastWork = hosts.get(0).getWorkLeft();
            int node = 0;

            for (int i = 1; i < hosts.size(); i++) {
                if (hosts.get(i).getWorkLeft() < leastWork) {
                    node = i;
                    leastWork = hosts.get(i).getWorkLeft();
                }
            }
            hosts.get(node).addTask(task);
        }
    }
}
