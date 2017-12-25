#ifndef MACHIAVELLI_MACH_VIEW_SERVERCLI_H_INCLUDED
#define MACHIAVELLI_MACH_VIEW_SERVERCLI_H_INCLUDED

#include <istream>
#include <memory>
#include <ostream>
#include <vector>

#include <mach/app/server.h>
#include <mach/infra/clicommandparser.h>
#include <mach/infra/commandmediator.h>

#include "mach/view/serverclistate.h"
#include "mach/view/statehandlers/statehandler.h"

namespace mach
{
namespace view
{
class ServerCli
{
  private:
    std::shared_ptr<app::Server> server;
    std::vector<app::ServerClient> clients;

    std::istream& inputStream;
    std::ostream& outputStream;

    infra::CliCommandParser commandParser;
    infra::CommandMediator commandMediator;

    std::shared_ptr<statehandlers::StateHandler> stateHandler;

    bool isRunning;
    bool shouldStop;

    void AcceptClient();

  public:
    ServerCli(std::shared_ptr<app::Server> server, std::istream& inputStream, std::ostream& outputStream);

    void Start();
    void Stop();

    bool IsRunning() const;

    void Render() const;

    void SetState(ServerCliState state);
};
}
}

#endif // MACHIAVELLI_MACH_VIEW_SERVERCLI_H_INCLUDED
