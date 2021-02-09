#include "CTServiceCharacter.h"
#include <Base/Archive.h>
#include <Unicode/UnicodeUtils.h>

#ifdef EXTERNAL_DISTRO
namespace NAMESPACE 
{
#endif


namespace Base
{

	using namespace Plat_Unicode;

	void get(ByteStream::ReadIterator & source, CTService::CTServiceCharacter & target)
	{
		bool value;
		Plat_Unicode::String temp;

		get(source, temp);
		target.SetCharacter(temp.c_str());

		get(source, value);
		target.SetCanRename(value);
		get(source, value);
		target.SetCanMove(value);
		get(source, value);
		target.SetCanTransfer(value);

		get(source, temp);
		target.SetRenameReason(temp.c_str());
		get(source, temp);
		target.SetMoveReason(temp.c_str());
		get(source, temp);
		target.SetTransferReason(temp.c_str());

//		printf("\nCTServiceCharacter::get() -> [%s] [%d,%d,%d] [%s] [%s] [%s]",
//			wideToNarrow(target.GetCharacter()).c_str(), target.GetCanRename(), target.GetCanMove(),
//			target.GetCanTransfer(), wideToNarrow(target.GetRenameReason()).c_str(),
//			wideToNarrow(target.GetMoveReason()).c_str(), wideToNarrow(target.GetTransferReason()).c_str());	//debug
	}

	void put(ByteStream & target, CTService::CTServiceCharacter & source)
	{
		Plat_Unicode::String temp;

		temp = source.GetCharacter();
		put(target, temp);

		put(target, source.GetCanRename());
		put(target, source.GetCanMove());
		put(target, source.GetCanTransfer());

		temp = source.GetRenameReason();
		put(target, temp);
		temp = source.GetMoveReason();
		put(target, temp);
		temp = source.GetTransferReason();
		put(target, temp);

//		printf("\nCTServiceCharacter::put() -> [%s] [%d,%d,%d] [%s] [%s] [%s]",
//			wideToNarrow(source.GetCharacter()).c_str(), source.GetCanRename(), source.GetCanMove(),
//			source.GetCanTransfer(), wideToNarrow(source.GetRenameReason()).c_str(),
//			wideToNarrow(source.GetMoveReason()).c_str(), wideToNarrow(source.GetTransferReason()).c_str());	//debug
	}

}
#ifdef EXTERNAL_DISTRO
}; // namespace
#endif

