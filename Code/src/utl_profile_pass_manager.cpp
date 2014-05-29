#include <utl_profile_pass_manager.h>
#include <utl_assert.h>
#include <utl_profile_pass.h>


utl::ProfilePassManager&
utl::ProfilePassManager::operator<<(ProfilePass* p)
{
	TRUE_ASSERT(p != nullptr, "ProfilePass is not valid: nullptr");
	_passes.emplace_back(std::unique_ptr<ProfilePass>(p));
	return *this;
}

void utl::ProfilePassManager::run()
{
	for (const auto &it : _passes){
		std::cout << "Profiling " << it->name() << std::endl;
		it->run();
	}
}

void utl::ProfilePassManager::write(std::ostream& out) const
{
	for (const uPPassPtr &it : _passes){
		out << *it;
		out << std::endl;
	}
	out << std::endl;
}
