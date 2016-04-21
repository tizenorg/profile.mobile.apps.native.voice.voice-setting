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

#include "voice_setting_main.h"
#include "voice_setting_language.h"
#include "tts_setting_view.h"
#include "tts_setting_mgr.h"

static Elm_Genlist_Item_Class *g_itc_group_title = NULL;
static Elm_Genlist_Item_Class *g_itc_tts_speed_slider = NULL;
static Elm_Genlist_Item_Class *g_itc_tts_language = NULL;

static int g_tts_voice_radio_mark = 0;
static Evas_Object *g_tts_voice_radio_group = NULL;

static int g_tts_speed = 0;

static GList* g_tts_voice_list = NULL;

static appdata *g_ad;

static char *g_voice_type_text[6][3] = {
	{"Male", "Female", "Child"},
	{"\xEB\x82\xA8", "\xEC\x97\xAC", "\xEC\x95\x84\xEC\x9D\xB4"},
	{"Mannlich", "Weiblich", "Kind"},
	{"Homme", "Femme", "Enfant"},
	{"Maschio", "Femmina", "Bambino"},
	{"Hombre", "Mujer", "Hijo"}
};

static char *__tts_setting_view_group_title_text_get(void *data, Evas_Object *obj, const char *part)
{
	if (NULL == data)
		return NULL;

	if (!strcmp("elm.text", part)) {
		if (!strcmp("speed", data)) {
			return strdup(_("IDS_ST_HEADER_SPEECH_RATE_ABB"));
		} else if (!strcmp("language", data)) {
			return strdup(_("IDS_ST_OPT_LANGUAGE"));
		}
	}
	return NULL;
}

char *__tts_setting_view_conv_from_voice_type(const char *lang, int type)
{
	if (NULL == lang)
		return NULL;

	if (1 > type || 3 < type)
		return NULL;

	if (!strcmp("en_US", lang)) {
		return g_voice_type_text[0][type - 1];
	} else if (!strcmp("ko_KR", lang)) {
		return g_voice_type_text[1][type - 1];
	} else if (!strcmp("de_DE", lang)) {
		return g_voice_type_text[2][type - 1];
	} else if (!strcmp("fr_FR", lang)) {
		return g_voice_type_text[3][type - 1];
	} else if (!strcmp("it_IT", lang)) {
		return g_voice_type_text[4][type - 1];
	} else if (!strcmp("es_ES", lang)) {
		return g_voice_type_text[5][type - 1];
	}

	return NULL;
}

static char *__tts_setting_view_language_text_get(void *data, Evas_Object *obj, const char *part)
{
	if (!strcmp("elm.text", part)) {
		int idx = GPOINTER_TO_INT(data);
		GList *iter = g_list_nth(g_tts_voice_list, idx);
		if (NULL != iter) {
			tts_voice_s *tmp = iter->data;
			if (NULL != tmp) {
				char *lang = tmp->language;
				int type = tmp->voice_type;

				char text[256] = {'\0', };
				if (!strcmp(lang, "Automatic")) {
					if (0 != g_tts_voice_radio_mark) {
						snprintf(text, 256, "%s", _("IDS_VC_BODY_AUTOMATIC"));
					} else {
						char *cur_lang = NULL;
						int cur_type;
						tts_setting_mgr_get_voice(&cur_lang, &cur_type);
						if (NULL != cur_lang) {
							snprintf(text, 256, "%s (%s, %s)", _("IDS_VC_BODY_AUTOMATIC"), voice_setting_language_conv_id_to_lang(cur_lang), __tts_setting_view_conv_from_voice_type(cur_lang, cur_type));
							free(cur_lang);
							cur_lang = NULL;
						}
					}
				} else {
					snprintf(text, 256, "%s, %s", voice_setting_language_conv_id_to_lang(lang), __tts_setting_view_conv_from_voice_type(lang, type));
				}
				return strdup(text);
			}
		}
	}
	return NULL;
}

static void __tts_setting_view_radio_cb(void *data, Evas_Object *obj, void *event_info)
{
	intptr_t pidx = (intptr_t)data;
	int idx = (int)pidx;

	elm_radio_value_set(g_tts_voice_radio_group, idx);
}

static void __tts_setting_view_radio_group_del_cb(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
	LOGD("");
	g_tts_voice_radio_group = NULL;
}

static Evas_Object *__tts_setting_view_language_content_get(void *data, Evas_Object *obj, const char *part)
{
	intptr_t pidx = (intptr_t)data;
	int idx = (int)pidx;

	if (!strcmp("elm.swallow.icon.1", part)) {
		Evas_Object *radio = elm_radio_add(obj);
		elm_radio_state_value_set(radio, idx);
		evas_object_propagate_events_set(radio, EINA_FALSE);

		if (NULL == g_tts_voice_radio_group) {
			g_tts_voice_radio_group = radio;
			evas_object_event_callback_add(g_tts_voice_radio_group, EVAS_CALLBACK_DEL, __tts_setting_view_radio_group_del_cb, NULL);
		} else {
			elm_radio_group_add(radio, g_tts_voice_radio_group);
		}
		evas_object_smart_callback_add(radio, "changed", __tts_setting_view_radio_cb, (void *)pidx);

		if (idx == g_tts_voice_radio_mark) {
			elm_radio_value_set(g_tts_voice_radio_group, g_tts_voice_radio_mark);
		}

		evas_object_show(radio);
		return radio;
	}
	return NULL;
}

static int __tts_setting_view_conv_to_speed(double speed)
{
	LOGD("speed %f", speed);

	if (1.5 > speed) {
		return 2;
	} else if (1.5 <= speed && 2.5 > speed) {
		return 5;
	} else if (2.5 <= speed && 3.5 > speed) {
		return 8;
	} else if (3.5 <= speed && 4.5 > speed) {
		return 11;
	} else if (4.5 <= speed) {
		return 14;
	}

	return 0;
}

static int __tts_setting_view_conv_from_speed(int speed)
{
	if (2 == speed) {
		return 1;
	} else if (5 == speed) {
		return 2;
	} else if (8 == speed) {
		return 3;
	} else if (11 == speed) {
		return 4;
	} else if (14 == speed) {
		return 5;
	}

	return 3;
}

static void __tts_setting_view_slider_cb(void *data, Evas_Object *obj, void *event_info)
{
	double val = elm_slider_value_get(obj);
	
	int tmp = __tts_setting_view_conv_to_speed(val);

	if (tmp != g_tts_speed) {
		g_tts_speed = tmp;
		tts_setting_mgr_set_speed(tmp);
	}
}

static char *__tts_setting_view_conv_index_to_lang(int index)
{
	GList *iter = g_list_nth(g_tts_voice_list, index);
	if (NULL != iter) {
		tts_voice_s *tmp = iter->data;
		if (NULL != tmp) {
			return strdup(tmp->language);
		}
	}
	return NULL;
}

static int __tts_setting_view_conv_index_to_voice_type(int index)
{
	GList *iter = g_list_nth(g_tts_voice_list, index);
	if (NULL != iter) {
		tts_voice_s *tmp = iter->data;
		if (NULL != tmp) {
			return tmp->voice_type;
		}
	}
	return -1;
}

static int __tts_setting_view_conv_voice_to_index(const char* lang, int voice_type)
{
	int i;
	GList *iter;
	for (i = 0; i < g_list_length(g_tts_voice_list); i++) {
		iter = g_list_nth(g_tts_voice_list, i);
		if (NULL != iter) {
			tts_voice_s *tmp = iter->data;
			if (!strcmp(lang, tmp->language)) {
				if (voice_type == tmp->voice_type) {
					return i;
				}
			}
		}
	}

	return -1;
}

static Evas_Object *__tts_setting_view_speed_slider_content_get(void *data, Evas_Object *obj, const char *part)
{
	if (!strcmp("elm.swallow.content", part)) {
		Evas_Object *slider = elm_slider_add(obj);
		elm_object_style_set(slider, "center_point");
		elm_slider_horizontal_set(slider, EINA_TRUE);
		elm_slider_indicator_show_set(slider, EINA_TRUE);
		evas_object_size_hint_weight_set(slider, EVAS_HINT_EXPAND, 0.0);
		evas_object_size_hint_align_set(slider, EVAS_HINT_FILL, 0.5);
		elm_slider_indicator_format_set(slider, "%1.0f");
		elm_slider_step_set(slider, 1);
		elm_slider_min_max_set(slider, 1, 5);
		evas_object_propagate_events_set(slider, EINA_FALSE);
		
		int speed;
		tts_setting_mgr_get_speed(&speed);
		int val = __tts_setting_view_conv_from_speed(speed);
		elm_slider_value_set(slider, val);
		g_tts_speed = val;

		evas_object_smart_callback_add(slider, "changed", __tts_setting_view_slider_cb, NULL);

		evas_object_show(slider);
		return slider;
	}
	return NULL;
}

static void __tts_setting_view_language_clicked_cb(void *data, Evas_Object *obj, void *event_info)
{
	Elm_Object_Item *item = (Elm_Object_Item *)event_info;
	elm_genlist_item_selected_set(item, EINA_FALSE);

	intptr_t pidx = (intptr_t)data;
	int idx = (int)pidx;
	if (idx != g_tts_voice_radio_mark) {
		g_tts_voice_radio_mark = idx;
		elm_radio_value_set(g_tts_voice_radio_group, idx);

		if (0 == idx) {
			tts_setting_mgr_set_auto_voice(true);
		} else {
			tts_setting_mgr_set_auto_voice(false);
			char *lang = NULL;
			lang = __tts_setting_view_conv_index_to_lang(idx);
			if (NULL != lang) {
				tts_setting_mgr_set_voice(lang, __tts_setting_view_conv_index_to_voice_type(idx));
				free(lang);
				lang = NULL;
			}
		}
	}

	elm_genlist_item_fields_update(g_ad->tts_lang_auto, "elm.text", ELM_GENLIST_ITEM_NONE);
}

static void __tts_setting_view_speed_clicked_cb(void *data, Evas_Object *obj, void *event_info)
{
	Elm_Object_Item *item = (Elm_Object_Item *)event_info;
	elm_genlist_item_selected_set(item, EINA_FALSE);
}

void tts_setting_view_create(void *data)
{
	appdata *ad = (appdata *)data;
	g_ad = ad;

	if (0 != tts_setting_mgr_init(ad)) {
		return;
	}

	g_tts_voice_list = tts_setting_mgr_get_supported_voice();

	bool value;
	tts_setting_mgr_get_auto_voice(&value);
	if (true == value) {
		g_tts_voice_radio_mark = 0;
	} else {
		char *lang = NULL;
		int voice_type;
		tts_setting_mgr_get_voice(&lang, &voice_type);
		g_tts_voice_radio_mark = __tts_setting_view_conv_voice_to_index(lang, voice_type);
		if (NULL != lang) {
				free(lang);
				lang = NULL;
		}
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
	g_itc_group_title->func.text_get = __tts_setting_view_group_title_text_get;
	g_itc_group_title->func.content_get = NULL;
	g_itc_group_title->func.state_get = NULL;
	g_itc_group_title->func.del = NULL;

	g_itc_tts_speed_slider = elm_genlist_item_class_new();
	if (NULL == g_itc_tts_speed_slider) {
		LOGE("Fail to item class new");
		return;
	}
	g_itc_tts_speed_slider->item_style = "full";
	g_itc_tts_speed_slider->func.text_get = NULL;
	g_itc_tts_speed_slider->func.content_get = __tts_setting_view_speed_slider_content_get;
	g_itc_tts_speed_slider->func.state_get = NULL;
	g_itc_tts_speed_slider->func.del = NULL;

	g_itc_tts_language = elm_genlist_item_class_new();
	if (NULL == g_itc_tts_language) {
		LOGE("Fail to item class new");
		return;
	}
	g_itc_tts_language->item_style = "type1";
	g_itc_tts_language->func.text_get = __tts_setting_view_language_text_get;
	g_itc_tts_language->func.content_get = __tts_setting_view_language_content_get;
	g_itc_tts_language->func.state_get = NULL;
	g_itc_tts_language->func.del = NULL;

	Elm_Object_Item *item = NULL;
	/* Speed title */
	item = elm_genlist_item_append(ad->genlist, g_itc_group_title, "speed", NULL, ELM_GENLIST_ITEM_NONE, NULL, NULL);
	elm_genlist_item_select_mode_set(item, ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY);

	/* Speed slider */
	elm_genlist_item_append(ad->genlist, g_itc_tts_speed_slider, NULL, NULL, ELM_GENLIST_ITEM_NONE, __tts_setting_view_speed_clicked_cb, NULL);

	/* Language title */
	item = elm_genlist_item_append(ad->genlist, g_itc_group_title, "language", NULL, ELM_GENLIST_ITEM_NONE, NULL, NULL);
	elm_genlist_item_select_mode_set(item, ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY);

	/* Language */
	int i;
	for (i = 0; i < g_list_length(g_tts_voice_list); i++) {
		intptr_t pi = (intptr_t)i;
		item = elm_genlist_item_append(ad->genlist, g_itc_tts_language, (void *)pi, NULL, ELM_GENLIST_ITEM_NONE, __tts_setting_view_language_clicked_cb, (void *)pi);
		if (0 == i) {
			ad->tts_lang_auto = item;
		}
	}
}

void tts_setting_view_destroy(void *data)
{
	appdata *ad = (appdata *)data;
	
	tts_setting_mgr_deinit(ad);

	if (NULL != g_itc_group_title) {
		elm_genlist_item_class_free(g_itc_group_title);
		g_itc_group_title = NULL;
	}

	if (NULL != g_itc_tts_speed_slider) {
		elm_genlist_item_class_free(g_itc_tts_speed_slider);
		g_itc_tts_speed_slider = NULL;
	}

	if (NULL != g_itc_tts_language) {
		elm_genlist_item_class_free(g_itc_tts_language);
		g_itc_tts_language = NULL;
	}
}
