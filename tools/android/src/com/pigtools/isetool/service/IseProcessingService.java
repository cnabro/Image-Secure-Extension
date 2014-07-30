package com.pigtools.isetool.service;

import java.io.File;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.os.RemoteException;
import android.util.Log;

import com.pigtools.isetool.service.container.JpgxDecompressContainer;

public class IseProcessingService extends Service {

	private IseProcessingServiceBinder mRemoteBinder = new IseProcessingServiceBinder();
	
	
	@Override
	public void onCreate() {
		super.onCreate();
	}
	
	@Override
	public IBinder onBind(Intent intent) {

		return mRemoteBinder;
	}

	public class IseProcessingServiceBinder extends IseProcessingInterface.Stub {

		@Override
		public JpgxDecompressContainer getSecureJpegBuffer(String path, String key) throws RemoteException {
			
			Log.e("test","IseProcessingServiceBinder getSecureJpegBuffer");
			
			File file = new File(path);
			if(file.exists())
			{
				return nativeGetSecureJpegBuffer(path, key);
			}
			else
			{
				return null;
			}
		}
	}
	
	/*
	 * native function
	 */
	
	public native JpgxDecompressContainer nativeGetSecureJpegBuffer(String path, String key);
	
	static {
		System.loadLibrary("ise-jni");
	}

}
