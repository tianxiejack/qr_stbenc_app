/*
 * main_core.cpp
 *
 *  Created on: Sep 27, 2018
 *      Author: wzk
 */

#include <opencv2/opencv.hpp>
#include <glew.h>
#include <glut.h>
#include <freeglut_ext.h>
#include <opencv/cv.hpp>
#include <linux/videodev2.h>
#include <unistd.h>
#include <osa_sem.h>
#include "main.h"
#include "crCore.hpp"
#include "crosd.hpp"
#include "secondScreen.hpp"
#include "targetRender.hpp"
#include "thread.h"
#include "MultiChVideo.hpp"

using namespace cv;

#define WHITECOLOR 		0x008080FF
#define YELLOWCOLOR 	0x009110D2
#define CRAYCOLOR		0x0010A6AA
#define GREENCOLOR		0x00223691
#define MAGENTACOLOR	0x00DECA6A
#define REDCOLOR		0x00F05A51
#define BLUECOLOR		0x006EF029
#define BLACKCOLOR		0x00808010
#define BLANKCOLOR		0x00000000

static ICore_1001 *core = NULL;
static CORE1001_STATS stats;

static void processFrame_core(int cap_chid,unsigned char *src, struct v4l2_buffer capInfo, int format)
{
	if(capInfo.flags & V4L2_BUF_FLAG_ERROR){
		//OSA_printf("ch%d V4L2_BUF_FLAG_ERROR", cap_chid);
		return;
	}

	if(core != NULL){
		if(cap_chid == stats.mainChId){
			cv::Mat frame;
			if(format == V4L2_PIX_FMT_GREY){
				frame = cv::Mat(SYS_CHN_HEIGHT(cap_chid), SYS_CHN_WIDTH(cap_chid), CV_8UC1, src);
			}else{
				OSA_assert(format == V4L2_PIX_FMT_YUYV);
				frame = cv::Mat(SYS_CHN_HEIGHT(cap_chid), SYS_CHN_WIDTH(cap_chid)*2, CV_8UC1, src);
			}
			#if 0
			//Uint32 curTm = (OSA_getCurTimeInMsec())>>3;
			static float curTm = 0.0;
			int offsetY = (frame.rows-20)*fabs(sin(curTm*CV_PI/180.0));
			cv::rectangle(frame, cv::Rect(frame.cols-20, offsetY, 20, 20), cv::Scalar(255), -1);
			int offsetX = (frame.cols-20)*fabs(sin(curTm*CV_PI/180.0));
			cv::rectangle(frame, cv::Rect(offsetX, frame.rows-20, 20, 20), cv::Scalar(255), -1);
			curTm += 0.5;
			#endif
		}
		core->processFrame(cap_chid, src, capInfo, format);
	}
}

enum {
	KEYDIR_UP = 0, KEYDIR_DOWN, KEYDIR_LEFT, KEYDIR_RIGHT
};
static void Track_armRefine(int dir, int step = 1)
{
	switch(dir)
	{
	case KEYDIR_UP:
		{
			cv::Point raf(0, -1*step);
			core->setTrackPosRef(raf);
		}
		break;
	case KEYDIR_DOWN:
		{
			cv::Point raf(0, step);
			core->setTrackPosRef(raf);
		}
		break;
	case KEYDIR_LEFT:
		{
			cv::Point raf(-1*step, 0);
			core->setTrackPosRef(raf);
		}
		break;
	case KEYDIR_RIGHT:
		{
			cv::Point raf(step, 0);
			core->setTrackPosRef(raf);
		}
		break;
	default:
		break;
	}
}

static void Axis_move(int dir, int step = 1)
{
	cv::Point2f curPos = core->m_stats.chn[core->m_stats.mainChId].axis;
	cv::Point pos;
	switch(dir)
	{
	case KEYDIR_UP:
		{
			pos = cv::Point(curPos.x+0.5, curPos.y+0.5-1*step);
			core->setAxisPos(pos);
		}
		break;
	case KEYDIR_DOWN:
		{
			pos = cv::Point(curPos.x+0.5, curPos.y+0.5+1*step);
			core->setAxisPos(pos);
		}
		break;
	case KEYDIR_LEFT:
		{
			pos = cv::Point(curPos.x+0.5-1*step, curPos.y+0.5);
			core->setAxisPos(pos);
		}
		break;
	case KEYDIR_RIGHT:
		{
			pos = cv::Point(curPos.x+0.5+1*step, curPos.y+0.5);
			core->setAxisPos(pos);
		}
		break;
	default:
		break;
	}
	OSA_printf("%s %d: cur(%f,%f) to(%d %d)", __func__, __LINE__,curPos.x, curPos.y, pos.x, pos.y);
}

static int iMenu = 0;
static int chrChId = 0;
static void keyboard_event(unsigned char key, int x, int y)
{
	#if 1
	cv::Size winSize(80, 60);
	static int fovId[SYS_CHN_CNT] = {0,0};
	static bool mmtdEnable = false;
	static bool trkEnable = false;
	static bool motionDetect = false;
	static bool blobEnable = false;

	char strMenus[2][2048] ={
			"\n"
			"----------------------------------------\n"
			"|---Main Menu -------------------------|\n"
			"----------------------------------------\n"
			" [t] Main Channel Choice chId++         \n"
			" [f] Main Channel Choice Fov++          \n"
			" [a] Enable/Disable Track               \n"
			" [b] Enable/Disable MMTD                \n"
			" [B] Enable/Disable INTELL              \n"
			" [c] Enable/Disable Enh                 \n"
			" [d] Change EZoomx (X1/X2/X4)           \n"
			" [e] Enable/Disable stab                \n"
			" [E] Enable/Disable OSD                 \n"
			" [g] Change OSD Color                   \n"
			" [h] Force Track To Coast               \n"
			" [i][k][j][l] Refine Track Pos          \n"
			" [m] Setup Axis                         \n"
			" [n] Start/Pause Encoder transfer       \n"
			" [u] Change EncTrans level (0/1/2)      \n"
			" [o] Enable/Disable motion detect       \n"
			" [p] Change Pinp channel ID (0/1/null)  \n"
			" [r] Bind/Unbind blend TV BY Flr        \n"
			" [s] Enable/Disable Blob detect         \n"
			" [v] Move sub window position           \n"
			" [w] EZoomx sub window video            \n"
			" [x] Force Track to keep locked         \n"
			" [1].[5] Enable Track By MMTD           \n"
			" [esc][q]Quit                           \n"
			"--> ",

			"\n"
			"----------------------------------------\n"
			"|---Axis Menu -------------------------|\n"
			"----------------------------------------\n"
			" [t] Main Channel Choice chId++         \n"
			" [f] Main Channel Choice Fov++          \n"
			" [i] Move Up                            \n"
			" [k] Move Down                          \n"
			" [j] Move Left                          \n"
			" [l] Move Right                         \n"
			" [s] Save to file                       \n"
			" [esc][q]Back                           \n"
			"--> ",
	};

	switch(key)
	{
	case 't':
		chrChId ++;
		if(chrChId == SYS_CHN_CNT)
			chrChId = 0;
		winSize.width *= ((float)SYS_CHN_WIDTH(chrChId))/1920.0f;
		winSize.height *= ((float)SYS_CHN_HEIGHT(chrChId))/1080.0f;
		core->setMainChId(chrChId, fovId[chrChId], 0, winSize);
		break;
	case 'f':
		fovId[chrChId] = (fovId[chrChId]<4-1) ? (fovId[chrChId]+1) : 0;
		winSize.width *= ((float)SYS_CHN_WIDTH(chrChId))/1920.0f;
		winSize.height *= ((float)SYS_CHN_HEIGHT(chrChId))/1080.0f;
		core->setMainChId(chrChId, fovId[chrChId], 0, winSize);
		break;
	case 'z':
		OSA_printf("%s %d: ...", __func__, __LINE__);
		if(chrChId == 2)
			fovId[chrChId] = (fovId[chrChId]<4-1) ? (fovId[chrChId]+1) : 0;
		chrChId = 2;
		winSize.width *= ((float)SYS_CHN_WIDTH(chrChId))/1920.0f;
		winSize.height *= ((float)SYS_CHN_HEIGHT(chrChId))/1080.0f;
		core->setMainChId(chrChId, fovId[chrChId], 0, winSize);
		break;
	case 'u':
		static int speedLevel = 0;
		speedLevel++;
		if(speedLevel>2)
			speedLevel=0;
		core->setEncTransLevel(speedLevel);
		break;
	case 'a':
		trkEnable ^= 1;
		//winSize.width = 40; winSize.height = 30;
		//winSize.width *= ((float)SYS_CHN_WIDTH(chrChId))/1920.0f;
		//winSize.height *= ((float)SYS_CHN_HEIGHT(chrChId))/1080.0f;
		core->enableTrack(trkEnable, winSize, true);
		//Track_armRefine(KEYDIR_UP, 1);
		break;
	//case 'w':
	//	winSize.width = 80; winSize.height = 60;
	//	core->enableTrack(trkEnable, winSize, true);
	//	break;
	case 'b':
	{
		mmtdEnable ^=1;
		//core->enableMMTD(mmtdEnable, 8, 8, cv::Rect(SYS_CHN_WIDTH(chrChId)/4, SYS_CHN_HEIGHT(chrChId)/4, SYS_CHN_WIDTH(chrChId)/2, SYS_CHN_HEIGHT(chrChId)/2));
		//core->enableMMTD(mmtdEnable, 8, 3);//, cv::Rect(SYS_CHN_WIDTH(chrChId)/2-300, SYS_CHN_HEIGHT(chrChId)/2-200, 600, 400));
		core->enableMMTD(mmtdEnable, 5, 5);//, cv::Rect(SYS_CHN_WIDTH(chrChId)/2-300, SYS_CHN_HEIGHT(chrChId)/2-200, 600, 400));
	}
		break;
	case 'B':
		mmtdEnable ^=1;
		core->enableIntellDetect(mmtdEnable, 11, 11);
		break;
	case 'o':
		motionDetect ^=1;
		core->enableMotionDetect(motionDetect);
		break;
	case 'c':
	{
		static bool enhEnable[SYS_CHN_CNT] = {false, };
		enhEnable[chrChId] ^= 1;
		core->enableEnh(enhEnable[chrChId]);
	}
		break;
	case 'd':
	{
		static int ezoomx[4] = {1,1,1,1};
		ezoomx[chrChId] = (ezoomx[chrChId] == 4 || ezoomx[chrChId] < 1) ? 1 : ezoomx[chrChId]<<1;
		core->setEZoomx(ezoomx[chrChId]);
	}
		break;
	case 'E':
	{
		static bool osdEnable = true;
		osdEnable ^= 1;
		core->enableOSD(osdEnable);
	}
		break;
	case 'e':
	{
		static bool stabEnable[SYS_CHN_CNT] = {false, };
		stabEnable[chrChId] ^= 1;
		core->enableStab(stabEnable[chrChId], -1.0f, false, false);
	}
		break;
	case 'g':
	{
		static int colorTab[] = {WHITECOLOR,YELLOWCOLOR,CRAYCOLOR,GREENCOLOR,MAGENTACOLOR,REDCOLOR,BLUECOLOR,BLACKCOLOR};
		static int icolor = 0;
		icolor = (icolor<sizeof(colorTab)/sizeof(int)-1) ? (icolor+1) : 0;
		core->setOSDColor(colorTab[icolor], 2);
		cr_osd::set(colorTab[icolor]);
	}
		break;
	case 'h':
	{
		static bool coastEnable = false;
		coastEnable ^= 1;
		core->setTrackCoast(((coastEnable) ? -1: 0));
	}
		break;
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	{
		//OSA_printf("enableTrackByMMTD winSize(%dx%d) chrChId=%d", winSize.width, winSize.height,chrChId);
		//winSize.width *= ((float)SYS_CHN_WIDTH(chrChId))/1920.0f;
		//winSize.height *= ((float)SYS_CHN_HEIGHT(chrChId))/1080.0f;
		//OSA_printf("enableTrackByMMTD winSize(%dx%d)", winSize.width, winSize.height);
		if(core->enableTrackByMMTD(key-'1', &winSize, true)==OSA_SOK){
			trkEnable = true;
			mmtdEnable = false;
			core->enableMMTD(mmtdEnable, 0);
		}
	}
		break;
	case 'i'://move up
		if(iMenu == 0 && trkEnable)
			Track_armRefine(KEYDIR_UP);
		if(iMenu == 1)
			Axis_move(KEYDIR_UP);
		break;
	case 'k'://move down
		if(iMenu == 0 && trkEnable)
			Track_armRefine(KEYDIR_DOWN);
		if(iMenu == 1)
			Axis_move(KEYDIR_DOWN);
		break;
	case 'j'://move left
		if(iMenu == 0 && trkEnable)
			Track_armRefine(KEYDIR_LEFT);
		if(iMenu == 1)
			Axis_move(KEYDIR_LEFT);
		break;
	case 'l'://move right
		if(iMenu == 0 && trkEnable)
			Track_armRefine(KEYDIR_RIGHT);
		if(iMenu == 1)
			Axis_move(KEYDIR_RIGHT);
		break;
	case 'm':
		printf("%s",strMenus[iMenu]);
		iMenu = 1;
		break;
	case 'n':
		static bool encEnable[2] = {true, true};
		encEnable[chrChId] ^=1;
		core->enableEncoder(chrChId, encEnable[chrChId]);
		break;
	case 's':
		if(iMenu == 1)
			core->saveAxisPos();
		else{
			blobEnable ^= 1;
			core->enableBlob(blobEnable);
		}
		break;
	case 'p':
		static int subChId = -1;
		subChId++;
		if(subChId==SYS_CHN_CNT)
			subChId = -1;
		core->setSubChId(subChId);
		break;
	case 'r':
	{
		static int blendchId = -1;
		blendchId = (blendchId == -1) ? 0 : -1;
		cv::Matx44f matricScale;
		cv::Matx44f matricRotate;
		cv::Matx44f matricTranslate;
		matricScale = cv::Matx44f::eye();
		matricRotate = cv::Matx44f::eye();
		matricTranslate = cv::Matx44f::eye();
		//matricScale.val[0] = 0.5f;
		//matricScale.val[5] = 0.5f;
		float rads = float(0) * 0.0174532925f;
		matricRotate.val[0] = cos(rads);
		matricRotate.val[1] = -sin(rads);
		matricRotate.val[4] = sin(rads);
		matricRotate.val[5] = cos(rads);
		//matricTranslate.val[3] = 0.5f;
		//matricTranslate.val[7] = -0.5f;
		core->bindBlend(1, blendchId, (matricTranslate*matricRotate*matricScale).t());
	}
		break;

	case 'v':
	{
		static int posflag = -1;
		posflag++;
		posflag = (posflag >= 4) ? 0 : posflag;
		cv::Rect rc;
		int width = SYS_DIS0_WIDTH, height = SYS_DIS0_HEIGHT;
		switch(posflag)
		{
		case 1:
			rc = cv::Rect(width*2/3, 0, width/3, height/3);
			break;
		case 2:
			rc = cv::Rect(0, 0, width/3, height/3);
			break;
		case 3:
			rc = cv::Rect(0, height*2/3, width/3, height/3);
			break;
		case 0:
		default:
			rc = cv::Rect(width*2/3, height*2/3, width/3, height/3);
			break;
		}
		core->setWinPos(1, rc);
	}
		break;
	case 'w':
	{
		static int ezoomxSub = 1;
		ezoomxSub = (ezoomxSub == 8) ? 1 : ezoomxSub<<1;
		cv::Matx44f matricScale;
		matricScale = cv::Matx44f::eye();
		matricScale.val[0] = ezoomxSub;
		matricScale.val[5] = ezoomxSub;
		core->setWinMatric(1, matricScale.t());
	}
		break;
	case 'x':
	{
		static bool keeplocked = false;
		keeplocked ^= 1;
		core->setTrackForce(keeplocked);
	}
		break;
	case 'q':
	case 27:
		if(iMenu == 0)
			glutLeaveMainLoop();
		else
			iMenu = 0;
		break;
	default:
		printf("%s",strMenus[iMenu]);
		break;
	}
	#endif
}


/*********************************************************
 *
 * test main
 */
static void *thrdhndl_glutloop(void *context)
{
	for(;;){
		int key = getc(stdin);
		if(key == 10)
			continue;
		if(key == 'q' || key == 'Q' || key == 27){
			if(iMenu == 0)
				break;
			else
				iMenu = 0;
		}
		keyboard_event(key, 0, 0);
	}
	if(*(bool*)context)
		glutLeaveMainLoop();
	OSA_printf("%s %d: leave.", __func__, __LINE__);
	//exit(0);
	return NULL;
}
static void *thrdhndl_keyevent(void *context)
{
	for(;;){
		int key = getchar();//getc(stdin);
		if(iMenu == 0 && (key == 'q' || key == 'Q' || key == 27)){
			break;
		}
		//OSA_printf("key = %d\n\r", key);
		keyboard_event(key, 0, 0);
	}
	if(*(bool*)context)
		glutLeaveMainLoop();
	OSA_printf("%s %d: leave.", __func__, __LINE__);
	//exit(0);
	return NULL;
}
static int callback_process(void *handle, int chId, Mat frame, struct v4l2_buffer capInfo, int format)
{
	processFrame_core(chId, frame.data, capInfo, format);
	return 0;
}


static void mouse_event(int button, int state, int x, int y)
{
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{

		printf("left  down \n");
	}
	else if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		printf("left  UP \n");

	}
	else if(button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
		printf("RIGHT  UP \n");
	else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
		printf("RIGHT  down \n");
	return;
}


static CORE1001_INIT_PARAM initParam;
static bool bLoop = true;
static char strIpAddr[32] = "192.168.1.88";

#include "UtcTrack.h"
extern UTCTRACK_HANDLE g_track;
#include "MMTD.h"
extern CMMTD *g_mmtd;

int main_core(int argc, char **argv)
{
	core = (ICore_1001 *)ICore::Qury(COREID_1001);
	memset(&initParam, 0, sizeof(initParam));
	initParam.renderRC = cv::Rect(SYS_DIS0_X, SYS_DIS0_Y, SYS_DIS0_WIDTH, SYS_DIS0_HEIGHT);
	initParam.renderFPS = SYS_DIS0_FPS;
	initParam.renderSched = 3.5f;
	initParam.nChannels = SYS_CHN_CNT;
	for(int i=0; i<SYS_CHN_CNT; i++){
		initParam.chnInfo[i].imgSize = cv::Size(SYS_CHN_WIDTH(i), SYS_CHN_HEIGHT(i));
		initParam.chnInfo[i].fps = SYS_CHN_FPS(i);
		initParam.chnInfo[i].format = SYS_CHN_FMT(i);
	}
	initParam.chnInfo[1].bZoomRender = true;
	initParam.chnInfo[1].zoomRatio.x = 1.0;
	initParam.chnInfo[1].zoomRatio.y = 1.0;
	initParam.bRender = true;
	initParam.bEncoder = false;
	initParam.bHideOSD = true;

	core->init(&initParam, sizeof(initParam));
	unsigned int mask = 0;
	core->setHideSysOsd(mask);
	//glClearColor(0.0,1.0,0,1.0);

	MultiChVideo MultiCh;
	MultiCh.m_user = NULL;
	MultiCh.m_usrFunc = callback_process;
	MultiCh.creat();
	MultiCh.run();

	core->enableOSD(false);

	if(initParam.bRender){
		start_thread(thrdhndl_keyevent, &initParam.bRender);
		glutKeyboardFunc(keyboard_event);
		glutMouseFunc(mouse_event);
		glutMainLoop();
	}else{
		thrdhndl_keyevent(&initParam.bRender);
	}
	bLoop = false;
	OSA_printf("%s %d: leave.", __func__, __LINE__);

	core->uninit();
	ICore::Release(core);
	core = NULL;

	return 0;
}


