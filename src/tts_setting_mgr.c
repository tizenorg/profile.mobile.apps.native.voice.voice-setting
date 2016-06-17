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

#include <tts_setting.h>

#include "voice_setting_main.h"
#include "tts_setting_mgr.h"
#include "tts_setting_view.h"

GList* g_tts_voice_list = NULL;
GList* g_tts_engine_list = NULL;

static appdata *g_ad = NULL;

static bool __tts_setting_supported_voice_cb(const char* engine_id, const char* language, int voice_type, void* user_data)
{
	if (NULL != language) {
		tts_voice_s *tmp = (tts_voice_s *)calloc(1, sizeof(tts_voice_s));
		if (NULL == tmp) {
			LOGE("Fail to memory allocation");
			return false;
		}
		tmp->language = strdup(language);
		tmp->voice_type = voice_type;
		g_tts_voice_list = g_list_append(g_tts_voice_list, tmp);

		LOGD("+ Language(%s), Type(%d)", language, voice_type);
		return true;
	}
	return false;
}

static void __tts_setting_voice_changed_cb(const char* language, int voice_type, bool auto_voice, void *user_data)
{
	LOGD("=== Voice changed to Language(%s) Type(%d) Auto(%d)", language, voice_type, auto_voice);
}

static void __tts_setting_speed_changed_cb(int speed, void *user_data)
{
	LOGD("=== Speed changed to (%d)", speed);
}

static void __tts_setting_engine_changed_cb(const char* engine_id, void* user_data)
{
	LOGE("=== Engine changed to (%s)", engine_id);
	tts_setting_view_destroy(g_ad);
	tts_setting_view_create(g_ad);
}

int tts_setting_mgr_init(void *data)
{
	LOGD("=== TTS setting mgr init");

	if (0 != tts_setting_initialize()) {
		LOGE("Fail to setting initialize");
		return -1;
	}

	if (0 != tts_setting_set_voice_changed_cb(__tts_setting_voice_changed_cb, NULL)) {
		LOGE("Fail to set voice changed cb");
		return -1;
	}

	if (0 != tts_setting_set_speed_changed_cb(__tts_setting_speed_changed_cb, NULL)) {
		LOGE("Fail to set speed changed cb");
		return -1;
	}

	if (0 != tts_setting_set_engine_changed_cb(__tts_setting_engine_changed_cb, NULL)) {
		LOGE("Fail to set engien chagned cb");
		return -1;
	}

	g_ad = (appdata *)data;
	LOGD("===");
	LOGD("");

	return 0;
}

int tts_setting_mgr_deinit(void *data)
{
	LOGD("=== TTS setting mgr deinit");

	if (0 != tts_setting_unset_speed_changed_cb()) {
		LOGE("Fail to unset speed changed cb");
		return -1;
	}

	if (0 != tts_setting_unset_voice_changed_cb()) {
		LOGE("Fail to unset voice changed cb");
		return -1;
	}

	if (0 != tts_setting_finalize()) {
		LOGE("Fail to setting finalize");
		return -1;
	}

	/* Todo - Free voice list */
	if (0 < g_list_length(g_tts_voice_list)) {
		GList *iter = NULL;
		iter = g_list_first(g_tts_voice_list);
		
		while (NULL != iter) {
			tts_voice_s *tmp = iter->data;
			
			if (NULL != tmp) {
				if (NULL != tmp->language) {
					free(tmp->language);
					tmp->language = NULL;
				}
				free(tmp);
			}

			g_tts_voice_list = g_list_remove_link(g_tts_voice_list, iter);

			iter = g_list_first(g_tts_voice_list);
		}
	}

	if (0 < g_list_length(g_tts_engine_list)) {
		GList *iter = NULL;
		iter = g_list_first(g_tts_engine_list);

		while (NULL != iter) {
			tts_engine_info_s *tmp = iter->data;
			
			if (NULL != tmp) {
				if (NULL != tmp->engine_id) {
					free(tmp->engine_id);
					tmp->engine_id = NULL;
				}
				if (NULL != tmp->engine_name) {
					free(tmp->engine_name);
					tmp->engine_name = NULL;
				}
				if (NULL != tmp->setting_path) {
					free(tmp->setting_path);
					tmp->setting_path = NULL;
				}

				free(tmp);
			}

			g_tts_engine_list = g_list_remove_link(g_tts_engine_list, iter);

			iter = g_list_first(g_tts_engine_list);
		}
	}

	LOGD("===");
	LOGD("");

	return 0;
}

int tts_setting_mgr_get_auto_voice(bool* value)
{
	bool val = false;

	if (0 != tts_setting_get_auto_voice(&val)) {
		LOGE("Fail to get auto voice");
		return -1;
	}

	*value = val;
	LOGD("=== Get auto voice (%d)", *value);

	return 0;
}

int tts_setting_mgr_set_auto_voice(bool value)
{
	if (0 != tts_setting_set_auto_voice(value)) {
		LOGE("Fail to set auto voice (%d)", value);
		return -1;
	}

	LOGD("=== Set auto voice (%d)", value);

	return 0;
}

int tts_setting_mgr_get_voice(char** language, int* voice_type)
{
	char* lang = NULL;
	int type = -1;

	if (0 != tts_setting_get_voice(&lang, &type)) {
		LOGE("Fail to get voice");
		if (NULL != lang) {
			free(lang);
		}
		return -1;
	}

	if (NULL != lang) {
		*language = strdup(lang);
		free(lang);
	}
	*voice_type = type;
	LOGD("=== Get voice (%s), (%d)", *language, *voice_type);

	return 0;
}

int tts_setting_mgr_set_voice(const char* language, int voice_type)
{
	if (0 != tts_setting_set_voice(language, voice_type)) {
		LOGE("Fail to set voice (%s), (%d)", language, voice_type);
		return -1;
	}

	LOGD("=== Set voice (%s), (%d)", language, voice_type);
	
	return 0;
}

int tts_setting_mgr_get_speed(int* speed)
{
	int tmp = -1;

	if (0 != tts_setting_get_speed(&tmp)) {
		LOGE("Fail to get speed");
	}

	*speed = tmp;
	LOGD("=== Get speed (%d)", *speed);

	return 0;
}

int tts_setting_mgr_set_speed(int speed)
{
	if (0 != tts_setting_set_speed(speed)) {
		LOGE("Fail to set speed (%d)", speed);
	}

	LOGD("=== Set speed (%d)", speed);

	return 0;
}

GList* tts_setting_mgr_get_supported_voice()
{
	LOGD("=== Foreach supported voices ===");

	/* For auto voice */
	tts_voice_s *tmp = (tts_voice_s *)calloc(1, sizeof(tts_voice_s));
	if (NULL == tmp) {
		LOGE("Fail to memory allocation");
		return NULL;
	}
	tmp->language = strdup("Automatic");
	tmp->voice_type = -1;
	g_tts_voice_list = g_list_append(g_tts_voice_list, tmp);
	LOGD("+ Language(%s), Type(%d)", tmp->language, tmp->voice_type);

	if (0 != tts_setting_foreach_supported_voices(__tts_setting_supported_voice_cb, NULL)) {
		LOGE("Fail to get supported voices");
	}
	LOGD("===");
	LOGD("");

	return g_tts_voice_list;
}

int tts_setting_mgr_get_engine(char** engine_id)
{
	LOGD("=== Get current engine");

	if (NULL == engine_id) {
		LOGE("Invalid parameter");
		return -1;
	}

	if (0 != tts_setting_get_engine(engine_id)) {
		LOGE("Fail to get engine");
		return -1;
	}

	return 0;
}

static bool __tts_setting_supported_engine_cb(const char* engine_id, const char* engine_name, const char* setting_path, void* user_data)
{
	tts_engine_info_s *tmp = (tts_engine_info_s *)calloc(1, sizeof(tts_engine_info_s));
	if (NULL == tmp) {
		LOGE("Fail to memory allocation");
		return false;
	}

	if (NULL != engine_id)		tmp->engine_id = strdup(engine_id);
	if (NULL != engine_name)	tmp->engine_name = strdup(engine_name);
	if (NULL != setting_path)	tmp->setting_path = strdup(setting_path);

	g_tts_engine_list = g_list_append(g_tts_engine_list, tmp);

	return true;
}

GList* tts_setting_mgr_get_supported_engine()
{
	LOGD("=== Foreach supported engines ===");

	if (0 != tts_setting_foreach_supported_engines(__tts_setting_supported_engine_cb, NULL)) {
		LOGE("Fail to get supported engines");
	}

	return g_tts_engine_list;
}

int tts_setting_mgr_get_current_engine_info(char* engine_id, char** engine_name, char** setting_path)
{
	LOGD("=== Get current engine info ===");

	if (NULL == engine_id || NULL == engine_name || NULL == setting_path) {
		LOGE("Invalid parameter");
		return -1;
	}

	if (0 < g_list_length(g_tts_engine_list)) {
		GList *iter = NULL;
		iter = g_list_first(g_tts_engine_list);

		while (NULL != iter) {
			tts_engine_info_s *tmp = iter->data;
			
			if (NULL != tmp) {
				if (NULL != tmp->engine_id) {
					if (!strcmp(tmp->engine_id, engine_id)) {
						if (NULL != tmp->engine_name) {
							*engine_name = strdup(tmp->engine_name);
						}
						if (NULL != tmp->setting_path) {
							*setting_path = strdup(tmp->setting_path);
						}
					}
				}
			}
			iter = g_list_next(iter);
		}
	}
	return 0;
}