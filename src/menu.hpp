/*
 * menu.hpp
 *
 *  Created on: 2019年7月11日
 *      Author: alex
 */

#ifndef MENU_HPP_
#define MENU_HPP_

#define MAX_SUBMENU 5

#define OUTPUTW 1920
#define OUTPUTH	1080


typedef enum{
	MENU_BLANK,
	MENU_MAIN,
	MENU_PARAM,
	MENU_MAX,
}MenuState_t;


typedef enum{
	MODE_AUTO,
	MODE_TRANS,
	MODE_TRANSSCALE,
	MODE_RIGID,
	MODE_PERSPECTIVE,
	MODE_MAX,
}StbMode_t;

typedef enum{
	FILTER_HIGHT,
	FILTER_MID,
	FILTER_LOW,
	FILTER_MAX,
}StbParam_t;

typedef struct
{
	bool bshow;
	unsigned char color;
	unsigned char alpha;
	unsigned int posx;
	unsigned int posy;
	wchar_t disMenu[33];
}osdInfopos_t;

typedef struct{
	int cnt;
	osdInfopos_t osdBuffer[MAX_SUBMENU];
}osdInfo_t;


class CMenu{
public:
	CMenu(void* pfun);
	~CMenu();

	void menuButton();
	void enter();
	void mouseMove(int xMove , int yMove);
	
	void mouseHandle_main(int x,int y);

	void showOsd();
	void eraseOsd();

	void gotoBlankMenu();
	void gotoMainMenu();
	void gotoParamMenu();

	void menuOsdInit_blank();
	void menuOsdInit_main();
	void menuOsdInit_param();

	void updateEnhStatOsd();
	void updateStbStatOsd();
	void updateStbModeOsd();
	void updateStbFilterOsd();
		

	void menuhandle_main();
	void menuhandle_param();

	unsigned char getIndex(int x,int y);

	void setStbworkmode();
	void setStbparam();
	void enhHandle();
	void stbHandle();

	void menuBackground();
	void eraseMenuBackground();

	void getRGBA(int color,unsigned char& r,unsigned char& g,unsigned char& b,unsigned char& a);

private:

	bool m_enhStat,m_stbStat,m_paramStat;
	osdInfo_t disMenuBuf;

	unsigned char m_menuPointer,m_stbmode,m_stbparam;
	signed char m_menuStat;
	
};



#endif /* MENU_HPP_ */
