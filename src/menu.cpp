/*
 * menu.cpp
 *
 *  Created on: 2019年7月11日
 *      Author: alex
 */


#include "menu.hpp"
#include <string.h>
#include "locale.h"
#include <stdio.h>
#include <wchar.h>
#include "crosd.hpp"


const int CHARPOSX = (int)((float)OUTPUTW *0.78125f);
const int CHARPOSY = (int)((float)OUTPUTH *0.056f);


CMenu::CMenu():m_menuStat(0),m_enhStat(false),m_stbStat(false),m_paramStat(false),
			m_menuPointer(255),m_stbmode(0),m_stbparam(0)
{
}


CMenu::~CMenu()
{
}


void CMenu::enter()
{
	switch(m_menuStat)
	{
		case MENU_MAIN:
			menuhandle_main();
			break;

		case MENU_PARAM:
			menuhandle_param();	
			break;
			
		default:
			break;
	
	}
  	showOsd();
	return;
}


void CMenu::menuhandle_main()
{
	switch(m_menuPointer)
	{
		case 0:
			m_enhStat = !m_enhStat;
			updateEnhStatOsd();
			break;
		case 1:
			m_stbStat = !m_stbStat;
			updateStbStatOsd();
			break;	

		case 2:
			m_paramStat = !m_paramStat;
			if(m_paramStat)
				gotoParamMenu();
			else
				gotoMainMenu();
			break;
		default:
			break;
	}
	return;
}


void CMenu::menuhandle_param()
{
	switch(m_menuPointer)
	{
		case 0:
			m_enhStat = !m_enhStat;
			updateEnhStatOsd();
			break;
		case 1:
			m_stbStat = !m_stbStat;
			updateStbStatOsd();
			break;	

		case 2:
			m_paramStat = !m_paramStat;
			if(m_paramStat)
				gotoParamMenu();
			else
				gotoMainMenu();
			break;
			
		case 3:
			m_stbmode = (m_stbmode+1+MODE_MAX)%MODE_MAX;
			updateStbModeOsd();
			break;
		case 4:
			m_stbparam = (m_stbparam+1+FILTER_MAX)%FILTER_MAX;
			updateStbFilterOsd();
			break;
		default:
			break;
	}
	return;
}


void CMenu::menuButton()
{
	switch(m_menuStat)
	{
		case MENU_BLANK:
			gotoMainMenu();
			break;
		case MENU_MAIN:
			gotoBlankMenu();
			break;

		default:
			break;
	}
	showOsd();
	return;
}


void CMenu::eraseOsd()
{
	for(int i = 0; i < MAX_SUBMENU; i++)
	{
		cr_osd::erase((void*)disMenuBuf.osdBuffer[i].disMenu);
	}
	return;
}

void CMenu::showOsd()
{
	unsigned char r, g, b, a, color;
	short x, y;

	char font = 1;
	char fontsize = 4;

	cv::Point pos;

	eraseOsd();
	for(int i = 0; i < disMenuBuf.cnt; i++)
	{
		if(disMenuBuf.osdBuffer[i].bshow)
		{
			x = disMenuBuf.osdBuffer[i].posx;
			y = disMenuBuf.osdBuffer[i].posy;
			a = disMenuBuf.osdBuffer[i].alpha;
			color = disMenuBuf.osdBuffer[i].color;
			getRGBA(color,r,g,b,a);
			pos.x = x;
			pos.y = y;
			cr_osd::put(disMenuBuf.osdBuffer[i].disMenu, pos, cv::Scalar(r,g,b,a));
		}
	}
	return;
}


void CMenu::gotoMainMenu()
{
	m_menuPointer = 255;
	m_menuStat = MENU_MAIN;
	menuOsdInit_main();
	updateEnhStatOsd();
	updateStbStatOsd();
	return;
}

void CMenu::gotoBlankMenu()
{
	m_menuPointer = 255;
	m_menuStat = MENU_BLANK;
	menuOsdInit_blank();
	return;
}


void CMenu::gotoParamMenu()
{
	m_menuStat = MENU_PARAM;
	menuOsdInit_param();
	updateStbModeOsd();
	updateStbFilterOsd();
	return;
}

void CMenu::updateEnhStatOsd()
{
	if(m_enhStat)
		swprintf(disMenuBuf.osdBuffer[0].disMenu, 33, L"增强       开");
	else
		swprintf(disMenuBuf.osdBuffer[0].disMenu, 33, L"增强       关");
	return;
}

void CMenu::updateStbStatOsd()
{
	if(m_stbStat)
		swprintf(disMenuBuf.osdBuffer[1].disMenu, 33, L"稳像       开");
	else
		swprintf(disMenuBuf.osdBuffer[1].disMenu, 33, L"稳像       关");
	return;
}


void CMenu::updateStbModeOsd()
{
	switch(m_stbmode)
	{
		case MODE_AUTO:
			swprintf(disMenuBuf.osdBuffer[3].disMenu, 33, L"稳像模式   自  动");
			break;
		case MODE_TRANS:
			swprintf(disMenuBuf.osdBuffer[3].disMenu, 33, L"稳像模式   平  移");
			break;
		case MODE_TRANSSCALE:
			swprintf(disMenuBuf.osdBuffer[3].disMenu, 33, L"稳像模式   平移缩放");
			break;
		case MODE_RIGID:
			swprintf(disMenuBuf.osdBuffer[3].disMenu, 33, L"稳像模式   刚  性");
			break;
		case MODE_PERSPECTIVE:
			swprintf(disMenuBuf.osdBuffer[3].disMenu, 33, L"稳像模式   透  视");
			break;
		default:
			break;
	}
	return;
}

void CMenu::updateStbFilterOsd()
{
	switch(m_stbparam)
		{
			case FILTER_HIGHT:
				swprintf(disMenuBuf.osdBuffer[4].disMenu, 33, L"滤波参数    高");
				break;
			case FILTER_MID:
				swprintf(disMenuBuf.osdBuffer[4].disMenu, 33, L"滤波参数    中");
				break;
			case FILTER_LOW:
				swprintf(disMenuBuf.osdBuffer[4].disMenu, 33, L"滤波参数    低");
				break;
			default:
				break;
		}
	return;
}



void CMenu::menuOsdInit_main()
{
	unsigned char menubuf[MAX_SUBMENU][128] = 	{"增强       ", "稳像      ","稳像配置"};
	disMenuBuf.cnt = 3;
	for(int j = 0; j < disMenuBuf.cnt; j++)
	{
		disMenuBuf.osdBuffer[j].bshow = true;
		disMenuBuf.osdBuffer[j].alpha = 2;
		disMenuBuf.osdBuffer[j].color = 6;
		disMenuBuf.osdBuffer[j].posx = CHARPOSX;
		disMenuBuf.osdBuffer[j].posy = (j + 1) * CHARPOSY;
		setlocale(LC_ALL, "zh_CN.UTF-8");
		swprintf(disMenuBuf.osdBuffer[j].disMenu, 33, L"%s", menubuf[j]);
	}
	return;
}

void CMenu::menuOsdInit_param()
{
	unsigned char menubuf[MAX_SUBMENU][128] = 	{"稳像模式    " ,"滤波参数    "};

	disMenuBuf.cnt = 5;
	for(int j = 3; j < disMenuBuf.cnt; j++)
	{
		disMenuBuf.osdBuffer[j].bshow = true;
		disMenuBuf.osdBuffer[j].alpha = 2;
		disMenuBuf.osdBuffer[j].color = 6;
		disMenuBuf.osdBuffer[j].posx = CHARPOSX;
		disMenuBuf.osdBuffer[j].posy = (j + 1) * CHARPOSY;
		setlocale(LC_ALL, "zh_CN.UTF-8");
		swprintf(disMenuBuf.osdBuffer[j].disMenu, 33, L"%s", menubuf[j]);
	}

	return;
}


void CMenu::menuOsdInit_blank()
{
	memset(&disMenuBuf,0,sizeof(disMenuBuf));
	return;
}


unsigned char CMenu::getIndex(int x,int y)
{
	unsigned char ret = 255;

	for(int i=0; i < MAX_SUBMENU; i++)
	if(x > CHARPOSX && x < CHARPOSX + 100)
	{
		if(y > (i+1)*CHARPOSY && y < (i+2)*CHARPOSY)
			ret = i;
	}
	return ret;	
}


void CMenu::mouseHandle_main(int x,int y)
{
	unsigned char index  = getIndex( x, y);

	if(m_menuPointer < MAX_SUBMENU)
		disMenuBuf.osdBuffer[m_menuPointer].color = 6;

	if(index < MAX_SUBMENU)
	{
		m_menuPointer = index;
		disMenuBuf.osdBuffer[m_menuPointer].color = 3;
	}
	return;
}


void CMenu::mouseMove(int xMove , int yMove)
{
	switch(m_menuStat)
	{
		case MENU_MAIN:
		case MENU_PARAM:
			mouseHandle_main(xMove,yMove);
			break;
		
		default:
			break;
	}
	showOsd();
	return;	
}


void CMenu::getRGBA(int color,unsigned char& r,unsigned char& g,unsigned char& b,unsigned char& a)
{
	switch(color)
	{
		case 1:
			r = 0;
			g = 0;
			b = 0;
			break;
		case 2:
			r = 255;
			g = 255;
			b = 255;
			break;
		case 3:
			r = 255;
			g = 0;
			b = 0;
			break;
		case 4:
			r = 255;
			g = 255;
			b = 0;
			break;
		case 5:
			r = 0;
			g = 0;
			b = 255;
			break;
		case 6:
			r = 0;
			g = 255;
			b = 0;
			break;
		default:
			break;
	}
	
	if(a > 0x0a)
		a = 0x0a;
	if(a == 0x0a)
		a = 0;
	else
		a = 255 - a*16;
	return ;
}


