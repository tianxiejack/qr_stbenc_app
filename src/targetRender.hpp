/*
 * targetRender.hpp
 *
 *  Created on: Jul 5, 2019
 *      Author: ubuntu
 */

#ifndef TARGET_RENDER_HPP_
#define TARGET_RENDER_HPP_

#include "crcoreRender.hpp"
#include "crosd.hpp"

class CTargetRender : public CCoreRenderBase
{
public:
	CTargetRender(int index = 0) :
		CCoreRenderBase(), m_index(index),m_bShow(false),m_bUpdate(true)
	{
		m_pos = cv::Rect(0,0,0,0);
		m_roi = cv::Rect(0,0,0,0);
	}

	virtual ~CTargetRender()
	{

	}

	void OnRender(void)
	{
		if(!m_bShow)
			return;

		if(m_bUpdate)
			CCoreRenderBase::Draw(m_pos, m_roi);
		else
			CCoreRenderBase::Draw(m_pos);

		m_bUpdate = false;
	}

	void Clear(void)
	{
		m_roi = cv::Rect(0,0,0,0);
		m_bUpdate = true;
	}

	int m_index;
	bool m_bShow;
	bool m_bUpdate;
	cv::Rect m_pos;
	cv::Rect m_roi;
};


#endif /* TARGET_RENDER_HPP_ */
