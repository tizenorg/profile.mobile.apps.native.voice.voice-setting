/*
* Copyright (c) 2015 Samsung Electronics Co., Ltd. All rights reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
*/
#ifndef __VOICE_SETTING_H__
#define __VOICE_SETTING_H__

#include <app.h>
#include <Elementary.h>
#include <dlog.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef  LOG_TAG
#undef  LOG_TAG
#endif
#define LOG_TAG "VOICE_SETTING"

enum {
	APP_STATE_PAUSE = 1,
	APP_STATE_SERVICE,
	APP_STATE_RESUME,
	APP_STATE_TERMINATE,
};

enum {
	SHOW_VOICE_CONTROL = 0,
	SHOW_TTS,
};

typedef struct _appdata {
	Evas_Object *win;
	Evas_Object *conform;
	Evas_Object *naviframe;
	Evas_Object *genlist;

	Elm_Object_Item *tts_lang_auto;

	int app_state;

	int current_page;
}appdata;

#ifdef __cplusplus
}
#endif

#endif /* __VOICE_SETTING_H__ */
