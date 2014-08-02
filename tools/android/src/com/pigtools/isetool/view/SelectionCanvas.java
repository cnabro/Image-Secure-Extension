package com.pigtools.isetool.view;

import java.util.ArrayList;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.DashPathEffect;
import android.graphics.Paint;
import android.graphics.Paint.Style;
import android.graphics.PointF;
import android.graphics.RectF;
import android.media.FaceDetector.Face;
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
	private ArrayList<RectF> mAdjustRectList = new ArrayList<RectF>();

	private int mImageWidth = 0;
	private int mImageHeight = 0;
	private float mRatio = 0;

	private boolean mIsEnable = false;

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

		mRectList.clear();
		mAdjustRectList.clear();

		invalidate();
	}

	public void setImageSize(int width, int height, float ratio) {
		mImageHeight = height;
		mImageWidth = width;
		mRatio = ratio;
	}

	@Override
	protected void onDraw(Canvas canvas) {
		super.onDraw(canvas);

		
		for (RectF rect : mRectList) {
			canvas.drawRect(rect.left, rect.top, rect.right, rect.bottom, mBorderPaint);
			canvas.drawRect(rect.left, rect.top, rect.right, rect.bottom, mInnerPaint);
		}
		canvas.drawRect(mDrawingRect.left, mDrawingRect.top, mDrawingRect.right, mDrawingRect.bottom, mBorderPaint);
		
	}

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		if (mIsEnable) {
			int action = event.getAction();

			switch (action & MotionEvent.ACTION_MASK) {
			case MotionEvent.ACTION_DOWN:
				mInitialX = (int) event.getX();
				mInitialY = (int) event.getY();
				break;
			case MotionEvent.ACTION_UP:
				if (mInitialX < event.getX() && mInitialY < event.getY()) {
					mDrawingRect.set(mInitialX, mInitialY, event.getX(), event.getY());
					mRectList.add(new RectF(mDrawingRect));
					mInitialX = 0;
					mInitialY = 0;

					/**
					 * add adjust rect for saving
					 */

					mAdjustRectList.add(new RectF(mDrawingRect.left / mRatio, mDrawingRect.top / mRatio, mDrawingRect.right / mRatio,
							mDrawingRect.bottom / mRatio));
				}
				mDrawingRect.set(0, 0, 0, 0);
				break;
			case MotionEvent.ACTION_MOVE:
				mDrawingRect.set(mInitialX, mInitialY, event.getX(), event.getY());
				break;
			}
			invalidate();
		}

		return true;
	}

	@Override
	public void setEnabled(boolean enabled) {
		super.setEnabled(enabled);

		mIsEnable = enabled;
	}

	public void setFaceArraySelection(Face[] faces) {
		mAdjustRectList.clear();
		mRectList.clear();

		for (Face face : faces) {

			if (face != null) {
				PointF p = new PointF();
				face.getMidPoint(p);
				float distance = face.eyesDistance();

				float left = (p.x - distance) < 0 ? 0 : (p.x - distance);
				float top = (p.y - distance) < 0 ? 0 : (p.y - distance);
				float right = (p.x + distance) < 0 ? 0 : (p.x + distance);
				float bottom = (p.y + distance) < 0 ? 0 : (p.y + distance);

				mAdjustRectList.add(new RectF(left, top, right, bottom));
				mRectList.add(new RectF(left * mRatio, top * mRatio, right * mRatio, bottom * mRatio));
			}
		}

		invalidate();
	}
	
	public ArrayList<RectF> getAdjustRectList()
	{
		return mAdjustRectList;
	}

}
