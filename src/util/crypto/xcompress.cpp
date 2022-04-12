#include "xcompress.h"

void xCompress::xCompressInit()
{
    if (!loaded)
    {
        xCompressDLL = LoadLibraryA("C:\\xcompress32.dll");

        if (xCompressDLL != NULL)
        {
            LoadFunction<XMemCreateDecompressionContext_CALL>(&XMemCreateDecompressionContext, "XMemCreateDecompressionContext");
            LoadFunction<XMemDestroyDecompressionContext_CALL>(&XMemDestroyDecompressionContext, "XMemDestroyDecompressionContext");
            LoadFunction<XMemDecompress_CALL>(&XMemDecompress, "XMemDecompress");
            LoadFunction<XMemDecompressStream_CALL>(&XMemDecompressStream, "XMemDecompressStream");
            LoadFunction<XMemCreateCompressionContext_CALL>(&XMemCreateCompressionContext, "XMemCreateCompressionContext");
            LoadFunction<XMemDestroyCompressionContext_CALL>(&XMemDestroyCompressionContext, "XMemDestroyCompressionContext");
            LoadFunction<XMemResetCompressionContext_CALL>(&XMemResetCompressionContext, "XMemResetCompressionContext");
            LoadFunction<XMemCompress_CALL>(&XMemCompress, "XMemCompress");
            LoadFunction<XMemCompressStream_CALL>(&XMemCompressStream, "XMemCompressStream");

            loaded = true;
        }
    }
}

int32_t xCompress::Decompress(uint8_t* compressedData, int32_t compressedLen, uint8_t* decompressedData, int32_t decompressedLen)
{
    // Setup our decompression context
    int32_t decompressionContext = 0;
    int32_t hr = XMemCreateDecompressionContext(XMEMCODEC_TYPE::XMEMCODEC_LZX, 0, 0, decompressionContext);

    try
    {
        hr = XMemDecompress(decompressionContext, decompressedData, decompressedLen, compressedData, compressedLen);
    }
    catch (std::exception ex)
    {
        XMemResetDecompressionContext(decompressionContext);
        XMemDestroyDecompressionContext(decompressionContext);

        return -1;
    }

    XMemResetDecompressionContext(decompressionContext);
    XMemDestroyDecompressionContext(decompressionContext);

    // Return our hr
    return hr;
}

int32_t xCompress::Compress(uint8_t *data, int32_t dataLen, uint8_t *compressedData, int32_t *outCompressedLen)
{
    // Setup our decompression context
    int32_t compressionContext = 0;

    int32_t hr = XMemCreateCompressionContext(XMEMCODEC_TYPE::XMEMCODEC_LZX, 0, 0, compressionContext);
    int32_t compressedLen = dataLen;

    try
    {
        hr = XMemCompress(compressionContext, compressedData, compressedLen, data, dataLen);
    }
    catch (std::exception ex)
    {
        XMemResetCompressionContext(compressionContext);
        XMemDestroyCompressionContext(compressionContext);

        return -1;
    }

    XMemResetCompressionContext(compressionContext);
    XMemDestroyCompressionContext(compressionContext);

    *outCompressedLen = compressedLen;

    // Return our hr
    return hr;
}
