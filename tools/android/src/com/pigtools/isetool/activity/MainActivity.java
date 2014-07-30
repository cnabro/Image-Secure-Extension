package com.pigtools.isetool.activity;

import android.app.Activity;
import android.app.Service;
import android.content.ComponentName;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.pm.ActivityInfo;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Point;
import android.os.Bundle;
import android.os.IBinder;
import android.os.RemoteException;
import android.support.v4.app.ActionBarDrawerToggle;
import android.support.v4.view.GravityCompat;
import android.support.v4.widget.DrawerLayout;
import android.view.Display;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;

import com.pigtools.isetool.R;
import com.pigtools.isetool.service.IseProcessingInterface;
import com.pigtools.isetool.service.container.JpgxDecompressContainer;
import com.pigtools.isetool.util.ImageUtil;
import com.pigtools.isetool.view.SelectionCanvas;

public class MainActivity extends Activity implements OnClickListener {

	private IseProcessingInterface mProcessingService;
	private ImageView mPreviewImage;
	private DrawerLayout mDrawerLayout;
	private LinearLayout mSlidingDrawer;
	private ActionBarDrawerToggle mDrawerToggle;
	private SelectionCanvas mSelectionCanvas;

	private int mCurrentImgWidth = 0;
	private int mCurrentImgHeight = 0;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		setContentView(R.layout.activity_main);

		mPreviewImage = (ImageView) findViewById(R.id.preview_image);
		mSelectionCanvas = (SelectionCanvas) findViewById(R.id.selection_canvas);

		mDrawerLayout = (DrawerLayout) findViewById(R.id.drawer_layout);
		mSlidingDrawer = (LinearLayout) findViewById(R.id.left_drawer);
		mDrawerLayout.setDrawerShadow(R.drawable.drawer_shadow, GravityCompat.START);

		getActionBar().setDisplayHomeAsUpEnabled(true);
		getActionBar().setHomeButtonEnabled(true);

		mDrawerToggle = new ActionBarDrawerToggle(this, mDrawerLayout, R.drawable.ic_drawer, R.string.drawer_open, R.string.drawer_close) {
			public void onDrawerClosed(View view) {
				getActionBar().setTitle(getResources().getString(R.string.app_name));
				invalidateOptionsMenu();
			}

			public void onDrawerOpened(View drawerView) {
				getActionBar().setTitle(getResources().getString(R.string.action_settings));
				invalidateOptionsMenu();
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
		if (mDrawerToggle.onOptionsItemSelected(item)) {
			return true;
		}

		switch (item.getItemId()) {
		case R.id.action_open:
			startActivityForResult(new Intent(this, FileListActivity.class), FileListActivity.REQUEST_CODE_OPEN_FILE);
			break;

		case R.id.action_save:
			startActivityForResult(new Intent(this, FileListActivity.class), FileListActivity.REQUEST_CODE_OPEN_FILE);
			break;

		default:
			break;
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

			break;

		default:
			break;
		}
	}

	@Override
	protected void onPostCreate(Bundle savedInstanceState) {
		super.onPostCreate(savedInstanceState);
		mDrawerToggle.syncState();
	}

	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		super.onConfigurationChanged(newConfig);
		mDrawerToggle.onConfigurationChanged(newConfig);
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);

		if (data != null) {
			mSelectionCanvas.init();
			mSelectionCanvas.setEnabled(true);

			String path = data.getStringExtra(FileListActivity.EXTRA_KEY_RESULT);

			switch (resultCode) {
			case FileListActivity.RESULT_CODE_OPEN_JPEG:
				Bitmap bitmap = BitmapFactory.decodeFile(path);
				mCurrentImgWidth = bitmap.getWidth();
				mCurrentImgHeight = bitmap.getHeight();

				loadImage(bitmap);

				break;

			case FileListActivity.RESULT_CODE_OPEN_JPGX:

				try {
					JpgxDecompressContainer jpgxcontainer = mProcessingService.getSecureJpegBuffer(path, "");

					mCurrentImgWidth = jpgxcontainer.getWidth();
					mCurrentImgHeight = jpgxcontainer.getHeight();

					loadImage(ImageUtil.byteArrayToBitmap(jpgxcontainer.getImage(), jpgxcontainer.getWidth(), jpgxcontainer.getHeight()));
					mSelectionCanvas.setEnabled(false);

				} catch (RemoteException e) {
					e.printStackTrace();
				}

				break;

			case FileListActivity.RESULT_CODE_OPEN_PNG:

				break;

			case FileListActivity.RESULT_CODE_OPEN_PNGX:
				mSelectionCanvas.setEnabled(false);
				break;

			default:
				break;
			}
		}
	}

	public void loadImage(Bitmap bitmap) {

		mPreviewImage.setImageBitmap(bitmap);

		/*
		 * 화면에 뿌릴때 비율 계산해야 함
		 */

		Display display = getWindowManager().getDefaultDisplay();
		Point size = new Point();
		display.getSize(size);

		int screenWidth = size.x;
		int screenHeight = size.y;
		float ratio = 1.0f;

		if (mCurrentImgWidth > mCurrentImgHeight) // 가로가 더 길면 가로에 맞춤
		{
			ratio = (float) screenWidth / (float) mCurrentImgWidth;

			RelativeLayout.LayoutParams layoutParams = new RelativeLayout.LayoutParams(screenWidth, (int) (mCurrentImgHeight * ratio));
			layoutParams.addRule(RelativeLayout.CENTER_IN_PARENT, RelativeLayout.TRUE);

			mPreviewImage.setLayoutParams(layoutParams);

			mSelectionCanvas.setImageSize(mCurrentImgWidth, mCurrentImgHeight, ratio);
			mSelectionCanvas.setLayoutParams(layoutParams);
		} else {
			ratio = (float) screenHeight / (float) mCurrentImgHeight;

			RelativeLayout.LayoutParams layoutParams = new RelativeLayout.LayoutParams((int) (screenWidth * ratio), mCurrentImgHeight);
			layoutParams.addRule(RelativeLayout.CENTER_IN_PARENT, RelativeLayout.TRUE);

			mPreviewImage.setLayoutParams(layoutParams);

			mSelectionCanvas.setImageSize(mCurrentImgWidth, mCurrentImgHeight, ratio);
			mSelectionCanvas.setLayoutParams(layoutParams);
		}

	}

}
