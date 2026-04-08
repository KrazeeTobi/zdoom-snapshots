/////////////////////
// WndProc example //
/////////////////////
#include "ptc.h"




// window procedure
LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

// globals
PTC *ptc=NULL;
Surface *image=NULL;
Surface *surface=NULL;





int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInst,LPSTR lpCmdLine,int nCmdShow)
{   
    // advoid warnings
    if (lpCmdLine);

    // register window class
    WNDCLASS wc;
    wc.style=CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc=WndProc;
    wc.cbClsExtra=0;
    wc.cbWndExtra=0;
    wc.hInstance=hInstance;
    wc.hIcon=LoadIcon(hInstance,"IDI_PTC_ICON");
    wc.hCursor=LoadCursor(NULL,IDC_ARROW);
    wc.hbrBackground=NULL;
    wc.lpszMenuName=NULL;
    wc.lpszClassName="WNDPROC_EXAMPLE_WINDOW";
    if (!RegisterClass(&wc)) return 1;

    // calculate window size to fit around 320x200 client area
    int width=320;
    int height=200;
    int frame_x=GetSystemMetrics(SM_CXFRAME);
    int frame_y=GetSystemMetrics(SM_CYFRAME);
    int title_y=GetSystemMetrics(SM_CYCAPTION);
    width+=frame_x*2;
    height+=frame_y*2+title_y;

    // create window    
    HWND hWnd=CreateWindow("WNDPROC_EXAMPLE_WINDOW","WndProc Example",WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,width,height,NULL,NULL,hInstance,NULL);
    if (!hWnd) return 1;

    // initialize ptc (GDI)
    ptc=new PTC("GDI",hWnd);
    if (!ptc->ok())
    {
        // failure
        ptc->Error("could not initialize");
        DestroyWindow(hWnd);
        return 1;
    }

    // load image
    image=new Surface(*ptc,"../base/image.tga");
    if (!image->Convert(ARGB8888)) 
    {
        // failure
        ptc->Error("could not load image");
        DestroyWindow(hWnd);
        return 1;
    }

    // create surface
    surface=new Surface(*ptc,320,200,ARGB8888);
    if (!surface->ok())
    {
        // failure
        ptc->Error("could not create surface");
        return 1;
    }

    // show the window
    ShowWindow(hWnd,nCmdShow);
    UpdateWindow(hWnd);

    // message loop
    MSG msg;
    while (GetMessage(&msg,NULL,0,0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (msg.wParam);
}




LRESULT CALLBACK WndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
    // message handler
    switch (message) 
    { 
        case WM_MOUSEMOVE:

            // check
            if (image && surface && wParam==MK_LBUTTON)
            {
                // get mouse x,y position
                int px=LOWORD(lParam);
                int py=HIWORD(lParam);

                // get window rect
                RECT stretch;
                GetClientRect(hWnd,&stretch);
                int dx=stretch.right-stretch.left;
                int dy=stretch.bottom-stretch.top;

                // adjust x,y for window stretch
                int x=(int)((float)px*320.0f/(float)dx);
                int y=(int)((float)py*200.0f/(float)dy);

                // bitblt from image(x,y) -> surface(x,y)
                RECTANGLE rect(x-10,y-10,x+10,y+10);
                image->BitBlt(*surface,rect,rect);

                // redraw window
                InvalidateRect(hWnd,NULL,FALSE);
                UpdateWindow(hWnd);
            }
            break;

        case WM_KEYDOWN:
            
            // handle keypress
            if (wParam==VK_ESCAPE)
            {
                // exit on escape key
                SendMessage(hWnd,WM_DESTROY,0,0);
            }
            break;

        case WM_PAINT:
            
            // draw surface to window
            if (surface) surface->Update();
            break;   
            
        case WM_DESTROY: 

            // cleanup
            delete ptc;
            delete image;
            delete surface;
            ptc=NULL;
            image=NULL;
            surface=NULL;

            // destroy window
            ShowWindow(hWnd,SW_HIDE);
            PostQuitMessage(0);
            break;

        default:

            // default window action
            return DefWindowProc(hWnd,message,wParam,lParam);
    }
    return 0;
}
