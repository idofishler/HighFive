package org.brainihack.hi5;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Set;
import java.util.UUID;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothServerSocket;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.content.Intent;
import android.media.MediaPlayer;
import android.media.Ringtone;
import android.media.RingtoneManager;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.os.Vibrator;
import android.util.Log;
import android.view.Menu;
import android.view.View;

public class MainActivity extends Activity {

	private static final int REQUEST_ENABLE_BT = 1;
	private static final String NAME = "hi5beep";

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		beep();
		try {
			connectBT();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
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

	public void beep(View view) {
		Uri notification = RingtoneManager
				.getDefaultUri(RingtoneManager.TYPE_NOTIFICATION);
		Ringtone r = RingtoneManager.getRingtone(getApplicationContext(),
				notification);
		r.play();
		// Get instance of Vibrator from current Context
		Vibrator v = (Vibrator) getSystemService(Context.VIBRATOR_SERVICE);
		// Vibrate for 300 milliseconds
		v.vibrate(300);
	}

	public void beep() {
//		Uri notification = RingtoneManager
//				.getDefaultUri(RingtoneManager.TYPE_NOTIFICATION);
//		Ringtone r = RingtoneManager.getRingtone(getApplicationContext(),
//				notification);
//		r.play();
		
//		File dir = Environment.getExternalStorageDirectory();
//		File yourFile = new File(dir, "Music/hiyah.mp3");
//		Uri hiyhaUri = Uri.fromFile(yourFile); 
		MediaPlayer mPlayer = MediaPlayer.create(MainActivity.this, R.raw.hiyha);
		mPlayer.start();
		// Get instance of Vibrator from current Context
		Vibrator v = (Vibrator) getSystemService(Context.VIBRATOR_SERVICE);
		// Vibrate for 300 milliseconds
		v.vibrate(300);
	}

	public void connectBT() throws IOException {
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
		// AcceptThread acceptThread = new AcceptThread(mBluetoothAdapter);
		// acceptThread.run();
		UUID uuid = UUID.fromString("446118f0-8b1e-11e2-9e96-0800200c9a66");
		Log.d("BT", "Waiting for connection");
		Log.d("BT", "NAME-UUID: " + NAME + "-" + uuid);
		BluetoothServerSocket bluetoothServerSocket = mBluetoothAdapter
				.listenUsingRfcommWithServiceRecord(NAME, uuid);
		while (true) {
			while (true) {
				try {
					BluetoothSocket socket = bluetoothServerSocket.accept();
					InputStream inputStream = socket.getInputStream();
					while (true) {
						try {
							// Read from the InputStream
							Log.d("BT", "Reading from the InputStream");
							byte[] buffer = new byte[1024]; // buffer store for
															// the
							// stream
							int bytes; // bytes returned from read()
							bytes = inputStream.read(buffer);
							// TODO: read the bytes and beep if needed
							Log.d("BT", "inputStream: " + bytes);
							if (bytes > 0) {
								beep();
							}
						} catch (IOException e) {
							break;
						}
					}
				} catch (IOException e) {
					break;
				}
			}
		}
	}

	// // Connect to device
	// Set<BluetoothDevice> pairedDevices = mBluetoothAdapter
	// .getBondedDevices();
	// // If there are paired devices
	// if (pairedDevices.size() > 0) {
	// // Loop through paired devices
	// for (BluetoothDevice device : pairedDevices) {
	// Log.d("BT", device.getName() + " at: " + device.getAddress());
	// }
	// }
	// // open socket

	// BluetoothDevice btDevice = bta.getRemoteDevice(macAddress);
	// BluetoothSocket btSocket =
	// InsecureBluetooth.createRfcommSocketToServiceRecord(
	// btDevice, UUID.fromString("00001101-0000-1000-8000-00805F9B34FB"),
	// false);
	//
	// btSocket.connect();
	// InputStream input = btSocket.getInputStream();
	// DataInputStream dinput = new DataInputStream(input);
	// }

	private class AcceptThread extends Thread {

		private final BluetoothServerSocket mmServerSocket;
		private BluetoothAdapter mBluetoothAdapter;

		@SuppressLint("NewApi")
		public AcceptThread(BluetoothAdapter bluetoothAdapter) {
			// Use a temporary object that is later assigned to mmServerSocket,
			// because mmServerSocket is final
			mBluetoothAdapter = bluetoothAdapter;
			BluetoothServerSocket bluetoothServerSocket = null;
			try {
				// MY_UUID is the app's UUID string, also used by the client
				// code
				UUID uuid = UUID
						.fromString("446118f0-8b1e-11e2-9e96-0800200c9a66");
				Log.d("BT", "Runnig Accept thread");
				System.out.println("Runnig Accept thread");
				Log.d("BT", "NAME-UUID: " + NAME + "-" + uuid);
				bluetoothServerSocket = mBluetoothAdapter
						.listenUsingRfcommWithServiceRecord(NAME, uuid);
				Log.d("BT",
						"bt server socket: " + bluetoothServerSocket.toString());
			} catch (IOException e) {
				Log.e("BT", "IOException", e);
			}
			mmServerSocket = bluetoothServerSocket;
		}

		public void run() {
			BluetoothSocket socket = null;
			// Keep listening until exception occurs or a socket is returned
			while (true) {
				try {
					socket = mmServerSocket.accept();
				} catch (IOException e) {
					break;
				}
				// If a connection was accepted
				if (socket != null) {
					// Do work to manage the connection (in a separate thread)
					ConnectedThread connectedThread = new ConnectedThread(
							socket);
					connectedThread.run();
					try {
						mmServerSocket.close();
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					break;
				}
			}
		}

		/** Will cancel the listening socket, and cause the thread to finish */
		public void cancel() {
			try {
				mmServerSocket.close();
			} catch (IOException e) {
			}
		}
	}

	private class ConnectedThread extends Thread {
		private final BluetoothSocket mmSocket;
		private final InputStream mmInStream;
		private final OutputStream mmOutStream;

		public ConnectedThread(BluetoothSocket socket) {
			mmSocket = socket;
			InputStream tmpIn = null;
			OutputStream tmpOut = null;

			// Get the input and output streams, using temp objects because
			// member streams are final
			try {
				tmpIn = socket.getInputStream();
				tmpOut = socket.getOutputStream();
			} catch (IOException e) {
			}

			mmInStream = tmpIn;
			mmOutStream = tmpOut;
		}

		public void run() {
			Log.d("BT", "Running connected thread");
			byte[] buffer = new byte[1024]; // buffer store for the stream
			int bytes; // bytes returned from read()

			// Keep listening to the InputStream until an exception occurs
			while (true) {
				try {
					// Read from the InputStream
					bytes = mmInStream.read(buffer);
					// TODO: read the bytes and beep if needed
					Log.d("BT", "inputStream: " + bytes);
					if (bytes > 0) {
						beep();
					}
				} catch (IOException e) {
					break;
				}
			}
		}

		/* Call this from the main activity to send data to the remote device */
		public void write(byte[] bytes) {
			try {
				mmOutStream.write(bytes);
			} catch (IOException e) {
			}
		}

		/* Call this from the main activity to shutdown the connection */
		public void cancel() {
			try {
				mmSocket.close();
			} catch (IOException e) {
			}
		}
	}
}
