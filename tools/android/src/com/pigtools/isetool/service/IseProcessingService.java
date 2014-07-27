package com.pigtools.isetool.service;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.os.RemoteException;

import com.pigtools.isetool.container.JpgxDecompressContainer;

public class IseProcessingService extends Service {

	private IseProcessingServiceBinder mRemoteBinder = new IseProcessingServiceBinder();
	
	@Override
	public IBinder onBind(Intent intent) {

		return mRemoteBinder;
	}

	public class IseProcessingServiceBinder extends IseProcessingInterface.Stub {

		@Override
		public JpgxDecompressContainer getSecureJpegBuffer(String path, String key) throws RemoteException {

			return getSecureJpegBuffer(path, key);
		}
	}
	
	/*
	 * native function
	 */
	
	public native JpgxDecompressContainer getSecureJpegBuffer(String path, String key);
	
	static {
		System.loadLibrary("ise-jni");
	}

}
