package com.pigtools.isetool.activity.adapter;

import java.util.ArrayList;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;

import com.pigtools.isetool.R;
import com.pigtools.isetool.activity.model.FileData;
import com.pigtools.isetool.activity.model.FileData.FileType;

public class FileListAdapter extends BaseAdapter {

	private Context mContext;
	private ArrayList<FileData> mFileList;
	private LayoutInflater mLayoutInflator;

	public FileListAdapter(Context c, ArrayList<FileData> list) {
		mContext = c;
		mFileList = list;
		mLayoutInflator = (LayoutInflater) mContext.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
	}

	@Override
	public int getCount() {
		if (mFileList != null) {
			return mFileList.size();
		}
		
		return 0;
	}

	@Override
	public FileData getItem(int position) {
		if (mFileList != null) {
			return mFileList.get(position);
		}
		return null;
	}

	@Override
	public long getItemId(int position) {

		return position;
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {

		convertView = mLayoutInflator.inflate(R.layout.layout_filelist_item, null);
		
		TextView tv = (TextView)convertView.findViewById(R.id.title_textview);
		tv.setText(mFileList.get(position).getName());
		
		ImageView view = (ImageView)convertView.findViewById(R.id.icon_image);
		if(getItem(position).getType() == FileType.TYPE_FILE)
		{
			view.setImageResource(R.drawable.btn_image_n);
		}
		else
		{
			view.setImageResource(R.drawable.btn_folder_n);
		}
		
		
		
		return convertView;
	}
	
	public void updateListItem(ArrayList<FileData> list)
	{
		mFileList = list;
		notifyDataSetChanged();
	}

}
