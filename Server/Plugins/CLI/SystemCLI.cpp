#include "SystemCLI.hpp"

#include "Common/Core/API/InputAPI.hpp"
#include "Common/Core/Graphics/OpenGLContext.hpp"
#include "Common/Core/Graphics/Window.hpp"

namespace Plugins
{
	Plugins::SystemCLI::SystemCLI()
	{
	}

	SystemCLI::~SystemCLI()
	{
	}

	const Filepath& SystemCLI::GetAssetsFilepath() const
	{
		static auto asset_path = std::filesystem::current_path() / "Assets";
		return asset_path;
	}

	std::unique_ptr<Graphics::OpenGLContext> SystemCLI::CreateOpenGLContext()
	{
		ASSERT( false, "SystemCLI doesn't support OpenGL" );
		return nullptr;
	}

	std::shared_ptr<Graphics::Window> SystemCLI::CreateWindow( const Graphics::WindowDefinition& )
	{
		ASSERT( false, "SystemCLI doesn't support creating window" );
		return nullptr;
	}

	void SystemCLI::DestroyWindow( std::shared_ptr<Graphics::Window> )
	{
		ASSERT( false, "SystemCLI doesn't support creating window" );
	}

	Size<uint32_t> SystemCLI::GetScreenSize() const
	{
		ASSERT( false, "SystemCLI doesn't support getting the screen size" );
		return { 0, 0 };
	}

	std::thread SystemCLI::CreateThread( std::string_view name_, std::function<void()> runnable_ )
	{
		std::string name = (std::string)name_;
		return std::thread( [=]()
			{
				SetThreadName( name );

				if (runnable_)
					runnable_();
			} );
	}

	bool SystemCLI::GenerateEvents( ::API::VideoAPI*, ::API::InputAPI* input, ::API::DearImGuiAPI* )
	{
		// TODO: Maybe get input??
		(void)input;

		return true;
	}

	void SystemCLI::Update( const PreciseTimestep& ts )
	{
		(void)ts;
	}

	std::string_view SystemCLI::GetName() const noexcept
	{
		return "CLI";
	}

	void SystemCLI::Init()
	{
	}

	void SystemCLI::Shutdown()
	{
	}
}
