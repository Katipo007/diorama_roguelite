#include "LoggingAPI.hpp"

#include <assert.h>

namespace API
{
    LoggingAPI::~LoggingAPI()
    {
        static_instance = nullptr;
    }

    void LoggingAPI::AddSink( ChannelId channel, const Sink& sink )
    {
        GetInstance().AddSinkInternal( channel, sink );
    }

    LoggingAPI::LoggingAPI( Filepath log_file_directory )
        : log_file_directory( log_file_directory )
    {
        assert( static_instance == nullptr );
        static_instance = this;
    }
}
