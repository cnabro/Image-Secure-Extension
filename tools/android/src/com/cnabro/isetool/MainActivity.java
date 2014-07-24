package com.cnabro.isetool;

import android.app.Activity;
import android.os.Bundle;
import android.os.Environment;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.Toast;

public class MainActivity extends Activity {

	/**
	 * test
	 */
	
	static {
		System.loadLibrary("ise");
        System.loadLibrary("ise-jni");
	}

	public native String getSecureJpegBuffer(String path, String key);
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		Button btn = (Button) findViewById(R.id.test);
		btn.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				
				Toast.makeText(MainActivity.this, getSecureJpegBuffer(""+Environment.getExternalStorageDirectory()+"/test.jpgx","1234"), Toast.LENGTH_SHORT).show();
			}
		});
		//
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

}
