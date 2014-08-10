using namespace Windows::Storage::Streams;
#include <robuffer.h>

IBuffer^ byteArrayToIBufferPtr(byte *source, int size)
{
	Platform::ArrayReference<uint8> blobArray(source, size);
	IBuffer ^buffer = CryptographicBuffer::CreateFromByteArray(blobArray);
	return buffer;
}