CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(SRC
    tts_setting_view.c
    tts_setting_mgr.c
    vc_setting_view.c
    vc_setting_mgr.c
    voice_setting_language.c
    voice_setting_main.c
)

SET(TARGET_NAME "${PROJECT_NAME}")
ADD_EXECUTABLE(${TARGET_NAME} ${SRC})
TARGET_LINK_LIBRARIES(${TARGET_NAME} ${pkgs_LDFLAGS} "-lm -pie")
INSTALL(TARGETS ${TARGET_NAME} DESTINATION ${BINDIR})
