#ifndef MACHIAVELLI_MACH_VIEW_CLIENTCLI_H_INCLUDED
#define MACHIAVELLI_MACH_VIEW_CLIENTCLI_H_INCLUDED

#include <istream>
#include <memory>
#include <ostream>

#include <mach/app/onlineclient.h>
#include <mach/infra/clicommandparser.h>
#include <mach/infra/subject.h>

#include "mach/view/clientclistate.h"
#include "mach/view/statehandlers/statehandler.h"

namespace mach
{
namespace view
{
class ClientCli
{
  private:
    app::OnlineClient client;

    std::istream& inputStream;
    std::ostream& outputStream;

    infra::CliCommandParser commandParser;
    infra::Subject commandSubject;

    std::shared_ptr<statehandlers::StateHandler> stateHandler;

    bool isRunning;
    bool shouldStop;

  public:
    ClientCli(std::istream& inputStream, std::ostream& outputStream);

    void Start();
    void Stop();

    bool IsRunning() const;

    void SetState(ClientCliState state);
};
}
}

#endif // MACHIAVELLI_MACH_VIEW_CLIENTCLI_H_INCLUDED
