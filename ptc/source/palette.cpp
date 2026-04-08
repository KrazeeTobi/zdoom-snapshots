///////////////////
// palette class //
///////////////////
#include "palette.h"







Palette::Palette()
{
    // zero attach count
    AttachCount=0;

    // zero lock count
    DataLockCount=0;

    // alloc palette data
    Data=(uint*)malloc(1024);

    // init black
    if (Data) memset(Data,0,1024);          // check!
}


Palette::Palette(Palette const &other)
{
    // zero attach count
    AttachCount=0;

    // zero lock count
    DataLockCount=0;

    // alloc and copy palette data
    Data=(uint*)malloc(1024);
    if (Data && other.Data) memcpy(Data,other.Data,1024);

    // note: no table list copy
}


Palette::Palette(char filename[])
{
    // zero attach count
    AttachCount=0;

    // zero lock count
    DataLockCount=0;

    // allocate
    Data=(uint*)malloc(1024);

    // load
    Load(filename);
}


Palette::~Palette()
{
    // free palette data
    free(Data);

    // close index table list
    ClearIndexTableList();
}




        
int Palette::Load(char filename[])
{
    // load palette data from file
    /*
    // fail on invalid data
    if (!Data) return 0;

    // read from file
    File file;
    if (!file.open(filename,"rb")) return 0;
    if (!file.read(Data,768)) return 0;
    file.close();

    // update index tables
    UpdateIndexTables();
    return 1;
    */

    // advoid warnings
    if (filename[0]);
    return 0;
}


int Palette::Save(char filename[])
{
    // write palette data to file
    /*
    // fail on invalid data
    if (!Data) return 0;
      
    // write to file
    File file;
    if (!file.open(filename,"w+b")) return 0;
    if (!file.write(Data,768)) return 0;
    file.close();
    return 1;
    */

    // advoid warnings
    if (filename[0]);
    return 0;
}







void* Palette::ReadOnly() const
{
    // read only lock
    return Data;
}


void* Palette::Lock()
{
    // increment lock count
    if (Data) DataLockCount++;
    
    // clear index tables
    ClearIndexTableList();

    // return palette data pointer
    return Data;
}


void Palette::Unlock()
{
    // decrement lock count
    if (DataLockCount) DataLockCount--;
}
        

int Palette::LockCount() const
{
    // return lock count
    return DataLockCount;
}
        
        
        
        



int Palette::Set(void *data)
{
    // check user data
    if (!data) return 0;
    
    // lock palette data
    void *buffer=Lock();
    if (!buffer) return 0;

    // set new data
    memcpy(buffer,data,1024);
    
    // unlock
    Unlock();
    return 1;
}


int Palette::Get(void *data) const
{
    // get palette data
    if (!data || !Data) return 0;
    else
    {
        // copy to user data
        memcpy(data,Data,1024);
        return 1;
    }
}






int Palette::ok() const
{
    // palette state
    if (!Data) return 0;
    else return 1;
}







int Palette::operator ==(Palette const &other) const
{
    // fail on invalid data
    if (!Data || !other.Data) return 0;

    // compare palette data
    if (!memcmp(Data,other.Data,1024)) return 1;
    else return 0;
}


void Palette::operator =(Palette const &other)
{
    // fail on invalid data
    if (!Data || !other.Data) return;           // should i uninitialize the palette?

    // copy palette data
    memcpy(Data,other.Data,1024);

    // free index table list
    ClearIndexTableList();
}


        
 

       
void* Palette::GetIndexTable(FORMAT const &format,int alignment)
{
    // quick check - if looking for ARGB8888 align 4, return straight palette data
    if (format.id==ARGB8888 && alignment==4) return Data;

    // otherwise search for matching index table
    List<INDEXTABLE>::Iterator iterator=IndexTableList.first();
    INDEXTABLE *table=iterator.current();
    while (table)
    {              
        // check for a match
        if (table->Format==format && table->Alignment==alignment) return table->Data; 

        // no match - try next index table in list...
        table=iterator.next();       
    }

    // couldnt find table in list, create it
    return CreateIndexTable(format,alignment);
}


void Palette::ClearIndexTableList()
{
    // free all the table->Data in list
    List<INDEXTABLE>::Iterator iterator=IndexTableList.first();
    INDEXTABLE *table=iterator.current();
    while (table)
    {
        free(table->Data);
        table=iterator.next();
    }

    // free all list items
    IndexTableList.free();
}


void* Palette::CreateIndexTable(FORMAT const &format,int alignment)
{
    // fail on invalid data
    if (!Data) return 0;

    // setup alignment from DEFAULT
    if (alignment==DEFAULT)
    {
        if (format.bytes==1) alignment=1;
        else if (format.bytes<=4) alignment=4;
        else alignment=format.bytes;
    }

    // check if format bytes fits into desired align
    if (format.bytes>alignment) return NULL;

    // alloc new table
    INDEXTABLE *newtable=new INDEXTABLE;
    if (!newtable) return 0;

    // alloc new table data
    newtable->Data=malloc(alignment*256);
    if (!newtable->Data)
    {    
        delete newtable;
        return NULL;
    }

    // clear table data
    memset(newtable->Data,0,256*alignment);

    // create table (direct color modes)
    if (format.type==DIRECT)
    {                                                    // CHECK COLOR MODEL
        uint *p=Data;
        uchar *q=(uchar*)newtable->Data;
        for (int i=0; i<256; i++)
        {
            // unpack palette ARGB8888 data
            uchar r,g,b,a;
            UnpackRGBA32(*p,r,g,b,a);

            // pack pixels into desired format
            format.pack(r,g,b,a,q);

            // move to next index
            q+=alignment;
            p++;
        }
    }
    else if (format.type==FAKEMODE)
    {
        if (format.id==FAKEMODE2A || format.id==FAKEMODE2B || format.id==FAKEMODE2C)
        {
            // FAKEMODE2x

            // fail on invalid alignment
            if (alignment!=4)
            {
                free(newtable->Data);
                delete newtable;
                return NULL;
            }

            uint *p=Data;
            uint *pmax=p+256;
            uint *q=(uint*)newtable->Data;
            while (p<pmax)
            {
                // unpack palette ARGB8888 data
                uchar r,g,b;
                UnpackRGB32(*p,r,g,b);

                // shift down to 6bits per component and setup
                r = (uchar)((r&0xF0)>>1);
                g = (uchar)((g>>3)|0x80);
                b = (uchar)(b>>5);

                // pack pixels into desired format
                *q=RGB32(r,g,b);

                // next index
                p++; 
                q++;
            }
        }
        else if (format.id==FAKEMODE1A || format.id==FAKEMODE1B || format.id==FAKEMODE1C ||
                 format.id==FAKEMODE3A || format.id==FAKEMODE3B || format.id==FAKEMODE3C)
        {
            // FAKEMODE1x/FAKEMODE3x
            
            // fail on invalid alignment
            if (alignment!=4)
            {
                free(newtable->Data);
                delete newtable;
                return NULL;
            }

            uint *p=Data;
            uint *pmax=p+256;
            uint *q=(uint*)newtable->Data;
            while (p<pmax)
            {
                // unpack palette ARGB8888 data
                uchar r,g,b;
                UnpackRGB32(*p,r,g,b);

                // shift down to 6bits per component and setup
                r=(uchar)(r>>2);
                g=(uchar)((g>>2)+0x40);
                b=(uchar)((b>>2)+0x80);

                // pack pixels into desired format
                *q=RGB32(r,g,b);

                // next index
                p++; 
                q++;
            }
        }
    }
    else if (format.id==GREY8)
    {
        // 8bit GREYSCALE
        uint *p=Data;
        uchar *q=(uchar*)newtable->Data;
        for (int i=0; i<256; i++)
        {
            // unpack palette ARGB8888 data
            uchar r,g,b;
            UnpackRGB32(*p,r,g,b);

            // adjust colors
            float dr=(float)r/(float)255.0 * (float)0.33333;//0.30;
            float dg=(float)g/(float)255.0 * (float)0.33333;//0.59;
            float db=(float)b/(float)255.0 * (float)0.33333;//0.11;

            // combine
            float intensity=(dr+dg+db);
            *q=(uchar)(intensity*255.0);
            
            q+=alignment;
            p++;
        }
    }
    else if (format.id==RGB332)
    {
        uint *p=Data;
        uchar *q=(uchar*)newtable->Data;
        for (int i=0; i<256; i++)
        {
            // unpack palette ARGB8888 data
            uchar r,g,b;
            UnpackRGB32(*p,r,g,b);

            // pack into RGB332
            r=(uchar)(r&0xE0);
            g=(uchar)((g&0xE0)>>3);
            b=(uchar)((b&0xC0)>>6);
            *q=(uchar)(r+g+b);

            q+=alignment;
            p++;
        }
    }

    // setup table info
    newtable->Format=format;
    newtable->Alignment=alignment;

    // add to list
    if (!IndexTableList.add(newtable))
    {
        // failure
        free(newtable->Data);
        delete newtable;
        return NULL;
    }

    // created ok: return it
    return newtable->Data;
}






int Palette::Attach()
{
    // increment attach count
    AttachCount++;
    return AttachCount;
}


int Palette::Detach()
{
    // decrement attach count
    AttachCount--;
    if (AttachCount<0) AttachCount=0;
    return AttachCount;
}
