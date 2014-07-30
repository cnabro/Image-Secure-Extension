package com.pigtools.isetool.util;

import java.nio.ByteBuffer;

import android.graphics.Bitmap;

public class ImageUtil {

	public static Bitmap byteArrayToBitmap(byte[] byteArray, int width, int height) {
		Bitmap bm = Bitmap.createBitmap(width, height, Bitmap.Config.RGB_565);
		bm.copyPixelsFromBuffer(ByteBuffer.wrap(byteArray));
		
		return bm;
	}
	
	
}
