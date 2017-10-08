set(mathreverb_sources
	source/cmathreverbview.h
	source/mathreverb.cpp
	source/mathreverb.h
	source/mathreverbcids.h
	source/mathreverbcontroller.cpp
	source/mathreverbcontroller.h
	source/mathreverbentry.cpp
	source/mathreverbprocess.h
	source/version.h
	source/mathreverbparams/coordiantes.h
	source/mathreverbparams/gain.h
	source/mathreverbparams/paramids.h
	source/mathreverbparams/reflection.h
)

set(target mathreverb)
smtg_add_vst3plugin(${target} ${SDK_ROOT} ${mathreverb_sources})
target_include_directories(${target} PUBLIC ${VSTGUI_ROOT}/vstgui4)
target_link_libraries(${target} PRIVATE base sdk vstgui_support)

smtg_add_vst3_resource(${target} "resource/mathreverb.uidesc")
smtg_add_vst3_resource(${target} "resource/vu_on.png")
smtg_add_vst3_resource(${target} "resource/vu_off.png")
smtg_add_vst3_resource(${target} "resource/knob_reflection.png")
smtg_add_vst3_resource(${target} "resource/knob_gain.png")
smtg_add_vst3_resource(${target} "resource/slider_background.png")
smtg_add_vst3_resource(${target} "resource/slider_handle.png")
smtg_add_vst3_resource(${target} "resource/background.png")

if(WIN)
	target_sources(${target} PRIVATE resource/mathreverb.rc)
endif()
