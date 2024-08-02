package pt.isec.pa.exerc29.utils;

import java.util.ArrayList;
import java.util.List;

// singleton
public class AppLog {
    private static AppLog _instance=null;

    public static AppLog getInstance() {
        if (_instance == null)
            _instance = new AppLog();
        return _instance;
    }

    protected ArrayList<String> log;

    private AppLog() {
        log = new ArrayList<>();
    }

    public void reset() {
        log.clear();
    }

    public void add(String msg) {
        log.add(msg);
    }

    public List<String> getLog() {
        return List.copyOf(log);
    }
}
