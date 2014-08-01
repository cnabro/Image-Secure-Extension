package com.pigtools.isetool.service;

import com.pigtools.isetool.service.container.JpgxDecompressContainer;
import com.pigtools.isetool.service.container.PngxDecompressContainer;
import com.pigtools.isetool.service.container.SecureContainer;

interface IseProcessingInterface {
	JpgxDecompressContainer getJpgxContainer(String path, String key);
	PngxDecompressContainer getPngxContainer(String path, String key);
	boolean makeJPGX(String path, in List<SecureContainer> list, String pwd);
	boolean makePNGX(String path, in List<SecureContainer> list, String pwd);
}
