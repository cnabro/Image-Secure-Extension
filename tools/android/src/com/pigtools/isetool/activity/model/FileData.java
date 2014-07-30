package com.pigtools.isetool.activity.model;

public class FileData {

	public static enum FileType {
		TYPE_FOLDER, TYPE_FILE
	}

	private String mName = "";
	private String mPath = "/";
	private FileType mType = FileType.TYPE_FILE;

	public FileData(String name, String path, FileType type) {
		mName = name;
		mPath = path;
		mType = type;
	}

	public String getPath() {
		return mPath;
	}

	public FileType getType() {
		return mType;
	}

	public String getName() {
		return mName;
	}

	public void setPath(String path) {
		this.mPath = path;
	}

	public void setType(FileType type) {
		this.mType = type;
	}

	public void setName(String name) {
		this.mName = name;
	}

}
