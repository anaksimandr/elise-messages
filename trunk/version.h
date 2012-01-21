#define __MAJOR_VERSION				0
#define __MINOR_VERSION				0
#define __RELEASE_NUM				0
#define __BUILD_NUM					3

#define __FILEVERSION_STRING		__MAJOR_VERSION,__MINOR_VERSION,__RELEASE_NUM,__BUILD_NUM
#define __FILEVERSION_DOTS			__MAJOR_VERSION.__MINOR_VERSION.__RELEASE_NUM.__BUILD_NUM

#define __STRINGIFY_IMPL(x)			#x
#define __STRINGIFY(x)				__STRINGIFY_IMPL(x)
#define __VERSION_STRING			__STRINGIFY(__FILEVERSION_DOTS)

#ifdef _UNICODE
#if defined(WIN64) || defined(_WIN64)
#define __PLUGIN_NAME "Elise Messages (Unicode x64)"
#else
#define __PLUGIN_NAME "Elise Messages (Unicode)"
#endif
#else
#define __PLUGIN_NAME "Elise Messages"
#endif
#define __INTERNAL_NAME				"EliseMessages"
#define __FILENAME					"EliseMessages.dll"
#define __DESCRIPTION 				"Alternate chat log."
#define __AUTHOR					"anaksimandr"
#define __AUTHOREMAIL				"s.andreenko@gmail.com"
#define __AUTHORWEB					"http://miranda-planet.com/"
#define __COPYRIGHT					"© 2012 anaksimandr"
