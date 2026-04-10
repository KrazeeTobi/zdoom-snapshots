

#include "m_menu.h"
#include "gl/gl_intern.h"


extern value_t YesNo[2];
extern value_t NoYes[2];
extern value_t OnOff[2];

void StartGLLightMenu (void);

EXTERN_CVAR (Bool, vid_vsync)
EXTERN_CVAR(Int, gl_spriteclip)
EXTERN_CVAR(Bool, gl_blendcolormaps)
EXTERN_CVAR(Bool, gl_texture_usehires)
EXTERN_CVAR(Bool, gl_precache)

static value_t SpriteclipModes[]=
{
	{ 0.0, "Never" },
	{ 1.0, "Smart" },
	{ 2.0, "Always" },
};

static value_t FilterModes[] =
{
	{ 0.0, "None" },
	{ 1.0, "None (mipmapped)" },
	{ 2.0, "Linear" },
	{ 3.0, "Bilinear" },
	{ 4.0, "Trilinear" },
};

static value_t TextureFormats[] =
{
	{ 0.0, "RGBA8" },
	{ 1.0, "RGB5_A1" },
	{ 2.0, "RGBA4" },
	{ 3.0, "RGBA2" },
};

static value_t Anisotropy[] =
{
	{ 1.0, "Off" },
	{ 2.0, "2x" },
	{ 4.0, "4x" },
	{ 8.0, "8x" },
	{ 16.0, "16x" },
};

static value_t Colormaps[] =
{
	{ 0.0, "Use as palette" },
	{ 1.0, "Blend" },
};


menuitem_t OpenGLItems[] = {
	{ more,     "Light Options", {NULL}, {0.0}, {0.0},	{0.0}, {(value_t *)StartGLLightMenu} },
	{ redtext,	" ",						{NULL},							{0.0}, {0.0}, {0.0}, {NULL} },
	{ discrete, "Vertical Sync",			{&vid_vsync},					{2.0}, {0.0}, {0.0}, {OnOff} },
	{ discrete, "Environment map on mirrors",{&gl_mirror_envmap},			{2.0}, {0.0}, {0.0}, {OnOff} },
	{ discrete, "Enhanced night vision mode",{&gl_enhanced_lightamp},		{2.0}, {0.0}, {0.0}, {OnOff} },
	{ discrete, "Adjust sprite clipping",	{&gl_spriteclip},				{3.0}, {0.0}, {0.0}, {SpriteclipModes} },
	{ discrete, "Depth Fog",				{&gl_depthfog},					{2.0}, {0.0}, {0.0}, {OnOff} },
	{ discrete, "Boom colormap handling",	{&gl_blendcolormaps},			{2.0}, {0.0}, {0.0}, {Colormaps} },
	{ redtext,	" ",						{NULL},							{0.0}, {0.0}, {0.0}, {NULL} },
	{ discrete, "Textures enabled",			{&gl_texture},					{2.0}, {0.0}, {0.0}, {YesNo} },
	{ discrete, "Texture Filter mode",		{&gl_texture_filter},			{5.0}, {0.0}, {0.0}, {FilterModes} },
	{ discrete, "Anisotropic filter",		{&gl_texture_filter_anisotropic},{5.0},{0.0}, {0.0}, {Anisotropy} },
	{ discrete, "Texture Format",			{&gl_texture_format},			{4.0}, {0.0}, {0.0}, {TextureFormats} },
	{ discrete, "Enable hires textures",	{&gl_texture_usehires},			{2.0}, {0.0}, {0.0}, {YesNo} },
	{ discrete, "Precache GL textures",		{&gl_precache},					{2.0}, {0.0}, {0.0}, {YesNo} },
};

menuitem_t GLLightItems[] = {
	{ discrete, "Dynamic Lights enabled",	{&gl_lights},			{2.0}, {0.0}, {0.0}, {YesNo} },
	{ discrete, "Enable light definitions",	{&gl_attachedlights},	{2.0}, {0.0}, {0.0}, {YesNo} },
	{ discrete, "Clip lights",				{&gl_lights_checkside},	{2.0}, {0.0}, {0.0}, {YesNo} },
	{ discrete, "Lights affect sprites",	{&gl_light_sprites},	{2.0}, {0.0}, {0.0}, {YesNo} },
	{ discrete, "Lights affect particles",	{&gl_light_particles},	{2.0}, {0.0}, {0.0}, {YesNo} },
	{ slider,	"Light intensity",			{&gl_lights_intensity}, {0.0}, {1.0}, {0.1f}, {NULL} },
	{ slider,	"Light size",				{&gl_lights_size},		{0.0}, {2.0}, {0.1f}, {NULL} },
};

menu_t OpenGLMenu = {
   "OPENGL OPTIONS",
   0,
   sizeof(OpenGLItems)/sizeof(OpenGLItems[0]),
   0,
   OpenGLItems,
   0,
};

menu_t GLLightMenu = {
   "LIGHT OPTIONS",
   0,
   sizeof(GLLightItems)/sizeof(GLLightItems[0]),
   0,
   GLLightItems,
   0,
};

void StartGLMenu (void)
{
   M_SwitchMenu(&OpenGLMenu);
}

void StartGLLightMenu (void)
{
	M_SwitchMenu(&GLLightMenu);
}


