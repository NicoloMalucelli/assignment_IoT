package iot.garden;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import android.Manifest;
import android.annotation.SuppressLint;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothServerSocket;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.ParcelUuid;
import android.util.Log;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.URL;
import java.net.URLConnection;
import java.nio.charset.StandardCharsets;
import java.util.Timer;
import java.util.TimerTask;

import java.util.UUID;

import btlib.BluetoothChannel;
import btlib.BluetoothServer;
import btlib.BluetoothUtils;
import btlib.ConnectToBluetoothServerTask;
import btlib.ConnectionTask;
import btlib.RealBluetoothChannel;
import btlib.exceptions.BluetoothDeviceNotFound;
import iot.garden.utils.C;

public class MainActivity extends AppCompatActivity {
    private BluetoothChannel btChannel;
    private String mode = "AUTO";

    @Override
    protected void onCreate(final Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        final BluetoothAdapter btAdapter = BluetoothAdapter.getDefaultAdapter();

        if (btAdapter != null && !btAdapter.isEnabled()) {
            if (ActivityCompat.checkSelfPermission(this, Manifest.permission.BLUETOOTH_CONNECT) != PackageManager.PERMISSION_GRANTED) {
                // TODO: Consider calling
                //    ActivityCompat#requestPermissions
                // here to request the missing permissions, and then overriding
                //   public void onRequestPermissionsResult(int requestCode, String[] permissions,
                //                                          int[] grantResults)
                // to handle the case where the user grants the permission. See the documentation
                // for ActivityCompat#requestPermissions for more details.
                return;
            }
            startActivityForResult(
                    new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE),
                    C.bluetooth.ENABLE_BT_REQUEST
            );
        }

        initUI();
    }

    private void initUI() {
        ((Button) findViewById(R.id.btnMode)).setOnClickListener(l -> {
            ((Button) findViewById(R.id.btnMode)).setEnabled(false);
            try {
                connectToBTServer();
            } catch (BluetoothDeviceNotFound bluetoothDeviceNotFound) {
                Toast.makeText(this, "Bluetooth device not found !", Toast.LENGTH_LONG)
                        .show();
                bluetoothDeviceNotFound.printStackTrace();
            } finally {
                //((Button) findViewById(R.id.switchMode)).setEnabled(true);
            }
        });

        //TODO gestire tutti i pulsanti

        findViewById(R.id.btnLed3Minus).setOnClickListener(l -> {
            int val = Integer.parseInt(((TextView) findViewById(R.id.textLed3)).getText().toString().trim());
            val -= 10;
            if (val < 0) {
                val = 0;
            }
            ((TextView) findViewById(R.id.textLed3)).setText(Integer.toString(val));
            String message = "l3:" + Integer.toString(val) + ";";
            btChannel.sendMessage(message);
        });

        findViewById(R.id.btnLed4Minus).setOnClickListener(l -> {
            int val = Integer.parseInt(((TextView) findViewById(R.id.textLed4)).getText().toString().trim());
            val -= 10;
            if (val < 0) {
                val = 0;
            }
            ((TextView) findViewById(R.id.textLed4)).setText(Integer.toString(val));
            String message = "l4:" + Integer.toString(val) + ";";
            btChannel.sendMessage(message);
        });

        findViewById(R.id.btnLed3Plus).setOnClickListener(l -> {
            int val = Integer.parseInt(((TextView) findViewById(R.id.textLed3)).getText().toString().trim());
            val += 10;
            if (val > 100) {
                val = 100;
            }
            ((TextView) findViewById(R.id.textLed3)).setText(Integer.toString(val));
            String message = "l3:" + Integer.toString(val) + ";";
            btChannel.sendMessage(message);
        });

        findViewById(R.id.btnLed4Plus).setOnClickListener(l -> {
            int val = Integer.parseInt(((TextView) findViewById(R.id.textLed4)).getText().toString().trim());
            val += 10;
            if (val > 100) {
                val = 100;
            }
            ((TextView) findViewById(R.id.textLed4)).setText(Integer.toString(val));
            String message = "l4:" + Integer.toString(val) + ";";
            btChannel.sendMessage(message);
        });

        ((Switch) findViewById(R.id.switchLed1)).setOnClickListener(l -> {
            if (((Switch) findViewById(R.id.switchLed1)).isChecked()) {
                btChannel.sendMessage("l1:on;");
            } else {
                btChannel.sendMessage("l1:off;");
            }
        });

        ((Switch) findViewById(R.id.switchLed2)).setOnClickListener(l -> {
            if (((Switch) findViewById(R.id.switchLed2)).isChecked()) {
                btChannel.sendMessage("l2:on;");
            } else {
                btChannel.sendMessage("l2:off;");
            }
        });

        findViewById(R.id.btnSpeedMinus).setOnClickListener(l -> {
            int val = Integer.parseInt(((TextView) findViewById(R.id.textSpeed)).getText().toString().trim());
            val -= 1;
            if (val < 0) {
                val = 0;
            }
            ((TextView) findViewById(R.id.textSpeed)).setText(Integer.toString(val));
            String message = "irr:" + Integer.toString(val) + ";";
            btChannel.sendMessage(message);
        });

        findViewById(R.id.btnSpeedPlus).setOnClickListener(l -> {
            int val = Integer.parseInt(((TextView) findViewById(R.id.textSpeed)).getText().toString().trim());
            val += 1;
            if (val > 5) {
                val = 5;
            }
            ((TextView) findViewById(R.id.textSpeed)).setText(Integer.toString(val));
            String message = "irr:" + Integer.toString(val) + ";";
            btChannel.sendMessage(message);
        });

        findViewById(R.id.btnAlarm).setOnClickListener(l -> {
            btChannel.sendMessage("mode:MANUAL;");
        });
    }

    @Override
    protected void onStop() {
        super.onStop();
        btChannel.sendMessage("mode:AUTO;");
        btChannel.close();
    }

    @Override
    protected void onActivityResult(final int requestCode, final int resultCode,
                                    @Nullable final Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == C.bluetooth.ENABLE_BT_REQUEST && resultCode == RESULT_OK) {
            Log.d(C.APP_LOG_TAG, "Bluetooth enabled!");
        }

        if (requestCode == C.bluetooth.ENABLE_BT_REQUEST && resultCode == RESULT_CANCELED) {
            Log.d(C.APP_LOG_TAG, "Bluetooth not enabled!");
        }
    }

    @SuppressLint({"MissingPermission", "NewApi"})
    private void connectToBTServer() throws BluetoothDeviceNotFound {
        final BluetoothDevice serverDevice = BluetoothUtils
                .getPairedDeviceByName(C.bluetooth.BT_DEVICE_ACTING_AS_SERVER_NAME);

        Log.e("serverDevice name:", serverDevice.getName());
        Log.e("serverDevice address:",serverDevice.getAddress());
        Log.e("serverDevice alias:",serverDevice.getAlias());
        Log.e("serverDevice btclass:",serverDevice.getBluetoothClass().toString());
        Log.e("serverDevice type:", Integer.toString(serverDevice.getType()));
        for(ParcelUuid u : serverDevice.getUuids()){
            Log.e("serverDevice uuid:", u.toString());
        }
        Log.e("serverDevice bond:", Integer.toString(serverDevice.getBondState()));

        // !!! Choose the right UUID value
        final UUID uuid = BluetoothUtils.getEmbeddedDeviceDefaultUuid();
//        final UUID uuid = BluetoothUtils.generateUuidFromString(C.bluetooth.BT_SERVER_UUID);

        if (serverDevice.getBondState()!=BluetoothDevice.BOND_BONDED){
            Log.e("serverDevice:", "not bonded");
        }

        ConnectionTask.EventListener eventListener = new ConnectionTask.EventListener() {
            @Override
            public void onConnectionActive(final BluetoothChannel channel) {

                Log.d("onConnectionActive", "onConnectionActive");

                Toast.makeText(MainActivity.this,
                        "Status : connected to server on device " + C.bluetooth.BT_DEVICE_ACTING_AS_SERVER_NAME,
                        Toast.LENGTH_LONG).show();

                btChannel = channel;
                Log.d("onConnectionActive", "onConnectionActive");
                btChannel.registerListener(new RealBluetoothChannel.Listener() {
                    @Override
                    public void onMessageReceived(String receivedMessage) {
                        Log.d("message complete", receivedMessage);
                        String newMode = "";
                        for(String s : receivedMessage.split(";")){
                            if(s.split(":").length != 2){
                                continue;
                            }
                            String k = s.split(":")[0];
                            String v = s.split(":")[1];

                            if(k.equals("mode")){
                                Log.d("message mode", "mode message received");
                                if(!v.equals(mode)) {
                                    newMode = v;
                                    if (v.equals("AUTO")) {
                                        Log.d("message mode", "system is now in AUTO mode");
                                        autoModeUI();
                                    } else if (v.equals("MANUAL")) {
                                        Log.d("message mode", "system is now in MANUAL mode");
                                        manualModeUI();
                                    } else if (v.equals("ALARM")) {
                                        Log.d("message mode", "system is now in ALARM mode");
                                        alarmModeUI();
                                    }
                                }
                            }
                            if(mode.equals("MANUAL")){
                                break;
                            }
                            if(k.equals("l1")){
                                if(v.equals("on")){
                                    ((Switch)findViewById(R.id.switchLed1)).setChecked(true);
                                } else {
                                    ((Switch)findViewById(R.id.switchLed1)).setChecked(false);
                                }
                            }else if(k.equals("l2")){
                                if(v.equals("on")){
                                    ((Switch)findViewById(R.id.switchLed2)).setChecked(true);
                                } else {
                                    ((Switch)findViewById(R.id.switchLed2)).setChecked(false);
                                }
                            }else if(k.equals("l3")){
                                ((TextView)findViewById(R.id.textLed3)).setText(v);
                            }else if(k.equals("l4")){
                                ((TextView)findViewById(R.id.textLed4)).setText(v);
                            }else if(k.equals("irrigator")){
                                ((TextView)findViewById(R.id.textSpeed)).setText(v);
                            }
                        }
                        mode = newMode;
                    }

                    @Override
                    public void onMessageSent(String sentMessage) {
                        Log.d("message sent", sentMessage);
                    }
                });

                Log.e("send message", "mode:MANUAL;");
                btChannel.sendMessage("mode:MANUAL;");
            }

            @Override
            public void onConnectionCanceled() {
                Toast.makeText(MainActivity.this,
                        "Status : unable to connect, device not found! " + C.bluetooth.BT_DEVICE_ACTING_AS_SERVER_NAME,
                        Toast.LENGTH_LONG).show();
            }
        };


        Log.e("eventlistener", "eventlistener in main: " + eventListener);
        new ConnectToBluetoothServerTask(serverDevice, uuid, eventListener).execute();
    }

    public void alarmModeUI(){
        ((Button)findViewById(R.id.btnLed3Minus)).setEnabled(false);
        ((Button)findViewById(R.id.btnLed3Plus)).setEnabled(false);
        ((Button)findViewById(R.id.btnLed4Minus)).setEnabled(false);
        ((Button)findViewById(R.id.btnLed4Plus)).setEnabled(false);
        ((Button)findViewById(R.id.btnSpeedMinus)).setEnabled(false);
        ((Button)findViewById(R.id.btnSpeedPlus)).setEnabled(false);
        ((Switch)findViewById(R.id.switchLed1)).setEnabled(false);
        ((Switch)findViewById(R.id.switchLed2)).setEnabled(false);

        ((Button)findViewById(R.id.btnAlarm)).setEnabled(true);
    }

    public void manualModeUI(){
        ((Button)findViewById(R.id.btnLed3Minus)).setEnabled(true);
        ((Button)findViewById(R.id.btnLed3Plus)).setEnabled(true);
        ((Button)findViewById(R.id.btnLed4Minus)).setEnabled(true);
        ((Button)findViewById(R.id.btnLed4Plus)).setEnabled(true);
        ((Button)findViewById(R.id.btnSpeedMinus)).setEnabled(true);
        ((Button)findViewById(R.id.btnSpeedPlus)).setEnabled(true);
        ((Switch)findViewById(R.id.switchLed1)).setEnabled(true);
        ((Switch)findViewById(R.id.switchLed2)).setEnabled(true);

        ((Button)findViewById(R.id.btnAlarm)).setEnabled(false);
    }

    public void autoModeUI(){
        ((Button)findViewById(R.id.btnLed3Minus)).setEnabled(false);
        ((Button)findViewById(R.id.btnLed3Plus)).setEnabled(false);
        ((Button)findViewById(R.id.btnLed4Minus)).setEnabled(false);
        ((Button)findViewById(R.id.btnLed4Plus)).setEnabled(false);
        ((Button)findViewById(R.id.btnSpeedMinus)).setEnabled(false);
        ((Button)findViewById(R.id.btnSpeedPlus)).setEnabled(false);
        ((Switch)findViewById(R.id.switchLed1)).setEnabled(false);
        ((Switch)findViewById(R.id.switchLed2)).setEnabled(false);

        ((Button)findViewById(R.id.btnAlarm)).setEnabled(false);
    }
}