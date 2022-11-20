#pragma once

#include <vector>
#include <map>


namespace Animation
{
	enum class AnimationType
	{
		SINGLE_SHOT,
		LOOP,
	};

	struct Frame
	{
		float time, value;
	};

	class KeyFrameAnimation 
	{
	public:	
		KeyFrameAnimation();
		KeyFrameAnimation(float duration, AnimationType type);
		~KeyFrameAnimation();

		void Update(float dt);

		void AddKey(int id, float startValue, float endKey);
		void RemoveKey(int id);
		float GetKey(int id);

		void AddFrame(int id, float time, float value);

		void Start();
		void Toggle();
		void Stop();
		void Reverse();

		void SetPlaying(bool playing);
		void SetReversed(bool reversed);

		bool IsPlaying();
		bool IsReversed();

	private:
		std::map<int, std::vector<Frame>> keyFrames;
		float t = 0.0f;
		bool isActive = false;
		bool reverse = false;

		float duration;
		AnimationType animationType;
	};
} // namespace Animation
