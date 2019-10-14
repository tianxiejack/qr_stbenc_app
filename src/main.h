/*
 * main.h
 *
 *  Created on: 2018年8月23日
 *      Author: fsmdn121
 */

#ifndef MAIN_H
#define MAIN_H

#include <linux/videodev2.h>

#define SYS_CHN_CNT	(2)
#define SYS_CHN_DEVID(chn)		tDeviceId[chn]
#define SYS_CHN_WIDTH(chn)		tWidth[chn]
#define SYS_CHN_HEIGHT(chn)		tHeight[chn]
#define SYS_CHN_FPS(chn)		tFPS[chn]
#define SYS_CHN_FMT(chn)		tFormat[chn]

#define SYS_DIS0_FPS	(60)
#define SYS_DIS0_X		(0)
#define SYS_DIS0_Y		(0)
#define SYS_DIS0_WIDTH	(1920)
#define SYS_DIS0_HEIGHT	(1080)

#undef SYS_DIS1
#ifdef SYS_DIS1
#	define SYS_DIS1_FPS		(60)
#	define SYS_DIS1_X		(SYS_DIS0_WIDTH>>1)
#	define SYS_DIS1_Y		(SYS_DIS0_HEIGHT>>1)
#	define SYS_DIS1_WIDTH	(SYS_DIS0_WIDTH>>1)
#	define SYS_DIS1_HEIGHT	(SYS_DIS0_HEIGHT>>1)
#endif

static int tDeviceId[4] = {0, 1, 2, 3};
static int tWidth[4] = {1920, 1280, 1920, 1920};
static int tHeight[4] = {1080, 720, 1080, 1080};
static int tFPS[4] = {60, 60, 60, 60};
static int tFormat[4] = {V4L2_PIX_FMT_YUYV, V4L2_PIX_FMT_YUYV, V4L2_PIX_FMT_YUYV, V4L2_PIX_FMT_YUYV};

#define M_MAIN main_core
int main_cap(int argc, char **argv);
int main_core(int argc, char **argv);


#endif /* MAIN_H_ */
