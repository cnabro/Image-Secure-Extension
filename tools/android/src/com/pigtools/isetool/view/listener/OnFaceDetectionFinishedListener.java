package com.pigtools.isetool.view.listener;

import android.media.FaceDetector.Face;

public interface OnFaceDetectionFinishedListener {
	public void onDetectionFinished(Face[] faces);
}
