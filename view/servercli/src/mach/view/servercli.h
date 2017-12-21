#ifndef MACHIAVELLI_MACH_VIEW_SERVERCLI_H_INCLUDED
#define MACHIAVELLI_MACH_VIEW_SERVERCLI_H_INCLUDED

#include <istream>
#include <memory>
#include <ostream>

#include <mach/app/server.h>
#include <mach/infra/clicommandparser.h>
#include <mach/infra/commandmediator.h>

namespace mach
{
namespace view
{
class ServerCli
{
  private:
    std::shared_ptr<app::Server> server;
    std::istream& inputStream;
    std::ostream& outputStream;

    infra::CliCommandParser commandParser;
    infra::CommandMediator commandMediator;

    bool isRunning;
    bool shouldStop;

  public:
    ServerCli(std::shared_ptr<app::Server> server, std::istream& inputStream, std::ostream& outputStream);

    void Start();
    void Stop();

    bool IsRunning() const;

    void RenderConsole() const;
};
}
}

#endif // MACHIAVELLI_MACH_VIEW_SERVERCLI_H_INCLUDED