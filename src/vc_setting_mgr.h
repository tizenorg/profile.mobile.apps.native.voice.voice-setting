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
#ifndef __VC_SETTING_MGR_H__
#define __VC_SETTING_MGR_H__

#include <glib.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

int vc_setting_mgr_init(void *data);

int vc_setting_mgr_deinit(void *data);

GList* vc_setting_mgr_get_supported_language();

int vc_setting_mgr_get_current_language(char** language);

int vc_setting_mgr_set_current_language(const char* language);

int vc_setting_mgr_get_auto_language(bool* value);

int vc_setting_mgr_set_auto_language(bool value);

int vc_setting_mgr_get_enabled(bool *value);

int vc_setting_mgr_set_enabled(bool value);

#ifdef __cplusplus
}
#endif

#endif /* __VC_SETTING_MGR_H__ */
