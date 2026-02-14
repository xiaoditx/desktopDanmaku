#ifndef AUDIO_PLAY_HPP
#define AUDIO_PLAY_HPP

/**
 * @brief 播放音频文件
 *
 * @param[in] fileName 欲播放的音频文件名，不包含扩展名，如果是相对路径，函数在内部会自动转换成绝对路径，如果文件不存在，函数将静默处理
 *
 * @note 音频文件必须是wav格式，且必须放在程序目录下的audios文件夹中，文件名必须是纯数字（不考虑扩展名），例如：1.wav、2.wav等
 */
void PlayAudioFile(const wchar_t *fileName);

#endif // AUDIO_PLAY_HPP
