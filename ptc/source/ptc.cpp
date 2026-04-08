//////////////////////////
// prometheus truecolor //
//////////////////////////
#include "ptc.h"
#include "idummy.h"
#include "ivga.h"
#include "ivesa.h"
#include "igdi.h"
#include "idirectx.h"
#include "isvgalib.h"
#include "iamiga.h"



        



// dummy interface static
IDummy PTC::DummyInterface;






PTC::PTC()
{   
    // defaults
    Defaults();
}


PTC::PTC(MODE const &mode)
{
    // defaults
    Defaults();

    // initialize
    Init((WINDOW)NULL,mode);
}


PTC::PTC(int x,int y,int id,int output,int frequency,int layout)
{
    // defaults
    Defaults();

    // initialize
    Init((WINDOW)NULL,x,y,id,output,frequency,layout);
}


PTC::PTC(int x,int y,FORMAT format,int output,int frequency,int layout)
{
    // defaults
    Defaults();

    // initialize
    Init((WINDOW)NULL,x,y,format,output,frequency,layout);
}
        

PTC::PTC(WINDOW const &window)
{
    // defaults
    Defaults();

    // initialize
    Init(window);
}
        

PTC::PTC(WINDOW const &window,MODE const &mode)
{
    // defaults
    Defaults();

    // initialize
    Init(window,mode);
}


PTC::PTC(WINDOW const &window,int x,int y,int id,int output,int frequency,int layout)
{
    // defaults
    Defaults();

    // initialize
    Init(window,x,y,id,output,frequency,layout);
}
        

PTC::PTC(WINDOW const &window,int x,int y,FORMAT format,int output,int frequency,int layout)
{
    // defaults
    Defaults();

    // initialize
    Init(window,x,y,format,output,frequency,layout);
}





PTC::PTC(char name[])
{
    // defaults
    Defaults();

    // initialize
    Init(name);
}


PTC::PTC(char name[],int x,int y,int id,int output,int frequency,int layout)
{
    // defaults
    Defaults();

    // initialize
    Init(name,(WINDOW)NULL,x,y,id,output,frequency,layout);
}


PTC::PTC(char name[],int x,int y,FORMAT format,int output,int frequency,int layout)
{
    // defaults
    Defaults();

    // initialize
    Init(name,(WINDOW)NULL,x,y,format,output,frequency,layout);
}


PTC::PTC(char name[],WINDOW const &window)
{
    // defaults
    Defaults();

    // initialize
    Init(name,window);
}


PTC::PTC(char name[],WINDOW const &window,int x,int y,int id,int output,int frequency,int layout)
{
    // defaults
    Defaults();

    // initialize
    Init(name,window,x,y,id,output,frequency,layout);
}
        

PTC::PTC(char name[],WINDOW const &window,int x,int y,FORMAT format,int output,int frequency,int layout)
{
    // defaults
    Defaults();

    // initialize
    Init(name,window,x,y,format,output,frequency,layout);
}





PTC::PTC(int argc,char *argv[],int output,int frequency,int layout)
{
    // defaults
    Defaults();

    // initialize
    Init(argc,argv,output,frequency,layout);
}


PTC::PTC(int x,int y,int argc,char *argv[],int output,int frequency,int layout)
{
    // defaults
    Defaults();

    // initialize
    Init(x,y,argc,argv,output,frequency,layout);
}





PTC::~PTC()
{   
    // close
    Close();
}









int PTC::Init(MODE const &mode)
{
    // initialize
    return Init((WINDOW)NULL,mode);
}


int PTC::Init(int x,int y,int id,int output,int frequency,int layout)
{
    // initialize
    return Init((WINDOW)NULL,x,y,id,output,frequency,layout);
}


int PTC::Init(int x,int y,FORMAT format,int output,int frequency,int layout)
{
    // initialize
    return Init((WINDOW)NULL,x,y,format,output,frequency,layout);
}


int PTC::Init(WINDOW const &window)
{
    // close
    Close();

    // create interface
    int index=0;
    LocalInterface=CreateInterface(index,window);
    while (LocalInterface)
    {
        // initialize interface
        if (LocalInterface->ok()) return 1;

        // next interface
        delete LocalInterface;
        LocalInterface=CreateInterface(++index,window);
    }

    // failure
    Defaults();
    return 0;
}


int PTC::Init(WINDOW const &window,MODE const &mode)
{
    // close
    Close();

    // create interface
    int index=0;
    LocalInterface=CreateInterface(index,window);
    while (LocalInterface)
    {
        // initialize interface
        if (LocalInterface->ok() && LocalInterface->SetMode(mode)) return 1;

        // next interface
        delete LocalInterface;
        LocalInterface=CreateInterface(++index,window);
    }

    // failure
    Defaults();
    return 0;
}


int PTC::Init(WINDOW const &window,int x,int y,int id,int output,int frequency,int layout)
{
    // close
    Close();

    // create interface
    int index=0;
    LocalInterface=CreateInterface(index,window);
    while (LocalInterface)
    {
        // initialize interface
        if (LocalInterface->ok() && LocalInterface->SetMode(x,y,id,output,frequency,layout)) return 1;

        // next interface
        delete LocalInterface;
        LocalInterface=CreateInterface(++index,window);
    }

    // failure
    Defaults();
    return 0;
}


int PTC::Init(WINDOW const &window,int x,int y,FORMAT format,int output,int frequency,int layout)
{
    // close
    Close();
    
    // create interface
    int index=0;
    LocalInterface=CreateInterface(index,window);
    while (LocalInterface)
    {
        // initialize interface
        if (LocalInterface->ok() && LocalInterface->SetMode(x,y,format,output,frequency,layout)) return 1;

        // next interface
        delete LocalInterface;
        LocalInterface=CreateInterface(++index,window);
    }

    // failure
    Defaults();
    return 0;
}







int PTC::Init(char name[])
{
    // initialize
    return Init(name,(WINDOW)NULL);
}


int PTC::Init(char name[],int x,int y,int id,int output,int frequency,int layout)
{
    // initialize
    return Init(name,(WINDOW)NULL,x,y,id,output,frequency,layout);
}


int PTC::Init(char name[],int x,int y,FORMAT format,int output,int frequency,int layout)
{
    // initialize
    return Init(name,(WINDOW)NULL,x,y,format,output,frequency,layout);
}


int PTC::Init(char name[],WINDOW const &window)
{
    // close
    Close();

    // create interface
    int index=0;
    LocalInterface=CreateInterface(index,window);
    while (LocalInterface)
    {
        // initialize interface
        if (LocalInterface->ok())
        {
            char local[32];
            LocalInterface->GetName(local);
            if (stricmp(name,local)==0) return 1;
        }

        // next interface
        delete LocalInterface;
        LocalInterface=CreateInterface(++index,window);
    }

    // failure
    Defaults();
    return 0;
}


int PTC::Init(char name[],WINDOW const &window,int x,int y,int id,int output,int frequency,int layout)
{
    // close
    Close();

    // create interface
    int index=0;
    LocalInterface=CreateInterface(index,window);
    while (LocalInterface)
    {
        // initialize interface
        if (LocalInterface->ok())
        {
            char local[32];
            LocalInterface->GetName(local);
            if (stricmp(name,local)==0 && LocalInterface->SetMode(x,y,id,output,frequency,layout)) return 1;
        }

        // next interface
        delete LocalInterface;
        LocalInterface=CreateInterface(++index,window);
    }

    // failure
    Defaults();
    return 0;
}


int PTC::Init(char name[],WINDOW const &window,int x,int y,FORMAT format,int output,int frequency,int layout)
{
    // close
    Close();
    
    // create interface
    int index=0;
    LocalInterface=CreateInterface(index,window);
    while (LocalInterface)
    {
        // initialize interface
        if (LocalInterface->ok())
        {
            char local[32];
            LocalInterface->GetName(local);
            if (stricmp(name,local)==0 && LocalInterface->SetMode(x,y,format,output,frequency,layout)) return 1;
        }

        // next interface
        delete LocalInterface;
        LocalInterface=CreateInterface(++index,window);
    }

    // failure
    Defaults();
    return 0;
}







int PTC::Init(int argc,char *argv[],int output,int frequency,int layout)
{
    // close
    Close();

    // command line init - all parms from command line (x,y,bits/id,output,frequency,layout)
    if (argc>3)
    {
        // parm1 = xres
        int x=atoi(argv[1]);
        
        // parm2 = yres
        int y=atoi(argv[2]);

        // initialize
        return Init(x,y,2,&argv[2],output,frequency,layout);
    }

    // failure
    return 0;
}


int PTC::Init(int x,int y,int argc,char *argv[],int output,int frequency,int layout)
{
    // close
    Close();

    // command line init - only get format, rest is specified
    if (argc>1)
    {
        // bits per pixel
        int bits=atoi(argv[1]);
        if (Init(x,y,bits,output,frequency,layout)) return 1;

        // format id
        FORMAT format(argv[1]);
        if (format.ok() && Init(x,y,format,output,frequency,layout)) return 1;

        // format type
        else if (stricmp(argv[1],"DIRECT")==0)     return Init(x,y,DIRECT,output,frequency,layout);
        else if (stricmp(argv[1],"INDEXED")==0)    return Init(x,y,INDEXED,output,frequency,layout);
        else if (stricmp(argv[1],"FAKEMODE")==0)   return Init(x,y,FAKEMODE,output,frequency,layout);

        // fuzzy modes (obsolete)
        else if (stricmp(argv[1],"FUZZY8")==0)     return Init(x,y,FUZZY8,output,frequency,layout);
        else if (stricmp(argv[1],"FUZZY16")==0)    return Init(x,y,FUZZY16,output,frequency,layout);
        else if (stricmp(argv[1],"FUZZY32")==0)    return Init(x,y,FUZZY32,output,frequency,layout);

        // virtual modes
        else if (stricmp(argv[1],"VIRTUAL8")==0)   return Init(x,y,VIRTUAL8,output,frequency,layout);
        else if (stricmp(argv[1],"VIRTUAL16")==0)  return Init(x,y,VIRTUAL16,output,frequency,layout);
        else if (stricmp(argv[1],"VIRTUAL32")==0)  return Init(x,y,VIRTUAL32,output,frequency,layout);
    }

    // failure
    return 0;
}
                                                       





void PTC::Error(char error[])
{
#ifdef __WIN32__

    // messagebox error
    Close();
    MessageBox(NULL,error,"ptc error",MB_OK | MB_ICONERROR | MB_SETFOREGROUND | MB_TOPMOST);

#else

    // printf error
    Close();
    printf("ptc error: %s\n",error);

#endif
}






void PTC::Close()
{
    // close
    if (LocalInterface!=&DummyInterface) delete LocalInterface;
    LocalInterface=&DummyInterface;
}








int PTC::GetModeList(List<MODE> &modelist)
{
    if (LocalInterface!=&DummyInterface)
    {
        // get modelist for current interface only
        return LocalInterface->GetModeList(modelist);
    }
    else
    {   
        // get modelist from all interfaces
        int index=0;
        Interface *i=CreateInterface(index,(WINDOW)NULL);
        while (i)
        {
            // get modelist
            List<MODE> list;
            i->GetModeList(list);
            modelist.merge(list);
            delete i;

            // next interface
            i=CreateInterface(++index,(WINDOW)NULL);
        }
        return 1;
    }
}







Interface* PTC::CreateInterface(int index,WINDOW const &window)
{
    // interface
    Interface *i=NULL;
    switch (index)
    {
        case 0: i=new IDirectX(window); break;
        case 1: i=new IGDI(window);     break;
        case 2: i=new IVESA(window);    break;
        case 3: i=new IVGA(window);     break;
        case 4: i=new ISVGALIB(window); break;
        case 5: i=new IAmiga(window);   break;
    }
    return i;
}







void PTC::Defaults()
{
    // setup data defaults
    LocalInterface=&DummyInterface;
}                                                                                                         
