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
#include <voice_control_setting.h>

#include "voice_setting_main.h"
#include "vc_setting_mgr.h"

GList* g_vc_language_list = NULL;

static bool __vc_supported_language_cb(const char* language, void* user_data)
{
	if (NULL != language) {
		LOGD("+ Language (%s)", language);
		char *tmp = strdup(language);
		g_vc_language_list = g_list_append(g_vc_language_list, tmp);
		return true;
	}
	return false;
}

static void __vc_setting_enabled_changed_cb(bool enabled, void* user_data)
{
	LOGD("=== Voice control ENABLED changed to (%d)", enabled);
}

int vc_setting_mgr_init(void *data)
{
	LOGD("=== VC setting mgr init");

	if (0 != vc_setting_initialize()) {
		LOGE("Fail to setting initialize");
		return -1;
	}

	if (0 != vc_setting_set_enabled_changed_cb(__vc_setting_enabled_changed_cb, NULL)) {
		LOGE("Fail to set enabled changed cb");
		return -1;
	}
	LOGD("===");
	LOGD("");

	return 0;
}

int vc_setting_mgr_deinit(void *data)
{
	LOGD("=== VC setting mgr deinit");

	if (0 != vc_setting_unset_enabled_changed_cb()) {
		LOGE("Fail to unset enabled changed cb");
		return -1;
	}

	if (0 != vc_setting_deinitialize()) {
		LOGE("Fail to setting deinitialize");
		return -1;
	}

	if (0 < g_list_length(g_vc_language_list)) {
		GList *iter = NULL;
		iter = g_list_first(g_vc_language_list);
		
		while (NULL != iter) {
			char *tmp = iter->data;
			if (NULL != tmp) {
				free(tmp);
				tmp = NULL;
			}

			g_vc_language_list = g_list_remove_link(g_vc_language_list, iter);
			
			iter = g_list_first(g_vc_language_list);
		}
	}

	LOGD("===");
	LOGD("");

	return 0;
}

GList* vc_setting_mgr_get_supported_language()
{
	LOGD("=== Foreach Supported Language ===");
	if (0 != vc_setting_foreach_supported_languages(__vc_supported_language_cb, NULL)) {
		LOGE("Fail to foreach supported language");
		return NULL;
	}
	LOGD("===");
	LOGD("");

	return g_vc_language_list;
}

int vc_setting_mgr_get_current_language(char** language)
{
	char *lang = NULL;
	if (0 != vc_setting_get_language(&lang)) {
		LOGE("Fail to get language");
		if (NULL != lang) {
			free(lang);
		}
		return -1;
	}

	if (NULL != lang) {
		*language = strdup(lang);
		free(lang);
	}

	LOGD("=== Get current language(%s)", *language);

	return 0;
}

int vc_setting_mgr_set_current_language(const char* language)
{
	if (0 != vc_setting_set_language(language)) {
		LOGE("Fail to set language (%s)", language);
		return -1;
	}

	LOGD("=== Set current language (%s)", language);

	return 0;
}

int vc_setting_mgr_get_auto_language(bool* value)
{
	bool val= false;
	if (0 != vc_setting_get_auto_language(&val)) {
		LOGE("Fail to get auto language");
		return -1;
	}

	*value = val;
	LOGD("=== Get auto language (%d)", *value);

	return 0;
}

int vc_setting_mgr_set_auto_language(bool value)
{
	if (0 != vc_setting_set_auto_language(value)) {
		LOGE("Fail to set auto language (%d)", value);
		return -1;
	}

	LOGD("=== Set auto language (%d)", value);

	return 0;
}

int vc_setting_mgr_get_enabled(bool *value)
{
	bool val = false;
	if (0 != vc_setting_get_enabled(&val)) {
		LOGE("Fail to get enabled");
		return -1;
	}

	*value = val;
	LOGD("=== Get enabled (%d)", *value);

	return 0;
}

int vc_setting_mgr_set_enabled(bool value)
{
	if (0 != vc_setting_set_enabled(value)) {
		LOGE("Fail to set enabled (%d)", value);
		return -1;
	}

	LOGD("=== Set enabled (%d)", value);

	return 0;
}
