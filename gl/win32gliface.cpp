#include "gl_pch.h"

#include "win32iface.h"
#include "win32gliface.h"
#include "gl/gl_basic.h"
#include "gl/gl_intern.h"
#include "gl/gl_struct.h"
#include "gl/gl_texture.h"
#include "gl/gl_functions.h"
#include "templates.h"
#include "version.h"
#include "c_console.h"
//#include "gl_defs.h"

CUSTOM_CVAR(Int, gl_vid_multisample, 0, CVAR_ARCHIVE | CVAR_GLOBALCONFIG)
{
	//Printf("ZGL: This won't take effect until ZDoomGL is restarted.\n");
}

RenderContext gl;


CVAR(Bool, gl_vid_allowsoftware, false, CVAR_ARCHIVE | CVAR_GLOBALCONFIG);
CVAR(Int, gl_vid_refreshHz, 0, CVAR_ARCHIVE | CVAR_GLOBALCONFIG);

Win32GLVideo::Win32GLVideo(int parm) : m_Modes(NULL), m_IsFullscreen(false)
{
	m_DisplayWidth = vid_defwidth;
	m_DisplayHeight = vid_defheight;
	m_DisplayBits = 32;
	m_DisplayHz = 60;
	MakeModesList();

	GetContextProc gc;

	hmRender = LoadLibrary("r_opengl.dll");
	if (hmRender) gc=(GetContextProc)GetProcAddress(hmRender, "_GetContext@4");
	if (gc) gc(gl);
}

Win32GLVideo::~Win32GLVideo()
{
	FreeModes();
	FGLTexture::FlushAll();
	// TODO: move into r_opengl.dll!
	ChangeDisplaySettings(0, 0);
	if (hmRender) FreeLibrary(hmRender);
}

void Win32GLVideo::SetWindowedScale(float scale)
{
}

bool Win32GLVideo::FullscreenChanged(bool fs)
{
	m_IteratorFS = fs;

	return true;
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

bool Win32GLVideo::GoFullscreen(bool yes)
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

	// TODO: move into r_opengl.dll!
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
	return yes;
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


static void CALLBACK DoPrintText(const char * out)
{
	PrintString(PRINT_HIGH, out);
}

Win32GLFrameBuffer::Win32GLFrameBuffer(int width, int height, int bits, int refreshHz, bool fullscreen) : BaseWinFB(width, height) 
{
	m_Width = width;
	m_Height = height;
	m_Bits = bits;
	m_RefreshHz = refreshHz;
	m_Fullscreen = fullscreen;

	memcpy (SourcePalette, GPalette.BaseColors, sizeof(PalEntry)*256);
	UpdatePalette ();

	RECT r;
	LONG style, exStyle;

	static_cast<Win32GLVideo *>(Video)->GoFullscreen(fullscreen);

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

	if (!gl.InitHardware(Window, gl_vid_allowsoftware, gl_vid_multisample, DoPrintText))
	{
		vid_renderer = 0;
		return;
	}

	m_supportsGamma = gl.GetGammaRamp((void *)m_origGamma);
	CalcGamma(Gamma, m_gammaTable);

	InitializeState();


}

Win32GLFrameBuffer::~Win32GLFrameBuffer()
{
	if (m_supportsGamma) 
	{
		gl.SetGammaRamp((void *)m_origGamma);
	}
	gl.Shutdown();
}


void Win32GLFrameBuffer::InitializeState()
{
	static bool first=true;

	gl.LoadExtensions();
	if (first)
	{
		first=false;
		gl.PrintStartupLog(DoPrintText);

		if (gl.flags&RFL_NPOT_TEXTURE)
		{
			Printf("Support for non power 2 textures enabled.\n");
		}
	}


	gl.ClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	gl.ClearDepth(1.0f);
	gl.DepthFunc(GL_LESS);
	gl.ShadeModel(GL_SMOOTH);

	gl.Enable(GL_DITHER);
	gl.Enable(GL_ALPHA_TEST);
	gl.Disable(GL_CULL_FACE);
	gl.Enable(GL_POLYGON_OFFSET_FILL);
	gl.Enable(GL_POLYGON_OFFSET_LINE);
	gl.Enable(GL_BLEND);
	gl.Enable(GL_DEPTH_CLAMP_NV);
	gl.Disable(GL_DEPTH_TEST);
	gl.Enable(GL_TEXTURE_2D);
	gl.Disable(GL_LINE_SMOOTH);
	gl.AlphaFunc(GL_GEQUAL,0.5f);
	gl.BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	gl.Hint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	gl.Hint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	gl.Hint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	gl.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gl.MatrixMode(GL_PROJECTION);
	gl.LoadIdentity();
	gl.Ortho(0.0, GetWidth() * 1.0, 0.0, GetHeight() * 1.0, -1.0, 1.0);

	gl.MatrixMode(GL_MODELVIEW);
	gl.LoadIdentity();
	//GL::SetPerspective(90.f, GetWidth() * 1.f / GetHeight(), 0.f, 1000.f);

	gl.Viewport(0, (GetTrueHeight()-GetHeight())/2, GetWidth(), GetHeight()); 


	gl_InitShaders();
	gl_InitFog();

	if (gl_vertices.Size())
	{
		gl.ArrayPointer(&gl_vertices[0], sizeof(GLVertex));
	}
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

		gl.Viewport(0, 0, GetWidth(), GetTrueHeight());
		gl.MatrixMode(GL_PROJECTION);
		gl.LoadIdentity();
		gl.Ortho(0.0, GetWidth() * 1.0, 0.0, GetTrueHeight(), -1.0, 1.0);
		gl.MatrixMode(GL_MODELVIEW);
		gl.Color3f(0.f, 0.f, 0.f);
		gl.Disable(GL_TEXTURE_2D);

		gl.Begin(GL_QUADS);
		// upper quad
		gl.Vertex2i(0, borderHeight);
		gl.Vertex2i(0, 0);
		gl.Vertex2i(GetWidth(), 0);
		gl.Vertex2i(GetWidth(), borderHeight);
		gl.End();

		gl.Begin(GL_QUADS);
		// lower quad
		gl.Vertex2i(0, GetTrueHeight());
		gl.Vertex2i(0, GetTrueHeight() - borderHeight);
		gl.Vertex2i(GetWidth(), GetTrueHeight() - borderHeight);
		gl.Vertex2i(GetWidth(), GetTrueHeight());
		gl.End();

		gl.Enable(GL_TEXTURE_2D);

		gl_Set2DMode();
		gl.Viewport(0, (GetTrueHeight() - GetHeight()) / 2, GetWidth(), GetHeight()); 

	}

	Finish.Reset();
	Finish.Start();
	gl.Finish();
	Finish.Stop();
	gl.SwapBuffers();
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
		gl.SetGammaRamp((void*)gammaTable);
	}
	return true;
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

bool Win32GLFrameBuffer::Lock(bool buffered)
{
	Buffer = MemBuffer;
	return true;
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

