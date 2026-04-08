///////////////////////
// Raster operations //
///////////////////////

#ifndef __RASTER_H
#define __RASTER_H

#include "misc.h"
#include "lang.h"
#include "globals.h"
#include "config.h"
#include "format.h"
#include "color.h"
#include "list.h"
#include "code.h"







class RASTER
{
    public:

        // converter class
        class CONVERTER
        {
            // friend classes
            friend class RASTER;

            public:

                // area data struct
                #pragma pack(1)
                struct AREA
                {
                    int src_x          PACKED;
                    int dest_x         PACKED;
                    int src_y          PACKED;
                    int dest_y         PACKED;
                    void* src_base     PACKED;
                    void* dest_base    PACKED;
                    void* src_start    PACKED;
                    void* dest_start   PACKED;
                    int src_adjust     PACKED;
                    int dest_adjust    PACKED;
                    int width          PACKED;
                    int height         PACKED;
                    int src_pitch      PACKED;
                    int dest_pitch     PACKED;
                };
                #pragma pack()

                // converter function types
                #ifdef __X86__
                typedef void* FUNCTION;
                typedef void* AREAFUNCTION;
                typedef void* PARTIALFUNCTION;
                #else
                typedef void (*FUNCTION)(void *src,void *dest,uint pixels,void *extra);
                typedef void (*AREAFUNCTION)(AREA *data,void *extra);
                typedef void (*PARTIALFUNCTION)(void *src,void *dest,uint start_byte,uint stop_byte,void *extra);
                #endif

            public:
                                
                // setup
                CONVERTER();
                ~CONVERTER();

                // interface
                int Convert(void *src,void *dest,uint pixels,void *extra); 
                int Convert(AREA data,void *extra);
                int Convert(void *src,void *dest,uint start_byte,uint stop_byte,void *extra); 

            private:

                // internal setup
                void SetupFormats(FORMAT const &src,FORMAT const &dest);
                void SetupFunctions(FUNCTION convert,AREAFUNCTION area,PARTIALFUNCTION partial);

                // internal convert
                int InternalConvert(void *src,void *dest,uint pixels,void *extra);
                int InternalConvert(AREA &data,void *extra);
                int InternalConvert(void *src,void *dest,uint start_byte,uint stop_byte,void *extra);

                // data
                FORMAT SourceFormat;
                FORMAT DestinationFormat;
                int Extra;
                int Copy;

                // functions
                FUNCTION ConvertFunction; 
                AREAFUNCTION AreaConvertFunction;
                PARTIALFUNCTION PartialConvertFunction;
        };

        // generated converter class
        #if defined(__SMC__) && defined(__X86__) && defined(__WIN32__)
        class GeneratedConverter
        {
            public:

                // data
                FORMAT src;
                FORMAT dest;
                Code code;
        };
        #endif

        /*
        // clear class
        class CLEAR
        {
            // friend classes
            friend class RASTER;

            public:

                // area data struct
                #pragma pack(1)
                struct AREA
                {
                    int x           PACKED;
                    int y           PACKED;
                    void* base      PACKED;
                    void* start     PACKED;
                    int src_adjust  PACKED;
                    int width       PACKED;
                    int height      PACKED;
                    int pitch       PACKED;
                };
                #pragma pack()

                // clear function types
                #ifdef __X86__
                typedef void* FUNCTION;
                typedef void* AREAFUNCTION;
                #else
                typedef void (*FUNCTION)(void *dest,uint pixels,void *color);
                typedef void (*AREAFUNCTION)(AREA *data,void *color);
                #endif

            public:
                                
                // setup
                CLEAR();
                ~CLEAR();

                // interface
                int Clear(void *dest,uint pixels,void *color); 
                int Clear(AREA data,void *color);

            private:

                // internal setup
                void SetupFormat(FORMAT const &format);
                void SetupFunctions(FUNCTION convert,AREAFUNCTION area);

                // internal convert
                int InternalConvert(void *dest,uint pixels,void *color);
                int InternalConvert(AREA &data,void *color);

                // data
                FORMAT Format;

                // functions
                FUNCTION ClearFunction; 
                AREAFUNCTION AreaClearFunction;
        };
        */

    public:
            
        // setup
        RASTER();
        ~RASTER();

        // converter request
        CONVERTER* RequestConverter(int src_id,int dest_id);
        CONVERTER* RequestConverter(FORMAT const &src,FORMAT const &dest);

        /*
        // clear request
        CLEAR *RequestClear(int src_id,int dest_id);
        CLEAR *RequestClear(FORMAT const &src,FORMAT const &dest);
        */

    private:

        // static converter setup
        static void InitConverters();
        static void CloseConverters();
        static void DefaultConverters();
        static int DefaultConverters_X86();
        static int DefaultConverters_CPP();
        static int DefaultConverters_MMX();
        static int SetupConverter(int src_id,int dest_id,CONVERTER::FUNCTION convert,CONVERTER::AREAFUNCTION area,CONVERTER::PARTIALFUNCTION partial);
        static int GenerateTable16_X86(void *dest,FORMAT const &format);

        // generate generic converter (smc)      
        #if defined(__SMC__) && defined(__X86__) && defined(__WIN32__)
        static int GenerateConverter(Code &code,FORMAT const &src,FORMAT const &dest);
        #endif
        
        // static copy converters
        static CONVERTER *StaticCopyConverter;

        // static 32bit -> X converters
        static CONVERTER *StaticConverter32;

        // static 16bit -> X converters
        static CONVERTER *StaticConverter16;

        // static 8bit -> X converters
        static CONVERTER *StaticConverter8;
        static CONVERTER *StaticConverter8_4BYTE;
        static CONVERTER *StaticConverter8_3BYTE;
        static CONVERTER *StaticConverter8_2BYTE;
        static CONVERTER *StaticConverter8_1BYTE;

        // static generated converter list
        #if defined(__SMC__) && defined(__X86__) && defined(__WIN32__)
        static List<GeneratedConverter> GeneratedConverterList;
        #endif
        
        /*
        // static clears
        static CLEAR *StaticClear_4BYTE;
        static CLEAR *StaticClear_3BYTE;
        static CLEAR *StaticClear_2BYTE;
        static CLEAR *StaticClear_1BYTE;
        */

        // static reference count
        static int ReferenceCount;
};








#endif
