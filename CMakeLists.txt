if (SMTG_CREATE_VST2_VERSION)
	set(mathreverb_vst2_sources
		${SDK_ROOT}/public.sdk/source/common/memorystream.cpp
		${SDK_ROOT}/public.sdk/source/vst/hosting/eventlist.cpp
		${SDK_ROOT}/public.sdk/source/vst/hosting/hostclasses.cpp
		${SDK_ROOT}/public.sdk/source/vst/hosting/parameterchanges.cpp
		${SDK_ROOT}/public.sdk/source/vst/hosting/processdata.cpp
		${SDK_ROOT}/public.sdk/source/vst/vst2wrapper/vst2wrapper.cpp
		${SDK_ROOT}/public.sdk/source/vst/vst2wrapper/vst2wrapper.h
		${SDK_ROOT}/public.sdk/source/vst2.x/audioeffect.cpp
		${SDK_ROOT}/public.sdk/source/vst2.x/audioeffectx.cpp
		source/mathreverbentry_vst2.cpp
	)
endif()

set(mathreverb_sources
	source/cmathreverbview.h
	source/mathreverb.cpp
	source/mathreverb.h
	source/mathreverbcids.h
	source/mathreverbcontroller.cpp
	source/mathreverbcontroller.h
	source/mathreverbentry.cpp
	source/mathreverbgraph.cpp
	source/mathreverbgraph.h
	source/mathreverbapex.cpp
	source/mathreverbapex.h
	source/mathreverbprocess.h
	source/version.h
	source/mathreverbparams/coordinate.h
	source/mathreverbparams/gain.h
	source/mathreverbparams/paramids.h
	source/mathreverbparams/reflection.h
)

# VST2 is only defined for macOS and Windows
if(MAC OR WIN)
	set(mathreverb_sources ${mathreverb_sources} ${mathreverb_vst2_sources})
endif()

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
