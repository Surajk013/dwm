/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>


/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int gappx     = 3;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "JetBrains Mono:size=11", "JoyPixels:pixelsize=11:antialias=true:autohint=true"};
static const char dmenufont[]       = "JetBrains Mono:size=11";
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
static const char *tags[] = { "📅","🌐","</>","🖥️","✏️","🎮" };

void view_adjacent(const Arg *arg) {
	int i, curtag = -1;
	for (i = 0; i < LENGTH(tags); i++) {
		if (selmon->tagset[selmon->seltags] & (1 << i)) {
			curtag = i;
			break;
		}
	}
	if (curtag >= 0) {
		int newtag = (curtag + arg->i + LENGTH(tags)) % LENGTH(tags);
		const Arg a = {.ui = 1 << newtag};
		view(&a);
	}
}

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class                        instance   title         tags mask  isfloating  isterminal  noswallow  monitor */
	{ "TelegramDesktop",              NULL,     NULL,           0,         1,          0,           0,        -1 },
	{ "whatsapp-for-linux",           NULL,     NULL,           0,         1,          0,           0,        -1 },
	{ "obs",                          NULL,     NULL,           1<<3,      0,          0,           0,        -1 },
	{ "Lutris",                       NULL,     NULL,           0,         1,          0,           0,        -1 },
	{ "firefox",   		                NULL,     NULL,           1<<2,      0,          0,          -1,        -1 },
	{ "St",                           NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ "gnome-calculator",             NULL,     NULL,           0,         1,          1,           0,        -1 },
	{ "magnus",		                    NULL,     NULL,           0,         1,          1,           0,        -1 },
	{ "kitty",		                    NULL,     NULL,           1<<2,      0,          1,           1,        -1 },
	{ "Google-chrome",      		      NULL,     NULL,           1<<1,      0,          0,           0,        -1 },
	{ "ONLYOFFICE",		                NULL,     NULL,           1<<0,      0,          0,           0,        -1 },
	{ "kdenlive",		                  NULL,     NULL,           1<<4,      0,          0,           0,        -1 },
	{  NULL,                          NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

/* volume functions */
static const char *upvol[] = { "sh", "-c", "pactl set-sink-mute @DEFAULT_SINK@ 0; pactl set-sink-volume @DEFAULT_SINK@ +1%", NULL };
static const char *downvol[] = { "pactl", "set-sink-volume", "@DEFAULT_SINK@", "-1%", NULL };
static const char *mutevol[] = { "pactl", "set-sink-mute", "@DEFAULT_SINK@", "toggle", NULL };
static const char *calcmd[]  = { "gnome-calculator" , NULL };
static const char *sss[] = { "sss" , NULL  };

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle }, // Each app takes the whole screen while the earlier one hides behind 
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
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont};
static const char *termcmd[]  = { "kitty", NULL };
static const char *termst[]  = { "st", NULL };

static Key keys[] = {
	/* modifier                           key               function                           argument */
	{ MODKEY|ControlMask,                 XK_l,                spawn,                SHCMD("xsecurelock") },
	{ MODKEY,                             XK_k,           focusstack,                          {.i = -1 } },
	{ MODKEY,                             XK_j,           focusstack,                          {.i = +1 } },
	{ MODKEY,                             XK_h,             setmfact,                        {.f = -0.05} },
	{ MODKEY,                             XK_l,             setmfact,                        {.f = +0.05} },
	{ MODKEY,                             XK_g,            togglebar,                                 {0} },
	{ MODKEY,                             XK_z,                 zoom,                                 {0} },
	{ MODKEY,	                  		      XK_c,                spawn,                      {.v = calcmd } },
	{ MODKEY,			                        XK_n,                spawn,                       SHCMD("nemo") },
	{ MODKEY|ShiftMask,         		      XK_v,                spawn,                       SHCMD("chis") },
	{ MODKEY|ShiftMask,		                XK_z,                spawn,                     SHCMD("magnus") },
	{ MODKEY,		                  	      XK_t,                spawn,                   SHCMD("nautilus") },
	{ MODKEY,                             XK_d,           incnmaster,                          {.i = -1 } },
	{ MODKEY,                             XK_s,           incnmaster,                          {.i = +1 } },
	{ MODKEY,		                          XK_q,           killclient,                                 {0} },
	{ MODKEY,                             XK_w,            setlayout,                  {.v = &layouts[0]} },
	{ MODKEY,                             XK_e,            setlayout,                  {.v = &layouts[1]} },
	{ MODKEY,                             XK_r,            setlayout,                  {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,                   XK_r,  	    togglefloating,                                 {0} },
	{ MODKEY,                             XK_t,  	         setlayout,                                 {0} },
  { MODKEY,                             XK_f,        togglefullscr,                                 {0} },
	{ MODKEY,                         XK_space,                spawn,                    {.v = dmenucmd } },
	{ MODKEY,			                         XK_x,          	   spawn,                     {.v = termcmd } },
	{ MODKEY,			                    XK_Return,               spawn,                      {.v = termst } },
	{ MODKEY|ControlMask,                  XK_s,               spawn,          SHCMD("systemctl suspend") },
	{ MODKEY,                            XK_Tab,                view,                                 {0} },
	{ MODKEY,                              XK_0,                view,                         {.ui = ~0 } },
	{ MODKEY|ShiftMask,                    XK_0,                 tag,                         {.ui = ~0 } },
	{ MODKEY,                          XK_comma,            focusmon,                          {.i = -1 } },
	{ MODKEY,                         XK_period,            focusmon,                          {.i = +1 } },
	{ MODKEY|ShiftMask,                XK_comma,              tagmon,                          {.i = -1 } },
	{ MODKEY|ShiftMask,                XK_period,             tagmon,                          {.i = +1 } },
	{ MODKEY,			                      XK_minus,            setgaps,	                         {.i = -1 } },
	{ MODKEY,			                      XK_equal,            setgaps,	                         {.i = +1 } },
	{ MODKEY|ShiftMask,		              XK_equal,            setgaps,	                         {.i =  0 } },
  { MODKEY,                           XK_Right,      view_adjacent,                          {.i = +1 } },
  { MODKEY,                            XK_Left,      view_adjacent,                          {.i = -1 } },
	{ MODKEY,                              XK_F5,               xrdb,                        {.v = NULL } },
	{ 0,                  XF86XK_MonBrightnessUp,              spawn,       SHCMD("brightnessctl set +1%") },
	{ 0,                XF86XK_MonBrightnessDown,              spawn,       SHCMD("brightnessctl set 1%-") },
	{ 0,                 XF86XK_AudioLowerVolume,              spawn,                      {.v = downvol } },
	{ 0,                        XF86XK_AudioMute,              spawn,                      {.v = mutevol } },
	{ 0,                 XF86XK_AudioRaiseVolume,              spawn,                      {.v = upvol   } },
	{ 0,                                XK_Print,              spawn,                      {.v = mutevol } }, // Print Screen
	{ 0,                          XK_Scroll_Lock,   	         spawn,                      {.v = downvol } }, // Scroll Lock
	{ 0,                                XK_Pause,         	   spawn,                        {.v = upvol } },  // Pause/Break
	{MODKEY|ControlMask, 			          XK_Print, 		         spawn,                           {.v = sss} }, // screenshot
	TAGKEYS(                           XK_KP_End,                                                         0)
  TAGKEYS(                          XK_KP_Down,                                                         1)
	TAGKEYS(                          XK_KP_Next,                                                         2)
  TAGKEYS(                          XK_KP_Left,                                                         3)
	TAGKEYS(                         XK_KP_Begin,                                                         4)
  TAGKEYS(                         XK_KP_Right,                                                         5)
	TAGKEYS(                          XK_KP_Home,                                                         6)
  TAGKEYS(                            XK_KP_Up,                                                         7)
	TAGKEYS(                         XK_KP_Prior,                                                         8)
  TAGKEYS(                        XK_KP_Insert,                                                         9)
	{ MODKEY|ShiftMask,                 		XK_q,              quit,                                   {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
/* click                event mask      button          function        argument */
  { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
  { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
  { ClkWinTitle,          0,              Button2,        zoom,           {0} },
  { ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
  { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
  { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
  { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
  { ClkTagBar,            0,              Button1,        view,           {0} },
  { ClkTagBar,            0,              Button3,        toggleview,     {0} },
  { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
  { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
