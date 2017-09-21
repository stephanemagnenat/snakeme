/*  ======== Snake Me Demo ======== *\
    (c) 1999-2000 Stephane Magnenat
\*  =============================== */

#include "SnakeMe.h"
#include "SGU.h"
#include "SGU_UI.h"
#include "CSnakeEngine.h"
#include "StringTable.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "SGU_List.h"
#include "SnakeUI.h"

#ifndef NOSOUND
#include "SDL_mixer.h"
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
CUIScreen *LocalMenu;
CUIScreen InGameMenu(2);
SGU_RegionSaver InGameMenuBG;
CSnakeEngine *SnakeEngine;

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

enum
{
	FIRSTDRAW,
	LIGHTREDRAW
} DrawState;

SUIRedraw UIRedraw;
SButtonMap PlayerButtons[2];
SButtonMap TempPlayerButtons[2];
SAdvancedOptions AdvancedOptions;
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
}
void LoadMusic(void)
{
	musiclist.AddElement("jingle.xm");
	printf("ZIK : Loading MOD [jingle.xm]\n");
}

void LoadSound(void)
{
	const sndvol=100;
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



void UIBuilder(void)
{
	int dx,dy,n;
	MainMenu = new CUIScreen(3);
	MainMenu->objects[0]= new CUINormalButton(120,40,410,45,theme,bigfont,lang.GetString("[GAME]"),true);
	MainMenu->objects[1]= new CUINormalButton(120,120,410,45,theme,bigfont,lang.GetString("[CONTROLS]"),true);
	MainMenu->objects[2]= new CUINormalButton(120,360,410,45,theme,bigfont,lang.GetString("[QUIT]"),true);
	
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
	
	LocalMenu = new CUIScreen(27);
	for (n=0;n<4;n++)
	{
		dx=((n%2)*320)+10;
		dy=155+5+(n>>1)*60;
		LocalMenu->objects[(n*5)]=new CUIBoolChooser(dx+25,dy+15,20,20,theme,middlefont,"");
		LocalMenu->objects[(n*5)+1]=new CUITextEdit(dx+60,dy+3,200,22,theme,middlefont,"",false);
		LocalMenu->objects[(n*5)+2]=new CUIMultiGfxButton(dx+60,dy+29,34,18,theme,ctrltypechoice,false);
		LocalMenu->objects[(n*5)+3]=new CUIMultiGfxButton(dx+110,dy+29,34,18,theme,ctrluserchoice,false);
		LocalMenu->objects[(n*5)+4]=new CUISnakeChooser(dx+160,dy+29,100,18,theme,snakespritelist);
	}
	LocalMenu->objects[20]=new CUINormalButton(20,420,170,45,theme,bigfont,lang.GetString("[CANCEL]"),true);
	LocalMenu->objects[21]=new CUINormalButton(450,420,170,45,theme,bigfont,lang.GetString("[PLAY]"),true);
	LocalMenu->objects[22]=new CUINormalButton(220,410,200,25,theme,middlefont,lang.GetString("[CONTROLS]"),true);
	LocalMenu->objects[23]=new CUIFileChooser(25,380-260,160,22,theme,middlefont,true);
	((CUIFileChooser *)(LocalMenu->objects[23]))->Add("blank.sml");
	((CUIFileChooser *)(LocalMenu->objects[23]))->Add("demolvl.sml");
	LocalMenu->objects[24]=new CUIFileChooser(450,380-260,160,22,theme,middlefont,true);
	((CUIFileChooser *)(LocalMenu->objects[24]))->Add("atomic.smt");
	((CUIFileChooser *)(LocalMenu->objects[24]))->Add("tourbillon.smt");
	LocalMenu->objects[25]=new CUILevelViewer(25,267-260,theme);
	LocalMenu->objects[26]=new CUIThemeViewer(450,267-260,theme);

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

char *playerdefaultname[8]={ "Player 1",
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
		for (int n=0;n<4;n++)
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
		for (int n=0;n<4;n++)
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
		for (int n=0;n<4;n++)
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
				case 20:
					{
						SavePlayerName();
						GameState=MAINMENU;
						DrawState=FIRSTDRAW;
						UIRedraw.newitem=-1;
						SnakeMeDraw();
						SDL_UpdateRect(ScrInfo,0,0,0,0);
					}
					break;
				case 21:
					{
						int n,gamesnknum,i;

						// count number of snakes
						numberofplayingsnakes=0;
						for (n=0;n<4;n++)
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
						
						SnakeEngine->params.themename=((CUIFileChooser *)(LocalMenu->objects[24]))->GetSelectedFile();
						SnakeEngine->params.levelname=((CUIFileChooser *)(LocalMenu->objects[23]))->GetSelectedFile();
						
						SnakeEngine->params.randseed=time(NULL);
						SnakeEngine->params.maxbonus=10;
						#ifndef NOSOUND
							SnakeEngine->SetSound(numbsnd,sounds);
						#endif
						
						// Init Engine
						SnakeEngine->InitEngine(numberofplayingsnakes,ScrInfo);

						// set player attributes
						gamesnknum=0;
						for (n=0;n<4;n++)
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
				case 22:
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
				case 23:
					{
						((CUILevelViewer *)(LocalMenu->objects[25]))->DrawArray(((CUIFileChooser *)(LocalMenu->objects[23]))->GetSelectedFile(),ScrInfo);
						SDL_UpdateRect(ScrInfo,0,0,0,0);
					}
					break;
				case 24:
					{
						((CUIThemeViewer *)(LocalMenu->objects[26]))->DrawTheme(((CUIFileChooser *)(LocalMenu->objects[24]))->GetSelectedFile(),ScrInfo);
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
				case 23:
					{
						((CUILevelViewer *)(LocalMenu->objects[25]))->DrawArray(((CUIFileChooser *)(LocalMenu->objects[23]))->GetSelectedFile(),ScrInfo);
						SDL_UpdateRect(ScrInfo,0,0,0,0);
					}
					break;
				case 24:
					{
						((CUIThemeViewer *)(LocalMenu->objects[26]))->DrawTheme(((CUIFileChooser *)(LocalMenu->objects[24]))->GetSelectedFile(),ScrInfo);
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
	}
};

void FakeButton(SDL_Surface *screen,SGU_Sprite *font,int x,int y,int w,int h,char *text)
{
	SGU_FillRectAlpha(ScrInfo,x,y,w,h,SGU_RGB(255,255,255,128));
	SGU_RectShade(ScrInfo,x,y,w,h,SGU_RGB(255,255,255,0),SGU_RGB(165,165,165,0),SGU_RGB(80,80,80,0));
	if (text)
		SGU_DisplayTextCenter(ScrInfo,font,x,y+2,w,text);
}

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

				FakeButton(ScrInfo,bigfont,120,200,410,45,lang.GetString("[ADS]"));
				FakeButton(ScrInfo,bigfont,120,280,410,45,lang.GetString("[LVLED]"));
		
				SGU_DisplayTextCenter(ScrInfo,littlefont,0,458,640,"SnakeMe DEMO, build 100");
				SGU_DisplayTextCenter(ScrInfo,littlefont,0,468,640,"copyright 2000 Ysagoon");	

				DrawState=LIGHTREDRAW;
			}
		}
		break;
	case LOCALMENU:
		{
			if (DrawState==FIRSTDRAW)
			{
				DrawBackground();
				// Fake stuff
				FakeButton(ScrInfo,middlefont,220,445,200,25,lang.GetString("[ADS]"));
				int n,dx,dy;
				for (n=4;n<8;n++)
				{
					dx=((n%2)*320)+10;
					dy=155+5+(n>>1)*60;
					FakeButton(ScrInfo,NULL,dx+25,dy+15,20,20,NULL);
					FakeButton(ScrInfo,middlefont,dx+60,dy+3,200,22,lang.GetString("[FULLVER]"));
					FakeButton(ScrInfo,NULL,dx+60,dy+29,34,18,NULL);
					FakeButton(ScrInfo,NULL,dx+110,dy+29,34,18,NULL);
					FakeButton(ScrInfo,NULL,dx+160,dy+29,100,18,NULL);
				}
				SGU_DisplayTextCenter(ScrInfo,bigfont,200,20,240,lang.GetString("[GMMENU1]"));
				SGU_DisplayTextCenter(ScrInfo,bigfont,200,70,240,lang.GetString("[GMMENU2]"));
				// draw rectangle
				for (n=0;n<8;n++)
				{
					dx=((n%2)*320)+20;
					dy=155+5+(n>>1)*60;
					SGU_RectShade(ScrInfo,dx,dy,280,50,SGU_RGB(255,255,255,0),SGU_RGB(0,0,127,0),SGU_RGB(0,0,255,0));
				}
				LocalMenu->DrawScreen(ScrInfo);
				((CUILevelViewer *)(LocalMenu->objects[25]))->DrawArray(((CUIFileChooser *)(LocalMenu->objects[23]))->GetSelectedFile(),ScrInfo);
				((CUIThemeViewer *)(LocalMenu->objects[26]))->DrawTheme(((CUIFileChooser *)(LocalMenu->objects[24]))->GetSelectedFile(),ScrInfo);
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
	}

};

inline void UpdateIntroAnim(int step,char *text,SGU_RegionSaver *reg)
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
		// load snakes

		snakespritelist=new List;
		SGU_Sprite *tempsprite;
		tempsprite=new SGU_Sprite;
		tempsprite->Load("boasprite.snk",screen->format);
		snakespritelist->AddElement(tempsprite);
		tempsprite=new SGU_Sprite;
		tempsprite->Load("zigzag.snk",screen->format);
		snakespritelist->AddElement(tempsprite);
		tempsprite=new SGU_Sprite;
		tempsprite->Load("feux.snk",screen->format);
		snakespritelist->AddElement(tempsprite);
		tempsprite=new SGU_Sprite;
		tempsprite->Load("tube.snk",screen->format);
		snakespritelist->AddElement(tempsprite);
		
		

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
		SetDefaultAdvancedOptions();

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
	
	// display end screen
	DrawBackground();
	SGU_DisplayTextCenter(ScrInfo,middlefont,0,340,640,lang.GetString("[ENDSCR1]"));
	SGU_DisplayTextCenter(ScrInfo,middlefont,0,380,640,lang.GetString("[ENDSCR2]"));
	SDL_UpdateRect(ScrInfo,0,0,0,0);
	run=true;
	while ((run) && (SDL_WaitEvent(&event) >= 0 ))
	{
		if ((event.type==SDL_QUIT) ||
			(event.type==SDL_MOUSEBUTTONDOWN) ||
			(event.type==SDL_KEYDOWN))
			run=false;
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
			delete (mynode->Element);
		}
		delete snakespritelist;
	}

	#ifndef NOSOUND
	// delete music
	CloseAudio();
	#endif

	return (0);
}



