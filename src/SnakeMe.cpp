/*  =========== Snake Me ========== *\
    (c) 1999-2000 Stephane Magnenat
\*  =============================== */

#include "SnakeMe.h"
#include "SGU.h"
#include "SGU_UI.h"
#include "CSnakeEngine.h"
#include "CSnakeLevelEditor.h"
#include "StringTable.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "SGU_List.h"
#include "SnakeUI.h"

#ifndef NOSOUND
#include <SDL_mixer.h>
#endif



// globals variables
StringTable lang;
CUITheme *theme;
SDL_Surface *ScrInfo;
SGU_Sprite *bigfont;
SGU_Sprite *middlefont;
SGU_Sprite *littlefont;
SGU_Sprite *backgroundsprite;
SGU_Sprite *ctrltypechoice;
SGU_Sprite *ctrluserchoice;
List *snakespritelist;
bool waitforakey;
int winner;
int firstanim;
int numberofplayingsnakes;
List *LocalPlayerType;
bool run=true;
bool fullscreen, noaudio;
SDL_Joystick *joysticks[2];
int numjoystick;

// User Interface variables
CUIScreen *MainMenu;
CUIScreen *ControlMenu;
CUIScreen *AdvancedSetup;
CUIScreen *LocalMenu;
CUIScreen InGameMenu(2);
SGU_RegionSaver InGameMenuBG;
CSnakeEngine *SnakeEngine;
CSnakeLevelEditor *LevelEditor;

typedef enum 
{	MAINMENU,
	CONTROLMENU, 
	NAMEINPUT, 
	LOCALMENU,
	CLIENTIPINPUT,
	SERVERPARAM,
	NETWORKGAME,
	ADVANCEDSETUP,
	LEVELEDITOR,
	HIGHSCORE
} EGameState;

EGameState GameState,ReturnScreen;

typedef enum
{
	FIRSTDRAW,
	LIGHTREDRAW
} EDrawState;

EDrawState DrawState;

SUIRedraw UIRedraw;
SButtonMap PlayerButtons[2];
SButtonMap TempPlayerButtons[2];
SAdvancedOptions AdvancedOptions;
SAdvancedOptions TempAdvancedOptions;
SGU_RGBA ClBlue100,ClWhite,ClGrey50,ClBlack,ClRed100;

#ifndef NOSOUND
// Audio variables
static int audio_open = 0;
static Mix_Music *music = NULL;
const int numbsnd=6;
Mix_Chunk *sounds[numbsnd];
int audio_rate = 44100;
Uint16 audio_format = AUDIO_S16;
int audio_channels = 2;
int audio_buffers = 4096;
List musiclist;
lpListNode actualmusic;

// Sound Handle

void InitAudio(void);
void CloseAudio(void);
void LoadMusic(void);
void LoadSound(void);
void HandleAudio(void);

void InitAudio(void)
{
	// Open Audio
	if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) < 0)
	{
		fprintf(stderr, "MIX : Couldn't open audio: %s\n", SDL_GetError());
		exit(2);
	}
	else
	{
		Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
		printf("MIX : Opened audio at %d Hz %d bit %s, %d bytes audio buffer\n", audio_rate,
			(audio_format&0xFF),
			(audio_channels > 1) ? "stereo" : "mono", 
			audio_buffers );
	}
	audio_open = 1;
	LoadSound();
	LoadMusic();
	actualmusic=musiclist.GetHead();
	if (actualmusic)
	{
		Mix_HookMusicFinished(HandleAudio);
		printf("ZIK : Playing [%s]\n",(char *)(actualmusic->Element));
		music=Mix_LoadMUS((char *)(actualmusic->Element));
		Mix_PlayMusic(music, 0);
	}
	else
	{
		fprintf(stderr,"ZIK : Error, no Music found\n");
	}
}

void CloseAudio(void)
{
	if ( music )
	{
		Mix_FreeMusic(music);
		music = NULL;
	}
	for (int n=0;n<numbsnd;n++)
	{
		Mix_FreeChunk(sounds[n]);
	}
	if ( audio_open )
	{
		Mix_CloseAudio();
		audio_open = 0;
	}
	printf("MIX : Audio closed\n");
	for (lpListNode node=musiclist.GetHead();node;node=node->Next)
	{
		free (node->Element);
	}
	
}
void LoadMusic(void)
{
	SGU_DirLister *dir;
	char *s,*ts;
	// load mod
	dir=new SGU_DirLister("mod");
	while (s=dir->GetNextName())
	{
		ts=(char *)malloc(strlen(s)+1);
		strcpy(ts,s);
		musiclist.AddElement(ts);
		printf("ZIK : Loading MOD [%s]\n",ts);
	}
	delete dir;

	// load xm
	dir=new SGU_DirLister("xm");
	while (s=dir->GetNextName())
	{
		ts=(char *)malloc(strlen(s)+1);
		strcpy(ts,s);
		musiclist.AddElement(ts);
		printf("ZIK : Loading XM [%s]\n",ts);
	}
	delete dir;

	// load s3m
	dir=new SGU_DirLister("s3m");
	while (s=dir->GetNextName())
	{
		ts=(char *)malloc(strlen(s)+1);
		strcpy(ts,s);
		musiclist.AddElement(ts);
		printf("ZIK : Loading S3M [%s]\n",ts);
	}
	delete dir;

	// load it
	dir=new SGU_DirLister("it");
	while (s=dir->GetNextName())
	{
		ts=(char *)malloc(strlen(s)+1);
		strcpy(ts,s);
		musiclist.AddElement(ts);
		printf("ZIK : Loading IT [%s]\n",ts);
	}
	delete dir;
}

void LoadSound(void)
{
	const int sndvol=100;
	sounds[0]=Mix_LoadWAV("0.wav");
	Mix_VolumeChunk(sounds[0],sndvol);
	sounds[1]=Mix_LoadWAV("1.wav");
	Mix_VolumeChunk(sounds[1],sndvol);
	sounds[2]=Mix_LoadWAV("2.wav");
	Mix_VolumeChunk(sounds[2],sndvol);
	sounds[3]=Mix_LoadWAV("3.wav");
	Mix_VolumeChunk(sounds[3],sndvol);
	sounds[4]=Mix_LoadWAV("4.wav");
	Mix_VolumeChunk(sounds[4],sndvol);
	sounds[5]=Mix_LoadWAV("5.wav");
	Mix_VolumeChunk(sounds[5],sndvol);
	// to play : Mix_PlayChannel(-1,sounds[n],0);
}

void HandleAudio(void)
{
	Mix_FreeMusic(music);
	if (actualmusic->Next)
	{
		actualmusic=actualmusic->Next;
	}
	else
	{
		actualmusic=musiclist.GetHead();
	}
	printf("ZIK : Playing [%s]\n",(char *)(actualmusic->Element));
	music=Mix_LoadMUS((char *)(actualmusic->Element));
	Mix_PlayMusic(music, 0);
}
#endif

void AdvancedSetupUpdateText(void)
{
	#define ASTEXTBASE 15
	char ctemp[32];
	sprintf(ctemp,"%d fps",AdvancedOptions.speed);
	((CUITextEdit *)(AdvancedSetup->objects[ASTEXTBASE]))->SendMessage(INTERNEV_SETTEXT,(ssize_t)(void *)ctemp,0,ScrInfo);
	sprintf(ctemp,"%d",AdvancedOptions.deflng);
	((CUITextEdit *)(AdvancedSetup->objects[ASTEXTBASE+1]))->SendMessage(INTERNEV_SETTEXT,(ssize_t)(void *)ctemp,0,ScrInfo);
	sprintf(ctemp,"%d",AdvancedOptions.inclng);
	((CUITextEdit *)(AdvancedSetup->objects[ASTEXTBASE+2]))->SendMessage(INTERNEV_SETTEXT,(ssize_t)(void *)ctemp,0,ScrInfo);
	sprintf(ctemp,"%d",AdvancedOptions.nbmeal);
	((CUITextEdit *)(AdvancedSetup->objects[ASTEXTBASE+3]))->SendMessage(INTERNEV_SETTEXT,(ssize_t)(void *)ctemp,0,ScrInfo);
	sprintf(ctemp,"%d",AdvancedOptions.mealchangetime*5);
	((CUITextEdit *)(AdvancedSetup->objects[ASTEXTBASE+4]))->SendMessage(INTERNEV_SETTEXT,(ssize_t)(void *)ctemp,0,ScrInfo);
	sprintf(ctemp,"%d",AdvancedOptions.nbbonus);
	((CUITextEdit *)(AdvancedSetup->objects[ASTEXTBASE+5]))->SendMessage(INTERNEV_SETTEXT,(ssize_t)(void *)ctemp,0,ScrInfo);
	sprintf(ctemp,"%d",AdvancedOptions.bonuschangetime*5);
	((CUITextEdit *)(AdvancedSetup->objects[ASTEXTBASE+6]))->SendMessage(INTERNEV_SETTEXT,(ssize_t)(void *)ctemp,0,ScrInfo);
	sprintf(ctemp,"%d",AdvancedOptions.maxpoint);
	((CUITextEdit *)(AdvancedSetup->objects[ASTEXTBASE+7]))->SendMessage(INTERNEV_SETTEXT,(ssize_t)(void *)ctemp,0,ScrInfo);
	sprintf(ctemp,"%d",AdvancedOptions.ptgetmeal);
	((CUITextEdit *)(AdvancedSetup->objects[ASTEXTBASE+8]))->SendMessage(INTERNEV_SETTEXT,(ssize_t)(void *)ctemp,0,ScrInfo);
	sprintf(ctemp,"%d",AdvancedOptions.ptgethit);
	((CUITextEdit *)(AdvancedSetup->objects[ASTEXTBASE+9]))->SendMessage(INTERNEV_SETTEXT,(ssize_t)(void *)ctemp,0,ScrInfo);
	sprintf(ctemp,"%d",AdvancedOptions.ptgetsnake);
	((CUITextEdit *)(AdvancedSetup->objects[ASTEXTBASE+10]))->SendMessage(INTERNEV_SETTEXT,(ssize_t)(void *)ctemp,0,ScrInfo);
	sprintf(ctemp,"%d",AdvancedOptions.ptloosecrash);
	((CUITextEdit *)(AdvancedSetup->objects[ASTEXTBASE+11]))->SendMessage(INTERNEV_SETTEXT,(ssize_t)(void *)ctemp,0,ScrInfo);
	sprintf(ctemp,"%d",AdvancedOptions.ptloosehit);
	((CUITextEdit *)(AdvancedSetup->objects[ASTEXTBASE+12]))->SendMessage(INTERNEV_SETTEXT,(ssize_t)(void *)ctemp,0,ScrInfo);
}

void AdvancedSetupUpdateVal(void)
{
	((CUIHorzValueBar *)(AdvancedSetup->objects[2]))->SetVal(AdvancedOptions.speed);
	((CUIHorzValueBar *)(AdvancedSetup->objects[3]))->SetVal(AdvancedOptions.deflng);
	((CUIHorzValueBar *)(AdvancedSetup->objects[4]))->SetVal(AdvancedOptions.inclng);

	((CUIHorzValueBar *)(AdvancedSetup->objects[5]))->SetVal(AdvancedOptions.nbmeal);
	((CUIHorzValueBar *)(AdvancedSetup->objects[6]))->SetVal(AdvancedOptions.mealchangetime);
	((CUIHorzValueBar *)(AdvancedSetup->objects[7]))->SetVal(AdvancedOptions.nbbonus);
	((CUIHorzValueBar *)(AdvancedSetup->objects[8]))->SetVal(AdvancedOptions.bonuschangetime);
	
	((CUIHorzValueBar *)(AdvancedSetup->objects[9]))->SetVal(AdvancedOptions.maxpoint);
	((CUIHorzValueBar *)(AdvancedSetup->objects[10]))->SetVal(AdvancedOptions.ptgetmeal);
	((CUIHorzValueBar *)(AdvancedSetup->objects[11]))->SetVal(AdvancedOptions.ptgethit);
	((CUIHorzValueBar *)(AdvancedSetup->objects[12]))->SetVal(AdvancedOptions.ptgetsnake);
	((CUIHorzValueBar *)(AdvancedSetup->objects[13]))->SetVal(AdvancedOptions.ptloosecrash);
	((CUIHorzValueBar *)(AdvancedSetup->objects[14]))->SetVal(AdvancedOptions.ptloosehit);
}

void AdvancedSetupSaveVal(void)
{
	AdvancedOptions.speed=((CUIHorzValueBar *)(AdvancedSetup->objects[2]))->GetVal();
	AdvancedOptions.deflng=((CUIHorzValueBar *)(AdvancedSetup->objects[3]))->GetVal();
	AdvancedOptions.inclng=((CUIHorzValueBar *)(AdvancedSetup->objects[4]))->GetVal();
	AdvancedOptions.nbmeal=((CUIHorzValueBar *)(AdvancedSetup->objects[5]))->GetVal();
	AdvancedOptions.mealchangetime=((CUIHorzValueBar *)(AdvancedSetup->objects[6]))->GetVal();
	AdvancedOptions.nbbonus=((CUIHorzValueBar *)(AdvancedSetup->objects[7]))->GetVal();
	AdvancedOptions.bonuschangetime=((CUIHorzValueBar *)(AdvancedSetup->objects[8]))->GetVal();
	AdvancedOptions.maxpoint=((CUIHorzValueBar *)(AdvancedSetup->objects[9]))->GetVal();
	AdvancedOptions.ptgetmeal=((CUIHorzValueBar *)(AdvancedSetup->objects[10]))->GetVal();
	AdvancedOptions.ptgethit=((CUIHorzValueBar *)(AdvancedSetup->objects[11]))->GetVal();
	AdvancedOptions.ptgetsnake=((CUIHorzValueBar *)(AdvancedSetup->objects[12]))->GetVal();
	AdvancedOptions.ptloosecrash=((CUIHorzValueBar *)(AdvancedSetup->objects[13]))->GetVal();
	AdvancedOptions.ptloosehit=((CUIHorzValueBar *)(AdvancedSetup->objects[14]))->GetVal();
}

void SetDefaultAdvancedOptions(void)
{
	AdvancedOptions.speed=12;
	AdvancedOptions.deflng=10;
	AdvancedOptions.inclng=5;

	AdvancedOptions.nbmeal=10;
	AdvancedOptions.mealchangetime=15;
	AdvancedOptions.nbbonus=10;
	AdvancedOptions.bonuschangetime=15;

	AdvancedOptions.maxpoint=10;
	AdvancedOptions.ptgetmeal=1;
	AdvancedOptions.ptgethit=2;
	AdvancedOptions.ptgetsnake=3;
	AdvancedOptions.ptloosecrash=3;
	AdvancedOptions.ptloosehit=1;
}

void SwapAdvancedOptionsLittleEndian(SAdvancedOptions *AdvancedOptions)
{
	(*AdvancedOptions).speed=SDL_SwapLE32((*AdvancedOptions).speed);
	(*AdvancedOptions).deflng=SDL_SwapLE32((*AdvancedOptions).deflng);
	(*AdvancedOptions).inclng=SDL_SwapLE32((*AdvancedOptions).inclng);

	(*AdvancedOptions).nbmeal=SDL_SwapLE32((*AdvancedOptions).nbmeal);
	(*AdvancedOptions).mealchangetime=SDL_SwapLE32((*AdvancedOptions).mealchangetime);
	(*AdvancedOptions).nbbonus=SDL_SwapLE32((*AdvancedOptions).nbbonus);
	(*AdvancedOptions).bonuschangetime=SDL_SwapLE32((*AdvancedOptions).bonuschangetime);

	(*AdvancedOptions).maxpoint=SDL_SwapLE32((*AdvancedOptions).maxpoint);
	(*AdvancedOptions).ptgetmeal=SDL_SwapLE32((*AdvancedOptions).ptgetmeal);
	(*AdvancedOptions).ptgethit=SDL_SwapLE32((*AdvancedOptions).ptgethit);
	(*AdvancedOptions).ptgetsnake=SDL_SwapLE32((*AdvancedOptions).ptgetsnake);
	(*AdvancedOptions).ptloosecrash=SDL_SwapLE32((*AdvancedOptions).ptloosecrash);
	(*AdvancedOptions).ptloosehit=SDL_SwapLE32((*AdvancedOptions).ptloosehit);
}



void UIBuilder(void)
{
	int dx,dy,n;
	MainMenu = new CUIScreen(5);
	MainMenu->objects[0]= new CUINormalButton(120,40,410,45,theme,bigfont,lang.GetString("[GAME]"),true);
	MainMenu->objects[1]= new CUINormalButton(120,120,410,45,theme,bigfont,lang.GetString("[CONTROLS]"),true);
	MainMenu->objects[2]= new CUINormalButton(120,200,410,45,theme,bigfont,lang.GetString("[ADS]"),true);
	MainMenu->objects[3]= new CUINormalButton(120,280,410,45,theme,bigfont,lang.GetString("[LVLED]"),true);
	MainMenu->objects[4]= new CUINormalButton(120,360,410,45,theme,bigfont,lang.GetString("[QUIT]"),true);
	
	ControlMenu = new CUIScreen(17);
	ControlMenu->objects[0]=new CUINormalButton(20,420,170,45,theme,bigfont,lang.GetString("[CANCEL]"),true);
	ControlMenu->objects[1]=new CUINormalButton(275,430,80,25,theme,middlefont,lang.GetString("[DEFAULT]"),true);
	ControlMenu->objects[2]=new CUINormalButton(450,420,170,45,theme,bigfont,lang.GetString("[SAVE]"),true);
	
	ControlMenu->objects[3]=new CUINormalButton(30,100,150,25,theme,middlefont,"",true);
	ControlMenu->objects[4]=new CUINormalButton(30,140,150,25,theme,middlefont,"",true);
	ControlMenu->objects[5]=new CUINormalButton(30,180,150,25,theme,middlefont,"",true);
	ControlMenu->objects[6]=new CUINormalButton(30,220,150,25,theme,middlefont,"",true);
	ControlMenu->objects[7]=new CUINormalButton(30,270,150,25,theme,middlefont,"",true);
	ControlMenu->objects[8]=new CUINormalButton(30,310,150,25,theme,middlefont,"",true);
	ControlMenu->objects[9]=new CUINormalButton(30,360,150,25,theme,middlefont,"",true);	
	ControlMenu->objects[10]=new CUINormalButton(460,100,150,25,theme,middlefont,"",true);
	ControlMenu->objects[11]=new CUINormalButton(460,140,150,25,theme,middlefont,"",true);
	ControlMenu->objects[12]=new CUINormalButton(460,180,150,25,theme,middlefont,"",true);
	ControlMenu->objects[13]=new CUINormalButton(460,220,150,25,theme,middlefont,"",true);
	ControlMenu->objects[14]=new CUINormalButton(460,270,150,25,theme,middlefont,"",true);
	ControlMenu->objects[15]=new CUINormalButton(460,310,150,25,theme,middlefont,"",true);
	ControlMenu->objects[16]=new CUINormalButton(460,360,150,25,theme,middlefont,"",true);
	
	LocalMenu = new CUIScreen(48);
	for (n=0;n<8;n++)
	{
		dx=((n%2)*320)+10;
		dy=155+5+(n>>1)*60;
		LocalMenu->objects[(n*5)]=new CUIBoolChooser(dx+25,dy+15,20,20,theme,middlefont,"");
		LocalMenu->objects[(n*5)+1]=new CUITextEdit(dx+60,dy+3,200,22,theme,middlefont,"",false);
		LocalMenu->objects[(n*5)+2]=new CUIMultiGfxButton(dx+60,dy+29,34,18,theme,ctrltypechoice,false);
		LocalMenu->objects[(n*5)+3]=new CUIMultiGfxButton(dx+110,dy+29,34,18,theme,ctrluserchoice,false);
		LocalMenu->objects[(n*5)+4]=new CUISnakeChooser(dx+160,dy+29,100,18,theme,snakespritelist);
	}
	LocalMenu->objects[40]=new CUINormalButton(20,420,170,45,theme,bigfont,lang.GetString("[CANCEL]"),true);
	LocalMenu->objects[41]=new CUINormalButton(450,420,170,45,theme,bigfont,lang.GetString("[PLAY]"),true);
	LocalMenu->objects[42]=new CUINormalButton(220,410,200,25,theme,middlefont,lang.GetString("[CONTROLS]"),true);
	LocalMenu->objects[43]=new CUINormalButton(220,445,200,25,theme,middlefont,lang.GetString("[ADS]"),true);
	LocalMenu->objects[44]=new CUIFileChooser(25,380-260,160,22,theme,middlefont,true);
	((CUIFileChooser *)(LocalMenu->objects[44]))->Scan("sml");
	LocalMenu->objects[45]=new CUIFileChooser(450,380-260,160,22,theme,middlefont,true);
	((CUIFileChooser *)(LocalMenu->objects[45u]))->Scan("smt");
	LocalMenu->objects[46]=new CUILevelViewer(25,267-260,theme);
	LocalMenu->objects[47]=new CUIThemeViewer(450,267-260,theme);

	AdvancedSetup=new CUIScreen(29);
	AdvancedSetup->objects[0]=new CUINormalButton(450,420,170,45,theme,bigfont,lang.GetString("[SAVE]"),true);
	AdvancedSetup->objects[1]=new CUINormalButton(20,420,170,45,theme,bigfont,lang.GetString("[CANCEL]"),true);
	
	AdvancedSetup->objects[2]=new CUIHorzValueBar(425,60, 100,16,theme,1,30,AdvancedOptions.speed);
	AdvancedSetup->objects[3]=new CUIHorzValueBar(425,85, 100,16,theme,2,20,AdvancedOptions.deflng);
	AdvancedSetup->objects[4]=new CUIHorzValueBar(425,110, 100,16,theme,0,20,AdvancedOptions.inclng);

	AdvancedSetup->objects[5]=new CUIHorzValueBar(425,145, 100,16,theme,0,20,AdvancedOptions.nbmeal);
	AdvancedSetup->objects[6]=new CUIHorzValueBar(425,170, 100,16,theme,0,20,AdvancedOptions.mealchangetime);
	AdvancedSetup->objects[7]=new CUIHorzValueBar(425,195, 100,16,theme,0,20,AdvancedOptions.nbbonus);
	AdvancedSetup->objects[8]=new CUIHorzValueBar(425,220, 100,16,theme,0,20,AdvancedOptions.bonuschangetime);
	
	AdvancedSetup->objects[9]=new CUIHorzValueBar(425,255, 100,16,theme,1,30,AdvancedOptions.maxpoint);
	AdvancedSetup->objects[10]=new CUIHorzValueBar(425,280, 100,16,theme,0,10,AdvancedOptions.ptgetmeal);
	AdvancedSetup->objects[11]=new CUIHorzValueBar(425,305, 100,16,theme,0,10,AdvancedOptions.ptgethit);
	AdvancedSetup->objects[12]=new CUIHorzValueBar(425,330, 100,16,theme,0,10,AdvancedOptions.ptgetsnake);
	AdvancedSetup->objects[13]=new CUIHorzValueBar(425,355, 100,16,theme,0,10,AdvancedOptions.ptloosecrash);
	AdvancedSetup->objects[14]=new CUIHorzValueBar(425,380, 100,16,theme,0,10,AdvancedOptions.ptloosehit);
	
	for (n=15;n<18;n++)
		AdvancedSetup->objects[n]=new CUITextEdit(540,60+(25*(n-15)),75,20,theme,middlefont,"",true);
	
	for (n=18;n<22;n++)
		AdvancedSetup->objects[n]=new CUITextEdit(540,145+(25*(n-18)),75,20,theme,middlefont,"",true);
	
	for (n=22;n<28;n++)
		AdvancedSetup->objects[n]=new CUITextEdit(540,255+(25*(n-22)),75,20,theme,middlefont,"",true);
	

	AdvancedSetup->objects[28]=new CUINormalButton(270,430,100,25,theme,middlefont,lang.GetString("[DEFAULT]"),true);
	AdvancedSetupUpdateText();
	InGameMenu.objects[0]= new CUINormalButton(180,180,280,50,theme,bigfont,lang.GetString("[RESGM]"),true);
	InGameMenu.objects[1]= new CUINormalButton(180,250,280,50,theme,bigfont,lang.GetString("[QUITGM]"),true);
	UIRedraw.lastitem=-1;
	UIRedraw.newitem=-1;
}

void UIDestructor(void)
{
	delete MainMenu;
	delete ControlMenu;
	delete LocalMenu;
	delete AdvancedSetup;
}

void DrawBackground(void)
{
	SDL_Rect r;
	r.x=0;
	r.y=0;
	r.w=ScrInfo->w;
	r.h=ScrInfo->h;
	theme->DrawBackground(ScrInfo,&r);
}

void DrawBackgroundRect(int x,int y,int w,int h)
{
	SDL_Rect r;
	r.x=x;
	r.y=y;
	r.w=w;
	r.h=h;
	SDL_Surface *surface=backgroundsprite->GetFrameSurface(0);
	SDL_BlitSurface(surface,&r,ScrInfo,&r);
}

void SetDefaultKeys(void)
{
	PlayerButtons[1].Adown=SDLK_d;
	PlayerButtons[1].Aleft=SDLK_s;
	PlayerButtons[1].Aright=SDLK_f;
	PlayerButtons[1].Aup=SDLK_e;
	PlayerButtons[1].Rleft=SDLK_s;
	PlayerButtons[1].Rright=SDLK_f;
	PlayerButtons[1].Uaction=SDLK_SPACE;
	PlayerButtons[0].Adown=SDLK_DOWN;
	PlayerButtons[0].Aleft=SDLK_LEFT;
	PlayerButtons[0].Aright=SDLK_RIGHT;
	PlayerButtons[0].Aup=SDLK_UP;
	PlayerButtons[0].Rleft=SDLK_LEFT;
	PlayerButtons[0].Rright=SDLK_RIGHT;
	PlayerButtons[0].Uaction=SDLK_RETURN;
}
void SwapLittleEndianKeys(SButtonMap* PlayerButtons)
{
	(*PlayerButtons).Adown=(SDLKey)SDL_SwapLE32((*PlayerButtons).Adown);
	(*PlayerButtons).Aleft=(SDLKey)SDL_SwapLE32((*PlayerButtons).Aleft);
	(*PlayerButtons).Aright=(SDLKey)SDL_SwapLE32((*PlayerButtons).Aright);
	(*PlayerButtons).Aup=(SDLKey)SDL_SwapLE32((*PlayerButtons).Aup);
	(*PlayerButtons).Rleft=(SDLKey)SDL_SwapLE32((*PlayerButtons).Rleft);
	(*PlayerButtons).Rright=(SDLKey)SDL_SwapLE32((*PlayerButtons).Rright);
	(*PlayerButtons).Uaction=(SDLKey)SDL_SwapLE32((*PlayerButtons).Uaction);
}

void SetControlButtonsNames(void)
{
	((CUINormalButton *)(ControlMenu->objects[3]))->SetCaption(SDL_GetKeyName(PlayerButtons[0].Aleft));
	((CUINormalButton *)(ControlMenu->objects[4]))->SetCaption(SDL_GetKeyName(PlayerButtons[0].Aright));
	((CUINormalButton *)(ControlMenu->objects[5]))->SetCaption(SDL_GetKeyName(PlayerButtons[0].Aup));
	((CUINormalButton *)(ControlMenu->objects[6]))->SetCaption(SDL_GetKeyName(PlayerButtons[0].Adown));
	((CUINormalButton *)(ControlMenu->objects[7]))->SetCaption(SDL_GetKeyName(PlayerButtons[0].Rleft));
	((CUINormalButton *)(ControlMenu->objects[8]))->SetCaption(SDL_GetKeyName(PlayerButtons[0].Rright));
	((CUINormalButton *)(ControlMenu->objects[9]))->SetCaption(SDL_GetKeyName(PlayerButtons[0].Uaction));
	((CUINormalButton *)(ControlMenu->objects[10]))->SetCaption(SDL_GetKeyName(PlayerButtons[1].Aleft));
	((CUINormalButton *)(ControlMenu->objects[11]))->SetCaption(SDL_GetKeyName(PlayerButtons[1].Aright));
	((CUINormalButton *)(ControlMenu->objects[12]))->SetCaption(SDL_GetKeyName(PlayerButtons[1].Aup));
	((CUINormalButton *)(ControlMenu->objects[13]))->SetCaption(SDL_GetKeyName(PlayerButtons[1].Adown));
	((CUINormalButton *)(ControlMenu->objects[14]))->SetCaption(SDL_GetKeyName(PlayerButtons[1].Rleft));
	((CUINormalButton *)(ControlMenu->objects[15]))->SetCaption(SDL_GetKeyName(PlayerButtons[1].Rright));
	((CUINormalButton *)(ControlMenu->objects[16]))->SetCaption(SDL_GetKeyName(PlayerButtons[1].Uaction));
}

const char *playerdefaultname[8]={ "Player 1",
							"Player 2",
							"Player 3",
							"Player 4",
							"Player 5",
							"Player 6",
							"Player 7",
							"Player 8" };

struct playerdatainfo
{
	unsigned char namelength;
	unsigned char on_off;
	unsigned char rel_abs;
	unsigned char a_b_ai;
	unsigned char snake;
	unsigned char bla1;
	unsigned char bla2;
	unsigned char bla3;
};


void LoadPlayerName(void)
{
	FILE *fp;
	char temp[256];
	playerdatainfo d;

	fp=fopen("pname.ctl","rb");
	if (fp!=NULL)
	{
		for (int n=0;n<8;n++)
		{
			// load info
			fread(&d,sizeof(playerdatainfo),1,fp);
			
			// load text
			fread(temp,1,d.namelength,fp);
			temp[d.namelength]=0;
			((CUITextEdit *)(LocalMenu->objects[(n*5)+1]))->SetText(temp);
			
			// load others infos
			((CUIBoolChooser *)(LocalMenu->objects[(n*5)]))->SetState((bool)d.on_off);
			((CUIMultiGfxButton *)(LocalMenu->objects[(n*5)+2]))->SetFrame(d.rel_abs);
			((CUIMultiGfxButton *)(LocalMenu->objects[(n*5)+3]))->SetFrame(MIN(d.a_b_ai,((CUIMultiGfxButton *)(LocalMenu->objects[(n*5)+3]))->GetNbFrames()));
			((CUISnakeChooser *)(LocalMenu->objects[(n*5)+4]))->SetVal(d.snake);
		}
		fclose(fp);		
	}
	else
	{
		// Set Default Player Name Text
		for (int n=0;n<8;n++)
		{
			((CUITextEdit *)(LocalMenu->objects[(n*5)+1]))->SetText(playerdefaultname[n]);			
		}
		((CUIBoolChooser *)(LocalMenu->objects[0]))->SetState(true);
		((CUIBoolChooser *)(LocalMenu->objects[5]))->SetState(true);
		((CUIMultiGfxButton *)(LocalMenu->objects[8]))->SetFrame(1);
		((CUIBoolChooser *)(LocalMenu->objects[10]))->SetState(true);
		((CUIMultiGfxButton *)(LocalMenu->objects[13]))->SetFrame(2);
		((CUIBoolChooser *)(LocalMenu->objects[15]))->SetState(true);
		((CUIMultiGfxButton *)(LocalMenu->objects[18]))->SetFrame(2);
	}
}

void SavePlayerName(void)
{
	FILE *fp;
	char *temp;
	playerdatainfo d;

	fp=fopen("pname.ctl","wb");
	if (fp!=NULL)
	{
		for (int n=0;n<8;n++)
		{
			// prepare data
			temp=((CUITextEdit *)(LocalMenu->objects[(n*5)+1]))->GetText();
			d.namelength=(unsigned char)strlen(temp);
			d.on_off=(unsigned char)((CUIBoolChooser *)(LocalMenu->objects[(n*5)]))->GetState();
			d.rel_abs=(unsigned char)((CUIMultiGfxButton *)(LocalMenu->objects[(n*5)+2]))->GetFrame();
			d.a_b_ai=(unsigned char)((CUIMultiGfxButton *)(LocalMenu->objects[(n*5)+3]))->GetFrame();
			d.snake=(unsigned char)((CUISnakeChooser *)(LocalMenu->objects[(n*5)+4]))->GetSel();

			// write data
			fwrite(&d,sizeof(playerdatainfo),1,fp);
			
			// write text
			fwrite(temp,1,d.namelength,fp);
		}
		fclose(fp);
	}
}

void SnakeMeDraw(void);

Uint32 SnakeMeTimer(Uint32 t)
{
	int res;
	res=SnakeEngine->DoStepAndDraw(ScrInfo);
	if (res>=0)
	{
		SDL_SetTimer(0, NULL); 
		winner=res;
		waitforakey=true;
	}
	SDL_UpdateRect(ScrInfo,0,0,0,0);
	return t;
}

// In Game Menu (pause, etc...)
bool SnakeGameMenu(void)
{
	SDL_Event ev;
	int wgt;
	InGameMenuBG.SaveRegion(ScrInfo,160,160,320,160);
	DrawBackgroundRect(160,160,320,160);
	SGU_RectShade(ScrInfo,160,160,320,160,theme->BHiCol,theme->BMidCol,theme->BLowCol);
	InGameMenu.DrawScreen(ScrInfo);
	SDL_UpdateRect(ScrInfo,160,160,320,160);
	while (SDL_WaitEvent(&ev) >= 0 )
	{
		InGameMenu.ProcessDefaultEvent(&ev,ScrInfo);
		wgt=InGameMenu.GetClicked();
		if (wgt>=0)
		{
			if (wgt==0)
			{
				InGameMenuBG.RestaurRegion(ScrInfo);
				return true;
			}
			if (wgt==1)
			{
				InGameMenuBG.RestaurRegion(ScrInfo);
				return false;
			}
		}
	}
	return true;
}

// Snake Game
void SnakeGame(void)
{
	bool gamerun=true;
	SDL_Event ev;
	int winner;
	int n,m,ne,id;

	SnakeEngine->ClearArray(ScrInfo);

	SDL_ShowCursor(0);
	while (gamerun)
	{
		// Wait for timer
		SDL_Delay(1000/AdvancedOptions.speed);

		// Pool events
		while (SDL_PollEvent(&ev)>0)
		{
			if (ev.type==SDL_JOYBUTTONDOWN) // joystick press support
			{
				// go through all players & configs
				for (n=0;n<numberofplayingsnakes;n++)
				{
					id = SnakeEngine->GetID(n);
					ne=((CUIMultiGfxButton *)(LocalMenu->objects[(id*5)+3]))->GetFrame();
					if ((ne>=3) && ((ne-3)==ev.jbutton.which))
					{
						SnakeEngine->GiveOrder(n,Uaction);
					}
				}
			}
			else if (ev.type==SDL_JOYAXISMOTION) // joystick motion support
			{
				// go through all players & configs
				for (n=0;n<numberofplayingsnakes;n++)
				{
					id = SnakeEngine->GetID(n);
					ne=((CUIMultiGfxButton *)(LocalMenu->objects[(id*5)+3]))->GetFrame();
					if ((ne>=3) && ((ne-3)==ev.jaxis.which))
					{
						// relative
						if (((CUIMultiGfxButton *)(LocalMenu->objects[(id*5)+2]))->GetFrame()==1)
						{
							if (ev.jaxis.axis==0)
							{
								if (ev.jaxis.value>0)
									SnakeEngine->GiveOrder(n,Rright);
								else if (ev.jaxis.value<0)
									SnakeEngine->GiveOrder(n,Rleft);
							}
							if (!SnakeEngine->IsUpcoming(n))
								continue;
						}
						// absolute
						if (ev.jaxis.axis==0)
						{
							if (ev.jaxis.value>0)
								SnakeEngine->GiveOrder(n,Aright);
							else if (ev.jaxis.value<0)
								SnakeEngine->GiveOrder(n,Aleft);
						}
						if (ev.jaxis.axis==1)
						{
							if (ev.jaxis.value<0)
								SnakeEngine->GiveOrder(n,Aup);
							else if (ev.jaxis.value>0)
								SnakeEngine->GiveOrder(n,Adown);
						}
					}
				}
			}
			else if (ev.type==SDL_KEYDOWN) // keyboard support
			{
				if (ev.key.keysym.sym==SDLK_ESCAPE)
				{
					SDL_ShowCursor(1);
					if (!SnakeGameMenu())
					{
						// we should quit game now
						return;
					}
					SDL_ShowCursor(0);
				}
				else
				{
					// go through all players & configs
					for (n=0;n<numberofplayingsnakes;n++)
					{
						id = SnakeEngine->GetID(n);
						ne=((CUIMultiGfxButton *)(LocalMenu->objects[(id*5)+3]))->GetFrame();
						if (ne<2)
						{
							// action
							if (ev.key.keysym.sym==PlayerButtons[ne].Uaction)
								SnakeEngine->GiveOrder(n,Uaction);
							// relative
							if (((CUIMultiGfxButton *)(LocalMenu->objects[(id*5)+2]))->GetFrame()==1)
							{
								if (ev.key.keysym.sym==PlayerButtons[ne].Rleft)
									SnakeEngine->GiveOrder(n,Rleft);
								if (ev.key.keysym.sym==PlayerButtons[ne].Rright)
									SnakeEngine->GiveOrder(n,Rright);
								if (!SnakeEngine->IsUpcoming(n))
									continue;
							}
							// absolute
							if (ev.key.keysym.sym==PlayerButtons[ne].Adown)
								SnakeEngine->GiveOrder(n,Adown);
							if (ev.key.keysym.sym==PlayerButtons[ne].Aleft)
								SnakeEngine->GiveOrder(n,Aleft);
							if (ev.key.keysym.sym==PlayerButtons[ne].Aright)
								SnakeEngine->GiveOrder(n,Aright);
							if (ev.key.keysym.sym==PlayerButtons[ne].Aup)
								SnakeEngine->GiveOrder(n,Aup);							
						}
					}
				}
			}
		}
		// Do SnakeEngine
		winner=SnakeEngine->DoStepAndDraw(ScrInfo);
		if (winner>=0)
			break;
		// Display
		SDL_UpdateRect(ScrInfo,0,0,0,0);
		// Next game loop
	}
	// Show high score
	SDL_ShowCursor(1);
	DrawBackground();
	char buf[256];
	int dx;
	sprintf(buf,"%s %s",SnakeEngine->GetSnakeOwnerName(winner),lang.GetString("[WINNER]"));
	dx=(ScrInfo->w-SGU_GetTextWidth(bigfont,buf))>>1;
	SGU_DisplayText(ScrInfo,bigfont,dx,50,buf);
	
	// sort :
	int score[8],t;
	// fill
	for (n=0;n<SnakeEngine->GetNumberOfSnake();n++)
	{
		score[n]=n;
	}
	// sort
	for (n=0;n<SnakeEngine->GetNumberOfSnake();n++)
	{
		for (m=0;m<n;m++)
		{
			// exchange
			if (SnakeEngine->GetSnakePoint(score[n])>SnakeEngine->GetSnakePoint(score[m]))
			{
				t=score[n];
				score[n]=score[m];
				score[m]=t;
			}
		}
	}

	// display
	for (n=0;n<SnakeEngine->GetNumberOfSnake();n++)
	{
		sprintf(buf,"%s has %d points\n",SnakeEngine->GetSnakeOwnerName(score[n]),SnakeEngine->GetSnakePoint(score[n]));
		SGU_DisplayText(ScrInfo,middlefont,40,130+n*35,buf);
	}
	SGU_DisplayTextCenter(ScrInfo,middlefont,0,420,640,lang.GetString("[PRESSENTER]"));
	SDL_UpdateRect(ScrInfo,0,0,0,0);
	// wait
	while (SDL_WaitEvent(&ev) >= 0 )
	{
		if (((ev.type==SDL_KEYDOWN)&&(ev.key.keysym.sym==SDLK_RETURN)) || (ev.type==SDL_MOUSEBUTTONDOWN))
			break;
	}
}

void SnakeMainInterface(SDL_Event ev)
{
	switch (GameState)
	{
	case LOCALMENU:
		{
			LocalMenu->ProcessDefaultEvent(&ev,ScrInfo);
			if (LocalMenu->GetClicked()>=0)
			{
				switch (LocalMenu->GetClicked())
				{
				case 40:
					{
						SavePlayerName();
						GameState=MAINMENU;
						DrawState=FIRSTDRAW;
						UIRedraw.newitem=-1;
						SnakeMeDraw();
						SDL_UpdateRect(ScrInfo,0,0,0,0);
					}
					break;
				case 41:
					{
						int n,gamesnknum,i;

						// count number of snakes
						numberofplayingsnakes=0;
						for (n=0;n<8;n++)
						{
							if (((CUIBoolChooser *)(LocalMenu->objects[n*5]))->GetState()==true)
							{
								numberofplayingsnakes++;
							}
						}

						// create Snake Engine
						SnakeEngine = new CSnakeEngine;
						SnakeEngine->theme=theme;
						
						// Set Engine Values
						SnakeEngine->params.defaultlength=AdvancedOptions.deflng;
						SnakeEngine->params.incrementlength=AdvancedOptions.inclng;
						
						SnakeEngine->params.numberofmeal=AdvancedOptions.nbmeal;
						SnakeEngine->params.mealdisplacementspeed=AdvancedOptions.mealchangetime;
						SnakeEngine->params.numberofbonus=AdvancedOptions.nbbonus;
						SnakeEngine->params.bonusdisplacementspeed=AdvancedOptions.bonuschangetime;

						SnakeEngine->params.maxpoint=AdvancedOptions.maxpoint;
						SnakeEngine->params.ptgetmeal=AdvancedOptions.ptgetmeal;
						SnakeEngine->params.ptgethit=AdvancedOptions.ptgethit;
						SnakeEngine->params.ptgetsnake=AdvancedOptions.ptgetsnake;
						SnakeEngine->params.ptloosecrash=AdvancedOptions.ptloosecrash;
						SnakeEngine->params.ptloosehit=AdvancedOptions.ptloosehit;
						
						SnakeEngine->params.themename=((CUIFileChooser *)(LocalMenu->objects[45]))->GetSelectedFile();
						SnakeEngine->params.levelname=((CUIFileChooser *)(LocalMenu->objects[44]))->GetSelectedFile();
						
						SnakeEngine->params.randseed=time(NULL);
						SnakeEngine->params.maxbonus=10;
						#ifndef NOSOUND
							SnakeEngine->SetSound(numbsnd,sounds);
						#endif
						
						// Init Engine
						SnakeEngine->InitEngine(numberofplayingsnakes,ScrInfo);

						// set player attributes
						gamesnknum=0;
						for (n=0;n<8;n++)
						{
							if (((CUIBoolChooser *)(LocalMenu->objects[n*5]))->GetState()==true)
							{
								if ( (((CUIMultiGfxButton *)(LocalMenu->objects[(n*5)+3]))->GetFrame())==2)
								{
									SnakeEngine->SetSnakeMode(gamesnknum,n,true);
								}
								else
								{
									if ((((CUIMultiGfxButton *)(LocalMenu->objects[(n*5)+3]))->GetFrame())==0)
										SnakeEngine->SetSnakeMode(gamesnknum,n,false);
									else
										SnakeEngine->SetSnakeMode(gamesnknum,n,false);
								}
								i=((CUISnakeChooser *)(LocalMenu->objects[(n*5)+4]))->GetSel();
								SnakeEngine->SetSnakeSprite(gamesnknum,(SGU_Sprite *)snakespritelist->GetElementN(i));
								SnakeEngine->SetSnakeOwnerName(gamesnknum,((CUITextEdit *)(LocalMenu->objects[(n*5)+1]))->GetText());
								SnakeEngine->SetSnakeUpcoming(gamesnknum,ScrInfo);
								gamesnknum++;
							}
						}

						// set level attributes
						SnakeEngine->SetLittleFont(littlefont);
						// do game
						SnakeGame();
						delete SnakeEngine;
						// redisplay screen
						DrawState=FIRSTDRAW;
						UIRedraw.newitem=-1;
						UIRedraw.lastitem=-1;
						SnakeMeDraw();
						SDL_UpdateRect(ScrInfo,0,0,0,0);
					}
					break;
				case 42:
					{
						GameState=CONTROLMENU;
						ReturnScreen=LOCALMENU;
						DrawState=FIRSTDRAW;
						UIRedraw.newitem=-1;
						UIRedraw.lastitem=-1;
						SetControlButtonsNames();
						memcpy(TempPlayerButtons,PlayerButtons,2*sizeof(SButtonMap));
						SnakeMeDraw();
						SDL_UpdateRect(ScrInfo,0,0,0,0);
					}
					break;
				case 43:
					{
						GameState=ADVANCEDSETUP;
						ReturnScreen=LOCALMENU;
						DrawState=FIRSTDRAW;
						UIRedraw.newitem=-1;
						UIRedraw.lastitem=-1;
						memcpy(&TempAdvancedOptions,&AdvancedOptions,1*sizeof(SAdvancedOptions));						
						AdvancedSetupUpdateVal();
						AdvancedSetupUpdateText();
						SnakeMeDraw();
						SDL_UpdateRect(ScrInfo,0,0,0,0);
					}
					break;
				case 44:
					{
						((CUILevelViewer *)(LocalMenu->objects[46]))->DrawArray(((CUIFileChooser *)(LocalMenu->objects[44]))->GetSelectedFile(),ScrInfo);
						SDL_UpdateRect(ScrInfo,0,0,0,0);
					}
					break;
				case 45:
					{
						((CUIThemeViewer *)(LocalMenu->objects[47]))->DrawTheme(((CUIFileChooser *)(LocalMenu->objects[45]))->GetSelectedFile(),ScrInfo);
						SDL_UpdateRect(ScrInfo,0,0,0,0);
					}
					break;
				default:
					{
						LocalMenu->ProcessDefaultEvent(&ev,ScrInfo);
					}
					break;
				}
			}
			else if (LocalMenu->GetKeyHit())
			{
				switch (LocalMenu->GetKeyHit())
				{
				case 44:
					{
						((CUILevelViewer *)(LocalMenu->objects[46]))->DrawArray(((CUIFileChooser *)(LocalMenu->objects[44]))->GetSelectedFile(),ScrInfo);
						SDL_UpdateRect(ScrInfo,0,0,0,0);
					}
					break;
				case 45:
					{
						((CUIThemeViewer *)(LocalMenu->objects[47]))->DrawTheme(((CUIFileChooser *)(LocalMenu->objects[45]))->GetSelectedFile(),ScrInfo);
						SDL_UpdateRect(ScrInfo,0,0,0,0);
					}
					break;
				}
			}
		}
		break;
	case MAINMENU:
		{
			MainMenu->ProcessDefaultEvent(&ev,ScrInfo);
			if (MainMenu->GetClicked()>=0)
			{
				switch (MainMenu->GetClicked())
				{
				case 0:
					{
						LoadPlayerName();
						GameState=LOCALMENU;
						DrawState=FIRSTDRAW;
						UIRedraw.newitem=-1;
						SnakeMeDraw();
						SDL_UpdateRect(ScrInfo,0,0,0,0);
					}
					break;
				case 1:
					{
						GameState=CONTROLMENU;
						ReturnScreen=MAINMENU;
						DrawState=FIRSTDRAW;
						UIRedraw.lastitem=-1;
						UIRedraw.newitem=-1;
						SetControlButtonsNames();
						memcpy(TempPlayerButtons,PlayerButtons,2*sizeof(SButtonMap));
						SnakeMeDraw();
						SDL_UpdateRect(ScrInfo,0,0,0,0);
						fprintf(stderr,"Control Screen OK\n");
					}
					break;
				case 2:
					{
						GameState=ADVANCEDSETUP;
						ReturnScreen=MAINMENU;
						DrawState=FIRSTDRAW;
						UIRedraw.lastitem=-1;
						UIRedraw.newitem=-1;
						AdvancedSetupUpdateVal();
						memcpy(&TempAdvancedOptions,&AdvancedOptions,1*sizeof(SAdvancedOptions));
						AdvancedSetupUpdateVal();
						AdvancedSetupUpdateText();
						SnakeMeDraw();
						SDL_UpdateRect(ScrInfo,0,0,0,0);
					}
					break;
				case 3:
					{
						GameState=LEVELEDITOR;
						DrawState=FIRSTDRAW;
						LevelEditor=new CSnakeLevelEditor;
						LevelEditor->theme=theme;
						LevelEditor->InitDefaultValues(((CUIFileChooser *)(LocalMenu->objects[45]))->GetSelectedFile(),littlefont,middlefont,ScrInfo,&lang);
						UIRedraw.newitem=-1;
						SnakeMeDraw();
						SDL_UpdateRect(ScrInfo,0,0,0,0);
					}
					break;
				case 4:
					{
						run=false;
					}
					break;
				}
			}
		}
		break;
	case CONTROLMENU:
		{
			ControlMenu->ProcessDefaultEvent(&ev,ScrInfo);		
			if (ControlMenu->GetClicked()>=0)
			{
				switch (ControlMenu->GetClicked())
				{
				case 0:
					{
						// cancel and restaur
						GameState=ReturnScreen;
						DrawState=FIRSTDRAW;
						memcpy(PlayerButtons,TempPlayerButtons,2*sizeof(SButtonMap));
						UIRedraw.newitem=-1;
						SnakeMeDraw();
						SDL_UpdateRect(ScrInfo,0,0,0,0);
					}
					break;
				case 1:
					{
						SetDefaultKeys();
						SetControlButtonsNames();
						DrawState=FIRSTDRAW;
						SnakeMeDraw();
						SDL_UpdateRect(ScrInfo,0,0,0,0);
					}
					break;
				case 2:
					{			 
						// save user info
						GameState=ReturnScreen;
						DrawState=FIRSTDRAW;
						UIRedraw.newitem=-1;
						SnakeMeDraw();
						SDL_UpdateRect(ScrInfo,0,0,0,0);
						FILE *fp;
						fp=fopen("control.ctl","wb");
						if (fp!=NULL)
						{
							SwapLittleEndianKeys(&(PlayerButtons[0]));
							SwapLittleEndianKeys(&(PlayerButtons[1]));
							fwrite(PlayerButtons,sizeof(SButtonMap),2,fp);
							fclose(fp);
							SwapLittleEndianKeys(&(PlayerButtons[0]));
							SwapLittleEndianKeys(&(PlayerButtons[1]));
						}
					}
					break;
				default:
					{
						int destkey=ControlMenu->GetClicked();
						if (destkey>0)
						{
							// Show Wait message
							SDL_Rect r;
							r.x=100;
							r.y=100;
							r.w=440;
							r.h=100;
							theme->DrawBackground(ScrInfo,&r);
							SGU_RectShade(ScrInfo,100,100,440,100,theme->FHiCol,theme->FMidCol,theme->FLowCol);
							SGU_DisplayText(ScrInfo,bigfont,200,120,"Press a key");
							SDL_UpdateRect(ScrInfo,0,0,0,0);
							// Wait for a key NOW
							while ( SDL_WaitEvent(&ev) >= 0 )
							{
								if (ev.type==SDL_KEYDOWN)
									break;
							}
							// Set the key NOW
							DrawState=FIRSTDRAW;
							switch (destkey)
							{
							case 3:
								PlayerButtons[0].Aleft=ev.key.keysym.sym;
								break;
							case 4:
								PlayerButtons[0].Aright=ev.key.keysym.sym;
								break;
							case 5:
								PlayerButtons[0].Aup=ev.key.keysym.sym;
								break;
							case 6:
								PlayerButtons[0].Adown=ev.key.keysym.sym;
								break;
							case 7:
								PlayerButtons[0].Rleft=ev.key.keysym.sym;
								break;
							case 8:
								PlayerButtons[0].Rright=ev.key.keysym.sym;
								break;
							case 9:
								PlayerButtons[0].Uaction=ev.key.keysym.sym;
								break;
							case 10:
								PlayerButtons[1].Aleft=ev.key.keysym.sym;
								break;
							case 11:
								PlayerButtons[1].Aright=ev.key.keysym.sym;
								break;
							case 12:
								PlayerButtons[1].Aup=ev.key.keysym.sym;
								break;
							case 13:
								PlayerButtons[1].Adown=ev.key.keysym.sym;
								break;
							case 14:
								PlayerButtons[1].Rleft=ev.key.keysym.sym;
								break;
							case 15:
								PlayerButtons[1].Rright=ev.key.keysym.sym;
								break;
							case 16:
								PlayerButtons[1].Uaction=ev.key.keysym.sym;
								break;
							}
							SetControlButtonsNames();
							SnakeMeDraw();
							SDL_UpdateRect(ScrInfo,0,0,0,0);
						}					
					}
					break;
				}
			}
		}
		break;
	case ADVANCEDSETUP:
		{
			AdvancedSetup->ProcessDefaultEvent(&ev,ScrInfo);
			if (AdvancedSetup->GetKeyHit()>=0)
			{
				AdvancedSetupSaveVal();
				AdvancedSetupUpdateText();							
				SDL_UpdateRect(ScrInfo,0,0,0,0);
			}
			if (AdvancedSetup->GetClicked()>=0)
			{
				switch (AdvancedSetup->GetClicked())
				{
				case 0:
					{
						// save user info
						GameState=ReturnScreen;
						DrawState=FIRSTDRAW;
						UIRedraw.newitem=-1;
						SnakeMeDraw();
						SDL_UpdateRect(ScrInfo,0,0,0,0);
						FILE *fp;
						fp=fopen("options.ctl","wb");
						if (fp!=NULL)
						{
							SwapAdvancedOptionsLittleEndian(&AdvancedOptions);
							fwrite(&AdvancedOptions,sizeof(SAdvancedOptions),1,fp);
							SwapAdvancedOptionsLittleEndian(&AdvancedOptions);
							fclose(fp);
						}
					}
					break;
				case 1:
					{
						// cancel and restaur
						GameState=ReturnScreen;
						DrawState=FIRSTDRAW;
						memcpy(&AdvancedOptions,&TempAdvancedOptions,sizeof(SAdvancedOptions));
						UIRedraw.newitem=-1;
						SnakeMeDraw();
						SDL_UpdateRect(ScrInfo,0,0,0,0);
					}
					break;
				case 28:
					{
						// default
						SetDefaultAdvancedOptions();
						AdvancedSetupUpdateVal();
						AdvancedSetupUpdateText();

						DrawState=FIRSTDRAW;
						SnakeMeDraw();
						SDL_UpdateRect(ScrInfo,0,0,0,0);
					}
					break;
				default:
					{
						AdvancedSetup->ProcessDefaultEvent(&ev,ScrInfo);
						AdvancedSetupSaveVal();
						AdvancedSetupUpdateText();							
						SDL_UpdateRect(ScrInfo,0,0,0,0);
					}
					break;
				}
			}
		}
		break;
	case LEVELEDITOR:
		{
			if (LevelEditor->SendMessage(ev,ScrInfo))
			{			
				GameState=MAINMENU;
				DrawState=FIRSTDRAW;
				UIRedraw.newitem=-1;
				delete LevelEditor;
				SnakeMeDraw();
				SDL_UpdateRect(ScrInfo,0,0,0,0);
			}
		}
		break;
	}
};

void SnakeMeDraw(void)
{
	switch (GameState)
	{
	case MAINMENU:
		{
			if (DrawState==FIRSTDRAW)
			{
				DrawBackground();
				MainMenu->DrawScreen(ScrInfo);

				char buf[128];
				//sprintf(buf,"SnakeMe Version 1.0, build %d",__FBUILDNUMBER);
				//SGU_DisplayTextCenter(ScrInfo,littlefont,0,458,640,buf);
				SGU_DisplayTextCenter(ScrInfo,littlefont,0,458,640,"SnakeMe Version 1.0.3");
				//SGU_DisplayTextCenter(ScrInfo,littlefont,0,468,640,__FBUILDTIME);

				DrawState=LIGHTREDRAW;
			}
		}
		break;
	case LOCALMENU:
		{
			if (DrawState==FIRSTDRAW)
			{
				DrawBackground();
				SGU_DisplayTextCenter(ScrInfo,bigfont,200,20,240,lang.GetString("[GMMENU1]"));
				SGU_DisplayTextCenter(ScrInfo,bigfont,200,70,240,lang.GetString("[GMMENU2]"));
				// draw rectangle
				int dx,dy;
				for (int n=0;n<8;n++)
				{
					dx=((n%2)*320)+20;
					dy=155+5+(n>>1)*60;
					SGU_RectShade(ScrInfo,dx,dy,280,50,SGU_RGB(255,255,255,0),SGU_RGB(0,0,127,0),SGU_RGB(0,0,255,0));
				}
				LocalMenu->DrawScreen(ScrInfo);
				((CUILevelViewer *)(LocalMenu->objects[46]))->DrawArray(((CUIFileChooser *)(LocalMenu->objects[44]))->GetSelectedFile(),ScrInfo);
				((CUIThemeViewer *)(LocalMenu->objects[47]))->DrawTheme(((CUIFileChooser *)(LocalMenu->objects[45]))->GetSelectedFile(),ScrInfo);
				DrawState=LIGHTREDRAW;
			}
		}
		break;

	case CONTROLMENU:
		{
			if (waitforakey)
			{
				SGU_DisplayText(ScrInfo,bigfont,200,260,lang.GetString("[PRESSKEY]"));
				return;
			}
			if (DrawState==FIRSTDRAW)
			{
				DrawBackground();
				SGU_DisplayTextCenter(ScrInfo,bigfont,0,10,640,lang.GetString("[CONTROLS]"));
				SGU_DisplayTextCenter(ScrInfo,middlefont,30,65,150,lang.GetString("[PLAYER1]"));
				SGU_DisplayTextCenter(ScrInfo,middlefont,460,65,150,lang.GetString("[PLAYER2]"));
				SGU_RectShade(ScrInfo,20,90,600,165,SGU_RGB(255,255,255,0),SGU_RGB(0,0,127,0),SGU_RGB(0,0,255,0));
				SGU_DisplayTextCenter(ScrInfo,middlefont,0,100,640,lang.GetString("[ABSLEFT]"));
				SGU_DisplayTextCenter(ScrInfo,middlefont,0,140,640,lang.GetString("[ABSRIGHT]"));
				SGU_DisplayTextCenter(ScrInfo,middlefont,0,180,640,lang.GetString("[ABSUP]"));
				SGU_DisplayTextCenter(ScrInfo,middlefont,0,220,640,lang.GetString("[ABSDOWN]"));
				
				SGU_RectShade(ScrInfo,20,260,600,85,SGU_RGB(255,255,255,0),SGU_RGB(0,0,127,0),SGU_RGB(0,0,255,0));
				SGU_DisplayTextCenter(ScrInfo,middlefont,0,270,640,lang.GetString("[RELLEFT]"));
				SGU_DisplayTextCenter(ScrInfo,middlefont,0,310,640,lang.GetString("[RELRIGHT]"));
				
				SGU_RectShade(ScrInfo,20,350,600,45,SGU_RGB(255,255,255,0),SGU_RGB(0,0,127,0),SGU_RGB(0,0,255,0));
				SGU_DisplayTextCenter(ScrInfo,middlefont,0,360,640,lang.GetString("[ACTION]"));
				ControlMenu->DrawScreen(ScrInfo);
				DrawState=LIGHTREDRAW;
			}
		}
		break;
	case ADVANCEDSETUP:
		{
			if (DrawState==FIRSTDRAW)
			{
				DrawBackground();
				
				SGU_RectShade(ScrInfo,20,55,600,80,SGU_RGB(255,255,255,0),SGU_RGB(0,0,127,0),SGU_RGB(0,0,255,0));
				SGU_RectShade(ScrInfo,20,140,600,105,SGU_RGB(255,255,255,0),SGU_RGB(0,0,127,0),SGU_RGB(0,0,255,0));
				SGU_RectShade(ScrInfo,20,250,600,155,SGU_RGB(255,255,255,0),SGU_RGB(0,0,127,0),SGU_RGB(0,0,255,0));
				
				SGU_DisplayTextCenter(ScrInfo,bigfont,0,10,640,lang.GetString("[ADS]"));
				SGU_DisplayText(ScrInfo,middlefont,30,60,lang.GetString("[GMSPEED]"));
				SGU_DisplayText(ScrInfo,middlefont,30,85,lang.GetString("[DEFSL]"));
				SGU_DisplayText(ScrInfo,middlefont,30,110,lang.GetString("[INCSL]"));

				SGU_DisplayText(ScrInfo,middlefont,30,145,lang.GetString("[NBMEAL]"));
				SGU_DisplayText(ScrInfo,middlefont,30,170,lang.GetString("[MEALCHGTIME]"));
				SGU_DisplayText(ScrInfo,middlefont,30,195,lang.GetString("[NBBONUS]"));
				SGU_DisplayText(ScrInfo,middlefont,30,220,lang.GetString("[BONUSCHGTIME]"));

				SGU_DisplayText(ScrInfo,middlefont,30,255,lang.GetString("[NBPTWIN]"));
				SGU_DisplayText(ScrInfo,middlefont,30,280,lang.GetString("[PTWINMEAL]"));
				SGU_DisplayText(ScrInfo,middlefont,30,305,lang.GetString("[PTWINBLOC]"));
				SGU_DisplayText(ScrInfo,middlefont,30,330,lang.GetString("[PTWINFIRE]"));
				SGU_DisplayText(ScrInfo,middlefont,30,355,lang.GetString("[PTLOSECRASH]"));
				SGU_DisplayText(ScrInfo,middlefont,30,380,lang.GetString("[PTLOSTHIT]"));
				
				AdvancedSetup->DrawScreen(ScrInfo);
				
				DrawState=LIGHTREDRAW;
			}
		}
		break;
	case LEVELEDITOR:
		{
			if (DrawState==FIRSTDRAW)
			{
				LevelEditor->FullDraw(ScrInfo);
				DrawState=LIGHTREDRAW;
			}
		}
		break;
	}

};

inline void UpdateIntroAnim(int step,const char *text,SGU_RegionSaver *reg)
{
	reg->RestaurRegion(ScrInfo);
	SGU_FillRectAlpha(ScrInfo,70,350,500,30,SGU_RGB(0,0,128,128));
	SGU_FillRectAlpha(ScrInfo,70,350,(500*step)/7,30,SGU_RGB(64,0,255,128));
	SGU_DisplayTextCenter(ScrInfo,middlefont,0,355,640,text);
	SDL_UpdateRect(ScrInfo,0,0,0,0);
}

int main( int argc, char* argv[] )
{
	SDL_Surface *screen;
	SDL_Event event;

	// process command line
	Uint32 sdlflags;
	int nlang;

	fullscreen=false;
	noaudio=false;
	nlang=0;
	for (int n=0;n<argc;n++)
	{
		// it is an option :-)
		if ( (strlen(argv[n])>=2) && (argv[n][0]=='-'))
		{
			switch (argv[n][1])
			{
			case 'f':
			case 'F':
				fullscreen=true;
				break;
			case 'n':
			case 'N':
				noaudio=true;
				break;
			case 'l':
			case 'L':
				{
					if (strlen(argv[n])>=3)
						nlang=argv[n][2]-'0';
				}
				break;
			case 'h':
			case 'H':
			case '?':
				{
					printf("Usage : %s [options]\n\n",argv[0]);
					printf("options :\n\n");
					printf("	-F or -f        : fullscreen\n");
					printf("	-N or -n        : no audio\n");
					printf("	-L[k] or -l[k]  : set language to k\n");
					printf("	-H or -h or -?  : this help screen\n\n");
				}
				break;
			default:
				printf("CMD : Command %s not recognised\n",argv[n]);
			}
		}
	}

    // Initialize the SDL library
	if (noaudio)
	{
		sdlflags=SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_JOYSTICK;
	}
	else
	{
		sdlflags=SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_AUDIO|SDL_INIT_JOYSTICK;
	}
    if ( SDL_Init(sdlflags) < 0 )
	{
		fprintf(stderr,"SDL : Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }
	else
	{
		if (noaudio)
		{
			printf("SDL : SDL correctly initialized without audio support\n");
		}
		else
		{
			printf("SDL : SDL correctly initialized with audio support\n");
		}
	}

    // Exit
	atexit(SDL_Quit);
	
	// key
	SDL_EnableUNICODE(true);
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,SDL_DEFAULT_REPEAT_INTERVAL);

	// Initialize the display in a 640x480 16 mode
    if (fullscreen)
	{
		screen = SDL_SetVideoMode(640, 480, 16, SDL_FULLSCREEN); // SDL_HWSURFACE|SDL_ANYFORMAT
		printf("VID : Screen set to 640x480x16 bpp fullscreen\n");
	}
	else
	{
		screen = SDL_SetVideoMode(640, 480, 16,SDL_ANYFORMAT); // SDL_HWSURFACE
		printf("VID : Screen set to 640x480x%d bpp window\n",screen->format->BitsPerPixel);
	}
	if ( screen == NULL )
	{
		fprintf(stderr, "VID : Couldn't set 640x480x16 video mode: %s\n",SDL_GetError());
		exit(1);
	}

	ScrInfo=screen;

	// Load background first
	backgroundsprite=new SGU_Sprite;
	backgroundsprite->Load("fond01.spr",screen->format);	
	backgroundsprite->Display(ScrInfo,0,0,0);
	bigfont=new SGU_Sprite;
	bigfont->Load("fntbigb.spr",screen->format);
	littlefont=new SGU_Sprite;
	littlefont->Load("fntltlw.spr",screen->format);
	middlefont=new SGU_Sprite;
	middlefont->Load("fntmdlw.spr",screen->format);
	ctrltypechoice=new SGU_Sprite;
	ctrltypechoice->Load("ctrltype.spr",screen->format);

	// then Load theme
	theme=new CUITheme();
	theme->FHiCol.R=255;
	theme->FHiCol.G=255;
	theme->FHiCol.B=255;
	theme->FLowCol.R=32;
	theme->FLowCol.G=32;
	theme->FLowCol.B=64;
	theme->BHiCol.R=0;
	theme->BHiCol.G=0;
	theme->BHiCol.B=255;
	theme->BLowCol.R=0;
	theme->BLowCol.G=0;
	theme->BLowCol.B=0;
	theme->WgtBack.R=40;
	theme->WgtBack.G=40;
	theme->WgtBack.B=255;
	theme->WgtBack.A=128;
	theme->RepackColor(screen);
	theme->bgpict=backgroundsprite->GetFrameSurface(0);

	{
		SGU_RegionSaver bgsaved;
		bgsaved.SaveRegion(ScrInfo,0,350,640,30);

		// 0
		UpdateIntroAnim(0,"Init audio & music support",&bgsaved);
		#ifndef NOSOUND
		if (!noaudio) InitAudio();
		#endif

		// 1
		UpdateIntroAnim(1,"Loading snakes graphics",&bgsaved);
		// snakes sprites
		// load all .snk in the directory
		snakespritelist=new List;
		{
			char *tempname;
			SGU_DirLister dir("snk");
			SGU_Sprite *tempsprite;

			while ((tempname=dir.GetNextName())!=NULL)
			{
				tempsprite=new SGU_Sprite;
				tempsprite->Load(tempname,screen->format);
				snakespritelist->AddElement(tempsprite);
			}
		}

		// 2
		UpdateIntroAnim(2,"Scanning for Joystick support",&bgsaved);
		// test if there is joystick avaible	
		ctrluserchoice=new SGU_Sprite;

		numjoystick=SDL_NumJoysticks();
		if (numjoystick==0)
		{
			joysticks[0]=0;
			joysticks[1]=0;
			ctrluserchoice->Load("ctrlwho.spr",screen->format);
		}
		else if (numjoystick==1)
		{
			joysticks[0]=SDL_JoystickOpen(0);
			joysticks[1]=0;
			ctrluserchoice->Load("ctrlwho1J.spr",screen->format);
			//SDL_JoystickEventState(SDL_ENABLE);
		}
		else
		{
			joysticks[0]=SDL_JoystickOpen(0);
			joysticks[1]=SDL_JoystickOpen(1);
			ctrluserchoice->Load("ctrlwho2J.spr",screen->format);
			//SDL_JoystickEventState(SDL_ENABLE);
		}
		

		// 3
		UpdateIntroAnim(3,"Setting keys",&bgsaved);
		// try to load keys
		FILE *fp;
		fp=fopen("control.ctl","rb");
		if (fp!=NULL)
		{
			fread(PlayerButtons,sizeof(SButtonMap),2,fp);//Here !! zzz
			fclose(fp);
			SwapLittleEndianKeys(&(PlayerButtons[0]));
			SwapLittleEndianKeys(&(PlayerButtons[1]));
		}
		else
		{
			SetDefaultKeys();
		}

		// 4
		UpdateIntroAnim(4,"Setting options",&bgsaved);
		// try to load advanced options
		fp=fopen("options.ctl","rb");
		if (fp!=NULL)
		{
			fread(&AdvancedOptions,sizeof(SAdvancedOptions),1,fp);
			fclose(fp);
			SwapAdvancedOptionsLittleEndian(&AdvancedOptions);
		}
		else
		{
			SetDefaultAdvancedOptions();
		}

		// 5
		UpdateIntroAnim(5,"Loading multilanguage support",&bgsaved);
		// load lang
		lang.Load("lang.txt");
		lang.SetLang(nlang);

		// 6
		UpdateIntroAnim(6,"Creating User Interface",&bgsaved);
		// create UI
		UIBuilder();
	}

	// Set variables
	GameState=MAINMENU;
	DrawState=FIRSTDRAW;

	// First draw
	DrawBackground();
	MainMenu->DrawScreen(ScrInfo);
	SnakeMeDraw();
	SDL_UpdateRect(ScrInfo,0,0,0,0);

	// Main Interface
	while ((run) && (SDL_WaitEvent(&event) >= 0 ))
	{
		switch (event.type) 
		{
			case SDL_QUIT:
				{
					run=false;
				}
				break;
			default:
				{
					SnakeMainInterface(event);
				}
				break;
		}
	}

	// close joysticks
	if (numjoystick==1)
	{
		SDL_JoystickClose(joysticks[0]);
	}
	else if (numjoystick==2)
	{
		SDL_JoystickClose(joysticks[0]);
		SDL_JoystickClose(joysticks[1]);
	}

	// Quit
	UIDestructor();
	delete bigfont;
	delete middlefont;
	delete littlefont;
	delete backgroundsprite;
	delete theme;
	delete ctrltypechoice;
	delete ctrluserchoice;

	// delete sprite list
	if (snakespritelist!=NULL)
	{
		lpListNode mynode;
		for (mynode=snakespritelist->GetHead();mynode;mynode=mynode->Next)
		{
			delete (SGU_Sprite*)(mynode->Element);
		}
		delete snakespritelist;
	}

	#ifndef NOSOUND
	// delete music
	CloseAudio();
	#endif

	return (0);
}



