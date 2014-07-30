package com.pigtools.isetool.service.container;

import android.os.Parcel;
import android.os.Parcelable;
import android.util.Log;

public class PngxDecompressContainer implements Parcelable{

	private int mHeight = 0;
	private int mWidth = 0;

	private int mColorSpace = 0;
	private int mStatus = 0;

	private byte[] mImage;

	public PngxDecompressContainer() {
		
	}
	
	public PngxDecompressContainer(Parcel in) {
		mHeight = in.readInt();
		mWidth = in.readInt();
		mColorSpace = in.readInt();
		mStatus = in.readInt();
		
		mImage = in.createByteArray();
	}

	public int getHeight() {
		return mHeight;
	}

	public int getWidth() {
		return mWidth;
	}

	public int getColorSpace() {
		return mColorSpace;
	}

	public int getStatus() {
		return mStatus;
	}

	public byte[] getImage() {
		return mImage;
	}

	public void setHeight(int mHeight) {
		this.mHeight = mHeight;
	}

	public void setWidth(int mWidth) {
		this.mWidth = mWidth;
	}

	public void setColorSpace(int colorspace) {
		this.mColorSpace = colorspace;
	}

	public void setStatus(int status) {
		this.mStatus = status;
	}

	public void setImage(byte[] image) {
		this.mImage = image;
	}

	@Override
	public int describeContents() {
		
		return 0;
	}

	@Override
	public void writeToParcel(Parcel dest, int flags) {
		dest.writeInt(mHeight);
		dest.writeInt(mWidth);
		dest.writeInt(mColorSpace);
		dest.writeInt(mStatus);
		dest.writeByteArray(mImage);
	}
	
	public static final Parcelable.Creator CREATOR = new Parcelable.Creator() {
        public PngxDecompressContainer createFromParcel(Parcel in) {
             return new PngxDecompressContainer(in);
       }

       public PngxDecompressContainer[] newArray(int size) {
            return new PngxDecompressContainer[size];
       }
   };
}
