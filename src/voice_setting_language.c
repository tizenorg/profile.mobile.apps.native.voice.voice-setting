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
#include <stdio.h>
#include <string.h>

#include "voice_setting_language.h"

#define LANG_EN_US "\x45\x6E\x67\x6C\x69\x73\x68"
#define LANG_KO_KR "\xED\x95\x9C\xEA\xB5\xAD\xEC\x96\xB4"
#define LANG_DE_DE "\x44\x65\x75\x74\x73\x63\x68"
#define LANG_FR_FR "\x46\x72\x61\x6E\xC3\xA7\x61\x69\x73"
#define LANG_IT_IT "\x49\x74\x61\x6C\x69\x61\x6E\x6F"
#define LANG_ES_ES "\x45\x73\x70\x61\xC3\xB1\x6F\x6C"

char *voice_setting_language_conv_id_to_lang(const char* id)
{
	if (!strcmp("en_US", id)) {
		return LANG_EN_US;
	} else if (!strcmp("ko_KR", id)) {
		return LANG_KO_KR;
	} else if (!strcmp("de_DE", id)) {
		return LANG_DE_DE;
	} else if (!strcmp("fr_FR", id)) {
		return LANG_FR_FR;
	} else if (!strcmp("it_IT", id)) {
		return LANG_IT_IT;
	} else if (!strcmp("es_ES", id)) {
		return LANG_ES_ES;
	}
	return NULL;
}

char *voice_setting_language_conv_lang_to_id(const char* lang)
{
	if (!strcmp(LANG_EN_US, lang)) {
		return "en_US";
	} else if (!strcmp(LANG_KO_KR, lang)) {
		return "ko_KR";
	} else if (!strcmp(LANG_DE_DE, lang)) {
		return "de_DE";
	} else if (!strcmp(LANG_FR_FR, lang)) {
		return "fr_FR";
	} else if (!strcmp(LANG_IT_IT, lang)) {
		return "it_IT";
	} else if (!strcmp(LANG_ES_ES, lang)) {
		return "es_ES";
	}
	return NULL;
}