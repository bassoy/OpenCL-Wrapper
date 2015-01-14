#include <utl_profile_pass_manager.h>
#include <utl_assert.h>
#include <utl_profile_pass.h>

namespace utl {
  
ProfilePassManager&
ProfilePassManager::operator<<( ProfilePass* p )
{
  return *this << std::unique_ptr<ProfilePass>( p );
}

ProfilePassManager&
ProfilePassManager::operator<<( std::unique_ptr< ProfilePass >&& p )
{
	TRUE_ASSERT(p != nullptr, "ProfilePass is not valid: nullptr");
	_passes.emplace_back(std::move(p));
	return *this;
}

void ProfilePassManager::run()
{
	for (const auto &it : _passes){
		std::cout << "Profiling " << it->name() << std::endl;
		it->run();
	}
}

void ProfilePassManager::write(std::ostream& out) const
{
	for (const uPPassPtr &it : _passes){
		out << *it;
		out << std::endl;
	}
	out << std::endl;
}


}