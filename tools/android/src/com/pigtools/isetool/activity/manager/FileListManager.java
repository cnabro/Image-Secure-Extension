package com.pigtools.isetool.activity.manager;

import java.io.File;
import java.io.FilenameFilter;
import java.util.ArrayList;

import android.os.Environment;
import android.util.Log;

import com.pigtools.isetool.activity.model.FileData;
import com.pigtools.isetool.activity.model.FileData.FileType;

public class FileListManager {

	public static class IseFilter implements FilenameFilter {
		@Override
		public boolean accept(File dir, String filename) {

			return (filename.endsWith(".jpg") || filename.endsWith(".jpgx") || filename.endsWith(".png") || filename.endsWith(".pngx") || (new File(
					dir + "/" + filename).isDirectory()));
		}
	}

	public ArrayList<FileData> getFileList(String path) {
		Log.e("test", "path : " + path);
		File files = new File(path);
		IseFilter filter = new IseFilter();
		ArrayList<FileData> list = new ArrayList<FileData>();

		if (!path.equals(Environment.getExternalStorageDirectory().getAbsolutePath())) {
			list.add(new FileData("../", path + "/../", FileType.TYPE_FOLDER));
		}

		if (files.listFiles(filter).length > 0) {
			for (File file : files.listFiles(filter)) {
				list.add(new FileData(file.getName(), file.getAbsolutePath(), file.isDirectory() ? FileType.TYPE_FOLDER : FileType.TYPE_FILE));
			}
		}

		return list;

	}
}
