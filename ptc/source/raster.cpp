///////////////////////
// raster operations //
///////////////////////
#include "raster.h"

#include "x86_8.h"
#include "x86_16.h"
#include "x86_32.h"
#include "x86_fake.h"
#include "x86_copy.h"

#include "mmx_8.h"
#include "mmx_16.h"
#include "mmx_32.h"
#include "mmx_fake.h"
#include "mmx_copy.h"

#include "cpp_8.h"
#include "cpp_16.h"
#include "cpp_32.h"
#include "cpp_copy.h"

#include "clear.h"

#ifdef __LINUX__
#include "elffix.h"
#endif







// static reference count
int RASTER::ReferenceCount=0;

// static copy converters
RASTER::CONVERTER *RASTER::StaticCopyConverter;

// static 32bit -> X converters
RASTER::CONVERTER *RASTER::StaticConverter32;

// static 16bit -> X converters
RASTER::CONVERTER *RASTER::StaticConverter16;

// static 8bit -> X converters
RASTER::CONVERTER *RASTER::StaticConverter8;
RASTER::CONVERTER *RASTER::StaticConverter8_4BYTE;
RASTER::CONVERTER *RASTER::StaticConverter8_3BYTE;
RASTER::CONVERTER *RASTER::StaticConverter8_2BYTE;
RASTER::CONVERTER *RASTER::StaticConverter8_1BYTE;

// static generated converter list
#if defined(__SMC__) && defined(__X86__) && defined(__WIN32__)
List<RASTER::GeneratedConverter> RASTER::GeneratedConverterList;
#endif






RASTER::RASTER()
{
    // increment reference
    ReferenceCount++;

    // initialize on first reference
    if (ReferenceCount==1)
    {
        // converters
        InitConverters();
    }
}


RASTER::~RASTER()
{
    // check reference
    if (ReferenceCount>0)
    {
        // decrement
        ReferenceCount--;
    
        // close on zero
        if (ReferenceCount==0)
        {
            // converters
            CloseConverters();
        }
    }
}







RASTER::CONVERTER* RASTER::RequestConverter(int src_id,int dest_id)
{
    // fail on invalid destination id
    if (dest_id<FORMATBASE || dest_id>FORMATMAX) return NULL;

    // allocate converter
    CONVERTER *converter=new CONVERTER;

    // initialize converter
    if (src_id==dest_id)       *converter = StaticCopyConverter [ src_id - FORMATBASE];
    else if (src_id==ARGB8888) *converter = StaticConverter32   [dest_id - FORMATBASE];
    else if (src_id==RGB565)   *converter = StaticConverter16   [dest_id - FORMATBASE];
    else if (src_id==INDEX8)   *converter = StaticConverter8    [dest_id - FORMATBASE];
    else if (src_id==GREY8)    *converter = StaticConverter8    [dest_id - FORMATBASE];
    else
    {
        // failure
        delete converter;
        return NULL;
    }

    // success
    return converter;
}


RASTER::CONVERTER* RASTER::RequestConverter(FORMAT const &src,FORMAT const &dest)
{
    // try to get converter by id
    CONVERTER *converter=RequestConverter(src.id,dest.id);
    if (converter) return converter;

    // allocate converter
    converter=new CONVERTER;

    // initialize converter
    if (src.id==INDEX8 || src.id==GREY8)
    {
        //printf("8bit->X converter\n");

        // 8bit -> X
        switch (dest.bytes)
        {
            case 4: *converter=*StaticConverter8_4BYTE; break;
            case 3: *converter=*StaticConverter8_3BYTE; break;
            case 2: *converter=*StaticConverter8_2BYTE; break;
            case 1: *converter=*StaticConverter8_1BYTE; break;
        }
    }
    else if (src==dest && src.ok())
    {
        //printf("custom copy converter\n");

        // custom copy converter
        converter->SetupFormats(src,dest);
        converter->SetupFunctions(StaticCopyConverter[0].ConvertFunction,
                                  StaticCopyConverter[0].AreaConvertFunction,
                                  StaticCopyConverter[0].PartialConvertFunction);
    }
    else
    {
        #if defined(__X86__) && defined(__SMC__) && defined(__WIN32__)

        // convert function
        void *function=NULL;

        // search for existing generic converter
        List<GeneratedConverter>::Iterator iterator=GeneratedConverterList.first();
        GeneratedConverter *current=iterator.current();
        while (current)
        {
            // check for match
            if (current->src==src && current->dest==dest)
            {
                //printf("found generated converter!\n");
                function=current->code.start();
                goto found;
            }

            // next converter
            current=iterator.next();
        }

        found:

        // check function
        if (!function)
        {
            // setup generated converter
            //printf("generating x86 converter!\n");
            GeneratedConverter *generated=new GeneratedConverter;
            generated->src=src;
            generated->dest=dest;
            if (GenerateConverter(generated->code,src,dest))
            {
                // setup function
                //printf("setting up generated function\n");

                // setup function
                function=generated->code.start();

                // print code
                //generated->code.print();

                // add to generated converter list
                GeneratedConverterList.add(generated);
            }
            else 
            {
                //printf("generation failure\n");
                delete generated;
            }
        }

        // check function
        if (function)
        {
            // setup generated generic custom converter
            //printf("generated x86 generic converter\n");
            converter->SetupFormats(src,dest);
            converter->SetupFunctions(function,NULL,NULL);
            return converter;
        }

        #endif

        // generic converter
        //printf("generic converter\n");
        converter->SetupFormats(src,dest);
    }

    // success
    return converter;
}








void RASTER::InitConverters()
{  
    // allocate static converters
    StaticCopyConverter=new CONVERTER[FORMATMAX-FORMATBASE+1];
    StaticConverter32=new CONVERTER[FORMATMAX-FORMATBASE+1];
    StaticConverter16=new CONVERTER[FORMATMAX-FORMATBASE+1];
    StaticConverter8=new CONVERTER[FORMATMAX-FORMATBASE+1];
    StaticConverter8_4BYTE=new CONVERTER;
    StaticConverter8_3BYTE=new CONVERTER;
    StaticConverter8_2BYTE=new CONVERTER;
    StaticConverter8_1BYTE=new CONVERTER;

    // X -> X copy converters
    int id; for (id=FORMATBASE; id<FORMATMAX+1; id++) StaticCopyConverter[id-FORMATBASE].SetupFormats(id,id);

    // 32bit -> X
    for (id=FORMATBASE; id<FORMATMAX+1; id++) StaticConverter32[id-FORMATBASE].SetupFormats(ARGB8888,id);

    // 16bit -> X
    for (id=FORMATBASE; id<FORMATMAX+1; id++) StaticConverter16[id-FORMATBASE].SetupFormats(RGB565,id);

    // 8bit -> X
    for (id=FORMATBASE; id<FORMATMAX+1; id++) StaticConverter8[id-FORMATBASE].SetupFormats(INDEX8,id);

    // 8bit -> 4BYTE
    StaticConverter8_4BYTE->SetupFormats(INDEX8,0);
    StaticConverter8_4BYTE->DestinationFormat.bytes=4;
    StaticConverter8_4BYTE->DestinationFormat.bits=32;

    // 8bit -> 3BYTE
    StaticConverter8_3BYTE->SetupFormats(INDEX8,0);
    StaticConverter8_3BYTE->DestinationFormat.bytes=3;
    StaticConverter8_3BYTE->DestinationFormat.bits=24;

    // 8bit -> 2BYTE
    StaticConverter8_2BYTE->SetupFormats(INDEX8,0);
    StaticConverter8_2BYTE->DestinationFormat.bytes=2;
    StaticConverter8_2BYTE->DestinationFormat.bits=16;

    // 8bit -> 1BYTE
    StaticConverter8_1BYTE->SetupFormats(INDEX8,0);
    StaticConverter8_1BYTE->DestinationFormat.bytes=1;
    StaticConverter8_1BYTE->DestinationFormat.bits=8;

    // setup defaults
    DefaultConverters();
}


void RASTER::CloseConverters()
{  
    // cleanup
    delete[] StaticCopyConverter;
    delete[] StaticConverter32;
    delete[] StaticConverter16;
    delete[] StaticConverter8;
    delete StaticConverter8_4BYTE;
    delete StaticConverter8_3BYTE;
    delete StaticConverter8_2BYTE;
    delete StaticConverter8_1BYTE;
}



void RASTER::DefaultConverters()
{    
#if defined(__X86__)

    // setup x86 converters
    DefaultConverters_X86();

#elif defined(__MMX__)

    // setup mmx converters
    DefaultConverters_MMX();

#else

    // setup c++ converters
    DefaultConverters_CPP();

#endif
}




int RASTER::DefaultConverters_CPP()
{
    // copy converters
    for (int id=FORMATBASE; id<FORMATMAX+1; id++)
    {
        StaticCopyConverter[id-FORMATBASE].ConvertFunction=(CONVERTER::FUNCTION)ConvertCopy_CPP;
        StaticCopyConverter[id-FORMATBASE].AreaConvertFunction=NULL;
        StaticCopyConverter[id-FORMATBASE].PartialConvertFunction=NULL;
    }

    // 32bit -> X
    #ifdef __32BIT__
    SetupConverter( ARGB8888, ARGB8888, (CONVERTER::FUNCTION)ConvertCopy_CPP,        NULL, NULL );
    SetupConverter( ARGB8888, ABGR8888, (CONVERTER::FUNCTION)Convert32_ABGR8888_CPP, NULL, NULL );
    SetupConverter( ARGB8888, RGBA8888, (CONVERTER::FUNCTION)Convert32_RGBA8888_CPP, NULL, NULL );
    SetupConverter( ARGB8888, BGRA8888, (CONVERTER::FUNCTION)Convert32_BGRA8888_CPP, NULL, NULL );
    SetupConverter( ARGB8888,   RGB888, (CONVERTER::FUNCTION)Convert32_RGB888_CPP,   NULL, NULL );
    SetupConverter( ARGB8888,   BGR888, (CONVERTER::FUNCTION)Convert32_BGR888_CPP,   NULL, NULL );
    SetupConverter( ARGB8888,   RGB565, (CONVERTER::FUNCTION)Convert32_RGB565_CPP,   NULL, NULL );
    SetupConverter( ARGB8888,   BGR565, (CONVERTER::FUNCTION)Convert32_BGR565_CPP,   NULL, NULL );
    SetupConverter( ARGB8888, ARGB1555, (CONVERTER::FUNCTION)Convert32_ARGB1555_CPP, NULL, NULL );
    SetupConverter( ARGB8888, ABGR1555, (CONVERTER::FUNCTION)Convert32_ABGR1555_CPP, NULL, NULL );
    SetupConverter( ARGB8888,    GREY8, (CONVERTER::FUNCTION)Convert32_GREY8_CPP,    NULL, NULL );
    SetupConverter( ARGB8888,   RGB332, (CONVERTER::FUNCTION)Convert32_RGB332_CPP,   NULL, NULL );
    #endif

    // 16bit -> X
    /*
    #ifdef __16BIT__
    SetupConverter( RGB565, ARGB8888, (CONVERTER::FUNCTION)Convert16_ARGB8888_CPP, NULL, NULL );
    SetupConverter( RGB565, ABGR8888, (CONVERTER::FUNCTION)Convert16_ABGR8888_CPP, NULL, NULL );
    SetupConverter( RGB565, RGBA8888, (CONVERTER::FUNCTION)Convert16_RGBA8888_CPP, NULL, NULL );
    SetupConverter( RGB565, BGRA8888, (CONVERTER::FUNCTION)Convert16_BGRA8888_CPP, NULL, NULL );
    SetupConverter( RGB565,   RGB888, (CONVERTER::FUNCTION)Convert16_RGB888_CPP,   NULL, NULL );
    SetupConverter( RGB565,   BGR888, (CONVERTER::FUNCTION)Convert16_BGR888_CPP,   NULL, NULL );
    SetupConverter( RGB565,   RGB565, (CONVERTER::FUNCTION)ConvertCopy_CPP,        NULL, NULL );
    SetupConverter( RGB565,   BGR565, (CONVERTER::FUNCTION)Convert16_BGR565_CPP,   NULL, NULL );
    SetupConverter( RGB565, ARGB1555, (CONVERTER::FUNCTION)Convert16_ARGB1555_CPP, NULL, NULL );
    SetupConverter( RGB565, ABGR1555, (CONVERTER::FUNCTION)Convert16_ABGR1555_CPP, NULL, NULL );
    SetupConverter( RGB565,    GREY8, (CONVERTER::FUNCTION)Convert16_GREY8_CPP,    NULL, NULL );
    SetupConverter( RGB565,   RGB332, (CONVERTER::FUNCTION)Convert16_RGB332_CPP,   NULL, NULL );
    #endif
    */

    // 8bit -> X
    #ifdef __8BIT__
    StaticConverter8_4BYTE->ConvertFunction=(CONVERTER::FUNCTION)Convert8_4BYTE_CPP;
    StaticConverter8_3BYTE->ConvertFunction=(CONVERTER::FUNCTION)Convert8_3BYTE_CPP;
    StaticConverter8_2BYTE->ConvertFunction=(CONVERTER::FUNCTION)Convert8_2BYTE_CPP;
    StaticConverter8_1BYTE->ConvertFunction=(CONVERTER::FUNCTION)Convert8_1BYTE_CPP;
    SetupConverter( INDEX8, ARGB8888, (CONVERTER::FUNCTION)Convert8_4BYTE_CPP, NULL, NULL );
    SetupConverter( INDEX8, ABGR8888, (CONVERTER::FUNCTION)Convert8_4BYTE_CPP, NULL, NULL );
    SetupConverter( INDEX8, RGBA8888, (CONVERTER::FUNCTION)Convert8_4BYTE_CPP, NULL, NULL );
    SetupConverter( INDEX8, BGRA8888, (CONVERTER::FUNCTION)Convert8_4BYTE_CPP, NULL, NULL );
    SetupConverter( INDEX8,   RGB888, (CONVERTER::FUNCTION)Convert8_3BYTE_CPP, NULL, NULL );
    SetupConverter( INDEX8,   BGR888, (CONVERTER::FUNCTION)Convert8_3BYTE_CPP, NULL, NULL );
    SetupConverter( INDEX8,   RGB565, (CONVERTER::FUNCTION)Convert8_2BYTE_CPP, NULL, NULL );
    SetupConverter( INDEX8,   BGR565, (CONVERTER::FUNCTION)Convert8_2BYTE_CPP, NULL, NULL );
    SetupConverter( INDEX8, ARGB1555, (CONVERTER::FUNCTION)Convert8_2BYTE_CPP, NULL, NULL );
    SetupConverter( INDEX8, ABGR1555, (CONVERTER::FUNCTION)Convert8_2BYTE_CPP, NULL, NULL );
    SetupConverter( INDEX8,    GREY8, (CONVERTER::FUNCTION)Convert8_1BYTE_CPP, NULL, NULL );
    SetupConverter( INDEX8,   RGB332, (CONVERTER::FUNCTION)Convert8_1BYTE_CPP, NULL, NULL );
    #endif

    // YCbCrA -> X
    #ifdef __YXX__
    GenerateTableYCbCr32_CPP();
    GenerateTableYCbCr16_CPP();
    #endif

    // done
    return 1;
}


int RASTER::DefaultConverters_X86()
{
#ifdef __X86__

    // copy converters
    for (int id=FORMATBASE; id<FORMATMAX+1; id++)
    {
        StaticCopyConverter[id-FORMATBASE].ConvertFunction=(CONVERTER::FUNCTION)ConvertCopy_X86;
        StaticCopyConverter[id-FORMATBASE].AreaConvertFunction=(CONVERTER::AREAFUNCTION)AreaConvertCopy_X86;
        StaticCopyConverter[id-FORMATBASE].PartialConvertFunction=NULL;
    }

    #ifdef __32BIT__
    // 32bit -> X
    SetupConverter( ARGB8888, ARGB8888, (CONVERTER::FUNCTION)ConvertCopy_X86,        (CONVERTER::AREAFUNCTION)AreaConvertCopy_X86,        NULL );
    SetupConverter( ARGB8888, ABGR8888, (CONVERTER::FUNCTION)Convert32_ABGR8888_X86, (CONVERTER::AREAFUNCTION)AreaConvert32_ABGR8888_X86, NULL );
    SetupConverter( ARGB8888, RGBA8888, (CONVERTER::FUNCTION)Convert32_RGBA8888_X86, (CONVERTER::AREAFUNCTION)AreaConvert32_RGBA8888_X86, NULL );
    SetupConverter( ARGB8888, BGRA8888, (CONVERTER::FUNCTION)Convert32_BGRA8888_X86, (CONVERTER::AREAFUNCTION)AreaConvert32_BGRA8888_X86, NULL );
    SetupConverter( ARGB8888,   RGB888, (CONVERTER::FUNCTION)Convert32_RGB888_X86,   (CONVERTER::AREAFUNCTION)AreaConvert32_RGB888_X86,   NULL );
    SetupConverter( ARGB8888,   BGR888, (CONVERTER::FUNCTION)Convert32_BGR888_X86,   (CONVERTER::AREAFUNCTION)AreaConvert32_BGR888_X86,   NULL );
    SetupConverter( ARGB8888,   RGB565, (CONVERTER::FUNCTION)Convert32_RGB565_X86,   (CONVERTER::AREAFUNCTION)AreaConvert32_RGB565_X86,   NULL );
    SetupConverter( ARGB8888,   BGR565, (CONVERTER::FUNCTION)Convert32_BGR565_X86,   (CONVERTER::AREAFUNCTION)AreaConvert32_BGR565_X86,   NULL );
    SetupConverter( ARGB8888, ARGB1555, (CONVERTER::FUNCTION)Convert32_ARGB1555_X86, (CONVERTER::AREAFUNCTION)AreaConvert32_ARGB1555_X86, NULL );
    SetupConverter( ARGB8888, ABGR1555, (CONVERTER::FUNCTION)Convert32_ABGR1555_X86, (CONVERTER::AREAFUNCTION)AreaConvert32_ABGR1555_X86, NULL );
    SetupConverter( ARGB8888,    GREY8, (CONVERTER::FUNCTION)Convert32_GREY8_X86,    (CONVERTER::AREAFUNCTION)AreaConvert32_GREY8_X86,    NULL );
    SetupConverter( ARGB8888,   RGB332, (CONVERTER::FUNCTION)Convert32_RGB332_X86,   (CONVERTER::AREAFUNCTION)AreaConvert32_RGB332_X86,   NULL );
    #endif

    // 16bit -> X
    #ifdef __16BIT__
    SetupConverter( RGB565, ARGB8888, (CONVERTER::FUNCTION)Convert16_ARGB8888_X86, (CONVERTER::AREAFUNCTION)AreaConvert16_ARGB8888_X86, NULL );
    SetupConverter( RGB565, ABGR8888, (CONVERTER::FUNCTION)Convert16_ABGR8888_X86, (CONVERTER::AREAFUNCTION)AreaConvert16_ABGR8888_X86, NULL );
    SetupConverter( RGB565, RGBA8888, (CONVERTER::FUNCTION)Convert16_RGBA8888_X86, (CONVERTER::AREAFUNCTION)AreaConvert16_RGBA8888_X86, NULL );
    SetupConverter( RGB565, BGRA8888, (CONVERTER::FUNCTION)Convert16_BGRA8888_X86, (CONVERTER::AREAFUNCTION)AreaConvert16_BGRA8888_X86, NULL );
    SetupConverter( RGB565,   RGB888, (CONVERTER::FUNCTION)Convert16_RGB888_X86,   (CONVERTER::AREAFUNCTION)AreaConvert16_RGB888_X86,   NULL );
    SetupConverter( RGB565,   BGR888, (CONVERTER::FUNCTION)Convert16_BGR888_X86,   (CONVERTER::AREAFUNCTION)AreaConvert16_BGR888_X86,   NULL );
    SetupConverter( RGB565,   RGB565, (CONVERTER::FUNCTION)ConvertCopy_X86,        (CONVERTER::AREAFUNCTION)AreaConvertCopy_X86,        NULL );
    SetupConverter( RGB565,   BGR565, (CONVERTER::FUNCTION)Convert16_BGR565_X86,   (CONVERTER::AREAFUNCTION)AreaConvert16_BGR565_X86,   NULL );
    SetupConverter( RGB565, ARGB1555, (CONVERTER::FUNCTION)Convert16_ARGB1555_X86, (CONVERTER::AREAFUNCTION)AreaConvert16_ARGB1555_X86, NULL );
    SetupConverter( RGB565, ABGR1555, (CONVERTER::FUNCTION)Convert16_ABGR1555_X86, (CONVERTER::AREAFUNCTION)AreaConvert16_ABGR1555_X86, NULL );
    SetupConverter( RGB565,    GREY8, (CONVERTER::FUNCTION)Convert16_GREY8_X86,    (CONVERTER::AREAFUNCTION)AreaConvert16_GREY8_X86,    NULL );
    SetupConverter( RGB565,   RGB332, (CONVERTER::FUNCTION)Convert16_RGB332_X86,   (CONVERTER::AREAFUNCTION)AreaConvert16_RGB332_X86,   NULL );
    #ifndef __CYGNUS__
    GenerateTable16_X86(Convert16_ARGB8888_LUT_X86,FORMAT(ARGB8888));
    GenerateTable16_X86(Convert16_ABGR8888_LUT_X86,FORMAT(ABGR8888));
    GenerateTable16_X86(Convert16_RGBA8888_LUT_X86,FORMAT(RGBA8888));
    GenerateTable16_X86(Convert16_BGRA8888_LUT_X86,FORMAT(BGRA8888));
    #endif
    #endif

    // 8bit -> X
    #ifdef __8BIT__                   
    StaticConverter8_4BYTE->ConvertFunction=(CONVERTER::FUNCTION)Convert8_4BYTE_X86;
    StaticConverter8_3BYTE->ConvertFunction=(CONVERTER::FUNCTION)Convert8_3BYTE_X86;
    StaticConverter8_2BYTE->ConvertFunction=(CONVERTER::FUNCTION)Convert8_2BYTE_X86;
    StaticConverter8_1BYTE->ConvertFunction=(CONVERTER::FUNCTION)Convert8_1BYTE_X86;
    StaticConverter8_4BYTE->AreaConvertFunction=(CONVERTER::AREAFUNCTION)AreaConvert8_4BYTE_X86;
    StaticConverter8_3BYTE->AreaConvertFunction=(CONVERTER::AREAFUNCTION)AreaConvert8_3BYTE_X86;
    StaticConverter8_2BYTE->AreaConvertFunction=(CONVERTER::AREAFUNCTION)AreaConvert8_2BYTE_X86;
    StaticConverter8_1BYTE->AreaConvertFunction=(CONVERTER::AREAFUNCTION)AreaConvert8_1BYTE_X86;
    SetupConverter( INDEX8, ARGB8888, (CONVERTER::FUNCTION)Convert8_4BYTE_X86, (CONVERTER::AREAFUNCTION)AreaConvert8_4BYTE_X86, NULL );
    SetupConverter( INDEX8, ABGR8888, (CONVERTER::FUNCTION)Convert8_4BYTE_X86, (CONVERTER::AREAFUNCTION)AreaConvert8_4BYTE_X86, NULL );
    SetupConverter( INDEX8, RGBA8888, (CONVERTER::FUNCTION)Convert8_4BYTE_X86, (CONVERTER::AREAFUNCTION)AreaConvert8_4BYTE_X86, NULL );
    SetupConverter( INDEX8, BGRA8888, (CONVERTER::FUNCTION)Convert8_4BYTE_X86, (CONVERTER::AREAFUNCTION)AreaConvert8_4BYTE_X86, NULL );
    SetupConverter( INDEX8,   RGB888, (CONVERTER::FUNCTION)Convert8_3BYTE_X86, (CONVERTER::AREAFUNCTION)AreaConvert8_3BYTE_X86, NULL );
    SetupConverter( INDEX8,   BGR888, (CONVERTER::FUNCTION)Convert8_3BYTE_X86, (CONVERTER::AREAFUNCTION)AreaConvert8_3BYTE_X86, NULL );
    SetupConverter( INDEX8,   RGB565, (CONVERTER::FUNCTION)Convert8_2BYTE_X86, (CONVERTER::AREAFUNCTION)AreaConvert8_2BYTE_X86, NULL );
    SetupConverter( INDEX8,   BGR565, (CONVERTER::FUNCTION)Convert8_2BYTE_X86, (CONVERTER::AREAFUNCTION)AreaConvert8_2BYTE_X86, NULL );
    SetupConverter( INDEX8, ARGB1555, (CONVERTER::FUNCTION)Convert8_2BYTE_X86, (CONVERTER::AREAFUNCTION)AreaConvert8_2BYTE_X86, NULL );
    SetupConverter( INDEX8, ABGR1555, (CONVERTER::FUNCTION)Convert8_2BYTE_X86, (CONVERTER::AREAFUNCTION)AreaConvert8_2BYTE_X86, NULL );
    SetupConverter( INDEX8,    GREY8, (CONVERTER::FUNCTION)Convert8_1BYTE_X86, (CONVERTER::AREAFUNCTION)AreaConvert8_1BYTE_X86, NULL );
    SetupConverter( INDEX8,   RGB332, (CONVERTER::FUNCTION)Convert8_1BYTE_X86, (CONVERTER::AREAFUNCTION)AreaConvert8_1BYTE_X86, NULL );
    SetupConverter( INDEX8,   INDEX8, (CONVERTER::FUNCTION)ConvertCopy_X86,    (CONVERTER::AREAFUNCTION)AreaConvertCopy_X86,    NULL );
    #endif

    // YCbCrA -> X
    #ifdef __YXX__
    GenerateTableYCbCr32_X86();
    GenerateTableYCbCr16_X86();
    #endif

    // X -> FAKEMODE
    #ifdef __FAKEMODE__
    SetupConverter( ARGB8888, FAKEMODE1A, (CONVERTER::FUNCTION)Convert32_FAKEMODE1A_X86, NULL, NULL );
    SetupConverter( ARGB8888, FAKEMODE1B, (CONVERTER::FUNCTION)Convert32_FAKEMODE1B_X86, NULL, NULL );
    SetupConverter( ARGB8888, FAKEMODE1C, (CONVERTER::FUNCTION)Convert32_FAKEMODE1C_X86, NULL, NULL );
    SetupConverter( ARGB8888, FAKEMODE2A, (CONVERTER::FUNCTION)Convert32_FAKEMODE2A_X86, NULL, NULL );
    SetupConverter( ARGB8888, FAKEMODE2B, (CONVERTER::FUNCTION)Convert32_FAKEMODE2B_X86, NULL, NULL );
    SetupConverter( ARGB8888, FAKEMODE2C, (CONVERTER::FUNCTION)Convert32_FAKEMODE2C_X86, NULL, NULL );
    SetupConverter( ARGB8888, FAKEMODE3A, (CONVERTER::FUNCTION)Convert32_FAKEMODE3A_X86, NULL, NULL );
    SetupConverter( ARGB8888, FAKEMODE3B, (CONVERTER::FUNCTION)Convert32_FAKEMODE3B_X86, NULL, NULL );
    SetupConverter( ARGB8888, FAKEMODE3C, (CONVERTER::FUNCTION)Convert32_FAKEMODE3C_X86, NULL, NULL );
    SetupConverter(   RGB565, FAKEMODE1A, (CONVERTER::FUNCTION)Convert16_FAKEMODE1A_X86, NULL, NULL );
    SetupConverter(   RGB565, FAKEMODE1B, (CONVERTER::FUNCTION)Convert16_FAKEMODE1B_X86, NULL, NULL );
    SetupConverter(   RGB565, FAKEMODE1C, (CONVERTER::FUNCTION)Convert16_FAKEMODE1C_X86, NULL, NULL );
    SetupConverter(   RGB565, FAKEMODE2A, (CONVERTER::FUNCTION)Convert16_FAKEMODE2A_X86, NULL, NULL );
    SetupConverter(   RGB565, FAKEMODE2B, (CONVERTER::FUNCTION)Convert16_FAKEMODE2B_X86, NULL, NULL );
    SetupConverter(   RGB565, FAKEMODE2C, (CONVERTER::FUNCTION)Convert16_FAKEMODE2C_X86, NULL, NULL );
    SetupConverter(   RGB565, FAKEMODE3A, (CONVERTER::FUNCTION)Convert16_FAKEMODE3A_X86, NULL, NULL );
    SetupConverter(   RGB565, FAKEMODE3B, (CONVERTER::FUNCTION)Convert16_FAKEMODE3B_X86, NULL, NULL );
    SetupConverter(   RGB565, FAKEMODE3C, (CONVERTER::FUNCTION)Convert16_FAKEMODE3C_X86, NULL, NULL );
    SetupConverter(   INDEX8, FAKEMODE1A, (CONVERTER::FUNCTION)Convert8_FAKEMODE1A_X86,  NULL, NULL );
    SetupConverter(   INDEX8, FAKEMODE1B, (CONVERTER::FUNCTION)Convert8_FAKEMODE1B_X86,  NULL, NULL );
    SetupConverter(   INDEX8, FAKEMODE1C, (CONVERTER::FUNCTION)Convert8_FAKEMODE1C_X86,  NULL, NULL );
    SetupConverter(   INDEX8, FAKEMODE2A, (CONVERTER::FUNCTION)Convert8_FAKEMODE2A_X86,  NULL, NULL );
    SetupConverter(   INDEX8, FAKEMODE2B, (CONVERTER::FUNCTION)Convert8_FAKEMODE2B_X86,  NULL, NULL );
    SetupConverter(   INDEX8, FAKEMODE2C, (CONVERTER::FUNCTION)Convert8_FAKEMODE2C_X86,  NULL, NULL );
    SetupConverter(   INDEX8, FAKEMODE3A, (CONVERTER::FUNCTION)Convert8_FAKEMODE3A_X86,  NULL, NULL );
    SetupConverter(   INDEX8, FAKEMODE3B, (CONVERTER::FUNCTION)Convert8_FAKEMODE3B_X86,  NULL, NULL );
    SetupConverter(   INDEX8, FAKEMODE3C, (CONVERTER::FUNCTION)Convert8_FAKEMODE3C_X86,  NULL, NULL );
    #endif

    // done
    return 1;

#else

    // no x86!
    return 0;

#endif
}


int RASTER::DefaultConverters_MMX()
{
    // not yet
    return 0;
}
    
    





int RASTER::SetupConverter(int src_id,int dest_id,CONVERTER::FUNCTION convert,CONVERTER::AREAFUNCTION area,CONVERTER::PARTIALFUNCTION partial)
{
    // get converter via id
    CONVERTER *converter=NULL;
    if (src_id==ARGB8888) converter=&StaticConverter32[dest_id-FORMATBASE];
    if (src_id==RGB565)   converter=&StaticConverter16[dest_id-FORMATBASE];
    if (src_id==INDEX8)   converter=&StaticConverter8 [dest_id-FORMATBASE];
    
    // check handle
    if (!converter) return 0;

    // setup converter functions
    converter->SetupFunctions(convert,area,partial);
    
    // done
    return 1;
}







int RASTER::GenerateTable16_X86(void *dest,FORMAT const &format)
{
    // 16bit->32bit tables
    if (format.bits==32)
    {
        // table (512 uints)
        uint *table=(uint*)dest;

        // interleaved tables
        for (int i=0; i<256; i++)
        {
            // lower byte
            float r = (float)0;
            float g = (float)((i&0xE0)>>5) * (float)(255.0 / 63.0);
            float b = (float)(i&0x1F) * (float)(255.0 / 31.0);
            float a = (float)0;
            format.pack((uchar)r,(uchar)g,(uchar)b,(uchar)a,&table[i*2]);

            // upper byte
            r = (float)((i&0xF8)>>3) * (float)(255.0 / 31.0);
            g = (float)((i&0x07)<<3) * (float)(255.0 / 63.0);
            b = (float)0;
            a = (float)0;
            format.pack((uchar)r,(uchar)g,(uchar)b,(uchar)a,&table[i*2+1]);
        }
    }

    // failure
    return 0;
}







#if defined(__SMC__) && defined(__X86__) && defined(__WIN32__)

int RASTER::GenerateConverter(Code &code,FORMAT const &src,FORMAT const &dest)
{
    // check pixel formats
    if (src.bytes!=4 || src.model!=dest.model || src==dest) return 0;
    
    // calculate component shifts (shr)
    int c1_shift=(signed int)src.c1.position-(signed int)dest.c1.position + (src.c1.size-dest.c1.size);
    int c2_shift=(signed int)src.c2.position-(signed int)dest.c2.position + (src.c2.size-dest.c2.size);
    int c3_shift=(signed int)src.c3.position-(signed int)dest.c3.position + (src.c3.size-dest.c3.size);
    int c4_shift=(signed int)src.c4.position-(signed int)dest.c4.position + (src.c4.size-dest.c4.size);
                 
    // calculate component masks
    unsigned c1_mask=dest.c1.mask;
    unsigned c2_mask=dest.c2.mask;
    unsigned c3_mask=dest.c3.mask;
    unsigned c4_mask=dest.c4.mask;

    // loop initialization
    code.push(EBP);
    code.move(EBP,ECX);

    // get code label
    void *label=code.current();

    // eax = c1
    code.load(EAX,src.bytes);
    code.shift(EAX,c1_shift);
    code.and(EAX,c1_mask);

    // ebx = c2
    code.load(EBX,src.bytes);
    code.shift(EBX,c2_shift);
    code.and(EBX,c2_mask);

    // ecx = c3
    code.load(ECX,src.bytes);
    code.shift(ECX,c3_shift);
    code.and(ECX,c3_mask);

    // edx = c4
    code.load(EDX,src.bytes);
    code.and(EDX,c4_mask);
    code.shift(EDX,c4_mask);
                   
    // sum components
    code.or(EAX,EBX);
    code.or(ECX,EDX);
    code.or(EAX,ECX);

    // store eax
    code.store(EAX,dest.bytes);

    // next pixel
    code.add(ESI,src.bytes);
    code.add(EDI,dest.bytes);
    
    // counter
    code.dec(EBP);
    code.jnz(label);
    
    // cleanup
    code.pop(EBP);
    code.ret();

    // success
    return 1;
}

#endif






RASTER::CONVERTER::CONVERTER()
{
    // defaults
    Extra=0;
    Copy=0;
    ConvertFunction=NULL;
    AreaConvertFunction=NULL;
    PartialConvertFunction=NULL;
}


RASTER::CONVERTER::~CONVERTER()
{
    // cleanup
    if (Extra);
}







void RASTER::CONVERTER::SetupFormats(FORMAT const &src,FORMAT const &dest)
{
    // setup formats
    SourceFormat=src;
    DestinationFormat=dest;
    if (SourceFormat==DestinationFormat) Copy=1;
    if (SourceFormat.type==INDEXED && DestinationFormat.type!=INDEXED) Extra=1;
}


void RASTER::CONVERTER::SetupFunctions(CONVERTER::FUNCTION convert,CONVERTER::AREAFUNCTION area,CONVERTER::PARTIALFUNCTION partial)
{
    // setup functions
    ConvertFunction=convert;
    AreaConvertFunction=area;
    PartialConvertFunction=partial;
}







int RASTER::CONVERTER::Convert(void *src,void *dest,uint pixels,void *extra)
{
    // copy converter needs bytes not pixels
    if (Copy) pixels*=SourceFormat.bytes;

    // check extra if it is needed
    if (Extra && !extra) return 0;
    
    // perform the conversion
    return InternalConvert(src,dest,pixels,extra);
}


int RASTER::CONVERTER::Convert(RASTER::CONVERTER::AREA data,void *extra)
{
    // copy convert needs bytes not pixels
    if (Copy) data.width*=SourceFormat.bytes;
                       
    // check that if converter needs extra its been given
    if (Extra && !extra) return 0;

    // perform area conversion
    return InternalConvert(data,extra);
}


int RASTER::CONVERTER::Convert(void *src,void *dest,uint start_byte,uint stop_byte,void *extra)
{
    // use parameters to advoid warnings
    if (src || dest || start_byte || stop_byte || extra);
    return 0;
}







// watcom c++ wrapper to setup registers for convert function pointer
#if defined(__WATCOMC__) && defined(__X86__)
extern void InternalConvert_WATCOMC(void *function,void *src,void *dest,uint pixels,void *extra);
#pragma aux InternalConvert_WATCOMC =       \
            "call eax"                      \
            parm [eax][esi][edi][ecx][ebx]  \
            modify [edx];
#endif


int RASTER::CONVERTER::InternalConvert(void *src,void *dest,uint pixels,void *extra)
{
    if (ConvertFunction)
    {
        #if defined(__WATCOMC__) && defined(__X86__)

        // setup register parms and call converter function (Watcom C++)
        InternalConvert_WATCOMC((void*)ConvertFunction,src,dest,pixels,extra);
        return 1;

        #elif defined(__VISUALC__) && defined(__X86__)
                     
        // setup register parms and call converter function (Visual C++)
        void *function=ConvertFunction;
        __asm
        {
            mov esi,src
            mov edi,dest
            mov ecx,pixels
            mov ebx,extra
            mov eax,function;
            call eax
        }
        return 1;

        #elif defined(__GNU__) && defined (__X86__)

        // setup register parms and call converter function (GNU C++)
        __asm__ __volatile__("call *%%eax" : : "a"((void*)ConvertFunction), "S"(src), "D"(dest), "c"(pixels), "b"(extra) : "edx", "memory", "cc");
        return 1;

        #elif !defined(__X86__)

        // normal C++ call
        (*ConvertFunction)(src,dest,pixels,extra);    
        return 1;

        #else

        // nothing
        return 0;

        #endif
    }
    else
    {
        // generic conversion - direct to direct only (slow!)
        if (SourceFormat.type==DIRECT && DestinationFormat.type==DIRECT && !Copy)
        {
            uchar *p=(uchar*)src;
            uchar *q=(uchar*)dest;
            while (pixels--)
            {
                uchar c1,c2,c3,c4;
                SourceFormat.unpack(p,c1,c2,c3,c4);
                DestinationFormat.pack(c1,c2,c3,c4,q);
                p+=SourceFormat.bytes;
                q+=DestinationFormat.bytes;
            }
            return 1;
        }
        else return 0;
    }
}


// watcom c++ wrapper to setup registers for AreaConvert function pointer
#if defined(__WATCOMC__) && defined (__X86__)
extern void InternalAreaConvert_WATCOMC(void *function,RASTER::CONVERTER::AREA *data,void *extra);
#pragma aux InternalAreaConvert_WATCOMC =       \
            "call eax"                          \
            parm [eax][esi][ebx]                \
            modify [ecx edx edi];
#endif


int RASTER::CONVERTER::InternalConvert(CONVERTER::AREA &data,void *extra)
{
    if (AreaConvertFunction)
    {
        #if defined(__WATCOMC__) && defined(__X86__)

        // setup register parms and call converter function (Watcom C++)
        InternalAreaConvert_WATCOMC((void*)AreaConvertFunction,&data,extra);
        return 1;

        #elif defined(__VISUALC__) && defined(__X86__)

        // setup register parms and call converter function (Visual C++)
        void *function=(void*)AreaConvertFunction;
        CONVERTER::AREA *area_data=&data;
        __asm
        {
            mov esi,area_data
            mov ebx,extra
            mov eax,function;
            call eax
        }
        return 1;

        #elif defined(__GNU__) && defined (__X86__)

        // setup register parms and call converter function (GNU C++) --- thanks to malc for this one!
        __asm__ __volatile__("call *%%eax" : : "a"((void*)AreaConvertFunction), "S"(&data), "b"(extra) : "ecx","edx","edi", "memory", "cc");
        return 1;

        #elif !defined(__X86__)

        // normal C++ call
        (*AreaConvertFunction)(&data,extra);    
        return 1;

        #else
        
        // nothing
        return 0;

        #endif
    }
    else
    {
        // manual area convert
        
        // no fakemode area converters
        if (DestinationFormat.type==FAKEMODE) return 0;

        // index pointers
        char *p=(char*)data.src_start;
        char *q=(char*)data.dest_start;

        // work through one line at a time
        for (int y=0; y<data.height; y++)
        {
            // perform one line of conversion
            InternalConvert(p,q,data.width,extra);

            // move to next line
            p+=data.src_pitch;
            q+=data.dest_pitch;
        }

        // done
        return 1;
    }
}
