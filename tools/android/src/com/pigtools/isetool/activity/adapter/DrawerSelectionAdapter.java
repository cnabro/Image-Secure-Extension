package com.pigtools.isetool.activity.adapter;

import java.util.ArrayList;

import android.content.Context;
import android.graphics.RectF;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import com.pigtools.isetool.R;

public class DrawerSelectionAdapter extends BaseAdapter {

	private Context mContext;
	private ArrayList<RectF> mRectList;
	private LayoutInflater mLayoutInflator;
	
	public DrawerSelectionAdapter(Context context, ArrayList<RectF> list)
	{
		mRectList = list;
		mContext = context;
		mLayoutInflator = (LayoutInflater) mContext.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
	}
	
	public void updateListItem(ArrayList<RectF> list)
	{
		mRectList = list;
		notifyDataSetChanged();
	}
	
	@Override
	public int getCount() {
		return mRectList.size();
	}

	@Override
	public RectF getItem(int position) {
		return mRectList.get(position);
	}

	@Override
	public long getItemId(int position) {
		return position;
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		
		convertView = mLayoutInflator.inflate(R.layout.layout_rectlist_item, null);
		
		RectF rect = mRectList.get(position);
		
		if(rect != null)
		{
			TextView posx = (TextView) convertView.findViewById(R.id.posx_textview);
			TextView posy = (TextView) convertView.findViewById(R.id.posy_textview);
			TextView width = (TextView) convertView.findViewById(R.id.width_textview);
			TextView height = (TextView) convertView.findViewById(R.id.height_textview);
			
			posx.setText(""+(int)rect.left);
			posy.setText(""+(int)rect.top);
			width.setText(""+(int)(rect.right-rect.left));
			height.setText(""+(int)(rect.bottom-rect.top));
		}
		
		return convertView;
	}

}
