#include "AudioManager.h"
#include "../GameObject/Audio/Audio.h"

// 静的メンバ変数の初期化
AudioManager* AudioManager::pInstance = nullptr;

void AudioManager::CreateInstance() {
	pInstance = new AudioManager();
}

AudioManager* AudioManager::GetInstance() {
	if (pInstance == nullptr)
		CreateInstance();

	return pInstance;
}

void AudioManager::DestroyInstance() {
	if (pInstance != nullptr) {
		delete pInstance;
		pInstance = nullptr;
	}
}


AudioManager::AudioManager()
	:audioResourceMap()
	, pAudioList() {
}

AudioManager::~AudioManager() {
	for (auto pA : pAudioList) {
		if (pA != nullptr) {
			delete pA;
			pA = nullptr;
		}
	}
	pAudioList.clear();

	DeleteData();
	audioResourceMap.clear();
}

void AudioManager::Load(std::string _filePath, std::string _name, bool _is3D) {
	if (audioResourceMap.find(_name) != audioResourceMap.end()) return;

	SetCreate3DSoundFlag(_is3D);
	int res = LoadSoundMem(_filePath.c_str());

	if (res != -1) {
		audioResourceMap.emplace(_name, res);
	}
}

void AudioManager::LoadAll() {
	for (int i = 0; i < AudioCount; ++i) {
		Load(AudioList[i].fullPath, AudioList[i].name, AudioList[i].is3D);
	}
}

void AudioManager::ChangeVolume(float _volume, std::string _name) {
	if (_name == "all" || _name == "All") {
		for (auto pA : pAudioList) {
			pA->SetVolume(_volume);
		}
	}
	else {
		for (auto pA : pAudioList) {
			if (pA->GetTag() == _name) {
				pA->SetVolume(_volume);
			}
		}
	}
}

void AudioManager::PlayBGM(std::string _name, float volume) {
	int sound = INVALID;
	if (GetSoundTotalTime(audioResourceMap[_name]) >= 1000 * 60 * 3) {
		sound = audioResourceMap[_name];
	}
	else {
		sound = DuplicateSoundMem(audioResourceMap[_name]);
	}

	Audio* pAudioObj = new Audio(sound);
	pAudioObj->SetTag(_name);
	pAudioObj->SetLoop(true);
	pAudioObj->SetVolume(volume);
	pAudioObj->Start();
	pAudioList.push_back(pAudioObj);
}

void AudioManager::PlayOneShot(std::string _name) {
	int sound = INVALID;
	if (GetSoundTotalTime(audioResourceMap[_name]) >= 1000 * 60 * 3) {
		sound = audioResourceMap[_name];
	}
	else {
		sound = DuplicateSoundMem(audioResourceMap[_name]);
	}

	Audio* pAudioObj = new Audio(sound);
	pAudioObj->SetTag(_name);
	pAudioObj->Start();
	pAudioList.push_back(pAudioObj);
}

void AudioManager::PlayClipAtPoint(std::string _name, VECTOR _pos, float _volume, bool _isLoop) {
	int sound = DuplicateSoundMem(audioResourceMap[_name]);

	Audio* pAudioObj = new Audio(sound, false);
	pAudioObj->SetVolume(_volume);
	pAudioObj->SetTag(_name);
	pAudioObj->SetPosition(_pos);
	pAudioObj->SetLoop(_isLoop);
	pAudioObj->Start();
	pAudioList.push_back(pAudioObj);
}

void AudioManager::Stop(std::string _name) {
	if (_name == "all" || _name == "All") {
		for (auto pA : pAudioList) {
			StopSoundMem(pA->GetSoundHandle());
		}
	}
	else {
		for (auto pA : pAudioList) {
			if (pA->GetTag() == _name) {
				StopSoundMem(pA->GetSoundHandle());
			}
		}
	}
}

void AudioManager::Update() {
	auto it = std::remove_if(pAudioList.begin(), pAudioList.end(), [](Audio* pA) {
		if (!pA->IsVisible()) {
			delete pA; // ここでメモリを解放！
			return true;
		}
		return false;
		});
	pAudioList.erase(it, pAudioList.end());
}

void AudioManager::DeleteData() {
	for (auto itr : audioResourceMap) {
		DeleteSoundMem(itr.second);
	}
}

void AudioManager::LoadPlay(std::string _filePath, std::string _name, bool _is3D, float volume) {
	auto a = audioResourceMap.find(_name);
	if (a == audioResourceMap.end()) {
		Load(_filePath, _name, _is3D);
		PlayBGM(_name, volume);
	}
	else {
		PlayBGM(_name, volume);
	}
}

