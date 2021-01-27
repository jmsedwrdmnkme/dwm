/* See LICENSE file for copyright and license details. */

/* appearance */
static unsigned int borderpx        = 1;        /* border pixel of windows */
static const unsigned int gappx     = 16;        /* gaps between windows */
static unsigned int snap            = 32;       /* snap pixel */
static int showbar                  = 1;        /* 0 means no bar */
static int topbar                   = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title           tags mask     isfloating   monitor */
	{ "Zathura",  NULL,       NULL,           0,            1,           -1 },
	{ "Sxiv",     NULL,       NULL,           0,            1,           -1 },
	{ "mpv",      NULL,       NULL,           0,            1,           -1 },
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "120x34", NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
		{ "normbgcolor",        STRING,  &normbgcolor },
		{ "normbordercolor",    STRING,  &normbordercolor },
		{ "normfgcolor",        STRING,  &normfgcolor },
		{ "selbgcolor",         STRING,  &selbgcolor },
		{ "selbordercolor",     STRING,  &selbordercolor },
		{ "selfgcolor",         STRING,  &selfgcolor },
		{ "borderpx",          	INTEGER, &borderpx },
		{ "snap",          		INTEGER, &snap },
		{ "showbar",          	INTEGER, &showbar },
		{ "topbar",          	INTEGER, &topbar },
		{ "nmaster",          	INTEGER, &nmaster },
		{ "resizehints",       	INTEGER, &resizehints },
		{ "mfact",      	 	FLOAT,   &mfact },
};

#include <X11/XF86keysym.h>

static Key keys[] = {
	/* modifier                     key                        function        argument */
	{ MODKEY,                       XK_d,                      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return,                 spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_grave,                  togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY|ShiftMask,             XK_b,                      togglebar,      {0} },
	{ MODKEY,                       XK_l,                      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_h,                      focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_j,                      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,                      incnmaster,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_h,                      setmfact,       {.f = -0.05} },
	{ MODKEY|ShiftMask,             XK_l,                      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return,                 zoom,           {0} },
	{ MODKEY,                       XK_Tab,                    view,           {0} },
	{ MODKEY,                       XK_q,                      killclient,     {0} },
	{ MODKEY,                       XK_t,                      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,                      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,                      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,                  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,                  togglefloating, {0} },
	{ MODKEY,                       XK_0,                      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,                      tag,            {.ui = ~0 } },
	{ 0,                            XK_Print,   	           spawn,          SHCMD("scrot '%Y-%m-%d_%H:%M:%S_$wx$h.png' -e 'mv $f ~/Pictures/'; notify-send 'Screenshot taken'") },
	{ ShiftMask,                    XK_Print,   	           spawn,          SHCMD("scrot '%Y-%m-%d_%H:%M:%S_$wx$h.png' --focused -e 'mv $f ~/Pictures/'; notify-send 'Window screenshot taken'") },
	{ 0,			        XF86XK_AudioMute,	   spawn,          SHCMD("pamixer -t; kill -39 $(pidof dwmblocks)") },
	{ 0,			        XF86XK_AudioRaiseVolume,   spawn,          SHCMD("pamixer --allow-boost -i 1; kill -39 $(pidof dwmblocks)") },
	{ 0,			        XF86XK_AudioLowerVolume,   spawn,          SHCMD("pamixer --allow-boost -d 1; kill -39 $(pidof dwmblocks)") },
	{ 0,			        XF86XK_AudioPrev,	   spawn,          SHCMD("mpc prev; kill -35 $(pidof dwmblocks)") },
	{ 0,			        XF86XK_AudioNext,	   spawn,          SHCMD("mpc next; kill -35 $(pidof dwmblocks)") },
	{ 0,			        XF86XK_AudioPlay,	   spawn,          SHCMD("mpc toggle; kill -35 $(pidof dwmblocks)") },
	{ 0,			        XF86XK_AudioStop,	   spawn,          SHCMD("mpc stop; kill -35 $(pidof dwmblocks)") },
	{ 0,                            XF86XK_MonBrightnessUp,    spawn,          SHCMD("xbacklight -inc 10") },
	{ 0,                            XF86XK_MonBrightnessDown,  spawn,          SHCMD("xbacklight -dec 10") },
	{ 0,                            XK_F1,                     spawn,          SHCMD("firefox") },
	{ 0,                            XK_F2,                     spawn,          SHCMD("st -t scratchpad -g 175x50 -e neomutt") },
	{ 0,                            XK_F3,                     spawn,          SHCMD("st -t scratchpad -g 175x50 -e newsboat") },
	{ 0,                            XK_F4,                     spawn,          SHCMD("st -t scratchpad -g 65x20 -e calc") },
	{ 0,                            XK_F5,                     spawn,          SHCMD("st -t scratchpad -g 170x32 -e ncmpcpp") },
	{ 0,                            XK_F6,                     spawn,          SHCMD("st -t scratchpad -g 175x50 -e calcurse") },
	{ 0,                            XK_F7,                     spawn,          SHCMD("st -t scratchpad -g 126x42 -e less -Srf ~/.local/share/weatherreport") },
	TAGKEYS(                        XK_1,                                      0)
	TAGKEYS(                        XK_2,                                      1)
	TAGKEYS(                        XK_3,                                      2)
	TAGKEYS(                        XK_4,                                      3)
	TAGKEYS(                        XK_5,                                      4)
	TAGKEYS(                        XK_6,                                      5)
	TAGKEYS(                        XK_7,                                      6)
	TAGKEYS(                        XK_8,                                      7)
	TAGKEYS(                        XK_9,                                      8)
	{ MODKEY|ShiftMask,             XK_q,                      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3} },
	{ ClkStatusText,        0,              Button4,        sigdwmblocks,   {.i = 4} },
	{ ClkStatusText,        0,              Button5,        sigdwmblocks,   {.i = 5} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

