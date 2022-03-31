#ifndef XCOMPRESS_H
#define XCOMPRESS_H

#include <cstdint>
#include <string>
#include <Windows.h>

#include <QDebug>

class xCompress
        {
        public:

            enum XMEMCODEC_TYPE
            {
                XMEMCODEC_DEFAULT = 0,
                XMEMCODEC_LZX = 1
            };
            struct XMEMCODEC_PARAMETERS_LZX
            {
                int Flags;
                int WindowSize;
                int CompressionPartitionSize;
            };

            const int32_t XMEMCOMPRESS_STREAM = 0x00000001;
            bool HasxCompressLoaded = false;
            HINSTANCE xCompressDLL;

            //{ Callback Pointer Allocation

            ///public static extern int XMemCreateDecompressionContext(XMEMCODEC_TYPE CodecType,int pCodecParams,int Flags, ref int pContext);
            typedef int32_t(CALLBACK* XMemCreateDecompressionContext_CALL)(XMEMCODEC_TYPE CodecType, int32_t pCodecParams, int32_t Flags, int32_t &pContext);
            XMemCreateDecompressionContext_CALL XMemCreateDecompressionContext;

            ///public static extern void XMemDestroyDecompressionContext(int Context);
            typedef void (CALLBACK* XMemDestroyDecompressionContext_CALL)(int32_t Context);
            XMemDestroyDecompressionContext_CALL XMemDestroyDecompressionContext;

            ///public static extern int XMemResetDecompressionContext(int Context);
            typedef int32_t(CALLBACK* XMemResetDecompressionContext_CALL)(int32_t Context);
            XMemResetDecompressionContext_CALL XMemResetDecompressionContext;

            ///public static extern int XMemDecompress(int Context, byte[] pDestination, ref int pDestSize, byte[] pSource, int pSrcSize);
            typedef int32_t(CALLBACK* XMemDecompress_CALL)(int32_t Context, uint8_t* pDestination, int32_t &pDestSize, uint8_t* pSource, int32_t pSrcSize);
            XMemDecompress_CALL XMemDecompress;

            ///public static extern int XMemDecompressStream(int Context, byte[] pDestination, ref int pDestSize, byte[] pSource, ref int pSrcSize);
            typedef int32_t(CALLBACK* XMemDecompressStream_CALL)(int32_t Context, uint8_t* pDestination, int32_t &pDestSize, uint8_t* pSource, int32_t &pSrcSize);
            XMemDecompressStream_CALL XMemDecompressStream;

            ///public static extern int XMemCreateCompressionContext(XMEMCODEC_TYPE CodecType, int pCodecParams, int Flags, ref int pContext);
            typedef int32_t(CALLBACK* XMemCreateCompressionContext_CALL)(XMEMCODEC_TYPE CodecType, int32_t pCodecParams, int32_t Flags, int32_t &pContext);
            XMemCreateCompressionContext_CALL XMemCreateCompressionContext;

            ///public static extern void XMemDestroyCompressionContext(int Context);
            typedef void (CALLBACK*XMemDestroyCompressionContext_CALL)(int32_t Context);
            XMemDestroyCompressionContext_CALL XMemDestroyCompressionContext;

            ///public static extern int XMemResetCompressionContext(int Context);
            typedef int32_t(CALLBACK* XMemResetCompressionContext_CALL)(int32_t Context);
            XMemResetCompressionContext_CALL XMemResetCompressionContext;

            ///public static extern int XMemCompress(int Context, byte[] pDestination, ref int pDestSize, byte[] pSource, int pSrcSize);
            typedef int32_t(CALLBACK* XMemCompress_CALL)(int32_t Context, uint8_t* pDestination, int32_t &pDestSize, uint8_t* pSource, int32_t pSrcSize);
            XMemCompress_CALL XMemCompress;

            ///public static extern int XMemCompressStream(int Context, byte[] pDestination, ref int pDestSize, byte[] pSource, ref int pSrcSize);
            typedef int32_t(CALLBACK* XMemCompressStream_CALL)(int32_t Context, uint8_t* pDestination, int &pDestSize, uint8_t* pSource, int32_t &pSrcSize);
            XMemCompressStream_CALL XMemCompressStream;

            //}
            //{ Template Functions
            template<typename T>
            void LoadFunction(T* ptr, std::string str)
            {
                *ptr = (T)GetProcAddress(xCompressDLL, str.c_str());
                if (!*ptr)
                {
                    // handle the error
                    FreeLibrary(xCompressDLL);
                    qDebug() << "Function: " << str.c_str();
                    //Utils::System::Throw("xCompress Function Not Found");
                }
            }
            //}

            void xCompressInit();
            int32_t Decompress(uint8_t* compressedData, int32_t compressedLen, uint8_t* decompressedData, int32_t decompressedLen);
            int32_t Compress(uint8_t* Data, int32_t DataLen, uint8_t* CompressedData, int32_t * OutCompressedLen);

        };

#endif // XCOMPRESS_H
