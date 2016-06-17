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

#define LANG_ZH_CN "\xE7\xAE\x80\xE4\xBD\x93\xE4\xB8\xAD\xE6\x96\x87"
#define LANG_EN_GB "\x45\x6e\x67\x6c\x69\x73\x68\x20\x28\x55\x6e\x69\x74\x65\x64\x20\x4b\x69\x6e\x67\x64\x6f\x6d\x29"
#define LANG_RU_RU "\x50\xD1\x83\xD1\x81\xD1\x81\xD0\xBA\xD0\xB8\xD0\xB9"
#define LANG_JA_JP "\xE6\x97\xA5\xE6\x9C\xAC\xE8\xAA\x9E"
#define LANG_PT_BR "\x50\x6f\x72\x74\x75\x67\x75\xC3\xAA\x73\x20\x28\x42\x72\x61\x73\x69\x6c\x29"
#define LANG_PT_PT "\x50\x6f\x72\x74\x75\x67\x75\xC3\xAA\x73\x20\x28\x50\x6f\x72\x74\x75\x67\x61\x6c\x29"
#define LANG_ES_MX "\x45\x73\x70\x61\xC3\xB1\x6f\x6c\x20\x28\x45\x73\x74\x61\x64\x6f\x73\x20\x55\x6e\x69\x64\x6f\x73\x29"
#define LANG_EL_GR "\xCE\x95\xCE\xBB\xCE\xBB\xCE\xB7\xCE\xBD\xCE\xB9\xCE\xBA\xCE\xAC"
#define LANG_ZH_TW "\xE7\xB9\x81\xE4\xBD\x93\xE4\xB8\xAD\xE6\x96\x87\x28\xE5\x8F\xB0\xE6\xB9\xBE\x29"
#define LANG_NB_NO "\x4e\x6f\x72\x73\x6b"
#define LANG_ZH_HK "\xE7\xB9\x81\xE4\xBD\x93\xE4\xB8\xAD\xE6\x96\x87\x28\xE9\xA6\x99\xE6\xB8\xAF\x29"
#define LANG_PL_PL "\x50\x6f\x6c\x73\x6b\x69"
#define LANG_HU_HU "\x4d\x61\x67\x79\x61\x72"
#define LANG_CS_CZ "\xC4\x8C\x65\xC5\xA1\x74\x69\x6e\x61"
#define LANG_FI_FI "\x53\x75\x6f\x6d\x69"
#define LANG_HI_IN "\xE0\xA4\xB9\xE0\xA4\xBF\xE0\xA4\xA8\xE0\xA5\x8D\xE0\xA4\xA6\xE0\xA5\x80"
#define LANG_EN_IN "\x45\x6e\x67\x6c\x69\x73\x68\x20\x28\x49\x6e\x64\x69\x61\x29"
#define LANG_SV_SE "\x53\x76\x65\x6e\x73\x6b\x61"
#define LANG_DA_DK "\x44\x61\x6e\x73\x6b"
#define LANG_TR_TR "\x54\xC3\xBC\x72\x6b\xC3\xA7\x65"
#define LANG_SK_SK "\x53\x6c\x6f\x76\x65\x6e\xC4\x8D\x69\x6e\x61"
#define LANG_NL_NL "\x4e\x65\x64\x65\x72\x6c\x61\x6e\x64\x73"

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
	} else if (!strcmp("zh_CN", id)) {
		return LANG_ZH_CN;
	} else if (!strcmp("en_GB", id)) {
		return LANG_EN_GB;
	} else if (!strcmp("ru_RU", id)) {
		return LANG_RU_RU;
	} else if (!strcmp("ja_JP", id)) {
		return LANG_JA_JP;
	} else if (!strcmp("pt_BR", id)) {
		return LANG_PT_BR;
	} else if (!strcmp("pt_PT", id)) {
		return LANG_PT_PT;
	} else if (!strcmp("es_MX", id)) {
		return LANG_ES_MX;
	} else if (!strcmp("el_GR", id)) {
		return LANG_EL_GR;
	} else if (!strcmp("zh_TW", id)) {
		return LANG_ZH_TW;
	} else if (!strcmp("nb_NO", id)) {
		return LANG_NB_NO;
	} else if (!strcmp("zh_HK", id)) {
		return LANG_ZH_HK;
	} else if (!strcmp("pl_PL", id)) {
		return LANG_PL_PL;
	} else if (!strcmp("hu_HU", id)) {
		return LANG_HU_HU;
	} else if (!strcmp("cs_CZ", id)) {
		return LANG_CS_CZ;
	} else if (!strcmp("fi_FI", id)) {
		return LANG_FI_FI;
	} else if (!strcmp("hi_IN", id)) {
		return LANG_HI_IN;
	} else if (!strcmp("en_IN", id)) {
		return LANG_EN_IN;
	} else if (!strcmp("sv_SE", id)) {
		return LANG_SV_SE;
	} else if (!strcmp("da_DK", id)) {
		return LANG_DA_DK;
	} else if (!strcmp("tr_TR", id)) {
		return LANG_TR_TR;
	} else if (!strcmp("sk_SK", id)) {
		return LANG_SK_SK;
	} else if (!strcmp("nl_NL", id)) {
		return LANG_NL_NL;
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
	} else if (!strcmp(LANG_ZH_CN, lang)) {
		return "zh_CN";
	} else if (!strcmp(LANG_EN_GB, lang)) {
		return "en_GB";
	} else if (!strcmp(LANG_RU_RU, lang)) {
		return "ru_RU";
	} else if (!strcmp(LANG_JA_JP, lang)) {
		return "ja_JP";
	} else if (!strcmp(LANG_PT_BR, lang)) {
		return "pt_BR";
	} else if (!strcmp(LANG_PT_PT, lang)) {
		return "pt_PT";
	} else if (!strcmp(LANG_ES_MX, lang)) {
		return "es_MX";
	} else if (!strcmp(LANG_EL_GR, lang)) {
		return "el_GR";
	} else if (!strcmp(LANG_ZH_TW, lang)) {
		return "zh_TW";
	} else if (!strcmp(LANG_NB_NO, lang)) {
		return "nb_NO";
	} else if (!strcmp(LANG_ZH_HK, lang)) {
		return "zh_HK";
	} else if (!strcmp(LANG_PL_PL, lang)) {
		return "pl_PL";
	} else if (!strcmp(LANG_HU_HU, lang)) {
		return "hu_HU";
	} else if (!strcmp(LANG_CS_CZ, lang)) {
		return "cs_CZ";
	} else if (!strcmp(LANG_FI_FI, lang)) {
		return "fi_FI";
	} else if (!strcmp(LANG_HI_IN, lang)) {
		return "hi_IN";
	} else if (!strcmp(LANG_EN_IN, lang)) {
		return "en_IN";
	} else if (!strcmp(LANG_SV_SE, lang)) {
		return "sv_SE";
	} else if (!strcmp(LANG_DA_DK, lang)) {
		return "da_DK";
	} else if (!strcmp(LANG_TR_TR, lang)) {
		return "tr_TR";
	} else if (!strcmp(LANG_SK_SK, lang)) {
		return "sk_SK";
	} else if (!strcmp(LANG_NL_NL, lang)) {
		return "nl_NL";
	}
	return NULL;
}