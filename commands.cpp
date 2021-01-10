#include "commands.hpp"

namespace bioconverter {

int Get_System_Info_1::masterCommand(const QList<QVariant> &input, QByteArray &output) const
{
	output.append(7, 0x0);

	return 0;
}

int Get_System_Info_1::slaveResponse(const QByteArray &input, QList<QVariant> &output) const
{

}




}
