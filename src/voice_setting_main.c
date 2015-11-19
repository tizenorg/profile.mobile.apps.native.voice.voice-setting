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
#include <app.h>
#include <unistd.h>
#include <vconf-keys.h>
#include <vconf.h>
#include <efl_extension.h>

#include "voice_setting_main.h"
#include "tts_setting_view.h"
#include "vc_setting_view.h"

static void __create_ui(void *data)
{
	appdata *ad = (appdata *)data;

	bindtextdomain("org.tizen.voice-setting", "/usr/apps/org.tizen.voice-setting/res/locale/");
	textdomain("org.tizen.voice-setting");

	/* Create window */
	Evas_Object *win = elm_win_add(NULL, "voice-setting", ELM_WIN_BASIC);
	elm_win_title_set(win, "voice-setting");
	elm_win_borderless_set(win, EINA_TRUE);
	elm_win_alpha_set(win, EINA_FALSE);
	elm_win_conformant_set(win, EINA_TRUE);
	elm_win_autodel_set(win, EINA_TRUE);

	int rots[4] = {0, 90, 180, 270};
	elm_win_wm_rotation_available_rotations_set(win, rots, 4);
	ad->win = win;

	/* Create bg */
	Evas_Object *bg = elm_bg_add(ad->win);
	evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(ad->win, bg);
	evas_object_show(bg);

	/* Create conformant */
	Evas_Object *conform = elm_conformant_add(ad->win);
	elm_win_indicator_mode_set(ad->win, ELM_WIN_INDICATOR_SHOW);
	elm_win_indicator_opacity_set(ad->win, ELM_WIN_INDICATOR_OPAQUE);
	evas_object_size_hint_weight_set(conform, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(conform, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_win_resize_object_add(ad->win, conform);
	evas_object_show(conform);
	ad->conform = conform;

	/* Create naviframe */
	Evas_Object *naviframe = elm_naviframe_add(ad->conform);
	elm_naviframe_prev_btn_auto_pushed_set(naviframe, EINA_FALSE);
	eext_object_event_callback_add(naviframe, EEXT_CALLBACK_BACK, eext_naviframe_back_cb, NULL);
	evas_object_size_hint_weight_set(naviframe, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(naviframe, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_object_part_content_set(ad->conform, "elm.swallow.content", naviframe);
	evas_object_show(naviframe);
	ad->naviframe = naviframe;

	/* Create Genlist */
	Evas_Object *genlist = elm_genlist_add(ad->naviframe);
	elm_genlist_mode_set(genlist, ELM_LIST_COMPRESS);
	elm_genlist_homogeneous_set(genlist, EINA_TRUE);
	evas_object_show(genlist);
	ad->genlist = genlist;
}

static bool app_create(void *data)
{
	LOGD("");

	__create_ui(data);

	return true;
}

static void __voice_setting_navi_back_btn_cb(void *data, Evas_Object *obj, void *event_info)
{
	evas_object_smart_callback_del(obj, "clicked", __voice_setting_navi_back_btn_cb);
	ui_app_exit();
}

static Eina_Bool __voice_setting_navi_item_pop_cb(void *data, Elm_Object_Item *it)
{
	ui_app_exit();
	return EINA_TRUE;
}

static void __content_show(void *data, const char* title)
{
	appdata *ad = (appdata *)data;

	Evas_Object *back_btn = elm_button_add(ad->naviframe);
	elm_object_style_set(back_btn, "naviframe/back_btn/default");
	evas_object_smart_callback_add(back_btn, "clicked", __voice_setting_navi_back_btn_cb, NULL);
	Elm_Object_Item *nf_main_item = elm_naviframe_item_push(ad->naviframe, title, back_btn, NULL, ad->genlist, NULL);
	elm_naviframe_item_title_enabled_set(nf_main_item, EINA_TRUE, EINA_TRUE);
	elm_naviframe_item_pop_cb_set(nf_main_item, __voice_setting_navi_item_pop_cb, ad);
	elm_object_content_set(ad->conform, ad->naviframe);

	evas_object_show(ad->win);
}

static void app_control(app_control_h app_control, void *data)
{
	/* Handle the launch request. */
	appdata *ad = (appdata *)data;
	LOGD("");

	if(ad->app_state == APP_STATE_PAUSE || ad->app_state == APP_STATE_RESUME) {
		if (ad->win) {
			elm_win_activate(ad->win);
		}
		ad->app_state = APP_STATE_SERVICE;
		return;
	} else if (ad->app_state != APP_STATE_SERVICE) {
		char* type = NULL;
		int res = app_control_get_extra_data(app_control, "show", &type);
		if(APP_CONTROL_ERROR_NONE == res && NULL != type) {
			if (!strcmp("voice-control", type)) {
				LOGD("==== Show Voice Control Setting ====");
				ad->current_page = SHOW_VOICE_CONTROL;
				vc_setting_view_create(ad);
				__content_show(ad, _("IDS_VOICE_BODY_VOICE_CONTROL_ABB2"));
			} else if (!strcmp("tts", type)) {
				LOGD("==== Show TTS Setting ====");
				ad->current_page = SHOW_TTS;
				tts_setting_view_create(ad);
				__content_show(ad, _("IDS_ST_BODY_TTS"));
			}
		}
		if(NULL != type) {
			free(type);
		}
	}
	LOGD("====");
	LOGD("");

	ad->app_state = APP_STATE_SERVICE;
}

static void app_pause(void *data)
{
	LOGD("");

	appdata *ad = (appdata *)data;
	ad->app_state = APP_STATE_PAUSE;
}

static void app_resume(void *data)
{
	LOGD("");

	appdata *ad = (appdata *)data;
	if(ad->app_state == APP_STATE_PAUSE) {
		if (ad->win) {
			elm_win_activate(ad->win);
		}
		ad->app_state = APP_STATE_RESUME;
		return;
	}
	ad->app_state = APP_STATE_RESUME;
}

static void app_terminate(void *data)
{
	LOGD("");

	appdata *ad = (appdata *)data;
	ad->app_state = APP_STATE_TERMINATE;
	if (SHOW_VOICE_CONTROL == ad->current_page) {
		vc_setting_view_destroy(ad);
	} else if (SHOW_TTS == ad->current_page) {
		tts_setting_view_destroy(ad);
	}
}

static void ui_app_lang_changed(app_event_info_h event_info, void *user_data)
{
	LOGD("");
}

static void ui_app_orient_changed(app_event_info_h event_info, void *user_data)
{
	LOGD("");
}

static void ui_app_region_changed(app_event_info_h event_info, void *user_data)
{
	LOGD("");
}

static void ui_app_low_battery(app_event_info_h event_info, void *user_data)
{
	LOGD("");
}

static void ui_app_low_memory(app_event_info_h event_info, void *user_data)
{
	LOGD("");
}

int main(int argc, char *argv[])
{
	appdata ad = {0,};
	int ret = 0;

	ui_app_lifecycle_callback_s event_callback = {0,};
	app_event_handler_h handlers[5] = {NULL, };

	event_callback.create = app_create;
	event_callback.terminate = app_terminate;
	event_callback.pause = app_pause;
	event_callback.resume = app_resume;
	event_callback.app_control = app_control;

	ui_app_add_event_handler(&handlers[APP_EVENT_LOW_BATTERY], APP_EVENT_LOW_BATTERY, ui_app_low_battery, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_LOW_MEMORY], APP_EVENT_LOW_MEMORY, ui_app_low_memory, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_DEVICE_ORIENTATION_CHANGED], APP_EVENT_DEVICE_ORIENTATION_CHANGED, ui_app_orient_changed, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_LANGUAGE_CHANGED], APP_EVENT_LANGUAGE_CHANGED, ui_app_lang_changed, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_REGION_FORMAT_CHANGED], APP_EVENT_REGION_FORMAT_CHANGED, ui_app_region_changed, &ad);

	ret = ui_app_main(argc, argv, &event_callback, &ad);
	if (ret != APP_ERROR_NONE) {
		LOGW("ui_app_main failed, Err=%d\n", ret);
	}

	return ret;
}

