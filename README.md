﻿Image Secure Extension
----------------------
> 스마트 폰, 타블렛 등 스마트 휴대기기 사용이 많아 지면서 의도하지 않은 이미지 노출 및 개인정보 유출이 많이 이루어 지고 있습니다.
> 이를 해결하기 위해 새로운 보안 확장자인 `jpgx, pngx`를 고안했습니다.
> 개별 이미지 파일의 보안 및 관리를 위해 `jpgx, pngx`를 `encoding` 및 `decoding` 할 수 있는 암호화 툴 및 라이브러리 개발이 목적입니다.

algorithm
----------------------
* Encoding
	1. Image file header 파싱
	2. parameter로 전달 받은 암호화 필요 영역 `RGB` data 추출 및 `DES3` 암호화
	3. 추출한 `RGB data`를 `DES3` 암호화 하여 `*.ise` 파일로 저장
	4. 암호화 영역`secure_container` 정보 및 key hash값의 정보는 `prop.xml`에 생성
	5. 생성된 파일들을 `*.jpgx/pngx` 로 packaging 하여 압축

* Decoding
	1. `pngx/jpgx` 압축 해제
	2. `prop.xml`파싱하여 암호화 영역`secure_container` 및 `*.ise`파일 매칭
	3. `*.ise` 파일 복호화 하여 `RGB` data 추출
	4. core bitmap 파일과 복호화 된 `RGB` merge
	5. 파싱 완료후 `jpgx container` 반환

structure
----------------------
![structure](http://pigtools.com/ise/structure_resize.png "ise structure")

libise
----------------------
* 다양한 플랫폼 지원 및 Performance향상을 위해 native c library로 구현
* 다음과 같은 API 지원

```c
jpgx_decompress_container get_jpgx_container(char *filename, char* key);
pngx_decompress_container get_pngx_container(char *filename, char* key);

jpgx_compress_container make_jpgx(char *filename, secure_container **sc_array, int sc_arr_count, char* key);
pngx_compress_container make_pngx(char *filename, secure_container **sc_array, int sc_arr_count, char* key);
```

libise wrapper
----------------------
* `WPF` : C++/CLI 이용하여 C# wrapper 구현

```c++
public ref class ImageSecureExtention
{
	public:
		static JpgxDecompressContainer^ getJpgxContainer(System::String^ filename, System::String^ key);
		static PngxDecompressContainer^ getPngxContainer(System::String^ filename, System::String^ key);

		static void makeJPGX(System::String^ filename, System::Collections::Generic::List<SecureContainer^>^ scList, System::String^ key);
		static void makePNGX(System::String^ filename, System::Collections::Generic::List<SecureContainer^>^ scList, System::String^ key);
};
```

* `Windows Phone 8.1/WinRT` : C++/CLI 이용하여 C# wrapper 구현

```c++
public ref class ImageSecureExtention sealed
{
	public:
		static JpgxDecompressContainer^ getJpgxContainer(String^ filename, String^ key);
		static PngxDecompressContainer^ getPngxContainer(String^ filename, String^ key);

		static void makeJPGX(String^ filename, Windows::Foundation::Collections::IVector<SecureContainer^>^ scList, String^ key);
		static void makePNGX(String^ filename, Windows::Foundation::Collections::IVector<SecureContainer^>^ scList, String^ key);
};
```

* `Android` : JNI 를 이용한 JAVA wrapper 구현

```c
Java_com_pigtools_isetool_service_IseProcessingService_nativeGetSecureJpegBuffer(JNIEnv *env,jobject obj, jstring strpath, jstring strkey);
Java_com_pigtools_isetool_service_IseProcessingService_nativeGetSecurePngBuffer(JNIEnv *env,jobject obj, jstring strpath, jstring strkey);
Java_com_pigtools_isetool_service_IseProcessingService_nativeMakeJPGX(JNIEnv *env, jobject obj, jstring strpath, jobject list, jstring strkey);
Java_com_pigtools_isetool_service_IseProcessingService_nativeMakePNGX(JNIEnv *env, jobject obj, jstring strpath, jobject list, jstring strkey);
```


* `IOS` : 준비중

isetool(ISE Studio)
----------------------
* `libise`를 이용하여 ISE STUDIO 구현
* `jpgx/pngx` Viewer & Maker

![ise studio](http://pigtools.com/ise/ise_studio.png "ise studio")

* `Windows` : .NET 4.5 WPF, MVVM모델로 구현
* `Android` : Application형태로 지원, ISE Service를 통한 Third-Party API 지원
* `Windows Phone 8.1` : WP8.1 Application형태로 지원, MVVM모델로 구현
* `IOS` : 지원 예정

how to build
----------------------
* `Android` : native library 빌드 후 어플리케이션에서 shared-library로 사용
```bash
$ sh ./library/lib-android/build.sh
$ cd ./tools/android/
$ ndk-build
```
* `Windows/Windows Phone 8.1/WinRT` : VS2013으로 Build, WP8.1의 경우 RC 2부터 지원함


opensource
----------------------
* Iselib
```bash
| PolarsSSL-1.3.6          | http://polarssl.org/            		      | GPLv2 or proprietary	|
| LibJpeg 		   | http://www.ijg.org/ 			      | libjpeg License		|
| LibPng  		   | http://www.libpng.org/pub/png/libpng.html 	      | libpng License		|
| Zlib 			   | http://www.zlib.net/ 			      | zlib license 		|
| MINI-XML		   | http://www.msweet.org/projects.php?Z3  	      | AMini-XML License	|
```
* isetool-wpf
```bash
| mahapps.metro 	| http://mahapps.com/			| ms public license|
| mvvm light toolkit	| https://mvvmlight.codeplex.com/	| ms public license|
```

* isetool-windowsphone
```bash
| mvvm light toolkit	| https://mvvmlight.codeplex.com/	| ms public license|
```

license
----------------------
> `GPLv2` 라이선스 하에 배포 됩니다.
