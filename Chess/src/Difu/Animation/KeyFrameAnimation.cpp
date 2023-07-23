#include "KeyFrameAnimation.h"

#include "Difu/Utils/Logger.h"
#include <raymath.h>

namespace Animation
{
	KeyFrameAnimation::KeyFrameAnimation()
	{
		duration = 1.0f;
		animationType = AnimationType::LOOP;
	}

	KeyFrameAnimation::KeyFrameAnimation(float _duration, AnimationType type)
	{
		duration = _duration;
		animationType = type;
	}

	KeyFrameAnimation::~KeyFrameAnimation()
	{
		for (auto& x : keyFrames)
		{
			x.second.clear();
		}

		keyFrames.clear();
	}

	void KeyFrameAnimation::Update(float dt)
	{
		if (isActive)
		{
			if (reverse)
				t -= dt;
			else
				t += dt;
			if (t > duration || t < 0.0f)
			{
				t = reverse ? 0.0f : duration;
				switch (animationType)
				{
					case AnimationType::LOOP:
						t = reverse ? duration : 0.0f;
						break;
					case AnimationType::SINGLE_SHOT:
						isActive = false;
						break;
				}
			}
		}
	}

	void KeyFrameAnimation::AddKey(int id, float startValue, float endValue)
	{
		if (keyFrames.find(id) != keyFrames.end())
		{
			LOG_WARN("Key {} already exists", id);
			return;
		}

		keyFrames[id].emplace_back(Frame{ 0.0f, startValue });
		AddFrame(id, duration, endValue);
	}
	
	void KeyFrameAnimation::RemoveKey(int id)
	{
		auto iter = keyFrames.find(id);
		if (iter == keyFrames.end())
		{
			LOG_WARN("Key {} does not exists exists", id);
			return;
		}

		keyFrames[id].clear();
		keyFrames.erase(iter);
	}

	float KeyFrameAnimation::GetKey(int id)
	{
		if (keyFrames.find(id) == keyFrames.end())
		{
			LOG_WARN("Key {} does not exists exists", id);
			return 0.0f;
		}

		int kfSize = keyFrames[id].size();
		int i;
		for (i = 0; i < kfSize; i++)
			if (t < keyFrames[id][i].time)
				break;

		Frame end = keyFrames[id][i];
		Frame start;
		if (i == 0)
			start = { 0.0f, 0.0f };
		else
			start = keyFrames[id][i - 1];

		float progress = (t - start.time)/(end.time - start.time);

		return Lerp(start.value, end.value, progress);
	}

	void KeyFrameAnimation::AddFrame(int id, float time, float value)
	{
		if (keyFrames.find(id) == keyFrames.end())
		{
			LOG_WARN("Key {} does not exists", id);
			return;
		}

		int kfSize = keyFrames[id].size();
		int i;
		for (i = 0; i < kfSize; i++)
			if (time < keyFrames[id][i].time)
				break;

		keyFrames[id].insert(keyFrames[id].begin() + i, Frame{time, value});
	}

	void KeyFrameAnimation::Start()
	{
		t = reverse ? duration : 0.0f;
		isActive = true;
	}

	void KeyFrameAnimation::Toggle()
	{
		isActive = !isActive;
	}

	void KeyFrameAnimation::Stop()
	{
		t = reverse ? duration : 0.0f;
		isActive = false;
	}

	void KeyFrameAnimation::Reverse()
	{
		reverse = !reverse;
	}

	void KeyFrameAnimation::SetPlaying(bool playing)
	{
		isActive = playing;
	}

	void KeyFrameAnimation::SetReversed(bool reversed)
	{
		reverse = reversed;
	}

	bool KeyFrameAnimation::IsPlaying()
	{
		return isActive;
	}

	bool KeyFrameAnimation::IsReversed()
	{
		return reverse;
	}

} // namespace Animation
