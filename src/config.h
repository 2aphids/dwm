/* See LICENSE file for copyright and license details. */

/* appearance */
static unsigned int borderpx        = 2;        /* border pixel of windows */
static unsigned int gappx           = 8;        /* gaps between windows */
static unsigned int snap            = 32;       /* snap pixel */
static const int swallowfloating    = 1;        /* 1 means swallow floating windows by default */
static int showbar                  = 1;        /* 0 means no bar */
static int topbar                   = 0;        /* 0 means bottom bar */
static char font[]                  = "IosevkaTerm Nerd Font Mono:size=10";
static char dmenufont[]             = "IosevkaTerm Nerd Font Mono:size=10";
static const char *fonts[]          = { font };
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
	/* class         instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",        NULL,     NULL,           0,         1,          0,           0,        -1 },
	{ "Firefox",     NULL,     NULL,           1 << 8,    0,          0,          -1,        -1 },
	{ "St",          NULL,     NULL,           0,         0,          1,           0,        -1 },
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

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
static const char *dmenucmd[] = { "dmenu_run", "-b", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *playprevcmd[]    = { "playerctl", "-i", "firefox", "previous", NULL };
static const char *playnextcmd[]    = { "playerctl", "-i", "firefox", "next", NULL };
static const char *playpausecmd[]   = { "playpause", NULL };
static const char *startupcmd[]     = { "startup", NULL };
static const char *termcmd[]        = { "st", NULL };
static const char *sscmd[]          = { "ss", NULL };
static const char *picomtogglecmd[] = { "picomtoggle", NULL };
// TODO: static const char *replaycmd[] = { "save-replay", NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
    { "font",            STRING,  &font            },
		{ "font",            STRING,  &dmenufont       },
		{ "normbgcolor",     STRING,  &normbgcolor     },
		{ "normbordercolor", STRING,  &normbordercolor },
		{ "normfgcolor",     STRING,  &normfgcolor     },
		{ "selbgcolor",      STRING,  &selbgcolor      },
		{ "selbordercolor",  STRING,  &selbordercolor  },
		{ "selfgcolor",      STRING,  &selfgcolor      },
		{ "borderpx",        INTEGER, &borderpx        },
		{ "snap",          	 INTEGER, &snap            },
		{ "showbar",         INTEGER, &showbar         },
		{ "topbar",          INTEGER, &topbar          },
		{ "nmaster",         INTEGER, &nmaster         },
		{ "resizehints",     INTEGER, &resizehints     },
    { "gapsize",         INTEGER, &gappx           },
		{ "mfact",      	 	 FLOAT,   &mfact           },
};

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY|ShiftMask,             XK_a,      spawn,          {.v = playprevcmd    } },
	{ MODKEY|ShiftMask,             XK_d,      spawn,          {.v = playnextcmd    } },
	{ MODKEY,                       XK_space,  spawn,          {.v = playpausecmd   } },
	// TODO: { MODKEY|ControlMask,           XK_g,      spawn,          {.v = replaycmd } },
	{ MODKEY|ShiftMask,             XK_w,      spawn,          {.v = startupcmd     } },
	{ MODKEY,                       XK_s,      spawn,          {.v = picomtogglecmd } },
	{ MODKEY|ShiftMask,             XK_s,      spawn,          {.v = sscmd          } },
	{ MODKEY,                       XK_t,      spawn,          {.v = termcmd        } },
	{ MODKEY,                       XK_r,      spawn,          {.v = dmenucmd       } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_g,      modgappx,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_g,      modgappx,       {.i = -1 } },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	// { MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	// { MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	// { MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	// { MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_f,      togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	// { MODKEY,                       XK_F5,     xrdb,           {.v = NULL } }, // reload xresources
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	TAGKEYS(                        XK_z,                      5)
	TAGKEYS(                        XK_x,                      6)
	TAGKEYS(                        XK_c,                      7)
	TAGKEYS(                        XK_v,                      8)
};

#define LMB Button1
#define MMB Button2
#define RMB Button3

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button      function        argument */
	{ ClkLtSymbol,          0,              LMB,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              RMB,        setlayout,      {.v = &layouts[2]} },
	// { ClkWinTitle,          0,           MMB,        zoom,           {0} },
	// { ClkStatusText,        0,           MMB,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         LMB,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         MMB,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         RMB,        resizemouse,    {0} },
	{ ClkTagBar,            0,              LMB,        view,           {0} },
	{ ClkTagBar,            0,              RMB,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         LMB,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         RMB,        toggletag,      {0} },
};

