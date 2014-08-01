package com.pigtools.isetool.service.container;

import android.os.Parcel;
import android.os.Parcelable;

public class SecureContainer implements Parcelable {
	private int mX;
	private int mY;
	private int mWidth;
	private int mHeight;

	public SecureContainer(int x, int y, int width, int height) {
		mX = x;
		mY = y;
		mWidth = width;
		mHeight = height;
	}

	public SecureContainer(Parcel in) {
		mX = in.readInt();
		mY = in.readInt();
		mWidth = in.readInt();
		mHeight = in.readInt();
	}

	public int getHeight() {
		return mHeight;
	}

	public int getPosX() {
		return mX;
	}

	public int getPosY() {
		return mY;
	}

	public int getWidth() {
		return mWidth;
	}

	public static final Parcelable.Creator CREATOR = new Parcelable.Creator() {
		public SecureContainer createFromParcel(Parcel in) {
			return new SecureContainer(in);
		}

		public SecureContainer[] newArray(int size) {
			return new SecureContainer[size];
		}
	};

	@Override
	public int describeContents() {

		return 0;
	}

	@Override
	public void writeToParcel(Parcel dest, int flags) {
		dest.writeInt(mX);
		dest.writeInt(mY);
		dest.writeInt(mWidth);
		dest.writeInt(mHeight);
	}
}
