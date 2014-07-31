package com.pigtools.isetool.view;

import android.content.Context;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.LinearLayout;

import com.pigtools.isetool.R;
import com.pigtools.isetool.view.listener.OnBottomToggleSelectedListener;

public class BottomToggleLayout extends LinearLayout implements OnClickListener {

	private Context mContext;
	private LayoutInflater mLayoutInflater;
	private OnBottomToggleSelectedListener mBottomToggleSelectedListener;

	private LinearLayout mSelectionRectBtn;
	private LinearLayout mSelectionPeopleBtn;
	private LinearLayout mSelectionTextBtn;
	private LinearLayout mSelectionCancelBtn;

	public BottomToggleLayout(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);

		init(context);
	}

	public BottomToggleLayout(Context context, AttributeSet attrs) {
		super(context, attrs);

		init(context);
	}

	public BottomToggleLayout(Context context) {
		super(context);

		init(context);
	}

	public void setOnItemSelectedListener(OnBottomToggleSelectedListener listener) {
		mBottomToggleSelectedListener = listener;
	}

	public void init(Context context) {
		mContext = context;
		mLayoutInflater = (LayoutInflater) mContext.getSystemService(Context.LAYOUT_INFLATER_SERVICE);

		mLayoutInflater.inflate(R.layout.layout_bottom_toggle, this);

		mSelectionRectBtn = (LinearLayout) findViewById(R.id.selection_rect_btn);
		mSelectionPeopleBtn = (LinearLayout) findViewById(R.id.selection_people_btn);
		mSelectionTextBtn = (LinearLayout) findViewById(R.id.selection_text_btn);
		mSelectionCancelBtn = (LinearLayout) findViewById(R.id.selection_cancel_btn);
		
		mSelectionRectBtn.setOnClickListener(this);
		mSelectionPeopleBtn.setOnClickListener(this);
		mSelectionTextBtn.setOnClickListener(this);
		mSelectionCancelBtn.setOnClickListener(this);
	}

	@Override
	public void onClick(View v) {

		if (mBottomToggleSelectedListener != null) {
			mBottomToggleSelectedListener.onToggleButtonSelected(v.getId());
		}

		switch (v.getId()) {
		case R.id.selection_rect_btn:
			mSelectionRectBtn.setSelected(true);
			mSelectionPeopleBtn.setSelected(false);
			mSelectionTextBtn.setSelected(false);
			break;

		case R.id.selection_people_btn:
			mSelectionRectBtn.setSelected(false);
			mSelectionPeopleBtn.setSelected(true);
			mSelectionTextBtn.setSelected(false);
			break;

		case R.id.selection_text_btn:
			mSelectionRectBtn.setSelected(false);
			mSelectionPeopleBtn.setSelected(false);
			mSelectionTextBtn.setSelected(true);
			break;

		case R.id.selection_cancel_btn:
			mSelectionRectBtn.setSelected(false);
			mSelectionPeopleBtn.setSelected(false);
			mSelectionTextBtn.setSelected(false);
			break;

		default:
			break;
		}

	}

}
