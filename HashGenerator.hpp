#pragma once
#include <string>

class IHashGenerator
{
public:
	virtual std::string GenerateHash (const std::string& theStr) const = 0;
};

class MD5Service : public IHashGenerator
{
public:
	std::string GenerateHash (const std::string& theStr) const override
	{
		return CalculateMD5 (theStr);
	}
private:
	std::string CalculateMD5 (const std::string& theStr) const;
};

class CRC32Service : public IHashGenerator
{
public:
	std::string GenerateHash (const std::string& theStr) const override
	{
		auto aRes = CalculateCRC32 (theStr);
		return CRC32ToMD5 (aRes);
	}
private:
	std::string CRC32ToMD5 (int theCRC32) const;
	int CalculateCRC32 (const std::string& theStr) const;

private:
	MD5Service myMD5;
};