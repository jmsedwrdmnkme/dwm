/* See LICENSE file for copyright and license details. */

/* appearance */
static unsigned int borderpx        = 1;        /* border pixel of windows */
static const unsigned int gappx     = 16;       /* gaps between windows */
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
	{ "Nsxiv",    NULL,       NULL,           0,            1,           -1 },
	{ "feh",      NULL,       NULL,           0,            1,           -1 },
	{ "mpv",      NULL,       NULL,           0,            1,           -1 },
	{ "St",       NULL,       "float",        0,            1,           -1 },
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int statusall   = 1;    /* 1 means status is shown in all bars, not just active monitor */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "﬿",      tile },    /* first entry is default */
	{ "",      NULL },    /* no layout function means floating behavior */
	{ "[M]",    monocle },
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
		{ "snap",          	INTEGER, &snap },
		{ "showbar",          	INTEGER, &showbar },
		{ "topbar",          	INTEGER, &topbar },
		{ "nmaster",          	INTEGER, &nmaster },
		{ "resizehints",       	INTEGER, &resizehints },
		{ "mfact",      	FLOAT,   &mfact },
};

#include <X11/XF86keysym.h>

static Key keys[] = {
	/* modifier                     key                        function        argument */
	{ MODKEY,                       XK_d,                      spawn,          {.v = dmenucmd } }, // Open dmenu
	{ MODKEY,                       XK_Return,                 spawn,          {.v = termcmd } }, // Spawn terminal window
	{ MODKEY,                       XK_grave,                  togglescratch,  {.v = scratchpadcmd } }, // Spawn terminal scratchpad window
	{ MODKEY|ShiftMask,             XK_b,                      togglebar,      {0} }, // Hide/Show top bar
	{ MODKEY,                       XK_l,                      focusstack,     {.i = +1 } }, // Move up window stack
	{ MODKEY,                       XK_h,                      focusstack,     {.i = -1 } }, // Move down window stack
	{ MODKEY|ShiftMask,             XK_j,                      incnmaster,     {.i = +1 } }, // Move up window layouts
	{ MODKEY|ShiftMask,             XK_k,                      incnmaster,     {.i = -1 } }, // Move down window layouts
	{ MODKEY|ShiftMask,             XK_h,                      setmfact,       {.f = -0.05} }, // Decrease size of master
	{ MODKEY|ShiftMask,             XK_l,                      setmfact,       {.f = +0.05} }, // Increase size of master
	{ MODKEY|ShiftMask,             XK_Return,                 zoom,           {0} }, // Switch master with focused window else second in stack
	{ MODKEY,                       XK_Tab,                    view,           {0} }, // Swtich focused tag to master else second tag
	{ MODKEY,                       XK_q,                      killclient,     {0} }, // Quit current focused window
	{ MODKEY,                       XK_t,                      setlayout,      {.v = &layouts[0]} }, // Set tiling layout
	{ MODKEY,                       XK_f,                      setlayout,      {.v = &layouts[1]} }, // Set floating layout
	{ MODKEY,                       XK_m,                      setlayout,      {.v = &layouts[2]} }, // Set monocle layout
	{ MODKEY,                       XK_space,                  setlayout,      {0} }, // Toggle between last two used layouts, else tiling and floating
	{ MODKEY|ShiftMask,             XK_space,                  togglefloating, {0} }, // Toggle selected window floating
	{ MODKEY,                       XK_0,                      view,           {.ui = ~0 } }, // View all tags
	{ MODKEY|ShiftMask,             XK_0,                      tag,            {.ui = ~0 } }, // View current window on all tags
	{ MODKEY,                       XK_comma,                  focusmon,       {.i = -1 } }, // Focus monitor(s) to left
	{ MODKEY,                       XK_period,                 focusmon,       {.i = +1 } }, // Focus monitor(s) to right
	{ MODKEY|ShiftMask,             XK_comma,                  tagmon,         {.i = -1 } }, // Send window to monitor(s) to left
	{ MODKEY|ShiftMask,             XK_period,                 tagmon,         {.i = +1 } }, // Send window to monitor(s) to right
	{ MODKEY,                       XK_a,                      spawn,          SHCMD("album-art") }, // View current track album art
	{ MODKEY,                       XK_c,                      spawn,          SHCMD("get-colour") }, // Copy HEX colour beneath cursor
	{ MODKEY,                       XK_o,                      spawn,          SHCMD("dmenu-websearch") }, // Dmenu as browser URL bar
	{ MODKEY,                       XK_p,                      spawn,          SHCMD("dmenu-pass -p") }, // Dmenu password selector
	{ MODKEY|ShiftMask,             XK_p,                      spawn,          SHCMD("power-menu") }, // Prompt with power menu
	{ MODKEY,                       XK_u,                      spawn,          SHCMD("dmenu-pass -u") }, // Dmenu pass username selector
	{ MODKEY,                       XK_a,                      spawn,          SHCMD("dmenu-pass -a") }, // Dmenu pass OTP code selector
	{ MODKEY,                       XK_y,                      spawn,          SHCMD("dmenu-ytfzf") }, // Dmenu YouTube fzf player
	{ MODKEY,                       XK_s,                      spawn,          SHCMD("screen-capture") }, // Start screen capture script
	{ MODKEY,                       XK_v,                      spawn,          SHCMD("mpvnohup") }, // Dmenu mpv video URL player
	{ MODKEY,                       XK_k,                      spawn,          SHCMD("keychron-functions-fix") }, // Set external keyboard map and repeat rate correctly
	{ MODKEY,                       XK_e,                      spawn,          SHCMD("external-monitor -e") }, // Xrandr external monitor setup hotkey
	{ MODKEY|ShiftMask,             XK_e,                      spawn,          SHCMD("external-monitor -d") }, // Xrandr external monitor remove hotkey
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
        { 0,                            XK_F1,                     spawn,          SHCMD("firefox") }, // Browser
        { 0,                            XK_F2,                     spawn,          SHCMD("st -t float -g 126x42 -e app-mail") }, // Mail
	{ 0,                            XK_F3,                     spawn,          SHCMD("st -t float -g 126x42 -e app-rss") }, // RSS
	{ 0,                            XK_F4,                     spawn,          SHCMD("st -t float -g 65x20 -e calc") }, // Calculator
	{ 0,                            XK_F5,                     spawn,          SHCMD("st -t float -g 170x32 -e ncmpcpp") }, // Music
	{ 0,                            XK_F6,                     spawn,          SHCMD("st -t float -g 126x42 -e app-time") }, // Calendar
	{ 0,                            XK_F7,                     spawn,          SHCMD("st -t float -g 126x39 -e sh -c app-weather") }, // Weather
	{ 0,                            XK_F8,                     spawn,          SHCMD("nicotine") }, // Nicotine
	TAGKEYS(                        XK_1,                                      0) // Navigate to tag 1
	TAGKEYS(                        XK_2,                                      1) // Navigate to tag 2
	TAGKEYS(                        XK_3,                                      2) // Navigate to tag 3
	TAGKEYS(                        XK_4,                                      3) // Navigate to tag 4
	TAGKEYS(                        XK_5,                                      4) // Navigate to tag 5
	TAGKEYS(                        XK_6,                                      5) // Navigate to tag 6
	TAGKEYS(                        XK_7,                                      6) // Navigate to tag 7
	TAGKEYS(                        XK_8,                                      7) // Navigate to tag 8
	TAGKEYS(                        XK_9,                                      8) // Navigate to tag 9
        { MODKEY|ShiftMask,             XK_q,                      quit,           {0} }, // Kill dwm process, will restart when dwm started within a loop
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
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

