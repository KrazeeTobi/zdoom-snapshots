#include "gl_pch.h"

#include "win32iface.h"
#include "win32gliface.h"
#include "gl/gl_intern.h"
#include "gl/gl_struct.h"
#include "gl/gl_texture.h"
#include "gl/gl_functions.h"
#include "templates.h"
#include "version.h"
//#include "gl_defs.h"

extern BOOL AppActive;
extern int palette_brightness;

EXTERN_CVAR (Float, dimamount)
EXTERN_CVAR (Color, dimcolor)

EXTERN_CVAR(Int, vid_defwidth);
EXTERN_CVAR(Int, vid_defheight);
EXTERN_CVAR(Int, vid_renderer);

CUSTOM_CVAR(Int, gl_vid_multisample, 0, CVAR_ARCHIVE | CVAR_GLOBALCONFIG)
{
	//Printf("ZGL: This won't take effect until ZDoomGL is restarted.\n");
}

CVAR(Bool, gl_vid_allowsoftware, false, CVAR_ARCHIVE | CVAR_GLOBALCONFIG);
CVAR(Int, gl_vid_refreshHz, 0, CVAR_ARCHIVE | CVAR_GLOBALCONFIG);
bool gl_nostencil;

extern HINSTANCE g_hInst;
extern HWND Window;
extern IVideo *Video;


extern PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;
PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB; // = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
PFNGLBLENDEQUATIONPROC glBlendEquation;
//PFNGLCOLORTABLEEXTPROC glColorTableEXT;


Win32GLVideo::Win32GLVideo(int parm) : m_Modes(NULL), m_IsFullscreen(false)
{
	MakeModesList();

	m_DisplayWidth = vid_defwidth;
	m_DisplayHeight = vid_defheight;
	m_DisplayBits = 32;
	m_DisplayHz = 60;
}


Win32GLVideo::~Win32GLVideo()
{
	FGLTexture::FlushAll();
	FreeModes();
	ChangeDisplaySettings(0, 0);
}

void Win32GLVideo::SetWindowedScale(float scale)
{
}

bool Win32GLVideo::FullscreenChanged(bool fs)
{
	m_IteratorFS = fs;

	return true;
}

void Win32GLVideo::GoFullscreen(bool yes)
{
	DEVMODE dm;

	m_IsFullscreen = yes;

	m_trueHeight = m_DisplayHeight;
	for (ModeInfo *mode = m_Modes; mode != NULL; mode = mode->next)
	{
		if (mode->width == m_DisplayWidth && mode->height == m_DisplayHeight)
		{
			m_trueHeight = mode->realheight;
			break;
		}
	}

	if (yes)
	{
		dm.dmSize = sizeof(dm);
		dm.dmSize = sizeof(DEVMODE);
		dm.dmPelsWidth = m_DisplayWidth;
		dm.dmPelsHeight = m_trueHeight;
		dm.dmBitsPerPel = m_DisplayBits;
		dm.dmDisplayFrequency = m_DisplayHz;
		dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL | DM_DISPLAYFREQUENCY;
		ChangeDisplaySettings(&dm, CDS_FULLSCREEN);
	}
	else
	{
		ChangeDisplaySettings(0, 0);
	}
}

DFrameBuffer *Win32GLVideo::CreateFrameBuffer(int width, int height, bool fs, DFrameBuffer *old)
{
	Win32GLFrameBuffer *fb;

	m_DisplayWidth = width;
	m_DisplayHeight = height;
	m_DisplayBits = 32;
	m_DisplayHz = 60;

	if (gl_vid_refreshHz == 0)
	{
		for (ModeInfo *mode = m_Modes; mode != NULL; mode = mode->next)
		{
			if (mode->width == m_DisplayWidth && mode->height == m_DisplayHeight && mode->bits == m_DisplayBits)
			{
				m_DisplayHz = MAX<int>(m_DisplayHz, mode->refreshHz);
			}
		}
	}
	else
	{
		m_DisplayHz = gl_vid_refreshHz;
	}

	if (old != NULL)
	{ // Reuse the old framebuffer if its attributes are the same
		fb = static_cast<Win32GLFrameBuffer *> (old);
		if (fb->m_Width == m_DisplayWidth &&
			fb->m_Height == m_DisplayHeight &&
			fb->m_Bits == m_DisplayBits &&
			fb->m_RefreshHz == m_DisplayHz &&
			fb->m_Fullscreen == fs)
		{
			return old;
		}
		//old->GetFlash(flashColor, flashAmount);
		delete old;
	}

	fb = new Win32GLFrameBuffer(m_DisplayWidth, m_DisplayHeight, m_DisplayBits, m_DisplayHz, fs);

	return fb;
}

void Win32GLVideo::StartModeIterator(int bits)
{
	m_IteratorMode = m_Modes;
	m_IteratorBits = bits;
}

bool Win32GLVideo::NextMode(int *width, int *height, bool *letterbox)
{
	if (m_IteratorMode)
	{
		while (m_IteratorMode && m_IteratorMode->bits != m_IteratorBits)
		{
			m_IteratorMode = m_IteratorMode->next;
		}

		if (m_IteratorMode)
		{
			*width = m_IteratorMode->width;
			*height = m_IteratorMode->height;
			if (letterbox != NULL) *letterbox = m_IteratorMode->realheight != m_IteratorMode->height;
			m_IteratorMode = m_IteratorMode->next;
			return true;
		}
	}

	return false;
}

void Win32GLVideo::MakeModesList()
{
	ModeInfo *pMode, *nextmode;
	DEVMODE dm;
	int mode = 0;

	memset(&dm, 0, sizeof(DEVMODE));
	dm.dmSize = sizeof(DEVMODE);

	while (EnumDisplaySettings(NULL, mode, &dm))
	{
		this->AddMode(dm.dmPelsWidth, dm.dmPelsHeight, dm.dmBitsPerPel, dm.dmPelsHeight, dm.dmDisplayFrequency);
		++mode;
	}

	for (pMode = m_Modes; pMode != NULL; pMode = nextmode)
	{
		nextmode = pMode->next;
		if (pMode->realheight == pMode->height && pMode->height * 4/3 == pMode->width)
		{
			if (pMode->width >= 360)
			{
				AddMode (pMode->width, pMode->width * 9/16, pMode->bits, pMode->height, pMode->refreshHz);
			}
			if (pMode->width > 640)
			{
				AddMode (pMode->width, pMode->width * 10/16, pMode->bits, pMode->height, pMode->refreshHz);
			}
		}
	}

#if 0
	FILE *f = fopen("modes.txt", "w");
	pMode = m_Modes;
	while (pMode)
	{
		fprintf(f, "width: %4d height: %4d bits: %2d hz: %3d\n", pMode->width, pMode->height, pMode->bits, pMode->refreshHz);
		pMode = pMode->next;
	}
	fclose(f);
#endif
}

void Win32GLVideo::AddMode(int x, int y, int bits, int baseHeight, int refreshHz)
{
	ModeInfo **probep = &m_Modes;
	ModeInfo *probe = m_Modes;

	// This mode may have been already added to the list because it is
	// enumerated multiple times at different refresh rates. If it's
	// not present, add it to the right spot in the list; otherwise, do nothing.
	// Modes are sorted first by width, then by height, then by depth. In each
	// case the order is ascending.
	for (; probe != 0; probep = &probe->next, probe = probe->next)
	{
		if (probe->width != x)		continue;
		// Width is equal
		if (probe->height != y)		continue;
		// Width is equal
		if (probe->realheight != baseHeight)	continue;
		// Height is equal
		if (probe->bits != bits)	continue;
		// Bits is equal
		if (probe->refreshHz > refreshHz) continue;
		probe->refreshHz = refreshHz;
		return;
	}

	*probep = new ModeInfo (x, y, bits, baseHeight, refreshHz);
	(*probep)->next = probe;
}

void Win32GLVideo::FreeModes()
{
   ModeInfo *mode = m_Modes;

	while (mode)
	{
		ModeInfo *tempmode = mode;
		mode = mode->next;
		delete tempmode;
	}

	m_Modes = NULL;
}


//
// FRAMEBUFFER IMPLEMENTATION
//

Win32GLFrameBuffer::Win32GLFrameBuffer(int width, int height, int bits, int refreshHz, bool fullscreen) : BaseWinFB(width, height) 
{
	RECT r;
	LONG style, exStyle;

	m_Width = width;
	m_Height = height;
	m_Bits = bits;
	m_RefreshHz = refreshHz;
	m_Fullscreen = fullscreen;
	m_hDC = NULL;
	m_hRC = NULL;

#if 0 // enable to find places that reference the screen framebuffer
	if (MemBuffer != NULL)
	{
		delete [] MemBuffer;
		MemBuffer = NULL;
	}
#endif

	static_cast<Win32GLVideo *>(Video)->GoFullscreen(fullscreen);

	memcpy (SourcePalette, GPalette.BaseColors, sizeof(PalEntry)*256);

	ShowWindow (Window, SW_SHOW);
	GetWindowRect(Window, &r);
	style = WS_VISIBLE | WS_CLIPSIBLINGS;
	exStyle = 0;

	if (fullscreen)
	{
		style |= WS_POPUP;
		MoveWindow(Window, 0, 0, width, static_cast<Win32GLVideo *>(Video)->GetTrueHeight(), FALSE);
	}
	else
	{
		style |= WS_OVERLAPPEDWINDOW;
		exStyle |= WS_EX_WINDOWEDGE;
		MoveWindow(Window, r.left, r.top, width + (GetSystemMetrics(SM_CXSIZEFRAME) * 2), height + (GetSystemMetrics(SM_CYSIZEFRAME) * 2) + GetSystemMetrics(SM_CYCAPTION), FALSE);
	}

	SetWindowLong(Window, GWL_STYLE, style);
	SetWindowLong(Window, GWL_EXSTYLE, exStyle);
	SetWindowPos(Window, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

	if (!ReadInitExtensions())
	{
		vid_renderer = 0;
		return;
	}

	m_hDC = GetDC(Window);

	if (!SetupPixelFormat())
	{
		Printf("ZGL: Reverting to software mode...\n");
		vid_renderer = 0;
		return;
	}

	m_hRC = wglCreateContext(m_hDC);

	if (m_hRC == NULL)
	{
		Printf("ZGL: Couldn't create render context. Reverting to software mode...\n");
		vid_renderer = 0;
		return;
	}

	wglMakeCurrent(m_hDC, m_hRC);

	if (!LoadRequiredExtensions())
	{
		Printf("ZGL: Reverting to software mode...\n");
		vid_renderer = 0;
		return;
	}

	m_supportsGamma = GetDeviceGammaRamp(m_hDC, (void *)m_origGamma);
	CalcGamma(Gamma, m_gammaTable);

	InitializeState();
}

Win32GLFrameBuffer::~Win32GLFrameBuffer()
{
	if (m_supportsGamma) 
	{
		SetDeviceGammaRamp(m_hDC, (void *)m_origGamma);
	}

	for (unsigned int i = 0; i < m_Extensions.Size(); ++i)
	{
		delete [] m_Extensions[i];
	}

	m_Extensions.Clear();

	if (m_hRC)
	{
		wglMakeCurrent(0, 0);
		wglDeleteContext(m_hRC);
	}
	if (m_hDC) ReleaseDC(Window, m_hDC);
}

void Win32GLFrameBuffer::Blank () 
{
}

bool Win32GLFrameBuffer::PaintToWindow () 
{ 
	return false; 
}

bool Win32GLFrameBuffer::CreateResources () 
{ 
	return false; 
}

void Win32GLFrameBuffer::ReleaseResources () 
{
}

bool Win32GLFrameBuffer::LoadRequiredExtensions()
{
	bool result = true;

	/*	I need none of these!
	glColorTableEXT = (PFNGLCOLORTABLEEXTPROC)wglGetProcAddress("glColorTable");
	if (glColorTableEXT == NULL)
	{
		Printf("ZGL: required extension glColorTable not found.\n");
		result = false;
	}

	if (!CheckExtension("GL_ARB_vertex_program"))
	{
		Printf("ZGL: required extension GL_ARB_vertex_program not found.\n");
		result = false;
	}
	else
	{
	}

	if (!CheckExtension("GL_ARB_fragment_program"))
	{
		Printf("ZGL: required extension GL_ARB_fragment_program not found.\n");
		result = false;
	}
	else
	{
	}
	*/

	return result;
}

HWND Win32GLFrameBuffer::InitDummy()
{
	HWND dummy;
	//Create a rect structure for the size/position of the window
	RECT windowRect;
	windowRect.left = 0;
	windowRect.right = 64;
	windowRect.top = 0;
	windowRect.bottom = 64;

	//Window class structure
	WNDCLASS wc;

	//Fill in window class struct
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC) DefWindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = g_hInst;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "OpenGL";

	//Register window class
	if(!RegisterClass(&wc))
	{
		return 0;
	}

	//Set window style & extended style
	DWORD style, exStyle;
	exStyle = WS_EX_CLIENTEDGE;
	style = WS_SYSMENU | WS_BORDER | WS_CAPTION;// | WS_VISIBLE;

	//Adjust the window size so that client area is the size requested
	AdjustWindowRectEx(&windowRect, style, false, exStyle);

	//Create Window
	if(!(dummy = CreateWindowEx(exStyle,
								"OpenGL",
								(LPCTSTR) "ZDOOM " DOTVERSIONSTR " (" __DATE__ ")",
								WS_CLIPSIBLINGS | WS_CLIPCHILDREN | style,
								0, 0,
								windowRect.right-windowRect.left,
								windowRect.bottom-windowRect.top,
								NULL, NULL,
								g_hInst,
								NULL)))
	{
		UnregisterClass("OpenGL", g_hInst);
		return 0;
	}
	ShowWindow(dummy, SW_HIDE);

	return dummy;
}

void Win32GLFrameBuffer::ShutdownDummy(HWND dummy)
{
	DestroyWindow(dummy);
	UnregisterClass("OpenGL", g_hInst);
}

bool Win32GLFrameBuffer::ReadInitExtensions()
{
	HDC hDC;
	HGLRC hRC;
	HWND dummy;

	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32, // color depth
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		16, // z depth
		0, // stencil buffer
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	int pixelFormat;

	// we have to create a dummy window to init stuff from or the full init stuff fails
	dummy = InitDummy();

	hDC = GetDC(dummy);
	pixelFormat = ChoosePixelFormat(hDC, &pfd);
	DescribePixelFormat(hDC, pixelFormat, sizeof(pfd), &pfd);

	SetPixelFormat(hDC, pixelFormat, &pfd);

	hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRC);

	CollectExtensions();

	wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");

	/*
	if (wglChoosePixelFormatARB == NULL)
	{
		Printf("ZGL: Couldn't find wglChoosePixelFormatARB.\n");
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(hRC);
		ReleaseDC(dummy, hDC);
		ShutdownDummy(dummy);
		return false;
	}
	*/

	// any extra stuff here?

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRC);
	ReleaseDC(dummy, hDC);
	ShutdownDummy(dummy);

	return true;
}

void Win32GLFrameBuffer::CollectExtensions()
{
	const char *supported = NULL;
	char *extensions, *extension;
	PROC wglGetExtString = wglGetProcAddress("wglGetExtensionsStringARB");

   if (wglGetExtString)
   {
      supported = ((char*(__stdcall*)(HDC))wglGetExtString)(m_hDC);
   }

	if (supported)
	{
		extensions = new char[strlen(supported) + 1];
		strcpy(extensions, supported);

		extension = strtok(extensions, " ");
		while(extension)
		{
			m_Extensions.Push(new char[strlen(extension) + 1]);
			strcpy(m_Extensions[m_Extensions.Size() - 1], extension);
			extension = strtok(NULL, " ");
		}

		delete [] extensions;
	}

	supported = (char *)glGetString(GL_EXTENSIONS);

	if (supported)
	{
		extensions = new char[strlen(supported) + 1];
		strcpy(extensions, supported);

		extension = strtok(extensions, " ");
		while(extension)
		{
			m_Extensions.Push(new char[strlen(extension) + 1]);
			strcpy(m_Extensions[m_Extensions.Size() - 1], extension);
			extension = strtok(NULL, " ");
		}

		delete [] extensions;
	}

#if 0
	FILE *f = fopen("extensions.txt", "w");
	for (unsigned int i = 0; i < m_Extensions.Size(); ++i)
	{
		fprintf(f, "%s\n", m_Extensions[i]);
	}
	fclose(f);
#endif
}

bool Win32GLFrameBuffer::SetupPixelFormat()
{
	int colorDepth;
	HDC deskDC;
	int attributes[26];
	int pixelFormat;
	unsigned int numFormats;
	float attribsFloat[] = {0.0f, 0.0f};
	
	deskDC = GetDC(GetDesktopWindow());
	colorDepth = GetDeviceCaps(m_hDC, BITSPIXEL);
	ReleaseDC(GetDesktopWindow(), deskDC);

	if (colorDepth < 32)
	{
		Printf("ZGL: Desktop not in 32 bit mode!\n");
		return false;
	}

	int stencil;
	for (stencil=1;stencil>=0;stencil--)
	{
		if (wglChoosePixelFormatARB && stencil)
		{
			attributes[0]	=	WGL_RED_BITS_ARB; //bits
			attributes[1]	=	8;
			attributes[2]	=	WGL_GREEN_BITS_ARB; //bits
			attributes[3]	=	8;
			attributes[4]	=	WGL_BLUE_BITS_ARB; //bits
			attributes[5]	=	8;
			attributes[6]	=	WGL_ALPHA_BITS_ARB;
			attributes[7]	=	8;
			attributes[8]	=	WGL_DEPTH_BITS_ARB;
			attributes[9]	=	24;
			attributes[10]	=	WGL_STENCIL_BITS_ARB;
			attributes[11]	=	8;
		
			attributes[12]	=	WGL_DRAW_TO_WINDOW_ARB;	//required to be true
			attributes[13]	=	true;
			attributes[14]	=	WGL_SUPPORT_OPENGL_ARB;
			attributes[15]	=	true;
			attributes[16]	=	WGL_DOUBLE_BUFFER_ARB;
			attributes[17]	=	true;
		
			attributes[18]	=	WGL_ACCELERATION_ARB;	//required to be FULL_ACCELERATION_ARB
			if (gl_vid_allowsoftware)
			{
				attributes[19]	=	WGL_NO_ACCELERATION_ARB;
			}
			else
			{
				attributes[19]	=	WGL_FULL_ACCELERATION_ARB;
			}
		
			if (gl_vid_multisample > 0)
			{
				attributes[20]	=	WGL_SAMPLE_BUFFERS_ARB;
				attributes[21]	=	true;
				attributes[22]	=	WGL_SAMPLES_ARB;
				attributes[23]	=	gl_vid_multisample;
			}
			else
			{
				attributes[20]	=	0;
				attributes[21]	=	0;
				attributes[22]	=	0;
				attributes[23]	=	0;
			}
		
			attributes[24]	=	0;
			attributes[25]	=	0;
		
			if (!wglChoosePixelFormatARB(m_hDC, attributes, attribsFloat, 1, &pixelFormat, &numFormats))
			{
				Printf("ZGL: Couldn't choose pixel format. Retrying in compatibility mode\n");
				continue;//return false;
			}
		
			if (numFormats == 0)
			{
				Printf("ZGL: No valid pixel formats found. Retrying in compatibility mode\n");
				continue;//return false;
			}

			break;
		}
		else
		{
			// If wglChoosePixelFormatARB is not found we have to do it the old fashioned way.
			static PIXELFORMATDESCRIPTOR pfd = {
				sizeof(PIXELFORMATDESCRIPTOR),
					1,
					PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
					PFD_TYPE_RGBA,
					32, // color depth
					0, 0, 0, 0, 0, 0,
					0,
					0,
					0,
					0, 0, 0, 0,
					32, // z depth
					stencil*8, // stencil buffer
					0,
					PFD_MAIN_PLANE,
					0,
					0, 0, 0
			};

			pixelFormat = ChoosePixelFormat(m_hDC, &pfd);
			DescribePixelFormat(m_hDC, pixelFormat, sizeof(pfd), &pfd);

			if (pfd.dwFlags & PFD_GENERIC_FORMAT)
			{
				if (!gl_vid_allowsoftware)
				{
					if (stencil==0)
					{
						// not accelerated!
						vid_renderer = 0;
						Printf("ZGL: OpenGL driver not accelerated!  Falling back to software renderer.\n");
						return false;
					}
					else
					{
						Printf("ZGL: OpenGL driver not accelerated! Retrying in compatibility mode\n");
						continue;
					}
				}
			}
			break;
		}
	}
	if (stencil==0)
	{
		gl_nostencil=true;
	}

	if (!SetPixelFormat(m_hDC, pixelFormat, NULL))
	{
		Printf("ZGL: Couldn't set pixel format.\n");
		return false;
	}

	return true;
}

void Win32GLFrameBuffer::InitializeState()
{
	static bool first=true;

	if (first)
	{
		first=false;
		Printf ("GL_VENDOR: %s\n",glGetString(GL_VENDOR));
		Printf ("GL_RENDERER: %s\n",glGetString(GL_RENDERER));
		Printf ("GL_VERSION: %s\n",glGetString(GL_VERSION));
		Printf ("GL_EXTENSIONS: %s\n",glGetString(GL_EXTENSIONS));

		GLTexture::supportsNonPower2 = CheckExtension("GL_ARB_texture_non_power_of_two");
		if (GLTexture::supportsNonPower2)
		{
			Printf("Support for non power 2 textures enabled.\n");
		}
		glGetIntegerv(GL_MAX_TEXTURE_SIZE,&GLTexture::max_texturesize);
	}
	

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glDepthFunc(GL_LESS);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_DITHER);
	glEnable(GL_ALPHA_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_CLAMP_NV);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LINE_SMOOTH);
	glAlphaFunc(GL_GEQUAL,0.5f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glShadeModel(GL_SMOOTH);

	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, GetWidth() * 1.0, 0.0, GetHeight() * 1.0, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//GL::SetPerspective(90.f, GetWidth() * 1.f / GetHeight(), 0.f, 1000.f);

	glViewport(0, (GetTrueHeight()-GetHeight())/2, GetWidth(), GetHeight()); 

	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");

	gl_InitShaders();
	gl_InitFog();

	if (gl_vertices.Size())
	{
		glTexCoordPointer(2,GL_FLOAT,sizeof(GLVertex),&gl_vertices[0].u);
		glVertexPointer(3,GL_FLOAT,sizeof(GLVertex),&gl_vertices[0].x);
	}
}

bool Win32GLFrameBuffer::Lock(bool buffered)
{
	Buffer = MemBuffer;
	return true;
}

void Win32GLFrameBuffer::Update()
{
	if (!AppActive) return;

	gl_Set2DMode();

	DrawRateStuff();

	if (GetTrueHeight() != GetHeight())
	{
		// Letterbox time! Draw black top and bottom borders.
		int borderHeight = (GetTrueHeight() - GetHeight()) / 2;

		glViewport(0, 0, GetWidth(), GetTrueHeight());
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0, GetWidth() * 1.0, 0.0, GetTrueHeight(), -1.0, 1.0);
		glMatrixMode(GL_MODELVIEW);
		glColor3f(0.f, 0.f, 0.f);
		glDisable(GL_TEXTURE_2D);

		glBegin(GL_QUADS);
		// upper quad
		glVertex2i(0, borderHeight);
		glVertex2i(0, 0);
		glVertex2i(GetWidth(), 0);
		glVertex2i(GetWidth(), borderHeight);
		glEnd();

		glBegin(GL_QUADS);
		// lower quad
		glVertex2i(0, GetTrueHeight());
		glVertex2i(0, GetTrueHeight() - borderHeight);
		glVertex2i(GetWidth(), GetTrueHeight() - borderHeight);
		glVertex2i(GetWidth(), GetTrueHeight());
		glEnd();

		glEnable(GL_TEXTURE_2D);

		gl_Set2DMode();
		glViewport(0, (GetTrueHeight() - GetHeight()) / 2, GetWidth(), GetHeight()); 

	}

	glFinish();
	SwapBuffers(m_hDC);
}

bool Win32GLFrameBuffer::Lock () 
{ 	
	return true; 
}

void Win32GLFrameBuffer::Unlock () 	
{ 
}

bool Win32GLFrameBuffer::IsLocked () 
{ 
	return true; 
}

PalEntry *Win32GLFrameBuffer::GetPalette()
{
	return SourcePalette;
}

void Win32GLFrameBuffer::GetFlashedPalette(PalEntry palette[256])
{
	memcpy(palette, SourcePalette, 256*sizeof(PalEntry));
}

void Win32GLFrameBuffer::UpdatePalette()
{
	int rr=0,gg=0,bb=0;
	for(int x=0;x<256;x++)
	{
		rr+=GPalette.BaseColors[x].r;
		gg+=GPalette.BaseColors[x].g;
		bb+=GPalette.BaseColors[x].b;
	}
	rr>>=8;
	gg>>=8;
	bb>>=8;

	palette_brightness = (rr*77 + gg*143 + bb*35)/255;
}

bool Win32GLFrameBuffer::SetGamma(float gamma)
{
	WORD gammaTable[768];

	CalcGamma(gamma, m_gammaTable);

	if (m_supportsGamma)
	{
		for (int i = 0; i < 256; i++)
		{
			gammaTable[i] = gammaTable[i + 256] = gammaTable[i + 512] = (WORD)(m_gammaTable[i] * 257);
		}
		SetDeviceGammaRamp(m_hDC, (void*)gammaTable);
	}
	return true;
}

bool Win32GLFrameBuffer::SetFlash(PalEntry rgb, int amount)
{
	return true;
}

void Win32GLFrameBuffer::GetFlash(PalEntry &rgb, int &amount)
{
}

int Win32GLFrameBuffer::GetPageCount()
{
	return 1;
}

bool Win32GLFrameBuffer::IsFullscreen()
{
	return m_Fullscreen;
}

void Win32GLFrameBuffer::PaletteChanged()
{
}

int Win32GLFrameBuffer::QueryNewPalette()
{
	return 0;
}

void Win32GLFrameBuffer::Clear(int left, int top, int right, int bottom, int color) const
{
	gl_Clear(left, top, right, bottom, color);
}

void Win32GLFrameBuffer::Dim() const
{
	gl_Dim((DWORD)dimcolor , dimamount, 0, 0, GetWidth(), GetHeight());
}

void Win32GLFrameBuffer::Dim(PalEntry color, float damount, int x1, int y1, int w, int h) const
{
	gl_Dim(color, damount, x1, y1, w, h);
}

void Win32GLFrameBuffer::FlatFill (int left, int top, int right, int bottom, FTexture *src)
{
	gl_FlatFill(left, top, right, bottom, src);
}

bool Win32GLFrameBuffer::CheckExtension(const char *ext)
{
	for (unsigned int i = 0; i < m_Extensions.Size(); ++i)
	{
		if (strcmp(ext, m_Extensions[i]) == 0) return true;
	}

	return false;
}
