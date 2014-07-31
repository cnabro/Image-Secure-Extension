package com.pigtools.isetool.util;

import java.nio.ByteBuffer;

import android.app.ProgressDialog;
import android.content.Context;
import android.graphics.Bitmap;
import android.media.FaceDetector;
import android.media.FaceDetector.Face;
import android.os.AsyncTask;

import com.pigtools.isetool.view.listener.OnFaceDetectionFinishedListener;

public class ImageUtil {

	public static Bitmap byteArrayToBitmap(byte[] byteArray, int width, int height) {
		Bitmap bm = Bitmap.createBitmap(width, height, Bitmap.Config.RGB_565);
		bm.copyPixelsFromBuffer(ByteBuffer.wrap(byteArray));

		return bm;
	}

	public static void findFaces(Context context, Bitmap bitmap, OnFaceDetectionFinishedListener listener) {
		AsyncFaceDetectionTask task = new AsyncFaceDetectionTask(context, listener);
		task.execute(bitmap);
	}

	private static class AsyncFaceDetectionTask extends AsyncTask<Bitmap, Void, Face[]> {
		private OnFaceDetectionFinishedListener mOnFaceDetectionFinishedListener;
		private ProgressDialog mProgressDialog;

		public AsyncFaceDetectionTask(Context context, OnFaceDetectionFinishedListener listener) {
			mOnFaceDetectionFinishedListener = listener;
			mProgressDialog = new ProgressDialog(context);
			mProgressDialog.setTitle("Loading");
		}

		@Override
		protected void onPreExecute() {
			super.onPreExecute();

			mProgressDialog.show();
		}

		@Override
		protected Face[] doInBackground(Bitmap... params) {

			if (params.length > 0) {
				Face[] faces = new Face[10];
				Bitmap bitmap = params[0];

				Bitmap face = bitmap.copy(Bitmap.Config.RGB_565, true);

				FaceDetector detector = new FaceDetector(face.getWidth(), face.getHeight(), 10);
				detector.findFaces(face, faces);

				face.recycle();

				return faces;
			}

			return null;
		}

		@Override
		protected void onPostExecute(Face[] result) {
			super.onPostExecute(result);

			if (mOnFaceDetectionFinishedListener != null) {
				mOnFaceDetectionFinishedListener.onDetectionFinished(result);
			}

			mProgressDialog.hide();
		}

	}
}
