#ifndef FILES_H
#define FILES_H

#include <wchar.h>

// 各种文件路径处理函数

/**
 * @brief 获取当前可执行程序的文件路径
 *
 * @param[out] outBuffer 存放查询结果的缓冲区
 * @param[in] bufferSize 缓冲区长度
 *
 * @return 当缓冲区足够时，返回True，不足时，返回False；缓冲区指针为nullptr时也返回False
 */
bool GetExePath(wchar_t *outBuffer, size_t bufferSize);

/**
 * @brief 获取当前可执行程序的文件目录也就是不包含文件名的路径
 *
 * @param[out] outBuffer 存放查询结果的缓冲区
 * @param[in] bufferSize 缓冲区长度
 *
 * @return 当缓冲区足够时，返回True，不足时，返回False；缓冲区指针为nullptr时也返回False
 */
bool GetExeDirectory(wchar_t *outBuffer, size_t bufferSize);

/**
 * @brief 将相对路径转换为以调用该函数的可执行程序的文件所在路径为基础的绝对路径
 *
 * @param[in] rawPath 原始的相对路径文本
 * @param[out] outBuffer 用于存放转换结果的缓冲区
 * @param[in] buffersize 缓冲区长度
 *
 * @return 当缓冲区足够时，返回True，缓冲区不足或传递的`wchar_t`指针为nullptr时，返回False
 *
 * @note 如果rawPath本来就是绝对路径、UNC路径或根相对路径的其中一种，则函数直接复制rawPath的内容到outBuffer
 */
bool GetExeRelativePath(const wchar_t *rawPath, wchar_t *outBuffer, size_t bufferSize);

/**
 * @brief 检查指定的文件是否存在
 *
 * @param[in] rawPath 欲查询的文件路径
 *
 * @return 查询结果，存在返回True，不存在返回False
 *
 * @note 文件路径可以为绝对路径或相对路径，相对路径会被视为“以调用该函数的程序文件所在路径为基础的相对路径”
 */
bool FileExists(const wchar_t *rawPath);

#endif // FILES_H
