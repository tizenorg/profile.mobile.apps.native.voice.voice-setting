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
static Elm_Genlist_Item_Class *g_itc_tts_setting = NULL;

static int g_tts_voice_radio_mark = 0;
static Evas_Object *g_tts_voice_radio_group = NULL;

static int g_tts_speed = 0;

static GList* g_tts_voice_list = NULL;

static appdata *g_ad;

static tts_engine_info_s *g_tts_engine = NULL;

static char *g_voice_type_text[28][3] = {
	{"Male", "Female", "Child"}, /* en_US */
	{"\xEB\x82\xA8", "\xEC\x97\xAC", "\xEC\x95\x84\xEC\x9D\xB4"}, /* ko_KR */
	{"Mannlich", "Weiblich", "Kind"}, /* de_DE */
	{"Homme", "Femme", "Enfant"}, /* fr_FR */
	{"Maschio", "Femmina", "Bambino"}, /* it_IT */
	{"Hombre", "Mujer", "Hijo/a"}, /* es_ES */
	{"\xE7\x94\xB7", "\xE5\xA5\xB3", "\xE5\xAD\x90\xE5\xA5\xB3"}, /* zh_CN */
	{"Male", "Female", "Child"}, /* en_GB */
	{"\xD0\x9C\xD1\x83\xD0\xB6\xD1\x81\xD0\xBA\xD0\xBE\xD0\xB9", "\xD0\x96\xD0\xB5\xD0\xBD\xD1\x81\xD0\xBA\xD0\xB8\xD0\xB9", "\xD0\xA0\xD0\xB5\xD0\xB1\xD0\xB5\xD0\xBD\xD0\xBE\xD0\xBA"}, /* ru_RU */
	{"\xE7\x94\xB7\xE6\x80\xA7", "\xE5\xA5\xB3\xE6\x80\xA7", "\xE5\xAD\x90\xE4\xBE\x9B"}, /* ja_JP */
	{"Masculino", "Feminino", "\x43\x72\x69\x61\x6e\xC3\xA7\x61"}, /* pt_BR */
	{"Masculino", "Feminino", "\x43\x72\x69\x61\x6e\xC3\xA7\x61"}, /* pt_PT */
	{"Hombre", "Mujer", "Hijo(a)"}, /* es_MX */
	{"\xCE\x91\xCF\x81\xCF\x83\xCE\xB5\xCE\xBD\xCE\xB9\xCE\xBA\xCF\x8C", "\xCE\x98\xCE\xB7\xCE\xBB\xCF\x85\xCE\xBA\xCF\x8C", "\xCE\xA0\xCE\xB1\xCE\xB9\xCE\xB4\xCE\xAF"}, /* el_GR */
	{"\xE7\x94\xB7", "\xE5\xA5\xB3", "\xE5\xAD\x90\xE5\xA5\xB3"}, /* zh_TW */
	{"Mann", "Kvinne", "Barn"}, /* nb_NO */
	{"\xE7\x94\xB7", "\xE5\xA5\xB3", "\xE5\xAD\x90\xE5\xA5\xB3"}, /* zh_HK */
	{"\x4d\xC4\x99\xC5\xBC\x63\x7a\x79\x7a\x6e\x61", "Kobieta", "Dziecko"}, /* pl_PL */
	{"\x46\xC3\xA9\x72\x66\x69", "\x4e\xC5\x91", "Gyermek"}, /* hu_HU */
	{"\x4d\x75\xC5\xBE", "\xC5\xBD\x65\x6e\x61", "\x44\xC3\xAD\x74\xC4\x9B"}, /* cs_CZ */
	{"Mies", "Nainen", "Lapsi"}, /* fi_FI */
	{"\xE0\xA4\xAA\xE0\xA5\x81\xE0\xA4\xB0\xE0\xA5\x82\xE0\xA4\xB7", "\xE0\xA4\xB8\xE0\xA5\x8D\xE0\xA4\xA4\xE0\xA5\x8D\xE0\xA4\xB0\xE0\xA5\x80", "\xE0\xA4\xAC\xE0\xA4\x9A\xE0\xA5\x8D\xE0\xA4\x9A\xE0\xA4\xBE"}, /* hi_IN */
	{"Male", "Female", "Child"}, /* en_IN */
	{"Man", "Kvinna", "Barn"}, /* sv_SE */
	{"Mand", "Kvinde", "Barn"}, /* da_DK */
	{"Erkek", "\x4b\x61\x64\xC4\xB1\x6e", "\xC3\x87\x6f\x63\x75\x6b"}, /* tr_TR */
	{"\x4d\x75\xC5\xBE", "\xC5\xBD\x65\x6e\x61", "\x44\x69\x65\xC5\xA5\x61"}, /* sk_SK */
	{"Man", "Vrouw", "Kind"} /* nl_NL */
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
	} else if (!strcmp("zh_CN", lang)) {
		return g_voice_type_text[6][type - 1];
	} else if (!strcmp("en_GB", lang)) {
		return g_voice_type_text[7][type - 1];
	} else if (!strcmp("ru_RU", lang)) {
		return g_voice_type_text[8][type - 1];
	} else if (!strcmp("ja_JP", lang)) {
		return g_voice_type_text[9][type - 1];
	} else if (!strcmp("pt_BR", lang)) {
		return g_voice_type_text[10][type - 1];
	} else if (!strcmp("pt_PT", lang)) {
		return g_voice_type_text[11][type - 1];
	} else if (!strcmp("es_MX", lang)) {
		return g_voice_type_text[12][type - 1];
	} else if (!strcmp("el_GR", lang)) {
		return g_voice_type_text[13][type - 1];
	} else if (!strcmp("zh_TW", lang)) {
		return g_voice_type_text[14][type - 1];
	} else if (!strcmp("nb_NO", lang)) {
		return g_voice_type_text[15][type - 1];
	} else if (!strcmp("zh_HK", lang)) {
		return g_voice_type_text[16][type - 1];
	} else if (!strcmp("pl_PL", lang)) {
		return g_voice_type_text[17][type - 1];
	} else if (!strcmp("hu_HU", lang)) {
		return g_voice_type_text[18][type - 1];
	} else if (!strcmp("cs_CZ", lang)) {
		return g_voice_type_text[19][type - 1];
	} else if (!strcmp("fi_FI", lang)) {
		return g_voice_type_text[20][type - 1];
	} else if (!strcmp("hi_IN", lang)) {
		return g_voice_type_text[21][type - 1];
	} else if (!strcmp("en_IN", lang)) {
		return g_voice_type_text[22][type - 1];
	} else if (!strcmp("sv_SE", lang)) {
		return g_voice_type_text[23][type - 1];
	} else if (!strcmp("da_DK", lang)) {
		return g_voice_type_text[24][type - 1];
	} else if (!strcmp("tr_TR", lang)) {
		return g_voice_type_text[25][type - 1];
	} else if (!strcmp("sk_SK", lang)) {
		return g_voice_type_text[26][type - 1];
	} else if (!strcmp("nl_NL", lang)) {
		return g_voice_type_text[27][type - 1];
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
		evas_object_propagate_events_set(radio, EINA_TRUE);

		if (NULL == g_tts_voice_radio_group) {
			g_tts_voice_radio_group = radio;
			evas_object_event_callback_add(g_tts_voice_radio_group, EVAS_CALLBACK_DEL, __tts_setting_view_radio_group_del_cb, NULL);
		} else {
			elm_radio_group_add(radio, g_tts_voice_radio_group);
		}

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
		evas_object_size_hint_min_set(slider, 0, 120);
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

static char *__tts_setting_view_setting_text_get(void *data, Evas_Object *obj, const char *part)
{
	if (!strcmp("elm.text", part)) {
		return strdup(_("IDS_VOICE_BODY_ENGINE_SETTINGS"));
	}
	return NULL;
}

static void __tts_setting_view_setting_clicked_cb(void *data, Evas_Object *obj, void *event_info)
{
	Elm_Object_Item *item = (Elm_Object_Item *)event_info;
	elm_genlist_item_selected_set(item, EINA_FALSE);
	
	app_control_h app_control;
	if (0 != app_control_create(&app_control)) {
		LOGE("Fail to app control create");
		return;
	}

	if (0 != app_control_set_app_id(app_control, g_tts_engine->setting_path)) {
		LOGE("Fail to set app id");
		app_control_destroy(app_control);
		return;
	}

	if (0 != app_control_send_launch_request(app_control, NULL, NULL)) {
		LOGE("Fail to send launch request");
	} else {
		LOGD("Send launch request");
	}

	app_control_destroy(app_control);

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

	GList *tmp_list = tts_setting_mgr_get_supported_engine();
	if (NULL != tmp_list) {
		g_tts_engine = (tts_engine_info_s *)calloc(1, sizeof(tts_engine_info_s));
		if (NULL != g_tts_engine) {
			if (0 != tts_setting_mgr_get_engine(&(g_tts_engine->engine_id))) {
				LOGE("Fail to get engine");
				return;
			}
			if (0 != tts_setting_mgr_get_current_engine_info(g_tts_engine->engine_id, &(g_tts_engine->engine_name), &(g_tts_engine->setting_path))) {
				LOGE("Fail to get engine info");
				return;
			}
		} else {
			LOGE("Fail to memory allocation");
			return;
		}
	} else {
		LOGE("Fail to get engine list");
		return;
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

	g_itc_tts_setting = elm_genlist_item_class_new();
	if (NULL == g_itc_tts_setting) {
		LOGE("Fail to item class new");
		return;
	}
	g_itc_tts_setting->item_style = "type1";
	g_itc_tts_setting->func.text_get = __tts_setting_view_setting_text_get;
	g_itc_tts_setting->func.content_get = NULL;
	g_itc_tts_setting->func.state_get = NULL;
	g_itc_tts_setting->func.del = NULL;

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
	/* Engine setting */
	if (NULL != g_tts_engine->setting_path) {
		elm_genlist_item_append(ad->genlist, g_itc_tts_setting, "setting", NULL, ELM_GENLIST_ITEM_NONE, __tts_setting_view_setting_clicked_cb, NULL);
	}

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

	if (NULL != g_itc_tts_setting) {
		elm_genlist_item_class_free(g_itc_tts_setting);
		g_itc_tts_setting = NULL;
	}

	if (NULL != g_tts_engine) {
		if (NULL != g_tts_engine->engine_id) {
			free(g_tts_engine->engine_id);
			g_tts_engine->engine_id = NULL;
		}
		if (NULL != g_tts_engine->engine_name) {
			free(g_tts_engine->engine_name);
			g_tts_engine->engine_name = NULL;
		}
		if (NULL != g_tts_engine->setting_path) {
			free(g_tts_engine->setting_path);
			g_tts_engine->setting_path = NULL;
		}

		free(g_tts_engine);
		g_tts_engine = NULL;
	}
}
