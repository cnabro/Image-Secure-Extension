package com.pigtools.isetool.activity;

import java.util.ArrayList;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.app.Service;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Point;
import android.media.FaceDetector.Face;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.IBinder;
import android.os.RemoteException;
import android.support.v4.app.ActionBarDrawerToggle;
import android.support.v4.view.GravityCompat;
import android.support.v4.widget.DrawerLayout;
import android.text.InputType;
import android.text.method.PasswordTransformationMethod;
import android.view.Display;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.RelativeLayout;
import android.widget.Toast;

import com.pigtools.isetool.R;
import com.pigtools.isetool.activity.adapter.DrawerSelectionAdapter;
import com.pigtools.isetool.service.IseProcessingInterface;
import com.pigtools.isetool.service.container.JpgxDecompressContainer;
import com.pigtools.isetool.service.container.PngxDecompressContainer;
import com.pigtools.isetool.service.container.SecureContainer;
import com.pigtools.isetool.util.ImageUtil;
import com.pigtools.isetool.view.BottomToggleLayout;
import com.pigtools.isetool.view.SelectionCanvas;
import com.pigtools.isetool.view.listener.OnBottomToggleSelectedListener;
import com.pigtools.isetool.view.listener.OnFaceDetectionFinishedListener;

public class MainActivity extends Activity implements OnClickListener, OnBottomToggleSelectedListener {

	private IseProcessingInterface mProcessingService;
	private ImageView mPreviewImage;
	private DrawerLayout mDrawerLayout;
	private LinearLayout mSlidingDrawer;
	private BottomToggleLayout mBottomToggleLayout;
	private ListView mDrawerSecureListView;

	private ActionBarDrawerToggle mDrawerToggle;
	private SelectionCanvas mSelectionCanvas;

	private int mCurrentImgWidth = 0;
	private int mCurrentImgHeight = 0;

	private String mCurrentPath = "/";
	private Bitmap mCurrentBitmap = null;

	private int mCurrentOpenType = 0;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		setContentView(R.layout.activity_main);

		mPreviewImage = (ImageView) findViewById(R.id.preview_image);
		mSelectionCanvas = (SelectionCanvas) findViewById(R.id.selection_canvas);

		mDrawerLayout = (DrawerLayout) findViewById(R.id.drawer_layout);
		mDrawerLayout.setDrawerShadow(R.drawable.drawer_shadow, GravityCompat.START);
		mSlidingDrawer = (LinearLayout) findViewById(R.id.left_drawer);
		mBottomToggleLayout = (BottomToggleLayout) findViewById(R.id.bottom_layout);
		mBottomToggleLayout.setOnItemSelectedListener(this);
		mDrawerSecureListView = (ListView) findViewById(R.id.secure_listview);

		mDrawerToggle = new ActionBarDrawerToggle(this, mDrawerLayout, R.drawable.ic_drawer, R.string.drawer_open, R.string.drawer_close) {
			public void onDrawerClosed(View view) {
				// getActionBar().setTitle(getResources().getString(R.string.app_name));
				invalidateOptionsMenu();
			}

			public void onDrawerOpened(View drawerView) {
				// getActionBar().setTitle(getResources().getString(R.string.action_option));
				invalidateOptionsMenu();
			}
		};

		mDrawerLayout.setDrawerListener(mDrawerToggle);
		setActionBarOptionEnabled(true);
		getActionBar().setTitle("");

		bindService(new Intent("com.pigtools.isetool.service"), mServiceConnection, Service.BIND_AUTO_CREATE);

	}

	public void setActionBarOptionEnabled(boolean value) {
		getActionBar().setDisplayHomeAsUpEnabled(value);
		getActionBar().setHomeButtonEnabled(value);
		mBottomToggleLayout.setVisibility(value ? View.VISIBLE : View.GONE);
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
			DrawerSelectionAdapter adapter = new DrawerSelectionAdapter(this, mSelectionCanvas.getAdjustRectList());
			mDrawerSecureListView.setAdapter(adapter);

			return true;
		}

		switch (item.getItemId()) {
		case R.id.action_open:
			startActivityForResult(new Intent(this, FileListActivity.class), FileListActivity.REQUEST_CODE_OPEN_FILE);
			break;

		case R.id.action_save:
			try {
				ArrayList<SecureContainer> list = new ArrayList<SecureContainer>();
				list.add(new SecureContainer(50, 50, 50, 50));

				boolean result = mProcessingService.makeJPGX(mCurrentPath, list, "1234");

				Toast.makeText(this, "result : " + result, Toast.LENGTH_SHORT).show();
			} catch (RemoteException e) {
				e.printStackTrace();
			}
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
	public boolean onPrepareOptionsMenu(Menu menu) {
		boolean drawerOpen = mDrawerLayout.isDrawerOpen(mSlidingDrawer);

		menu.findItem(R.id.action_open).setVisible(!drawerOpen);
		menu.findItem(R.id.action_save).setVisible(drawerOpen);

		return super.onPrepareOptionsMenu(menu);
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
	public void onToggleButtonSelected(int id) {
		if (mCurrentBitmap != null) {
			switch (id) {
			case R.id.selection_rect_btn:

				break;

			case R.id.selection_people_btn:
				ImageUtil.findFaces(MainActivity.this, mCurrentBitmap, new OnFaceDetectionFinishedListener() {

					@Override
					public void onDetectionFinished(Face[] faces) {
						mSelectionCanvas.setFaceArraySelection(faces);
					}
				});

				break;

			case R.id.selection_text_btn:

				break;

			case R.id.selection_cancel_btn:
				mSelectionCanvas.init();
				break;

			default:
				break;
			}
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
			mCurrentOpenType = resultCode;

			mSelectionCanvas.init();
			mSelectionCanvas.setEnabled(true);

			mCurrentPath = data.getStringExtra(FileListActivity.EXTRA_KEY_RESULT);
			LoadImageAsyncTask task = new LoadImageAsyncTask(MainActivity.this, resultCode);
			task.execute();
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

			RelativeLayout.LayoutParams layoutParams = new RelativeLayout.LayoutParams((int) (mCurrentImgWidth * ratio), screenHeight);
			layoutParams.addRule(RelativeLayout.CENTER_IN_PARENT, RelativeLayout.TRUE);

			mPreviewImage.setLayoutParams(layoutParams);

			mSelectionCanvas.setImageSize(mCurrentImgWidth, mCurrentImgHeight, ratio);
			mSelectionCanvas.setLayoutParams(layoutParams);
		}

	}

	public void showPasswordDialog() {
		AlertDialog.Builder alert = new AlertDialog.Builder(this);

		alert.setTitle("Input Password");

		final EditText input = new EditText(this);
		input.setInputType(InputType.TYPE_CLASS_TEXT | InputType.TYPE_TEXT_VARIATION_PASSWORD);
		input.setTransformationMethod(PasswordTransformationMethod.getInstance());

		alert.setView(input);

		alert.setPositiveButton("Ok", new DialogInterface.OnClickListener() {
			public void onClick(DialogInterface dialog, int whichButton) {
				String pwd = input.getText().toString();

				LoadImageAsyncTask task = new LoadImageAsyncTask(MainActivity.this, mCurrentOpenType);
				task.execute(pwd);

			}
		});

		alert.show();
	}

	public class LoadImageAsyncTask extends AsyncTask<String, Void, Bitmap> {

		private int mResultCode = 0;
		private String mPassword = null;
		private ProgressDialog mProgressDialog;

		public LoadImageAsyncTask(Context context, int resultCode) {
			mResultCode = resultCode;
			mProgressDialog = new ProgressDialog(context);
			mProgressDialog.setTitle("Loading");
		}

		@Override
		protected void onPreExecute() {
			super.onPreExecute();

			mProgressDialog.show();
		}

		@Override
		protected Bitmap doInBackground(String... params) {
			Bitmap bitmap = null;
			mPassword = params.length > 0 ? params[0] : null;

			switch (mResultCode) {
			case FileListActivity.RESULT_CODE_OPEN_JPEG:
			case FileListActivity.RESULT_CODE_OPEN_PNG:
				bitmap = BitmapFactory.decodeFile(mCurrentPath);

				mCurrentImgWidth = bitmap.getWidth();
				mCurrentImgHeight = bitmap.getHeight();
				break;

			case FileListActivity.RESULT_CODE_OPEN_JPGX:
				try {
					JpgxDecompressContainer jpgxcontainer = mProcessingService.getJpgxContainer(mCurrentPath, mPassword == null ? "" : mPassword);

					mCurrentImgWidth = jpgxcontainer.getWidth();
					mCurrentImgHeight = jpgxcontainer.getHeight();

					bitmap = ImageUtil.byteArrayToBitmap(jpgxcontainer.getImage(), jpgxcontainer.getWidth(), jpgxcontainer.getHeight());

				} catch (RemoteException e) {
					e.printStackTrace();
				}

				break;

			case FileListActivity.RESULT_CODE_OPEN_PNGX:
				try {
					PngxDecompressContainer jpgxcontainer = mProcessingService.getPngxContainer(mCurrentPath, mPassword == null ? "" : mPassword);

					mCurrentImgWidth = jpgxcontainer.getWidth();
					mCurrentImgHeight = jpgxcontainer.getHeight();

					bitmap = ImageUtil.byteArrayToBitmap(jpgxcontainer.getImage(), jpgxcontainer.getWidth(), jpgxcontainer.getHeight());

				} catch (RemoteException e) {
					e.printStackTrace();
				}
				break;

			default:
				break;
			}

			return bitmap;
		}

		@Override
		protected void onPostExecute(Bitmap result) {
			super.onPostExecute(result);

			mCurrentBitmap = result;
			mProgressDialog.hide();
			
			switch (mResultCode) {
			case FileListActivity.RESULT_CODE_OPEN_JPEG:
			case FileListActivity.RESULT_CODE_OPEN_PNG:
				setActionBarOptionEnabled(true);
				mSelectionCanvas.setEnabled(true);
				loadImage(result);
				break;

			case FileListActivity.RESULT_CODE_OPEN_JPGX:
			case FileListActivity.RESULT_CODE_OPEN_PNGX:
				
				
				setActionBarOptionEnabled(false);
				mSelectionCanvas.setEnabled(false);
				loadImage(result);

				if (mPassword == null)
					showPasswordDialog();
				break;
			}

			
		}

	}

}
