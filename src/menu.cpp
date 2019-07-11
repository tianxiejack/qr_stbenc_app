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


CMenu::CMenu():m_menuStat(0),m_enhStat(false),m_stbStat(false)
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

		case MENU_SEC:
			
				
			break;
			
		default:
			break;
	
	}

	return;
}


void CMenu::menuhandle_main()
{
	switch(m_menuPointer)
	{
		case 0:
			
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
			showOsd();
			break;
		case MENU_MAIN:
			gotoBlankMenu();
			showOsd();
			break;

		default:
			break;
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
	m_menuPointer = -1;
	m_menuStat = MENU_MAIN;
	menuOsdInit_main();
	updateEnhStatOsd();
	updateStbStatOsd();
	return;
}

void CMenu::gotoBlankMenu()
{
	m_menuPointer = -1;
	m_menuStat = MENU_BLANK;
	menuOsdInit_blank();
	return;
}


void CMenu::gotoSecMenu()
{
	
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
		swprintf(disMenuBuf.osdBuffer[0].disMenu, 33, L"稳像       开");
	else
		swprintf(disMenuBuf.osdBuffer[0].disMenu, 33, L"稳像       关");
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

void CMenu::menuOsdInit_blank()
{
	memset(&disMenuBuf,0,sizeof(disMenuBuf));
	return;
}


char CMenu::getIndex(int x,int y)
{
	char ret = 0;

	for(int i=1; i <= MAX_SUBMENU; i++)
	if(x > CHARPOSX && x < CHARPOSX + 100)
	{
		if(y > (i)*CHARPOSY && y < (i+1)*CHARPOSY)
			ret = i;
	}
	return ret;	
}


void CMenu::mouseHandle_main(int x,int y)
{
	char ret  = getIndex( x, y);


	printf("ret = %d \n" , ret);

	
	return;
}


void CMenu::mouseMove(int xMove , int yMove)
{
	switch(m_menuStat)
	{
		case MENU_MAIN:
			mouseHandle_main(xMove,yMove);
			break;

		default:
			break;
	}
	
	
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


