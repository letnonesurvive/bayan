#include "HashGenerator.hpp"
#include <boost/uuid/detail/md5.hpp>
#include <boost/algorithm/hex.hpp>

using namespace std;

HashGenerator::HashGenerator()
{

}

string HashGenerator::Calculate (const std::string& str_md5)
{
	const char* const buffer = str_md5.data();
	size_t buffer_size = str_md5.size();

	if (buffer == nullptr) {
		return {};
	}

	boost::uuids::detail::md5 boost_md5;
	boost_md5.process_bytes(buffer, buffer_size);
	boost::uuids::detail::md5::digest_type digest;
	boost_md5.get_digest(digest);
	const auto char_digest = reinterpret_cast<const char*> (&digest);

	string aRes;
	boost::algorithm::hex(char_digest, char_digest + sizeof (boost::uuids::detail::md5::digest_type), std::back_inserter(aRes));

	return aRes;
}