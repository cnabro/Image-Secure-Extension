package com.pigtools.isetool.service;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.os.RemoteException;
import android.util.Log;

import com.pigtools.isetool.service.container.JpgxDecompressContainer;
import com.pigtools.isetool.service.container.PngxDecompressContainer;
import com.pigtools.isetool.service.container.SecureContainer;

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
		public JpgxDecompressContainer getJpgxContainer(String path, String key) throws RemoteException {

			File file = new File(path);
			if (file.exists()) {
				return nativeGetSecureJpegBuffer(path, key);
			} else {
				return null;
			}
		}

		@Override
		public PngxDecompressContainer getPngxContainer(String path, String key) throws RemoteException {
			File file = new File(path);
			if (file.exists()) {
				return nativeGetSecurePngBuffer(path, key);
			} else {
				return null;
			}
		}

		@Override
		public boolean makeJPGX(String path, List<SecureContainer> list, String pwd) throws RemoteException {

			return nativeMakeJPGX(path, (ArrayList<SecureContainer>) list, pwd);
		}

		@Override
		public boolean makePNGX(String path, List<SecureContainer> list, String pwd) throws RemoteException {

			return nativeMakePNGX(path, (ArrayList<SecureContainer>) list, pwd);
		}
	}

	/*
	 * native function
	 */

	public native JpgxDecompressContainer nativeGetSecureJpegBuffer(String path, String key);

	public native PngxDecompressContainer nativeGetSecurePngBuffer(String path, String key);

	public native boolean nativeMakeJPGX(String path, ArrayList<SecureContainer> list, String pwd);

	public native boolean nativeMakePNGX(String path, ArrayList<SecureContainer> list, String pwd);

	static {
		System.loadLibrary("ise-jni");
	}

}
