#include "xcompress.h"

void xCompress::xCompressInit()
        {
            if (!HasxCompressLoaded)
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
                    HasxCompressLoaded = true;
                }
                else
                    qDebug("xCompress Not Found");


            }
        }

        int32_t xCompress::Decompress(uint8_t* compressedData, int32_t compressedLen, uint8_t* decompressedData, int32_t decompressedLen)
        {

            // Setup our decompression context
            int32_t DecompressionContext = 0;
            int32_t hr = XMemCreateDecompressionContext(XMEMCODEC_TYPE::XMEMCODEC_LZX, 0, 0, DecompressionContext);

            try
            {
                hr = XMemDecompress(DecompressionContext, decompressedData, decompressedLen, compressedData, compressedLen);
            }
            catch (std::exception ex)
            {
                XMemResetDecompressionContext(DecompressionContext);
                XMemDestroyDecompressionContext(DecompressionContext);
                qDebug() << "exception";
                return -1;
            }


            XMemResetDecompressionContext(DecompressionContext);
            XMemDestroyDecompressionContext(DecompressionContext);

            // Return our hr
            return hr;
        }

        int32_t xCompress::Compress(uint8_t* Data, int32_t DataLen, uint8_t* CompressedData, int32_t * OutCompressedLen)
        {

            // Setup our decompression context
            int32_t CompressionContext = 0;

            int32_t hr = XMemCreateCompressionContext(XMEMCODEC_TYPE::XMEMCODEC_LZX, 0, 0, CompressionContext);
            int32_t CompressedLen = DataLen;

            try
            {
                hr = XMemCompress(CompressionContext, CompressedData, CompressedLen, Data, DataLen);
            }
            catch (std::exception ex)
            {
                XMemResetCompressionContext(CompressionContext);
                XMemDestroyCompressionContext(CompressionContext);
                qDebug() << "exception";
                return -1;
            }

            XMemResetCompressionContext(CompressionContext);
            XMemDestroyCompressionContext(CompressionContext);

            *OutCompressedLen = CompressedLen;

            // Return our hr
            return hr;
        }
