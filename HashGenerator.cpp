#include "HashGenerator.hpp"
#include <boost/uuid/detail/md5.hpp>
#include <boost/algorithm/hex.hpp>
#include <boost/crc.hpp>

using namespace std;

std::string MD5Service::CalculateMD5 (const std::string& theStr) const
{
	const char* const aBuffer = theStr.data();
	size_t aBufferSize = theStr.size();

	if (aBuffer == nullptr) {
		return {};
	}

	boost::uuids::detail::md5 md5;
	md5.process_bytes (aBuffer, aBufferSize);
	boost::uuids::detail::md5::digest_type aDigest;
	md5.get_digest(aDigest);
	const auto aCharDigest = reinterpret_cast<const char*> (&aDigest);

	string aRes;
	boost::algorithm::hex (aCharDigest, aCharDigest + sizeof (boost::uuids::detail::md5::digest_type), std::back_inserter(aRes));

	return aRes;
}

int CRC32Service::CalculateCRC32 (const std::string& theStr) const
{
	boost::crc_32_type aResult;
	aResult.process_bytes (theStr.data(), theStr.length());
	return aResult.checksum();
}

std::string CRC32Service::CRC32ToMD5(int theCRC32) const
{
	auto aStr = std::to_string (theCRC32);
	return myMD5.GenerateHash(aStr);
}