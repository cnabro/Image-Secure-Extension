package com.pigtools.isetool.activity;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Environment;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ListView;
import android.widget.Toast;

import com.pigtools.isetool.R;
import com.pigtools.isetool.activity.adapter.FileListAdapter;
import com.pigtools.isetool.activity.manager.FileListManager;
import com.pigtools.isetool.activity.model.FileData;

public class FileListActivity extends Activity implements OnItemClickListener {

	public static final int REQUEST_CODE_OPEN_FILE = 0x01;
	public static final int RESULT_CODE_OPEN_JPGX = 0x10;
	public static final int RESULT_CODE_OPEN_JPEG = 0x11;
	public static final int RESULT_CODE_OPEN_PNGX = 0x12;
	public static final int RESULT_CODE_OPEN_PNG = 0x13;

	public static String EXTRA_KEY_RESULT = "com.pigtools.isetool.filelist.result";

	private ListView mFileListView;
	private FileListAdapter mFileAdapter;
	private FileListManager mFileListManager;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		setContentView(R.layout.activity_filelist);
		
		getActionBar().setDisplayHomeAsUpEnabled(true);
		getActionBar().setHomeButtonEnabled(true);
		
		mFileListView = (ListView) findViewById(R.id.file_listview);
		mFileListManager = new FileListManager();

		if (!init()) {
			Toast.makeText(this, "SD card를 읽을 수 없습니다.", Toast.LENGTH_SHORT).show();
		}
	}
	
	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		
		finish();
		
		return super.onOptionsItemSelected(item);
	}

	public boolean init() {
		String ext = Environment.getExternalStorageState();

		if (ext.equals(Environment.MEDIA_MOUNTED)) {
			mFileAdapter = new FileListAdapter(this, mFileListManager.getFileList(Environment.getExternalStorageDirectory().getAbsolutePath()));
			mFileListView.setAdapter(mFileAdapter);
			mFileListView.setOnItemClickListener(this);

			return true;
		} else {
			return false;
		}
	}

	@Override
	public void onItemClick(AdapterView<?> parent, View view, int position, long id) {

		FileData data = mFileAdapter.getItem(position);

		switch (data.getType()) {
		case TYPE_FILE:
			Intent intent = new Intent();
			intent.putExtra(EXTRA_KEY_RESULT, data.getPath());

			String path = data.getPath();

			if (path.endsWith(".jpgx")) {
				setResult(RESULT_CODE_OPEN_JPGX, intent);
			} else if (path.endsWith(".jpg")) {
				setResult(RESULT_CODE_OPEN_JPEG, intent);
			} else if (path.endsWith(".pngx")) {
				setResult(RESULT_CODE_OPEN_PNGX, intent);
			} else if (path.endsWith(".png")) {
				setResult(RESULT_CODE_OPEN_PNG, intent);
			}
			
			finish();

			break;

		case TYPE_FOLDER:
			mFileAdapter.updateListItem(mFileListManager.getFileList(data.getPath()));
			break;
		}

		mFileListView.setOnItemClickListener(this);
	}
}
