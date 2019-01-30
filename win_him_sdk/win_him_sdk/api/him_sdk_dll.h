/** @file him_sdk_dll.h
  * @brief 导出宏定义
  * @author Captain China
  * @date 2019/01/27
  */

#ifdef HIM_SDK_EXPORTS
#define HIM_SDK_API __declspec(dllexport)
#else
#define HIM_SDK_API __declspec(dllimport)
#endif // HIM_SDK_EXPORTS