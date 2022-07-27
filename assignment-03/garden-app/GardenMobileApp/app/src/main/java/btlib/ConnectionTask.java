package btlib;

import android.os.AsyncTask;
import android.util.Log;

public abstract class ConnectionTask extends AsyncTask<Void, Void, Integer> {
    static final int CONNECTION_DONE = 1;
    static final int CONNECTION_CANCELED = 2;

    BluetoothChannel connectedChannel;
    EventListener eventListener;

    @Override
    protected void onPostExecute(Integer result) {
        Log.e("socket", "onPostExecute() " + (result == CONNECTION_DONE));
        switch (result){
            case CONNECTION_DONE:
                Log.e("socket", "connection done, eventListener: " + eventListener);
                if(eventListener != null){
                    eventListener.onConnectionActive(connectedChannel);
                }
                break;

            case CONNECTION_CANCELED:
                if(eventListener != null){
                    eventListener.onConnectionCanceled();
                }
                break;
        }
    }

    public interface EventListener {
        void onConnectionActive(final BluetoothChannel channel);
        void onConnectionCanceled();
    }
}
