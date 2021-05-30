#pragma once

#include "Common/Core/API/LoggingAPI.hpp"

namespace Plugins
{
	class SpdlogPlugin final
		: public API::LoggingAPI
	{
	public:
		explicit SpdlogPlugin( Filepath log_file_directory );
		~SpdlogPlugin();

		std::string_view GetName() const noexcept override { return "Spdlog"; }

	protected:
		void Init() override;
		void Shutdown() override;

		void AddSinkInternal( ChannelId channel, const Sink& sink ) override;
		void LogInternal( ChannelId channel, Level level, std::string_view msg ) override;
	};
}
