package com.pigtools.isetool.view;

import java.util.ArrayList;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.DashPathEffect;
import android.graphics.Paint;
import android.graphics.Paint.Style;
import android.graphics.RectF;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;

public class SelectionCanvas extends View {

	private Paint mBorderPaint;
	private Paint mInnerPaint;
	private int mInitialX = 0;
	private int mInitialY = 0;

	private RectF mDrawingRect = new RectF();
	private ArrayList<RectF> mRectList = new ArrayList<RectF>();

	public SelectionCanvas(Context context) {
		super(context);
		init();
	}

	public SelectionCanvas(Context context, AttributeSet attrs) {
		super(context, attrs);
		init();
	}

	public void init() {
		mBorderPaint = new Paint();
		mBorderPaint.setARGB(255, 255, 255, 255);
		mBorderPaint.setStyle(Style.STROKE);
		mBorderPaint.setPathEffect(new DashPathEffect(new float[] { 20, 5 }, 0));
		mBorderPaint.setStrokeWidth(5);
		
		mInnerPaint = new Paint();
		mInnerPaint.setARGB(150, 22, 22, 22);
		mInnerPaint.setStyle(Style.FILL);
		mInnerPaint.setStrokeWidth(5);

	}

	@Override
	protected void onDraw(Canvas canvas) {
		super.onDraw(canvas);
		for(RectF rect : mRectList)
		{
			canvas.drawRect(rect.left, rect.top, rect.right, rect.bottom, mBorderPaint);
			canvas.drawRect(rect.left, rect.top, rect.right, rect.bottom, mInnerPaint);
		}
		canvas.drawRect(mDrawingRect.left, mDrawingRect.top, mDrawingRect.right, mDrawingRect.bottom, mBorderPaint);
	}

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		int action = event.getAction();

		switch (action & MotionEvent.ACTION_MASK) {
		case MotionEvent.ACTION_DOWN:
			mInitialX = (int) event.getX();
			mInitialY = (int) event.getY();
			break;
		case MotionEvent.ACTION_UP:
			mDrawingRect.set(mInitialX, mInitialY, event.getX(), event.getY());
			mRectList.add(new RectF(mDrawingRect));
			mDrawingRect.set(0, 0, 0, 0);
			break;
		case MotionEvent.ACTION_MOVE:
			mDrawingRect.set(mInitialX, mInitialY, event.getX(), event.getY());
			break;
		}
		invalidate();
		return true;
	}

}
