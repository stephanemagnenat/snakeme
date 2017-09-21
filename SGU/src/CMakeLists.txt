add_library(SGU SGU_GenUtils.cpp SGU_Math.cpp SGU_UI_Base.cpp SGU_UI_Theme.cpp SGU_UI_WGFile.cpp SGU_UI_WGList.cpp SGU_UI_WGText.cpp SGU_List.cpp SGU_Sprite.cpp SGU_UI_SDB.cpp SGU_UI_WGButton.cpp SGU_UI_WGIntBar.cpp SGU_UI_WGScrollBar.cpp SGU_UI_WGValueBar.cpp)
target_include_directories(SGU PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)
