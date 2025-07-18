/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int gappx     = 16;       /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#0e0c0d";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_gray1, col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask  iscentered   isfloating   monitor */
	{ "Zathura",  NULL,       NULL,       0,         1,           1,           -1 },
	{ "Nsxiv",    NULL,       NULL,       0,         1,           1,           -1 },
	{ "feh",      NULL,       NULL,       0,         1,           1,           -1 },
	{ "mpv",      NULL,       NULL,       0,         1,           1,           -1 },
	{ "St",       NULL,       "float",    0,         1,           1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "﬿",        tile },    /* first entry is default */
	{ "",        NULL },    /* no layout function means floating behavior */
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

#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "120x34", NULL };

#include <X11/XF86keysym.h>

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,                      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return,                 spawn,          {.v = termcmd } },
        { MODKEY,                       XK_grave,                  togglescratch,  {.v = scratchpadcmd } }, // Spawn terminal scratchpad window
	{ MODKEY,                       XK_b,                      togglebar,      {0} },
	{ MODKEY,                       XK_l,                      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_h,                      focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_i,                      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_d,                      incnmaster,     {.i = -1 } },
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
	{ MODKEY,                       XK_comma,                  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,                 focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,                  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,                 tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_c,                      spawn,          SHCMD("get-colour") }, // Copy HEX colour beneath cursor
	{ MODKEY,                       XK_o,                      spawn,          SHCMD("dmenu-websearch") }, // Dmenu as browser URL bar
	{ MODKEY,                       XK_p,                      spawn,          SHCMD("dmenu-pass -p") }, // Dmenu password selector
	{ MODKEY|ShiftMask,             XK_p,                      spawn,          SHCMD("power-menu") }, // Prompt with power menu
	{ MODKEY,                       XK_u,                      spawn,          SHCMD("dmenu-pass -u") }, // Dmenu pass username selector
	{ MODKEY,                       XK_a,                      spawn,          SHCMD("dmenu-pass -a") }, // Dmenu pass OTP code selector
	{ MODKEY,                       XK_v,                      spawn,          SHCMD("mpvnohup") }, // Dmenu mpv video URL player
	{ MODKEY|ShiftMask,             XK_v,                      spawn,          SHCMD("dmenu-vpn") }, // Dmenu openvpn
	{ MODKEY|ShiftMask,             XK_t,                      spawn,          SHCMD("dmenu-translate") }, // Dmenu translate
	{ 0,                            XK_Print,   	           spawn,          SHCMD("scrot '%Y-%m-%d_%H:%M:%S_$wx$h.png' -e 'mv $f ~/Pictures/'; notify-send 'Screenshot taken'") }, // Take full display screen shot
	{ ShiftMask,                    XK_Print,   	           spawn,          SHCMD("scrot '%Y-%m-%d_%H:%M:%S_$wx$h.png' -s -e 'mv $f ~/Pictures/'; notify-send 'Window screenshot taken'") }, // Take selected area screen shot
	{ 0,			        XF86XK_AudioMute,	   spawn,          SHCMD("wpctl set-mute @DEFAULT_AUDIO_SINK@ toggle; kill -39 $(pidof dwmblocks)") }, // Mute audio
	{ 0,			        XF86XK_AudioRaiseVolume,   spawn,          SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 1%+; kill -39 $(pidof dwmblocks) && vol=\"$(wpctl get-volume @DEFAULT_AUDIO_SINK@ | tr -d '.' | egrep -o '[0-9]{3}' | sed 's/^0*//')\"") }, // Raise volume
	{ 0,			        XF86XK_AudioLowerVolume,   spawn,          SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 1%-; kill -39 $(pidof dwmblocks) && vol=\"$(wpctl get-volume @DEFAULT_AUDIO_SINK@ | tr -d '.' | egrep -o '[0-9]{3}' | sed 's/^0*//')\"") }, // Lower volume
	{ ShiftMask,		        XF86XK_AudioMute,	   spawn,          SHCMD("wpctl set-mute @DEFAULT_AUDIO_SOURCE@ toggle; kill -45 $(pidof dwmblocks)") }, // Mute microphone
	{ ShiftMask,		        XF86XK_AudioRaiseVolume,   spawn,          SHCMD("wpctl set-volume @DEFAULT_AUDIO_SOURCE@ 1%+; kill -45 $(pidof dwmblocks) && mic=\"$(wpctl get-volume @DEFAULT_AUDIO_SOURCEK@ | tr -d '.' | egrep -o '[0-9]{3}' | sed 's/^0*//')\"") }, // Raise microphone input
	{ ShiftMask,		        XF86XK_AudioLowerVolume,   spawn,          SHCMD("wpctl set-volume @DEFAULT_AUDIO_SOURCE@ 1%-; kill -45 $(pidof dwmblocks) && mic=\"$(wpctl get-volume @DEFAULT_AUDIO_SOURCEK@ | tr -d '.' | egrep -o '[0-9]{3}' | sed 's/^0*//')\"") }, // Lower microphone input
	{ MODKEY,		        XK_Left,           	   spawn,          SHCMD("mpc prev; kill -35 $(pidof dwmblocks)") }, // Previous track
	{ MODKEY,		        XK_Right,          	   spawn,          SHCMD("mpc next; kill -35 $(pidof dwmblocks)") }, // Next track
	{ MODKEY,		        XK_Up,             	   spawn,          SHCMD("mpc toggle; kill -35 $(pidof dwmblocks)") }, // Play/Pause track
	{ MODKEY,		        XK_Down,           	   spawn,          SHCMD("mpc stop; kill -35 $(pidof dwmblocks)") }, // Stop playing
	{ 0,                            XF86XK_MonBrightnessUp,    spawn,          SHCMD("xbacklight -inc 5 && dunstify -h string:x-dunst-stack-tag:backlight \"🖵 Backlight $(xbacklight -get)%\"") }, // Screen brightness up
	{ 0,                            XF86XK_MonBrightnessDown,  spawn,          SHCMD("xbacklight -dec 5 && dunstify -h string:x-dunst-stack-tag:backlight \"🖵 Backlight $(xbacklight -get)%\"") }, // Screen brightness down
        { 0,                            XK_F1,                     spawn,          SHCMD("$BROWSER") }, // Browser
        { 0,                            XK_F2,                     spawn,          SHCMD("st -t float -g 126x42 -e app-mail") }, // Mail
	{ 0,                            XK_F3,                     spawn,          SHCMD("st -t float -g 126x42 -e app-rss") }, // RSS
	{ 0,                            XK_F4,                     spawn,          SHCMD("st -t float -g 65x20 -e calc") }, // Calculator
	{ 0,                            XK_F5,                     spawn,          SHCMD("st -t float -g 170x32 -e ncmpcpp") }, // Music
	{ 0,                            XK_F6,                     spawn,          SHCMD("st -t float -g 126x42 -e app-time") }, // Calendar
        { 0,                            XK_F7,                     spawn,          SHCMD("nicotine") }, // Nicotine
	{ 0,                            XK_F8,                     spawn,          SHCMD("st -t float -g 126x39 -e ollama-run") }, // Ollama LLM client
	TAGKEYS(                        XK_1,                                      0)
	TAGKEYS(                        XK_2,                                      1)
	TAGKEYS(                        XK_3,                                      2)
	TAGKEYS(                        XK_4,                                      3)
	TAGKEYS(                        XK_5,                                      4)
	TAGKEYS(                        XK_6,                                      5)
	TAGKEYS(                        XK_7,                                      6)
	TAGKEYS(                        XK_8,                                      7)
	TAGKEYS(                        XK_9,                                      8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigstatusbar,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigstatusbar,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigstatusbar,   {.i = 3} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

