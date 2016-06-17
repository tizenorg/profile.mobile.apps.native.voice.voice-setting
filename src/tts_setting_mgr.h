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
#ifndef __TTS_SETTING_MGR_H__
#define __TTS_SETTING_MGR_H__

#include <glib.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct{
	char* language;
	int voice_type;
}tts_voice_s;

typedef struct{
	char* engine_id;
	char* engine_name;
	char* setting_path;
}tts_engine_info_s;

int tts_setting_mgr_init(void *data);

int tts_setting_mgr_deinit(void *data);

int tts_setting_mgr_get_voice(char** language, int* voice_type);

int tts_setting_mgr_set_voice(const char* language, int voice_type);

int tts_setting_mgr_get_auto_voice(bool* value);

int tts_setting_mgr_set_auto_voice(bool value);

int tts_setting_mgr_get_speed(int* speed);

int tts_setting_mgr_set_speed(int speed);

GList* tts_setting_mgr_get_supported_voice();

int tts_setting_mgr_get_engine(char** engine_id);

GList* tts_setting_mgr_get_supported_engine();

int tts_setting_mgr_get_current_engine_info(const char* engine_id, char** engine_name, char** setting_path);

#ifdef __cplusplus
}
#endif

#endif /* __TTS_SETTING_MGR_H__ */
