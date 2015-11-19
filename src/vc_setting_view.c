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

#include <glib.h>
#include <stdbool.h>

#include "voice_setting_main.h"
#include "vc_setting_view.h"
#include "vc_setting_mgr.h"

static Elm_Genlist_Item_Class *g_itc_group_title = NULL;
static Elm_Genlist_Item_Class *g_itc_voice_control_onoff = NULL;
static Elm_Genlist_Item_Class *g_itc_voice_control_language = NULL;

static Evas_Object *g_vc_language_radio_group = NULL;
static int g_vc_language_radio_mark = 0;

static GList* g_vc_language_list = NULL;

static int __vc_setting_view_conv_id_to_index(const char* lang)
{
	int i;
	GList *iter;
	for (i = 0; i < g_list_length(g_vc_language_list); i++) {
		iter = g_list_nth(g_vc_language_list, i);
		if (NULL != iter) {
			if (!strcmp(lang, iter->data)) {
				return i;
			}
		}
	}

	return -1;
}

static char* __vc_setting_view_conv_index_to_lang(int index)
{
	GList *iter = g_list_nth(g_vc_language_list, index);
	if (NULL != iter) {
		return strdup(iter->data);
	}

	return NULL;
}

static char *__vc_setting_view_group_title_text_get(void *data, Evas_Object *obj, const char *part)
{
	if (!strcmp("elm.text", part)) {
		return strdup(_("IDS_ST_OPT_LANGUAGE"));
	}
	return NULL;
}

static char *__vc_setting_view_onoff_text_get(void *data, Evas_Object *obj, const char *part)
{
	if (!strcmp("elm.text", part)) {
		return strdup(_("IDS_VOICE_BODY_VOICE_CONTROL_ABB2"));
	}
	return NULL;
}

static char *__vc_setting_view_language_text_get(void *data, Evas_Object *obj, const char *part)
{
	if (!strcmp("elm.text", part)) {
		int idx = (int *)data;
		GList *iter = g_list_nth(g_vc_language_list, idx);
		if (NULL != iter) {
			char *lang = iter->data;
			char *conv = voice_setting_language_conv_id_to_lang(lang);
			if (NULL != conv) {
					return strdup(conv);
			}
		}
	}
	return NULL;
}

static void __vc_setting_view_onoff_cb(void *data, Evas_Object *obj, void *event_info)
{
	Eina_Bool state = elm_check_state_get(obj);
	vc_setting_mgr_set_enabled(state);
}

static Evas_Object *__vc_setting_view_onoff_content_get(void *data, Evas_Object *obj, const char *part)
{
	if (!strcmp("elm.swallow.icon.1", part)) {
		Evas_Object *check = elm_check_add(obj);
		elm_object_style_set(check, "on&off");
		
		bool state;
		vc_setting_mgr_get_enabled(&state);
		elm_check_state_set(check, state);

		evas_object_propagate_events_set(check, EINA_FALSE);
		evas_object_smart_callback_add(check, "changed", __vc_setting_view_onoff_cb, NULL);
		evas_object_show(check);
		return check;
	}
	return NULL;
}

static void __vc_setting_view_radio_cb(void *data, Evas_Object *obj, void *event_info)
{
	int idx = (int *)data;
	elm_radio_value_set(g_vc_language_radio_group, idx);
}

static void __vc_setting_view_radio_group_del_cb(void *data, Evas_Object *obj, void *event_info)
{
	LOGD("");
	g_vc_language_radio_group = NULL;
}

static Evas_Object *__vc_setting_view_language_content_get(void *data, Evas_Object *obj, const char *part)
{
	int idx = (int *)data;

	if (!strcmp("elm.swallow.icon.1", part)) {
		Evas_Object *radio = elm_radio_add(obj);
		elm_radio_state_value_set(radio, idx);
		evas_object_propagate_events_set(radio, EINA_FALSE);
		
		if (NULL == g_vc_language_radio_group) {
			g_vc_language_radio_group = radio;
			evas_object_event_callback_add(g_vc_language_radio_group, EVAS_CALLBACK_DEL, __vc_setting_view_radio_group_del_cb, NULL);
		} else {
			elm_radio_group_add(radio, g_vc_language_radio_group);
		}
		evas_object_smart_callback_add(radio, "changed", __vc_setting_view_radio_cb, (void *)idx);

		if (idx == g_vc_language_radio_mark) {
			elm_radio_value_set(g_vc_language_radio_group, g_vc_language_radio_mark);
		}

		evas_object_show(radio);
		return radio;
	}
	return NULL;
}

static void __vc_setting_view_onoff_clicked_cb(void *data, Evas_Object *obj, void *event_info)
{
	Elm_Object_Item *item = (Elm_Object_Item *)event_info;
	elm_genlist_item_selected_set(item, EINA_FALSE);

	bool state;
	vc_setting_mgr_get_enabled(&state);

	state = (state == false) ? true : false;

	vc_setting_mgr_set_enabled(state);

	elm_genlist_item_update(item);
}

static void __vc_setting_view_language_clicked_cb(void *data, Evas_Object *obj, void *event_info)
{
	Elm_Object_Item *item = (Elm_Object_Item *)event_info;
	elm_genlist_item_selected_set(item, EINA_FALSE);

	int idx = (int *)data;
	if (idx != g_vc_language_radio_mark) {
		g_vc_language_radio_mark = idx;
		elm_radio_value_set(g_vc_language_radio_group, idx);
		vc_setting_mgr_set_auto_language(false);
		vc_setting_mgr_set_current_language(__vc_setting_view_conv_index_to_lang(idx));
	}
}

void vc_setting_view_create(void *data)
{
	appdata *ad = (appdata *)data;

	vc_setting_mgr_init(ad);

	g_vc_language_list = vc_setting_mgr_get_supported_language();

	char* lang = NULL;
	vc_setting_mgr_get_current_language(&lang);
	if (NULL != lang) {
		g_vc_language_radio_mark = __vc_setting_view_conv_id_to_index(lang);
		free(lang);
		lang = NULL;
	}

	if (NULL != ad->genlist) {
		elm_genlist_clear(ad->genlist);
	}

	g_itc_group_title = elm_genlist_item_class_new();
	if (NULL == g_itc_group_title) {
		LOGE("Fail to item class new");
		return;
	}
	g_itc_group_title->item_style = "group_index";
	g_itc_group_title->func.text_get = __vc_setting_view_group_title_text_get;
	g_itc_group_title->func.content_get = NULL;
	g_itc_group_title->func.state_get = NULL;
	g_itc_group_title->func.del = NULL;

	g_itc_voice_control_onoff = elm_genlist_item_class_new();
	if (NULL == g_itc_voice_control_onoff) {
		LOGE("Fail to item class new");
		return;
	}
	g_itc_voice_control_onoff->item_style = "type1";
	g_itc_voice_control_onoff->func.text_get = __vc_setting_view_onoff_text_get;
	g_itc_voice_control_onoff->func.content_get = __vc_setting_view_onoff_content_get;
	g_itc_voice_control_onoff->func.state_get = NULL;
	g_itc_voice_control_onoff->func.del = NULL;

	g_itc_voice_control_language = elm_genlist_item_class_new();
	if (NULL == g_itc_voice_control_language) {
		LOGE("Fail to item class new");
		return;
	}
	g_itc_voice_control_language->item_style = "type1";
	g_itc_voice_control_language->func.text_get = __vc_setting_view_language_text_get;
	g_itc_voice_control_language->func.content_get = __vc_setting_view_language_content_get;
	g_itc_voice_control_language->func.state_get = NULL;
	g_itc_voice_control_language->func.del = NULL;

	/* Voice control onoff*/
	elm_genlist_item_append(ad->genlist, g_itc_voice_control_onoff, NULL, NULL, ELM_GENLIST_ITEM_NONE, __vc_setting_view_onoff_clicked_cb, NULL);

	/* Language group title */
	Elm_Object_Item *item = elm_genlist_item_append(ad->genlist, g_itc_group_title, NULL, NULL, ELM_GENLIST_ITEM_NONE, NULL, NULL);
	elm_genlist_item_select_mode_set(item, ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY);

	/* Language */
	int i;
	for (i = 0; i < g_list_length(g_vc_language_list); i++) {
		elm_genlist_item_append(ad->genlist, g_itc_voice_control_language, (void *)i, NULL, ELM_GENLIST_ITEM_NONE, __vc_setting_view_language_clicked_cb, (void *)i);
	}
}

void vc_setting_view_destroy(void *data)
{
	appdata *ad = (appdata *)data;
	
	vc_setting_mgr_deinit(ad);

	if (NULL != g_itc_group_title) {
		elm_genlist_item_class_free(g_itc_group_title);
		g_itc_group_title = NULL;
	}
	
	if (NULL != g_itc_voice_control_onoff) {
		elm_genlist_item_class_free(g_itc_voice_control_onoff);
		g_itc_voice_control_onoff = NULL;
	}

	if (NULL != g_itc_voice_control_language) {
		elm_genlist_item_class_free(g_itc_voice_control_language);
		g_itc_voice_control_language = NULL;
	}
}
