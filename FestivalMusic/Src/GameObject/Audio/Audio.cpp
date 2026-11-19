#include "Audio.h"
Audio::Audio(int _soundHandle, bool _is3D)
	:is3Dsound(_is3D)
	, isLoop(false)
	, soundHandle(_soundHandle)
	, volume(255 * 0.7f)
	, distance(800) {
}

Audio::~Audio() {
	// 再生を停止してから削除
	StopSoundMem(soundHandle);
	DeleteSoundMem(soundHandle);
}

void Audio::Start() {
	// 3Dサウンドの設定は再生前に必要
	if (is3Dsound) {
		Set3DPositionSoundMem(position, soundHandle);
		Set3DRadiusSoundMem(distance, soundHandle);
	}

	ChangeVolumeSoundMem((int)volume, soundHandle);

	// PlaySoundMem の第2引数は 3D かどうかではなく「ループするか」で決める
	int playType = isLoop ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_BACK;
	PlaySoundMem(soundHandle, playType);
}

void Audio::Update() {
	if (!isVisible)
		return;

	// 再生中か
	if (CheckSoundMem(soundHandle) == 0)
		isVisible = false;

	// 音量の設定
	ChangeVolumeSoundMem((int)volume, soundHandle);

	// 3dなら
	if (is3Dsound) {
		// 音源の位置の設定
		Set3DPositionSoundMem(position, soundHandle);
		// 音が聞こえる範囲の設定
		Set3DRadiusSoundMem(distance, soundHandle);
	}

}

void Audio::Render() {}
