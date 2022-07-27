package btlib;

import android.annotation.SuppressLint;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.util.Log;

import java.io.IOException;
import java.util.UUID;

public final class ConnectToBluetoothServerTask extends ConnectionTask {

    private BluetoothSocket btSocket = null;

    @SuppressLint("MissingPermission")
    public ConnectToBluetoothServerTask(
            final BluetoothDevice serverBtDevice,
            final UUID uuid,
            final EventListener eventListener
    ){
        this.eventListener = eventListener;
        try {
            btSocket = serverBtDevice.createInsecureRfcommSocketToServiceRecord(uuid);
            Log.e("socket", "socket created");
        } catch (IOException e) {
            Log.e("socket", e.getMessage());
            try {
                btSocket.close( );
            } catch (IOException e1) {
                Log.e("socket", e1.getMessage());
            }
        }
    }

    @SuppressLint("MissingPermission")
    @Override
    protected Integer doInBackground(Void... unused) {
        Log.e("socket", "doInBackground()");
        try {
            Log.e("socket", "connecting socket");
            btSocket.connect();
            Log.e("socket", "socket connected");
        } catch (IOException e) {
            Log.e("socket", e.getMessage());
            try {
                btSocket.close( );
            } catch (IOException e1) {
                Log.e("socket", e1.getMessage());
            }

            return CONNECTION_CANCELED;
        }

        connectedChannel = new RealBluetoothChannel(btSocket);

        return CONNECTION_DONE;
    }

}
