//  [4/7/2013 OMEGA] created

#ifndef _DUELGLCONFIG_H_
#define _DUELGLCONFIG_H_

#pragma warning(disable : 4355) // ��this��: ���ڻ���Ա��ʼֵ�趨���б�, ����Ǹ�vs��ȱ��

#ifdef	DUELGL_EXPORT
#define	DUELGL_API	__declspec(dllexport)
#else
#define	DUELGL_API	__declspec(dllimport)
#endif
#endif