#ifndef MACHIAVELLI_MACH_VIEW_SERVERCLI_H_INCLUDED
#define MACHIAVELLI_MACH_VIEW_SERVERCLI_H_INCLUDED

#include <istream>
#include <memory>
#include <ostream>

#include <mach/app/server.h>
#include <mach/infra/clicommandparser.h>
#include <mach/infra/subject.h>

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

    std::istream& inputStream;
    std::ostream& outputStream;

    infra::CliCommandParser commandParser;
    infra::Subject commandSubject;

    std::shared_ptr<statehandlers::StateHandler> stateHandler;

    bool isRunning;
    bool shouldStop;

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
