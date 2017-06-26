/*
 * HintLoader.h
 *
 *  Created on: 2016年3月15日
 *      Author: root
 */

#ifndef HINTLOADER_H_
#define HINTLOADER_H_

#include <Common.h>
#include <Singleton.h>
#include "UIHLDefine.h"

struct HintTypeInfo
{
	UIHLType type;
	vector<E_HINT_SHOW> show;
};
class HintLoader : public CommBaseOut::Singleton<HintLoader>
{
public:
	HintLoader();
	~HintLoader();

	int Init(string& path);
	int InitFile(string& file);

	vector<E_HINT_SHOW> ShowWhat(UIHLType type);

private:
	map<UIHLType, HintTypeInfo> m_hintList;
};

#endif /* HINTLOADER_H_ */
