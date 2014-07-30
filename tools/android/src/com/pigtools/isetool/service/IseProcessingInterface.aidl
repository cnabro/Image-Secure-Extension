package com.pigtools.isetool.service;

import com.pigtools.isetool.service.container.JpgxDecompressContainer;

interface IseProcessingInterface {
	JpgxDecompressContainer getSecureJpegBuffer(String path, String key);
}
