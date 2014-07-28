package com.pigtools.isetool.activity;

import android.app.Activity;
import android.app.Service;
import android.content.ComponentName;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.res.Configuration;
import android.os.Bundle;
import android.os.Environment;
import android.os.IBinder;
import android.os.RemoteException;
import android.support.v4.app.ActionBarDrawerToggle;
import android.support.v4.view.GravityCompat;
import android.support.v4.widget.DrawerLayout;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.Window;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ListView;

import com.pigtools.isetool.R;
import com.pigtools.isetool.container.JpgxDecompressContainer;
import com.pigtools.isetool.service.IseProcessingInterface;
import com.pigtools.isetool.util.ImageUtil;

public class MainActivity extends Activity implements OnClickListener {

	private IseProcessingInterface mProcessingService;
	private Button mOpenButton;
	private ImageView mPreviewImage;
	private DrawerLayout mDrawerLayout;
	private LinearLayout mSlidingDrawer;
	private ActionBarDrawerToggle mDrawerToggle;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		mOpenButton = (Button) findViewById(R.id.open_btn);
		mOpenButton.setOnClickListener(this);

		mPreviewImage = (ImageView) findViewById(R.id.preview_image);

		mDrawerLayout = (DrawerLayout) findViewById(R.id.drawer_layout);
		mSlidingDrawer = (LinearLayout) findViewById(R.id.left_drawer);
		mDrawerLayout.setDrawerShadow(R.drawable.drawer_shadow, GravityCompat.START);

		getActionBar().setDisplayHomeAsUpEnabled(true);
		getActionBar().setHomeButtonEnabled(true);

		mDrawerToggle = new ActionBarDrawerToggle(this, mDrawerLayout, R.drawable.ic_drawer, R.string.drawer_open, R.string.drawer_close) {
			public void onDrawerClosed(View view) {
				getActionBar().setTitle(getResources().getString(R.string.app_name));
				invalidateOptionsMenu(); // creates call to onPrepareOptionsMenu()
			}

			public void onDrawerOpened(View drawerView) {
				getActionBar().setTitle(getResources().getString(R.string.action_settings));
				invalidateOptionsMenu(); // creates call to onPrepareOptionsMenu()
			}
		};

		mDrawerLayout.setDrawerListener(mDrawerToggle);

		bindService(new Intent("com.pigtools.isetool.service"), mServiceConnection, Service.BIND_AUTO_CREATE);

	}

	private ServiceConnection mServiceConnection = new ServiceConnection() {

		@Override
		public void onServiceDisconnected(ComponentName name) {
			mProcessingService = null;
		}

		@Override
		public void onServiceConnected(ComponentName name, IBinder service) {
			mProcessingService = IseProcessingInterface.Stub.asInterface(service);
		}
	};

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// The action bar home/up action should open or close the drawer.
		// ActionBarDrawerToggle will take care of this.
		if (mDrawerToggle.onOptionsItemSelected(item)) {
			return true;
		}

		return super.onOptionsItemSelected(item);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		getMenuInflater().inflate(R.menu.main, menu);
		return super.onCreateOptionsMenu(menu);
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();

		unbindService(mServiceConnection);
	}

	@Override
	public void onClick(View v) {
		switch (v.getId()) {
		case R.id.open_btn:
			try {
				JpgxDecompressContainer container = mProcessingService.getSecureJpegBuffer(Environment.getExternalStorageDirectory() + "/test2.jpgx",
						"1234");
				mPreviewImage.setImageBitmap(ImageUtil.byteArrayToBitmap(container.getImage(), container.getWidth(), container.getHeight()));
			} catch (RemoteException e) {
				e.printStackTrace();
			}

			break;

		default:
			break;
		}
	}

	@Override
	protected void onPostCreate(Bundle savedInstanceState) {
		super.onPostCreate(savedInstanceState);
		// Sync the toggle state after onRestoreInstanceState has occurred.
		mDrawerToggle.syncState();
	}

	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		super.onConfigurationChanged(newConfig);
		// Pass any configuration change to the drawer toggls
		mDrawerToggle.onConfigurationChanged(newConfig);
	}
}
