package org.brainihack.hi5;

import java.io.IOException;
import java.io.InputStream;
import java.util.UUID;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothServerSocket;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.content.Intent;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.os.Vibrator;
import android.util.Log;
import android.view.Menu;

public class BTBeep extends Activity {

	private static final String PRE_SET_UUID = "446118f0-8b1e-11e2-9e96-0800200c9a66"; // client must use this UUID to connect.
	private static final int REQUEST_ENABLE_BT = 1;
	private static final String NAME = "hi5beep";

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		// make one noise for warm up
		makeSomeNoise();
		doBluetoothMagic();
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);
		if (requestCode == REQUEST_ENABLE_BT) {
			if (requestCode == RESULT_OK) {
				Log.d("BT", "BT enabled!");
			} else {
				Log.e("BT", "BT disabled!");
			}
		}
	}

	public void makeSomeNoise() {
		// Hi-Yha! sound
		MediaPlayer mPlayer = MediaPlayer.create(BTBeep.this, R.raw.hiyha);
		mPlayer.start();
		// Get instance of Vibrator from current Context
		Vibrator v = (Vibrator) getSystemService(Context.VIBRATOR_SERVICE);
		// Vibrate for 300 milliseconds
		v.vibrate(300);
	}

	public void doBluetoothMagic() {
		BluetoothAdapter mBluetoothAdapter = BluetoothAdapter
				.getDefaultAdapter();
		if (mBluetoothAdapter == null) {
			Log.e("BT", "Device does not support BT");
		}
		// enable BT
		if (!mBluetoothAdapter.isEnabled()) {
			Intent enableBtIntent = new Intent(
					BluetoothAdapter.ACTION_REQUEST_ENABLE);
			startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
		}
		
		// at this point BT should be enabled and device is already paired and connected.
		UUID uuid = UUID.fromString(PRE_SET_UUID);
		Log.d("BT", "Waiting for connection");
		Log.d("BT", "NAME-UUID: " + NAME + "-" + uuid);
		BluetoothServerSocket bluetoothServerSocket = null;
		try {
			bluetoothServerSocket = mBluetoothAdapter
					.listenUsingRfcommWithServiceRecord(NAME, uuid);
		} catch (IOException e) {
			Log.w("BT", "IO when tring to open BT socket", e);
		}
		// run forever
		while (true) {
			// try to open BT connection
			while (true) {
				try {
					// will return when socket is accessed by client.
					BluetoothSocket socket = bluetoothServerSocket.accept();
					InputStream inputStream = socket.getInputStream();
					// read form input stream 
					while (true) {
						try {
							// Read from the InputStream
							Log.d("BT", "Reading from the InputStream");
							byte[] buffer = new byte[1024]; // buffer store for
															// the
							// stream
							int bytes; // bytes returned from read()
							bytes = inputStream.read(buffer);
							// read the bytes and beep if needed
							Log.d("BT", "inputStream: " + bytes);
							if (bytes > 0) {
								makeSomeNoise();
							}
						} catch (IOException e) {
							Log.w("BT", "IO when reading stream", e);
						}
					}
				} catch (IOException e) {
					Log.w("BT", "IO while listening on BT socket", e);
				}
			}
		}
	}
}
