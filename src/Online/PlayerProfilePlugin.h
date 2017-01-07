#pragma once
#include <stormancer.h>

namespace Stormancer
{

	template<class T>
	class PlayerProfileService
	{
	public:
		PlayerProfileService(ScenePtr scene)
		{
			_scene = scene;
		}
	public:
		pplx::task<T> Get()
		{
			std::shared_ptr<IRpcService> rpc = _scene.lock()->dependencyResolver()->template resolve<IRpcService>();
			return rpc->rpc<T>("profiles.getsingle");
		}

		pplx::task<void> Reset()
		{
			auto rpc = _scene.lock()->dependencyResolver()->template resolve<IRpcService>();
			return rpc->rpcVoid("profiles.delete", 0);
		}

	private:
		ScenePtr _scene;
	};


	template<class T>
	class PlayerProfilePlugin : public IPlugin
	{
		virtual void registerSceneDependencies(Scene* scene) override
		{
			if (scene)
			{
				auto name = scene->getHostMetadata("stormancer.profiles");

				if (name.length() > 0)
				{
					auto service = std::make_shared<PlayerProfileService<T>>(scene->shared_from_this());
					scene->dependencyResolver()->registerDependency(service);
				}
			}
			
		};

		void destroy() override
		{
			delete this;
		}
	};

	
}