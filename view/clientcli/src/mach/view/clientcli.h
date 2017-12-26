#ifndef MACHIAVELLI_MACH_VIEW_CLIENTCLI_H_INCLUDED
#define MACHIAVELLI_MACH_VIEW_CLIENTCLI_H_INCLUDED

#include <istream>
#include <memory>
#include <ostream>

#include <mach/app/multiplayerclient.h>
#include <mach/infra/clicommandparser.h>
#include <mach/infra/subject.h>

#include "mach/view/clientclistate.h"

namespace mach
{
namespace view
{
class ClientCli
{
  private:
    app::MultiplayerClient multiplayerClient;

    std::istream& inputStream;
    std::ostream& outputStream;

    infra::CliCommandParser commandParser;
    infra::Subject commandSubject;

    bool isRunning;
    bool shouldStop;

  public:
    ClientCli(std::istream& inputStream, std::ostream& outputStream);

    void Start();
    void Stop();

    bool IsRunning() const;

    void Render() const;

    void SetState(ClientCliState state);
};
}
}

#endif // MACHIAVELLI_MACH_VIEW_CLIENTCLI_H_INCLUDED
