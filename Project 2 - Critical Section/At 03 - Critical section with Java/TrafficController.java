public class TrafficController {

    private ReentrantLock mutex = new ReentrantLock();

    public void enterLeft() {
          mutex.lock();
    }
    public void enterRight() {
          mutex.lock();
    }
    public void leaveLeft() {
          mutex.unlock ();
    }
    public void leaveRight() {
          mutex.unlock();
    }

}
