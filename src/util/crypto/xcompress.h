#ifndef XCOMPRESS_H
#define XCOMPRESS_H

#include <cstdint>
#include <string>
#include <Windows.h>

class xCompress
{
public:
    enum XMEMCODEC_TYPE
    {
        XMEMCODEC_DEFAULT = 0,
        XMEMCODEC_LZX     = 1
    };

    struct XMEMCODEC_PARAMETERS_LZX
    {
        int flags;
        int windowSize;
        int compressionPartitionSize;
    };

    const int32_t stream = 0x00000001;
    HINSTANCE xCompressDLL;
    bool loaded = false;

    typedef int32_t(CALLBACK* XMemCreateDecompressionContext_CALL)(XMEMCODEC_TYPE codecType, int32_t codecParams, int32_t flags, int32_t &context);
    XMemCreateDecompressionContext_CALL XMemCreateDecompressionContext;

    typedef void (CALLBACK* XMemDestroyDecompressionContext_CALL)(int32_t context);
    XMemDestroyDecompressionContext_CALL XMemDestroyDecompressionContext;

    typedef int32_t(CALLBACK* XMemResetDecompressionContext_CALL)(int32_t context);
    XMemResetDecompressionContext_CALL XMemResetDecompressionContext;

    typedef int32_t(CALLBACK* XMemDecompress_CALL)(int32_t context, uint8_t* destination, int32_t &destSize, uint8_t* source, int32_t srcSize);
    XMemDecompress_CALL XMemDecompress;

    typedef int32_t(CALLBACK* XMemDecompressStream_CALL)(int32_t context, uint8_t* destination, int32_t &destSize, uint8_t* source, int32_t &srcSize);
    XMemDecompressStream_CALL XMemDecompressStream;

    typedef int32_t(CALLBACK* XMemCreateCompressionContext_CALL)(XMEMCODEC_TYPE codecType, int32_t codecParams, int32_t flags, int32_t &context);
    XMemCreateCompressionContext_CALL XMemCreateCompressionContext;

    typedef void (CALLBACK* XMemDestroyCompressionContext_CALL)(int32_t context);
    XMemDestroyCompressionContext_CALL XMemDestroyCompressionContext;

    typedef int32_t(CALLBACK* XMemResetCompressionContext_CALL)(int32_t context);
    XMemResetCompressionContext_CALL XMemResetCompressionContext;

    typedef int32_t(CALLBACK* XMemCompress_CALL)(int32_t context, uint8_t* destination, int32_t &destSize, uint8_t* source, int32_t srcSize);
    XMemCompress_CALL XMemCompress;

    typedef int32_t(CALLBACK* XMemCompressStream_CALL)(int32_t context, uint8_t* destination, int &destSize, uint8_t* source, int32_t &srcSize);
    XMemCompressStream_CALL XMemCompressStream;

    template<typename T>
    void LoadFunction(T* ptr, std::string str)
    {
        *ptr = (T)GetProcAddress(xCompressDLL, str.c_str());

        if (!*ptr)
        {
            FreeLibrary(xCompressDLL);
        }
    }

    void xCompressInit();
    int32_t Decompress(uint8_t* compressedData, int32_t compressedLen, uint8_t* decompressedData, int32_t decompressedLen);
    int32_t Compress(uint8_t* Data, int32_t DataLen, uint8_t* CompressedData, int32_t * OutCompressedLen);
};

#endif // XCOMPRESS_H
