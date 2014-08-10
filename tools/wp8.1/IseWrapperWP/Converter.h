#include <robuffer.h>

using namespace Windows::Storage::Streams;
using namespace Windows::Security::Cryptography;

IBuffer^ byteArrayToIBufferPtr(byte *source, int size)
{
	Platform::ArrayReference<uint8> blobArray(source, size);
	IBuffer ^buffer = CryptographicBuffer::CreateFromByteArray(blobArray);
	return buffer;
}